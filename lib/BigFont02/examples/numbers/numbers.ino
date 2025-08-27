#include <Wire.h>
#include <LiquidCrystal.h>
#include <BigFont02.h>

const int lcdD7Pin = 7; // LCD D7 pin
const int lcdD6Pin = 6; // LCD D6 pin
const int lcdD5Pin = 5; // LCD D5 pin
const int lcdD4Pin = 4; // LCD D4 pin
const int lcdEPin =  9; // LCD E Pin
const int lcdRSPin = 8; // LCD RS pin

LiquidCrystal lcd(lcdRSPin, lcdEPin, lcdD4Pin, lcdD5Pin, lcdD6Pin, lcdD7Pin); // construct LCD object
BigFont02     big(&lcd); // construct large font object, passing to it the name of our lcd object

void setup()
{
  lcd.begin(16,2);
  big.begin();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.write ("   BigFont02    ");
  lcd.setCursor(0,1);
  lcd.write ("   =========    ");
  delay(2000);
  lcd.clear();
}

void loop()
{
  big.writeint(0,3,millis(),4,true); // display milliseconds value using 4 digits with leading zeros enabled starting at row 0 column 5
}
