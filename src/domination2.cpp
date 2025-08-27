#include "domination2.h"
void domination_lora(){
  cls();
  redrawFull = true;
  setup_progressbar();
  inGame = true;
  gameSelector = 1;
  lora_inSync = false; // initiate lora status
  digitalWrite(BUTTONLEDGREEN, HIGH);
  digitalWrite(BUTTONLEDRED, HIGH);
  startListeningToNFC();
  //SETUP INITIAL TIME 
  int minutos = GAMEMINUTES-1;
  unsigned long battMillis;
  unsigned long iTime=millis(); //  initialTime in millisec 
  aTime = 0;
  redTime=0;
  greenTime=0;
  team=0;
  WSclsRGB(); 
  int largoTono = 50;
// 0 = neutral, 1 = green team, 2 = red team

  battMillis=millis();  // Fix screen flickering

  
  //Starting Game Code
  while(inGame)
  {
    runLoop();
    keypad.getKey();
    aTime=millis()- iTime;

     //Code for led blinking
    timeCalcVar=(millis()- iTime)%1000;
    if(timeCalcVar >= 0 && timeCalcVar <= 40)
    {
      if(team == 0) WSblueRGB();
      else if(team == 2) WSredRGB();  
      else if(team == 1) WSgreenRGB();
    }
    if(timeCalcVar >= 50 && timeCalcVar <= 100)
    {    
      WSclsRGB();
    }
    
    runLoop();
    
    // Sound!!! same as Destroy 
    if(timeCalcVar >= 0 && timeCalcVar <= 40 && soundEnable)bipp(300);
    if(timeCalcVar >= 245 && timeCalcVar <= 255 && minutos-aTime/60000<2 && soundEnable)bipp(largoTono);
    if(timeCalcVar >= 495 && timeCalcVar <= 510 && minutos-aTime/60000<4 && soundEnable)bipp(largoTono);
    if(timeCalcVar >= 745 && timeCalcVar <= 760 && minutos-aTime/60000<2 && soundEnable)bipp(largoTono);
    
    runLoop();
    
    //Battery check timer
    if(enableBattery and battMillis+20000<millis()){
      battMillis=millis();   
      batteryManager.quickCheck();
      redrawFull = true; 
    }
    
    //THE NEXT TO METHODS SHOW "GAME TIME" AND "CONTROLED ZONE TIME" IT SHOWS 2 AND 2 SEC EACH

      if (redrawFull) {
          lcd.setCursor(0,0);
          if(team == 0)lcd.print(F("====NEUTRAL ZONE===="));
          if(team == 1)lcd.print(F("=====GREEN ZONE====="));
          if(team == 2)lcd.print(F("======RED ZONE======"));
          lcd.setCursor(0,1);
          lcd.print(F("Red Time    |"));
          lcd.setCursor(0,2);
          lcd.print(F("Green Time  |"));
          lcd.setCursor(0,3);
          lcd.print(F("Game Time   |"));
          redrawFull = false;
      }

      if (lora_myOwnID == 1) {
          if (team == 2) {
                lcd.setCursor(14,1);
                printTimeDom(millis()-iZoneTime + redTime,false);
          } else {
                lcd.setCursor(14,1);
                printTeamTime(2);
          }
          if (team == 1) {
                lcd.setCursor(14,2);
                printTimeDom(millis()-iZoneTime + greenTime,false);
          } else {
                lcd.setCursor(14,2);
                printTeamTime(1);
          }
          lcd.setCursor(14,3);
          printTime(minutos, aTime, false);      
      } else {
          if (team == 2) {
              lcd.setCursor(14,1);
              lcd.print(F(" DOM"));
          } else {
              lcd.setCursor(14,1);
              lcd.print(F(" --- "));
          }
          if (team == 1) {
              lcd.setCursor(14,2);
              lcd.print(F(" DOM"));
          } else {
              lcd.setCursor(14,2);
              lcd.print(F(" --- "));
          }
          lcd.setCursor(14,3);
          printTime(minutos, aTime, false);      
      }

    //###########################CHECKS##################

    //Check If Game End
    if((minutos-aTime/60000==0 && 59-((aTime/1000)%60)==0) or (minutos-aTime/60000 >= 6000000.00))
    {
      runLoop();
      gameOver2();
    }

    //Check If IS neutral
    while((defuseando || cancelando) && team > 0)
    {
      cls();
      redrawFull = true;
      if(team>0) {
        lcd.setCursor(0,1);
        lcd.print(F("     DEFUZING...    "));        
      }
      lcd.setCursor(0,1);
      unsigned int percent=0;
      unsigned long xTime=millis(); //start disabling time
      runLoop();
      while(defuseando || cancelando)
      {
        keypad.getKey();
     
        timeCalcVar = (millis()- xTime)%1000;

        if( timeCalcVar >= 0 && timeCalcVar <= 20)
        {
          bipp(200);
        }
        if(timeCalcVar >= 480 && timeCalcVar <= 500)
        {
          bipp(200);
        }
        
        unsigned long seconds= millis() - xTime;
        percent = (seconds)/(ACTIVATESECONDS*10);
        drawPorcent(percent);
        WSwriteProcent (percent, team, action);
        runLoop();
        
        if(percent >= 100)
        {
          runLoop();
          loraCallRemote(10, 0); //capture to Neutral
          if(team==1){ 
            greenTime+=millis()-iZoneTime;
            iZoneTime=0; 

          }
          if(team==2){ 
            redTime+=millis()-iZoneTime;
            iZoneTime=0; 
          }
          team=0;
          break;
        }
      }
      cls();
    }

//Capturing red

    while(defuseando && team == 0 )
    {
      runLoop();
      cls();
      redrawFull = true;
      if(team==0) {
        lcd.setCursor(3,1);
        lcd.print(F("CAPTURING ZONE"));
      }
      lcd.setCursor(0,1);
      unsigned int percent=0;
      unsigned long xTime=millis(); //start disabling time
      while(defuseando)
      {
        keypad.getKey();
        runLoop();
        timeCalcVar = (millis()- xTime)%1000;

        if( timeCalcVar >= 0 && timeCalcVar <= 20)
        {
          bipp(200);//tone(tonepin,tonoAlarma1,200);
        }
        if(timeCalcVar >= 480 && timeCalcVar <= 500)
        {
          bipp(200);//tone(tonepin,tonoAlarma2,200);
        }

        unsigned long seconds= millis() - xTime;
        percent = (seconds)/(ACTIVATESECONDS*10);
        drawPorcent(percent);
        // WS handler
        WSwriteProcent (percent, team, action);
        runLoop();
        if(percent >= 100)
        {
          WSclsRGB();
          loraCallRemote(12,0);
          break;
        }
      }
      cls();
      WSclsRGB();
      runLoop();
    }

    //getting to green zone
    while(cancelando && team == 0 )
    {
      cls();
      redrawFull = true;
      if(team==0) {
        lcd.setCursor(3,1);
        lcd.print(F("CAPTURING ZONE"));
      }
      lcd.setCursor(0,1);
      unsigned int percent=0;
      unsigned long xTime=millis(); //start disabling time
      while(cancelando)
      {
         keypad.getKey();
         runLoop();
        timeCalcVar = (millis()- xTime)%1000;

        if( timeCalcVar >= 0 && timeCalcVar <= 20)
        {
          bipp(200);//tone(tonepin,tonoAlarma1,200);
        }
        if(timeCalcVar >= 480 && timeCalcVar <= 500)
        {
          bipp(200);//tone(tonepin,tonoAlarma2,200);
        }

        unsigned long seconds= millis() - xTime;
        percent = (seconds)/(ACTIVATESECONDS*10);
        drawPorcent(percent);
        runLoop();
        WSwriteProcent (percent, team, action);
        
        if(percent >= 100)
        {
          WSclsRGB();
          loraCallRemote(11,0);
          break;
        }
      }
      runLoop();
      cls();
      WSclsRGB(); 
    }
  }
  restore_left_focus();
}

void gameOver2(){
  if(team==1)greenTime+=millis()-iZoneTime;
  if(team==2)redTime+=millis()-iZoneTime;
  cls();
  if (lora_myOwnID > 1) {
    lora_syncDomResults(); //sync team time to master
  }
  else if(lora_myOwnID == 1) {
    //i'm the master - send results
    lora_sendResults();
  }
  redrawFull = true;
  lcd.setCursor(0,0);
  lcd.print(F("====Time is up!!===="));
  lcd.setCursor(0,1);
  //Write red time
    if (redTime > greenTime) {
      lcd.print(F("*Red time*   | "));
      WSredRGB();
    } else {
      lcd.print(F("Red time     | "));
    }
  printTimeDom(redTime,false);
  lcd.setCursor(0,2);
    if (greenTime > redTime) {
      lcd.print(F("*Green time* | ")); 
      WSgreenRGB();
    } else {
      lcd.print(F("Green time   | "));
    }
   printTimeDom(greenTime,false);
   lcd.setCursor(9,3);
   lcd.print(F("[5]Continue"));
   DominationSaveHistory(redTime, greenTime);
   while(1){
      var = keypad.getKey();
      runLoop();
      if(var == '5' ){
          Bbipp(50);
          if (lora_send(203, 0, 0)) {
              software_Reset();
              break;
          }
      }  
   }
}