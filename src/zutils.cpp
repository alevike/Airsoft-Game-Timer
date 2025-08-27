#include "zutils.h"


void drawPorcent(byte percent){
  if (customCharset != 1) setup_progressbar();
  /* Move the cursor to the second line */ 
  lcd.setCursor ( 0 , 2 );
 
  /* Map range (0 ~ 100) to range (0 ~ LCD_NB_COLUMNS * 2 - 2) */ 
  byte nb_columns = map (percent, 0 , 100 , 0 , LCD_NB_COLUMNS *  2  -  2 );
  //Each character displays 2 vertical bars, but the first and last character displays only one.

  /* Draw each character in the line */ 
  for (byte i =  0; i < LCD_NB_COLUMNS; ++ i) {

    if (i ==  0 ) { // First case

      /* Display the starting char according to the number of columns */ 
      if (nb_columns > 0 ) {
        lcd.write ( 1 ); // Char start 1/1 
        nb_columns -= 1 ;

      } else {
        lcd.write ((byte) 0 ); // Char start 0/1
      }

    } else  if (i == LCD_NB_COLUMNS -1 ) { // Last case

      /* Display the end char according to the number of columns */ 
      if (nb_columns >  0 ) {
        lcd.write ( 6 ); // Char thin 1/1

      } else {
        lcd.write ( 5 ); // Char end 0/1
      }

    } else { // Other boxes

      /* Display the correct char according to the number of columns */ 
      if (nb_columns >=  2 ) {
        lcd.write ( 4 ); // Char div 2/2 
        nb_columns -=  2 ;

      } else  if (nb_columns ==  1 ) {
        lcd.write ( 3 ); // Char div 1/2 
        nb_columns -= 1 ;

      } else {
        lcd.write ( 2 ); // Char div 0/2
      }
    }
  }

}


void  setup_progressbar()  {
  /* Save custom characters in LCD display memory */
  lcd.createChar (0,START_DIV_0_OF_1 );
  lcd.createChar (1,START_DIV_1_OF_1 );
  lcd.createChar (2,DIV_0_OF_2 );
  lcd.createChar (3,DIV_1_OF_2 );
  lcd.createChar (4,DIV_2_OF_2 );
  lcd.createChar (5,END_DIV_0_OF_1 );
  lcd.createChar (6,END_DIV_1_OF_1 );
  customCharset = 1;
}


void restore_left_focus(){
  uint8_t leftFocus[8] = {
  0b00000,
  0b00100,
  0b00010,
  0b11111,
  0b00010,
  0b00100,
  0b00000,
  0b00000
  };
  lcd.createChar (6,leftFocus);
  customCharset = 0;
}


void cls(){
  lcd.clear();
  lcd.setCursor(0,0);
  redrawFull = true;
}


void printTime(unsigned long minutos, unsigned long aTiempo, boolean showMillis){
  //minutes
  if((minutos-aTiempo/60000)<10)
  {
    lcd.print("0");
    lcd.print(minutos-aTiempo/60000);
  }
  else
  {
    lcd.print(minutos-aTiempo/60000);
  }
  lcd.print(":");
  //seconds
  if((59-((aTiempo/1000)%60))<10)
  {
    lcd.print("0");
    lcd.print(59-((aTiempo/1000)%60));
  }
  else
  {
    lcd.print(59-((aTiempo/1000)%60));
  }
  if(showMillis){
    lcd.print(":");
    //this not mach with real time, is just a effect, it says 999 because millis%1000 sometimes give 0 LOL
    lcd.print(999-(millis()%1000));
  }
}


void printBigTime(unsigned long minutos, unsigned long aTiempo, boolean showMillis){
  if (customCharset != 2)setBigChars();
  //minutes
  big.writeint(1,2, minutos-aTiempo/60000, 3, true);
  lcd.setCursor(11,1);
  lcd.write(5);
  lcd.setCursor(11,2);
  lcd.write(7);
  //Seconds
  big.writeint(1,12, 59-((aTiempo/1000)%60), 2, true);
}


void printTimeDom(unsigned long aTiempo, boolean showMillis){
  //minutes
  if((aTiempo/60000)<10)
  {
    lcd.print("0");
    lcd.print(aTiempo/60000);
  }
  else
  {
    lcd.print(aTiempo/60000);
  }
  lcd.print(":");
  //seconds
  if(((aTiempo/1000)%60)<10)
  {
    lcd.print("0");
    lcd.print((aTiempo/1000)%60);
  }
  else
  {
    lcd.print((aTiempo/1000)%60);
  }
  if(showMillis){
    lcd.print(":");
    //this not mach with real time, is just a effect, it says 999 because millis%1000 sometimes give 0 LOL
      lcd.print(999-millis()%1000);

  }
}


void printTeamTime(byte team) {
  unsigned long timeValue;
  if (team == 1)timeValue = greenTime;
  if (team == 2)timeValue = redTime;
  if((timeValue/60000)<10)
  {
    lcd.print("0");
    lcd.print(timeValue/60000);
  }
  else
  {
    lcd.print(timeValue/60000);
  }
  lcd.print(":");
  //seconds
  if(((timeValue/1000)%60)<10)
  {
    lcd.print("0");
    lcd.print((timeValue/1000)%60);
  }
  else
  {
    lcd.print((timeValue/1000)%60);
  }
}


void printHistoryTime(unsigned long timeValue) {
  if((timeValue/60000)<10)
  {
    lcd.print("0");
    lcd.print(timeValue/60000);
  }
  else if ((timeValue/60000)<10000)
  {
    lcd.print(timeValue/60000);
  } else {
    lcd.print("--");
    }
  lcd.print(":");
  //seconds
  if(((timeValue/1000)%60)<10 and (timeValue/60000) < 10000)
  {
    lcd.print("0");
    lcd.print((timeValue/1000)%60);
  }
  else if (((timeValue/1000)%60) and (timeValue/60000) < 10000)
  {
    lcd.print((timeValue/1000)%60);
  } else {
    lcd.print("--");
  }
}


void startGameCount(){
  if (customCharset != 2)setBigChars();
  WSclsRGB();
  cls();
  lcd.setCursor(3,1);
  lcd.print(F("Ready to Begin"));
  lcd.setCursor(2,2);
  lcd.print(F("[Press a button]"));
  keypad.waitForKey();//if you press a button game start
  cls();
  lcd.setCursor(5,0);
  lcd.print(F("Starting in"));
  for(int i = 5; i > 0 ; i--){ // START COUNT GAME INIT
    Bbipp(100);  // -100 from delay
    big.writeint(1,9, i, 1, false);
    delay(900);
  }
  cls();
}


void runLoop() {
  timer.run();
  checkCard();

  //LoRa testing
  loraReceiveBroadcast();

  // Interrupt-driven Power management with short/long press
  if (powerButtonReleased) {
    powerButtonReleased = false;
    unsigned long pressDuration = powerReleasedTime - powerPressedTime;
    // Only process if release is after press and duration is reasonable
    if (powerReleasedTime > powerPressedTime && pressDuration > 10) {
        Serial.print(F("Power button press duration: "));
        Serial.println(pressDuration);
        if (pressDuration < POWER_SHORT_PRESS_TIME) {
            Serial.println(F("Event triggered: Short press"));
        }
        else if (pressDuration > POWER_LONG_PRESS_TIME) {
            Serial.println(F("Event triggered: Long press"));
            Serial.println(F("Powering off"));
            cls();
            lcd.print(F("POWER OFF!"));
            digitalWrite(KEEP_POWER_PIN, LOW);
            while(1); // stop the controller
        }
        // Reset times only after a valid event
        powerPressedTime = 0;
        powerReleasedTime = 0;
    }
  }
}


void setBigChars() {
  byte bar0[8] = {B11100, B11110, B11110, B11110, B11110, B11110, B11110, B11100};
  byte bar1[8] = {B00111, B01111, B01111, B01111, B01111, B01111, B01111, B00111};
  byte bar2[8] = {B11111, B11111, B00000, B00000, B00000, B00000, B11111, B11111};
  byte bar3[8] = {B11110, B11100, B00000, B00000, B00000, B00000, B11000, B11100};
  byte bar4[8] = {B01111, B00111, B00000, B00000, B00000, B00000, B00011, B00111};
  byte bar5[8] = {B00000, B00000, B00000, B00000, B00000, B00000, B11111, B11111};
  byte bar6[8] = {B00000, B00000, B00000, B00000, B00000, B00000, B00111, B01111};
  byte bar7[8] = {B11111, B11111, B00000, B00000, B00000, B00000, B00000, B00000};
  lcd.createChar (0, bar0);
  lcd.createChar (1, bar1);
  lcd.createChar (2, bar2);
  lcd.createChar (3, bar3);
  lcd.createChar (4, bar4);
  lcd.createChar (5, bar5);
  lcd.createChar (6, bar6);
  lcd.createChar (7, bar7);
  customCharset = 2;
}


void software_Reset() {   // Mem clean - Restarts controller without impacting the status of peripherals and registers - basically invisible restart
  soft_reboot_flag = 0xAA;
  asm volatile ("  jmp 0");  
}