#include "domination.h"
#include "domination_tools.h"
#include "domination_globals.h"
#include "arduinoGlue.h"

// Define shared variables
unsigned int minutes = GAMEMINUTES - 1; // Initialize minutes
unsigned long battMillis = 0;           // Initialize battMillis
const int largoTono = 500;              // Define largoTono

void domination() {
    initializeDominationGame();
    while (inGame) {
        handleDominationGameLoop();
    }

    cleanupDominationGame();
}

void handleDominationGameLoop() {
    runLoop();
    keypad.getKey(); // Get the button pressed
    updateTimers();
    handleLEDs();
    handleSound();
    handleBatteryCheck();
    updateLCD();
    checkGameEnd();

    if (cancelando && action == 1) {
        handleZoneCapturing(); // Handle green team capture "*"
    } else if (defuseando && action == 2) {
        handleZoneCapturing(); // Handle red team capture "#"
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

void updateLCD() {
    if (redrawFull) {
        displayZoneStatus();
        redrawFull = false;
    }
    updateZoneTimes();
    updateGameTime();
}

void updateZoneTimes() {
    lcd.setCursor(0, 2);
    lcd.print(F("Red:         | "));
    printTimeDom(redTime, false);
    lcd.setCursor(0, 3);
    lcd.print(F("Green:       | "));
    printTimeDom(greenTime, false);
}

void handleZoneDefusing() {
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

void handleZoneCapturing() {
    if ((cancelando && team == 1) || (defuseando && team == 2)) {
        // Zone is already active for the same team, do nothing
        return;
    }

    // Start defusing if the zone is active for the opposite team
    if ((cancelando && team == 2) || (defuseando && team == 1)) {
        handleZoneDefusing();
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
                team = 1; // Set to green team
            } else if (defuseando) {
                team = 2; // Set to red team
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

void checkGameEnd() {
    // Calculate remaining time in milliseconds
    long remainingTime = static_cast<long>(minutes * 60000) - static_cast<long>(aTime);

    // Check if the remaining time is zero or less
    if (remainingTime <= 0) {
        inGame = false; // Stop the game loop
        runLoop();
        gameOver();
    }
}

void gameOver() {
    if (team == 1) greenTime += millis() - iZoneTime;
    if (team == 2) redTime += millis() - iZoneTime;
    cls();
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
    lcd.setCursor(9, 3);
    lcd.print(F("[5]Continue"));
    DominationSaveHistory(redTime, greenTime);
    while (1) {
        var = keypad.waitForKey();
        runLoop();
        if (var == '5') {
            if (soundEnable) Bbipp(30);
            break;
        }
    }
    WSclsRGB();
    lcd.clear();
    lcd.setCursor(5, 1);
    lcd.print(F("Play Again?"));
    lcd.setCursor(0, 3);
    lcd.print(F("[*]No         [#]Yes"));
    while (1) {
        var = keypad.waitForKey();
        runLoop();
        if (var == '#') {
            if (soundEnable) Bbipp(30);
            team = 0;
            startGameCount();
            domination();
            break;
        }
        if (var == '*') {
            restore_left_focus();
            if (soundEnable) Bbipp(30);
            software_Reset();
        }
    }
}
