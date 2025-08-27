#include "searchdestroy.h"
void search(){
  inGame = true;
  digitalWrite(BUTTONLEDGREEN, LOW);
  digitalWrite(BUTTONLEDRED, HIGH);
  setup_progressbar();
  cls();
  startListeningToNFC();
//  digitalWrite(REDLED, LOW); 
//  digitalWrite(GREENLED, LOW);   
  //SETUP INITIAL TIME 
  int minutos = GAMEMINUTES-1;
  unsigned long iTime=millis(); //  initialTime in millisec 
  aTime = 0;
  unsigned long battMillis;
  battMillis=millis();
  //var='o';
  team = 0;

  //Starting Game Code
  while(inGame){  // this is the important code, is a little messy but works good.
    //Code for led blinking
    runLoop();
    timeCalcVar=(millis()- iTime)%1000;
    if(timeCalcVar >= 0 && timeCalcVar <= 50)
    {
      WSblueRGB();
    }
    if(timeCalcVar >= 90 && timeCalcVar <= 130)
    {   
      WSclsRGB(); 
    }

    lcd.setCursor(5,0);
    lcd.print(F("GAME TIME"));
    aTime=millis()- iTime;
    lcd.setCursor(3,1);

    //PRINT TIME ON LCD

    printBigTime(minutos, aTime, true);

    //###########################CHECKINGS##################

    //Battery Check
    if(enableBattery and battMillis+20000<millis()){
      battMillis=millis();   
      batteryManager.quickCheck(); 
    }

    //Check If Game End
    //if(minutos-aTime/60000==0 && 59-((aTime/1000)%60)==0)
    if((minutos-aTime/60000==0 && 59-((aTime/1000)%60)==0) or (minutos-aTime/60000 >= 6000000.00))
    {
        inGame = false;
        doAction = 1;
        break;
        //disarmedSplash();
    }
    //Serial.println(keypad.getKey());
    //USED IN PASSWORD GAME 
    if('#' == keypad.getKey() && passwordEnable){
      lcd.clear();
      lcd.setCursor(2,0);
      lcd.print(F("ARMING THE BOMB"));
      delay(1000);//a little delay to think in the password
      lcd.clear();
      lcd.setCursor(3,0);
      lcd.print(F("Enter Code"));

      setCode();// we need to set the comparation variable first it writes on enteredText[]

      //then compare :D

      if(comparePassword()){
        doAction = 2;
        break;
        //destroy();
      }        
      lcd.clear();
      lcd.setCursor(4,2);
      lcd.print(F("Code Error!"));
      Bbipp(200); //tone(tonepin,errorTone,200);
      delay(500);
      cls();
    }
    //Check If Is Activating
    while(defuseando && !passwordEnable && inGame)
    {
      runLoop();
      WSclsRGB();
      lcd.clear();
      lcd.setCursor(2,1);
      lcd.print(F("ARMING THE BOMB"));
      lcd.setCursor(0,1);
      unsigned int percent=0;
      unsigned long xTime=millis(); //start disabling time
      while(defuseando)
      {
        runLoop();
        keypad.getKey();
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
          runLoop();
          WSclsRGB();
          team = 2;
          inGame = false;
          doAction = 2;
          break;
          //destroy();// jump to the next gamemode
        }
      }
      cls();
      WSclsRGB();
    }
  }
  if (doAction == 1) {
     restore_left_focus();
     disarmedSplash();
  }
  else if (doAction == 2) {
    destroy();
  } 
}

void destroy(){
  inGame = true;
  doAction=0;
  setup_progressbar();
  lcd.clear();
  lcd.setCursor(5,1);
  lcd.print(F("BOMB ARMED"));
  startListeningToNFC();
  delay(1000);
  int minutos=BOMBMINUTES-1;
  unsigned long iTime=millis();
  unsigned long aTime;
  unsigned long battMillis = millis();
  int largoTono = 50;
  runLoop();
  //MAIN LOOP
  while(inGame){
    runLoop();
    //If you fail disarm. 
    if(endGame){
      doAction = 1;
      inGame = false;
      break;
      //explodeSplash();
    }

    //Led Blink

    timeCalcVar=(millis()- iTime)%1000;
    if(timeCalcVar >= 0 && timeCalcVar <= 40)
    {
      WSredRGB();
      bipp(largoTono); //tone(tonepin,tonoActivada,largoTono);
    }
    if(timeCalcVar >= 180 && timeCalcVar <= 220){
      WSclsRGB();
    }
    //Check battery
    if(enableBattery and battMillis+20000<millis()){
      battMillis=millis();   
      batteryManager.quickCheck(); 
    }
    //Sound 
    aTime= millis()- iTime;
    timeCalcVar=(millis()- iTime)%1000;
    if(timeCalcVar >= 245 && timeCalcVar <= 255 && minutos-aTime/60000<2 && soundEnable)bipp(largoTono); //tone(tonepin,tonoActivada,largoTono);
    if(timeCalcVar >= 495 && timeCalcVar <= 510 && minutos-aTime/60000<4 && soundEnable)bipp(largoTono); //tone(tonepin,tonoActivada,largoTono);
    if(timeCalcVar >= 745 && timeCalcVar <= 760 && minutos-aTime/60000<2 && soundEnable)bipp(largoTono); //tone(tonepin,tonoActivada,largoTono);
    if( minutos-aTime/60000==0 && 59-((aTime/1000)%60) < 10)largoTono = 300;

    lcd.setCursor(3,0);
    lcd.print(F("DETONATION IN "));
    //Passed Time
   
    lcd.setCursor(3,1);

    ////////HERE ARE THE TWO OPTIONS THAT ENDS THE GAME///////////

    ////TIME PASED AWAY AND THE BOMB EXPLODES
    if(minutos-aTime/60000==0 && 59-((aTime/1000)%60)==0)// Check if game ends
    {
      doAction = 1;
      inGame = false;
      //explodeSplash();
      start = true;
      break;
    }
    //print time

    printBigTime(minutos, aTime, true);

    //// SECOND OPTION: YOU PRESS DISARMING BUTTON  

    //IF IS A PASSWORD GAME 

    if('#' == keypad.getKey() && passwordEnable){

      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(F("DISARMING BOMB"));
      delay(1000);//a little delay to think in the password

      lcd.clear();
      lcd.setCursor(3,0);
      lcd.print(F("Enter Code"));

      setCode();// we need to set the compare variable first

      //then compare :D

      if(comparePassword()){
        doAction = 2;
        inGame = false;
        break;
        //disarmedSplash();
      }        
      lcd.clear();
      lcd.setCursor(3,0);
      lcd.print(F("Code Error!"));
      bipp(200); //tone(tonepin,errorTone,200);
      delay(500);
      cls();
    }

    if(defuseando && !passwordEnable)// disarming bomb
    {
      lcd.clear();
      //digitalWrite(REDLED, LOW);  
      WSclsRGB();
      lcd.setCursor(4,1);
      lcd.print(F(" DISARMING "));
      lcd.setCursor(0,1);
      unsigned int percent=0;
      unsigned long xTime=millis();
      while(defuseando && inGame)
      {
        runLoop();
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
          bipp(200); //tone(tonepin,tonoAlarma1,200);
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
          inGame = false;
          doAction = 2;
          break;
          //disarmedSplash();   
        }
      }
      WSclsRGB();
      cls();
      runLoop();
    }
  }
  if (doAction == 1) {
     restore_left_focus();
     explodeSplash();
  }
  else if (doAction == 2) {
     restore_left_focus();
     disarmedSplash();
  }
}