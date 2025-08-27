#ifndef DOMINATION_TOOLS_H
#define DOMINATION_TOOLS_H

#include "arduinoGlue.h"
#include "domination.h"
#include "LoRa_domination.h"


// Function prototypes
void initializeDominationGame();
void updateTimers();
void handleLEDs();
void updateTeamLED();
void handleSound();
void playSound(int duration);
bool isCriticalTime(int minutesLeft);
void handleBatteryCheck();
void displayZoneStatus();
void updateGameTime();
void cleanupDominationGame();

#endif // DOMINATION_TOOLS_H