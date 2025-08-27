#ifndef NFC_UTILS_H
#define NFC_UTILS_H

//============ Includes ====================
#include "arduinoGlue.h"

//=========Functions prototypes ============


void startListeningToNFC();
void handleCardDetected();
void checkCard();
void NFCCardSetupWizard();
void NFCPrintInfo();
bool isNFCPresent();

//============ Added by Convertor ==========
#endif // NFC_UTILS_H
