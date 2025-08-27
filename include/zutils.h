#ifndef ZUTILS_H
#define ZUTILS_H

//============ Includes ====================
#include "arduinoGlue.h"

//============ Refactored code ====================
//============ Shared fucntions ============
void drawPorcent(byte percent);
void setup_progressbar();
void restore_left_focus();
void cls();
void printTime(unsigned long minutos, unsigned long aTiempo, boolean showMillis);
void printBigTime(unsigned long minutos, unsigned long aTiempo, boolean showMillis);
void printTimeDom(unsigned long aTiempo, boolean showMillis);
void printTeamTime(byte team);
void printHistoryTime(unsigned long timeValue);
void startGameCount();
void runLoop();
void setBigChars();
void software_Reset();

//============ Added by Convertor ==========
#endif // ZUTILS_H
