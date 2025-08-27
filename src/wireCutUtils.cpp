#include "wireCutUtils.h"
void distribWires() {
  /*
   *  int defuseWire = x;       2 4 3 0 // 2 4 1 3
   *  int detinationWire = y;
   *  int tCutWire1 = 0;
   *  int tCutWire2 = 0;
   */
    while(tCutWire1 == 0 or tCutWire2 == 0) {
      for (byte i=1; i<=4; i++) {
        if (i != defuseWire and i != detonationWire) {
          if (tCutWire1 == 0) tCutWire1 = i;
        }
        if (i != defuseWire and i != detonationWire and i != tCutWire1) {
         tCutWire2 = i;
        }
      }
    } 
}

void checkConnectivityDetailed() {
  if (digitalRead(wire4) == LOW) { 
     lcd.setCursor(0,0);
     lcd.print(F("Wire1:        OK"));
  } else {
     lcd.setCursor(0,0);
     lcd.print(F("Wire1:    BROKEN"));
     Bbipp(500);
  }
  if (digitalRead(wire3) == LOW) {
     lcd.setCursor(0,1);
     lcd.print(F("Wire2:        OK"));
  } else {
     lcd.setCursor(0,1);
     lcd.print(F("Wire2:    BROKEN"));
     Bbipp(500);
  }
  if (digitalRead(wire2) == LOW) {
     lcd.setCursor(0,2);
     lcd.print(F("Wire3:        OK"));
  } else {
     lcd.setCursor(0,2);
     lcd.print(F("Wire3:    BROKEN"));
     Bbipp(500);
  }
  if (digitalRead(wire1) == LOW) {
     lcd.setCursor(0,3);
     lcd.print(F("Wire4:        OK"));
  } else {
     lcd.setCursor(0,3);
     lcd.print(F("Wire4:    BROKEN"));
     Bbipp(500);
  }
    while(1){
      delay(5000);
      cls();
      lcd.print(F("Fix broken wires!"));
      lcd.setCursor(0,1);
      lcd.print(F("then press #"));
      Bbipp(500);
      delay(2000);
      var = keypad.waitForKey();
      if(var == '*') // Retry button :')
      {
        Bbipp(30); //tone(tonepin,2400,30);
        menuPrincipal();
      } 
      if(var =='#') // Cancel or Back Button :')
      {
        Bbipp(30); //tone(tonepin,2400,30);
        quickCheckWires();
        break;
      }        
   }
}

void quickCheckWires() {
  cls();
  if (digitalRead(wire1)==HIGH or digitalRead(wire2)==HIGH or digitalRead(wire3)==HIGH or digitalRead(wire4)==HIGH) {
    lcd.setCursor(0,0);
    lcd.print(F("Broken Wires!!"));
    checkConnectivityDetailed();
  } else {
    lcd.setCursor(0,0);
    lcd.print(F("Connections...OK"));
    Bbipp(200);
    delay(1000);
  }
}