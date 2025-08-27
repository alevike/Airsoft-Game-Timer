#include "LoRa_domination.h"
#include "domination_tools.h"
#include "domination_globals.h"
#include "arduinoGlue.h"


void LoRa_domination() {
    initializeDominationGame();
    while (inGame) {
        LoRa_handleDominationGameLoop();
    }

    cleanupDominationGame();
}

void LoRa_handleDominationGameLoop() {
    runLoop();
    keypad.getKey(); // Get the button pressed
    updateTimers();
    handleLEDs();
    handleSound();
    handleBatteryCheck();
    LoRa_updateLCD();
    LoRa_checkGameEnd();

    if (cancelando && action == 1) {
        LoRa_handleZoneCapturing(); // Handle green team capture "*"
    } else if (defuseando && action == 2) {
        LoRa_handleZoneCapturing(); // Handle red team capture "#"
    }

    // Update domination time for the green team
    if (team == 1 && iZoneTime > 0) {
        greenTime += millis() - iZoneTime;
        iZoneTime = millis(); // Reset zone time for continuous counting
    }

    // Update domination time for the red team
    if (team == 2 && iZoneTime > 0) {
        redTime += millis() - iZoneTime;
        iZoneTime = millis(); // Reset zone time for continuous counting
    }
}

void LoRa_updateLCD() {
    if (redrawFull) {
        displayZoneStatus();
        redrawFull = false;
    }
    LoRa_updateZoneTimes();
    updateGameTime();
}

void LoRa_updateZoneTimes() {
    lcd.setCursor(0, 2);
    lcd.print(F("Red:         | "));
    if (lora_myOwnID == 1)  {
        printTimeDom(redTime, false);
    } else {
        if (team == 2) {
            lcd.print (F("DOM"));
        } else {
            lcd.print (F("---"));
        }
    }
    lcd.setCursor(0, 3);
    lcd.print(F("Green:       | "));
    if (lora_myOwnID == 1) {
        printTimeDom(greenTime, false);
    } else {
        if (team == 1) {
            lcd.print (F("DOM"));
        } else {
            lcd.print (F("---"));
        }
    }
}

void LoRa_handleZoneDefusing() {
    if (team == 0) {
        // Zone is neutral, no defusing needed
        return;
    }
    keypad.getKey();
    runLoop();
    cls();
    redrawFull = true;

    lcd.setCursor(3, 1);
    lcd.print(F("DEFUSING ZONE"));

    unsigned int percent = 0;
    unsigned long xTime = millis(); // Start defusing time
    while (defuseando || cancelando) { // Check if the defusing flag is set
        keypad.getKey();
        timeCalcVar = (millis() - xTime) % 1000;

        if (timeCalcVar >= 0 && timeCalcVar <= 20) {
            if (soundEnable) bipp(200);
        }
        if (timeCalcVar >= 480 && timeCalcVar <= 500) {
            if (soundEnable) bipp(200);
        }

        unsigned long seconds = millis() - xTime;
        percent = (seconds) / (ACTIVATESECONDS * 10);

        // Update progress bar and LED colors
        drawPorcent(percent);
        WSwriteProcent(percent, team, action);
        runLoop();

        if (percent >= 100) {
            delay(1000);
            runLoop();
            if (team == 1) {
                greenTime += millis() - iZoneTime;
            } else if (team == 2) {
                redTime += millis() - iZoneTime;
            }
            loraCallRemote(10, 0); //capture to Neutral
            iZoneTime = 0;
            team = 0; // Set zone to neutral

            break;
        }
    }

    // If the button is released before completion, reset the process
    if ((!cancelando || !defuseando) && percent < 100) {
        cls();
        lcd.setCursor(0, 1);
        lcd.print(F("   DEFUSE FAILED    "));
        delay(1000);
    }

    cls();
    WSclsRGB();
}

void LoRa_handleZoneCapturing() {
    if ((cancelando && team == 1) || (defuseando && team == 2)) {
        // Zone is already active for the same team, do nothing
        return;
    }

    // Start defusing if the zone is active for the opposite team
    if ((cancelando && team == 2) || (defuseando && team == 1)) {
        LoRa_handleZoneDefusing();
    }

    // Start capturing the zone for the current team
    runLoop();
    cls();
    redrawFull = true;

    lcd.setCursor(3, 1);
    lcd.print(F("CAPTURING ZONE"));

    unsigned int percent = 0;
    unsigned long xTime = millis(); // Start capturing time
    while (cancelando || defuseando) { // Check if the capturing flag is set
        keypad.getKey();
        timeCalcVar = (millis() - xTime) % 1000;

        if (timeCalcVar >= 0 && timeCalcVar <= 20) {
            if (soundEnable) bipp(200);
        }
        if (timeCalcVar >= 480 && timeCalcVar <= 500) {
            if (soundEnable) bipp(200);
        }

        unsigned long seconds = millis() - xTime;
        percent = (seconds) / (ACTIVATESECONDS * 10);

        // Update progress bar and LED colors
        drawPorcent(percent);
        WSwriteProcent(percent, team, action);
        runLoop();

        if (percent >= 100) {
            WSclsRGB();
            if (cancelando) {
                // team = 1; // Set to green team
                loraCallRemote(11,0);
            } else if (defuseando) {
                // team = 2; // Set to red team
                loraCallRemote(12,0);
            }
            iZoneTime = millis(); // Initialize zone time for the capturing team
            delay(1000);
            runLoop();
            break;
        }
    }

    // If the button is released before completion, reset the process
    if ((!cancelando && !defuseando ) && percent < 100) {
        cls();
        lcd.setCursor(0, 1);
        lcd.print(F("   CAPTURE FAILED   "));
        delay(1000);
    }

    cls();
    WSclsRGB();
}

void LoRa_checkGameEnd() {
    // Calculate remaining time in milliseconds
    long remainingTime = static_cast<long>(minutes * 60000) - static_cast<long>(aTime);

    // Check if the remaining time is zero or less
    if (remainingTime <= 0) {
        inGame = false; // Stop the game loop
        runLoop();
        LoRa_gameOver();
    }
}

void LoRa_gameOver() {
    if (team == 1) greenTime += millis() - iZoneTime;
    if (team == 2) redTime += millis() - iZoneTime;
    cls();
    //Publish results to LoRa network
    if (lora_myOwnID > 1) {
        greenTime = redTime = 0; // Reset times - we need to sync them from master
        lora_syncDomResults(); //sync team time from master
    } else if(lora_myOwnID == 1) {
        //i'm the master - send results
        lora_sendResults();
    }

    redrawFull = true;
    lcd.setCursor(0, 0);
    lcd.print(F("====Time is up!!===="));
    lcd.setCursor(0, 1);
    if (redTime > greenTime) {
        lcd.print(F("*Red time*   | "));
        WSredRGB(); digitalWrite(BUTTONLEDRED, LOW); digitalWrite(BUTTONLEDGREEN, LOW);
    } else {
        lcd.print(F("Red time     | "));
    }
    printTimeDom(redTime, false);
    lcd.setCursor(0, 2);
    if (greenTime > redTime) {
        lcd.print(F("*Green time* | "));
        WSgreenRGB(); digitalWrite(BUTTONLEDRED, LOW); digitalWrite(BUTTONLEDGREEN, LOW);
    } else {
        lcd.print(F("Green time   | "));
    }
    printTimeDom(greenTime, false);
    DominationSaveHistory(redTime, greenTime); // save hostory to eeprom - on all devices

    if (lora_myOwnID == 1) {
        lcd.setCursor(9, 3);
        lcd.print(F("[5]Continue"));
        while (1) {
            var = keypad.waitForKey();
            runLoop();
            if (var == '5') {
                if (soundEnable) Bbipp(30);
                if (lora_send(203, 0, 0)) {
                    software_Reset();
                    break;
                }
            }
        }
    } else {
        lcd.setCursor(0, 3);
        lcd.print(F("-Waiting for master-"));
        while (1) {
            runLoop();
        }
    }
}
    

