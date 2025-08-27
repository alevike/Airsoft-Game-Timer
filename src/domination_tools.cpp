#include "arduinoGlue.h"
#include "domination.h"
#include "LoRa_domination.h"
#include "domination_tools.h"
#include "domination_globals.h"

void initializeDominationGame() {
    cls(); // Clear the LCD
    inGame = true;
    gameSelector = 1;
    keypad.getKey(); // Get the button pressed
    digitalWrite(BUTTONLEDGREEN, HIGH);
    digitalWrite(BUTTONLEDRED, HIGH);
    startListeningToNFC();
    setup_progressbar();

    iTime = millis(); // Initialize game timer
    aTime = 0;
    redTime = 0;
    greenTime = 0;
    team = 0;
    redrawFull = true; // Redraw full screen on first loop

    // Use the updated value of GAMEMINUTES
    minutes = GAMEMINUTES;

    WSclsRGB(); // Turn off LEDs
    if (enableBattery) {
        battMillis = millis(); // Initialize battery check timer
    }
}

void updateTimers() {
    aTime = millis() - iTime; // Calculate elapsed time
    timeCalcVar = aTime % 1000; // Calculate time for LED and sound effects
}

void updateTeamLED() {
    switch (team) {
        case 0: WSblueRGB(); digitalWrite(BUTTONLEDRED, HIGH); digitalWrite(BUTTONLEDGREEN, HIGH); break; // Neutral
        case 1: WSgreenRGB(); digitalWrite(BUTTONLEDRED, LOW); digitalWrite(BUTTONLEDGREEN, HIGH); break; // Green team
        case 2: WSredRGB(); digitalWrite(BUTTONLEDRED, HIGH); digitalWrite(BUTTONLEDGREEN, LOW); break; // Red team
    }
}

void handleLEDs() {
    if (timeCalcVar >= 0 && timeCalcVar <= 40) {
        updateTeamLED();
    }
    if (timeCalcVar >= 50 && timeCalcVar <= 100) {
        WSclsRGB();
    }
}

void handleSound() {
    if (timeCalcVar >= 0 && timeCalcVar <= 40) playSound(300);
    if (timeCalcVar >= 245 && timeCalcVar <= 255 && isCriticalTime(2)) playSound(largoTono);
    if (timeCalcVar >= 495 && timeCalcVar <= 510 && isCriticalTime(4)) playSound(largoTono);
    if (timeCalcVar >= 745 && timeCalcVar <= 760 && isCriticalTime(2)) playSound(largoTono);
}

void playSound(int duration) {
    if (soundEnable) bipp(duration);
}

bool isCriticalTime(int minutesLeft) {
    return (minutes - static_cast<int>(aTime / 60000)) < minutesLeft;
}

void handleBatteryCheck() {
    if (enableBattery && battMillis + 20000 < millis()) {
        battMillis = millis();
        batteryManager.quickCheck();
        redrawFull = true;
    }
}

void displayZoneStatus() {
    lcd.setCursor(0, 0);
    if (team == 0) lcd.print(F("====NEUTRAL ZONE===="));
    else if (team == 1) lcd.print(F("=====GREEN ZONE====="));
    else if (team == 2) lcd.print(F("======RED ZONE======"));
}

void updateGameTime() {
    lcd.setCursor(0, 1);
    lcd.print(F("Time Left:   | "));
    unsigned long remainingTime = minutes * 60000 - aTime;
    printTimeDom(remainingTime, false); // Avoid unnecessary decimals
}

void cleanupDominationGame() {
    WSclsRGB(); // Turn off LEDs
    restore_left_focus(); // Restore LCD focus
}