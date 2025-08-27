#ifndef BigFont02_h
#define BigFont02_h

#include "Arduino.h"
#include "LiquidCrystal.h"

class BigFont02 {
	public:
		BigFont02(LiquidCrystal*);
		void begin();
		int  writechar(int, int, char);                // col, row, single ascii character
		void clear(int, int);                          // col, row
		void writeint (int, int, int, uint8_t, bool);  // col, row, number to display, digits, leading 
	private:
		LiquidCrystal* _lcd;
};

#endif