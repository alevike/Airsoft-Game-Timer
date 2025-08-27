#ifndef LORA_DOMINATION_H
#define LORA_DOMINATION_H

// Include necessary headers
#include "arduinoGlue.h"
#include "domination_tools.h"

// Function prototypes
void LoRa_domination();
void LoRa_handleDominationGameLoop();
void LoRa_updateLCD();
void LoRa_updateZoneTimes();
void LoRa_handleZoneDefusing();
void LoRa_handleZoneCapturing(); // Updated to include char parameter
void LoRa_checkGameEnd();
void LoRa_gameOver();

#endif // LORA_DOMINATION_H
