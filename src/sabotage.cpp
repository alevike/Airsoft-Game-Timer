#include "sabotage.h"

void sabotage(){
  digitalWrite(BUTTONLEDGREEN, LOW);
  digitalWrite(BUTTONLEDRED, HIGH);
  setup_progressbar();
  startListeningToNFC();
  runLoop();
  WSclsRGB();
    //SETUP INITIAL TIME 
  team = 0;
  int minutos = GAMEMINUTES-1;
  cls();
  lcd.setCursor(3,1);
  lcd.print(F("BOMB DISARMED"));
  delay(1000);
  cls();
  
 if(start){
  iTime=millis(); //  initialTime in millisec 
  start=false;
 }
  
  aTime = 0;
  unsigned long battMillis;
  battMillis=millis();

  //Starting Game Code
  while(1){  // this is the important code, is a little messy but works good.
    runLoop();
    
    //code for battery check
    _checkBattery(battMillis);
    
    //Code for led blinking
    timeCalcVar=(millis()- iTime)%1000;
    if(timeCalcVar >= 0 && timeCalcVar <= 50)
    {
       WSblueRGB();
       runLoop();
    }
    if(timeCalcVar >= 90 && timeCalcVar <= 130)
    {    
      WSclsRGB();
      runLoop();
    }

    lcd.setCursor(5,0);
    lcd.print(F("GAME TIME"));
    aTime=millis()- iTime;
    lcd.setCursor(3,1);

    //PRINT TIME ON LCD
    printBigTime(minutos, aTime, true);
    //printTime(minutos, aTime, true);

    lcd.setCursor(12,3);
    lcd.print(F("DISARMED"));
    

    //###########################CHECKINGS##################

    //Check If Game End
    //if(minutos-aTime/60000==0 && 59-((aTime/1000)%60)==0)
    if((minutos-aTime/60000==0 && 59-((aTime/1000)%60)==0) or (minutos-aTime/60000 >= 6000000.00))
    {
      
        disarmedSplash();
        
    }
    //USED IN PASSWORD GAME 
    if('#' == keypad.getKey() && passwordEnable){
      lcd.clear();
      lcd.setCursor(0,0);    
      lcd.print(F("ARMING BOMB"));
      delay(1000);//a little delay to think in the password
      lcd.clear();
      lcd.setCursor(3, 0);
      lcd.print(F("Enter Code"));

      setCode();// we need to set the compare variable first

      //then compare :D

      if(comparePassword()){
        destroySabotage();
      }        
      lcd.clear();
      lcd.setCursor(2,0);
      lcd.print(F("Code Error!"));
      Bbipp(200); //tone(tonepin,errorTone,200);
      delay(500);
      cls();
    }

    //Check If Is Activating
    while(defuseando && !passwordEnable)
    {
      runLoop();
      keypad.getKey();
      cls();
      WSclsRGB();
      lcd.clear();
      lcd.setCursor(0,1);
      lcd.print(F("ARMING BOMB"));
      lcd.setCursor(0,1);
      unsigned int percent=0;
      unsigned long xTime=millis(); //start disabling time
      while(defuseando)
      {
        keypad.getKey();
        runLoop();
        timeCalcVar = (millis()- xTime)%1000;

        if( timeCalcVar >= 0 && timeCalcVar <= 40)
        {
          bipp(200); //tone(tonepin,tonoAlarma1,200);
        }
        if(timeCalcVar >= 480 && timeCalcVar <= 520)
        {
          bipp(200); //tone(tonepin,tonoAlarma2,200);
        }
        unsigned long seconds= millis() - xTime;
        percent = (seconds)/(ACTIVATESECONDS*10);
        drawPorcent(percent);
        WSwriteProcent (percent, team, action);


        if(percent >= 100)
        {
          WSclsRGB();
          runLoop();
          team = 2;
          destroySabotage();// jump to the next gamemode
        }
      }
      cls();
      WSclsRGB();
      runLoop();
    }
  }
  restore_left_focus();
}

void destroySabotage(){
  setup_progressbar();
  startListeningToNFC();
  lcd.clear();
  lcd.setCursor(5,1);
  lcd.print(F("BOMB ARMED"));
  delay(1000);
  int minutos=BOMBMINUTES-1;
  unsigned long iTime=millis();
  unsigned long aTime;
  unsigned long battMillis = millis();
  int largoTono = 50;

  //MAIN LOOP
  while(1){
    runLoop();
    //Check battery
    _checkBattery(battMillis);

    //If you fail disarm. 
    if(endGame){
      start = true;
      explodeSplash();
    }

    //Led Blink

    timeCalcVar=(millis()- iTime)%1000;
    if(timeCalcVar >= 0 && timeCalcVar <= 40)
    {
      WSredRGB(); 
      bipp(200); //tone(tonepin,tonoActivada,largoTono);
    }
    if(timeCalcVar >= 180 && timeCalcVar <= 220){
      WSclsRGB();
    }
    //Sound 

    timeCalcVar=(millis()- iTime)%1000;
    aTime= millis()- iTime;
    if(timeCalcVar >= 245 && timeCalcVar <= 255 && minutos-aTime/60000<2 && soundEnable)bipp(largoTono); //tone(tonepin,tonoActivada,largoTono);
    if(timeCalcVar >= 495 && timeCalcVar <= 510 && minutos-aTime/60000<4 && soundEnable)bipp(largoTono); //tone(tonepin,tonoActivada,largoTono);
    if(timeCalcVar >= 745 && timeCalcVar <= 760 && minutos-aTime/60000<2 && soundEnable)bipp(largoTono); //tone(tonepin,tonoActivada,largoTono);
    if( minutos-aTime/60000==0 && 59-((aTime/1000)%60) < 10)largoTono = 300;

    lcd.setCursor(3,0);
    lcd.print(F("DETONATION IN"));
    //Passed Time
    
    lcd.setCursor(3,1);

    ////////HERE ARE THE TWO OPTIONS THAT ENDS THE GAME///////////

    ////TIME PASED AWAY AND THE BOMB EXPLODES
    if(minutos-aTime/60000==0 && 59-((aTime/1000)%60)==0)// Check if game ends
    {
      start = true;
      explodeSplash();
    }
    //print time
     printBigTime(minutos, aTime, true);
     //printTime(minutos, aTime, true);
     lcd.setCursor(15,3);
     lcd.print(F("ARMED"));

    //// SECOND OPTION: YOU PRESS DISARMING BUTTON  

    //IF IS A PASSWORD GAME 

    if('#' == keypad.getKey() && passwordEnable){

      cls();
      WSclsRGB(); 
      WSblueRGB(); 
      lcd.print(F("DISARMING BOMB"));
      delay(1000);//a little delay to think on the password

      lcd.clear();
      lcd.setCursor(3,0);
      lcd.print(F("Enter Code"));

      setCode();// we need to set the compare variable first

      //then compare :D

      if(comparePassword()){
        sabotage();
      }        
      lcd.clear();
      lcd.setCursor(2,0);
      lcd.print(F("Code Error!"));
      Bbipp(200);// tone(tonepin,errorTone,200);
      delay(500);
      cls();
    }

    if(defuseando && !passwordEnable)// disarming bomb
    {
      lcd.clear();
      WSclsRGB();
      runLoop();
      lcd.setCursor(4,1);
      lcd.print(F("DISARMING"));
      lcd.setCursor(0,1);
      unsigned int percent=0;
      unsigned long xTime=millis();
      while(defuseando)
      {
        keypad.getKey();
        //check if game time runs out during the disabling
        aTime= millis()- iTime;
        if((minutos-aTime/60000==0 && 59-((aTime/1000)%60)==0) || minutos-aTime/60000>4000000000){ 
          endGame = true;
          start = true;
        }
        timeCalcVar=(millis()- xTime)%1000;
        if(timeCalcVar>= 0 && timeCalcVar <= 20)
        { 
          bipp(200);//tone(tonepin,tonoAlarma1,200);
        }
        if(timeCalcVar >= 480 && timeCalcVar <= 500)
        {
          bipp(200); //tone(tonepin,tonoAlarma2,200);  
        }
        unsigned long seconds=(millis()- xTime);
        percent= seconds/(ACTIVATESECONDS*10);
        drawPorcent(percent);  
        WSwriteProcent (percent, team, action);
        runLoop();

        //BOMB DISARMED GAME OVER
        if(percent >= 100)
        {
          team = 0;
          sabotage();
            
        }
      }
      WSclsRGB();
      cls(); 
      runLoop();
    }
  }
  restore_left_focus();
}

void _checkBattery(unsigned long &battMillis) {
  if (enableBattery && battMillis + 20000 < millis()) {
      battMillis = millis();
      batteryManager.quickCheck();
  }
}