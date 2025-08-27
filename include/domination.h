#ifndef DOMINATION_H
#define DOMINATION_H

// Include necessary headers
#include "arduinoGlue.h"

// Function prototypes
void domination();
void handleDominationGameLoop();
void updateLCD();
void updateZoneTimes();
void checkGameEnd();
void handleZoneDefusing();
void handleZoneCapturing(); // Updated to include char parameter
void gameOver();

#endif // DOMINATION_H
