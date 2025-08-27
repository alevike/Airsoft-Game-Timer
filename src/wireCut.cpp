#include "wireCut.h"
void wireCut(){
  digitalWrite(BUTTONLEDGREEN, LOW);
  digitalWrite(BUTTONLEDRED, HIGH);
  cls();
  //SETUP INITIAL TIME 
  int minutos = GAMEMINUTES-1;
  unsigned long iTime=millis(); //  initialTime in millisec 
  aTime = 0;
  unsigned long battMillis;
  battMillis=millis();
  //var='o';
  team = 0;
  tCutWire1Status = 0;
  tCutWire2Status = 0;
  //Starting Game Code
  if (autoArm == true) { destroy2(); } // jump to armed state
  while(1){  // this is the important code, is a little messy but works good.
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

    lcd.setCursor(0,0);
    lcd.print("GAME TIME ");
       
    aTime=millis()- iTime;
    lcd.setCursor(3,1);

    //PRINT TIME ON LCD

    printTime(minutos, aTime, true);

    //###########################CHECKINGS##################
    //Check battery
    if(enableBattery and battMillis+20000<millis()){
      battMillis=millis();   
      batteryManager.quickCheck(); 
    }
    //Check If Game End
    //if(minutos-aTime/60000==0 && 59-((aTime/1000)%60)==0)
    if((minutos-aTime/60000==0 && 59-((aTime/1000)%60)==0) or (minutos-aTime/60000 >= 6000000.00))
    {
      lcd.clear();
      while(1){
        runLoop();
        disarmedSplash();
      }
    }
    //Serial.println(keypad.getKey());
    //USED IN PASSWORD GAME 
    if('#' == keypad.getKey() && passwordEnable){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("ARMING THE BOMB"));
      Bbipp(500);
      delay(500);//a little delay to think in the password
      lcd.clear();
      lcd.setCursor(3, 0);
      lcd.print(F("Enter Code"));

      setCode();// we need to set the comparation variable first it writes on enteredText[]

      //then compare :D

      if(comparePassword()){
        destroy2();
      }        
      lcd.clear();
      lcd.setCursor(3,0);
      lcd.print(F("Code Error!"));
      Bbipp(200); //tone(tonepin,errorTone,200);
      delay(500);
      cls();
    }
  }
}

void destroy2(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F(" BOMB ARMED"));
  delay(1000);
  int minutos=BOMBMINUTES-1;
  byte defusePort = map (defuseWire, 1, 4, 46, 49);
  byte detonationPort = map (detonationWire, 1, 4, 46, 49);
  byte tCut1Port = map (tCutWire1, 1, 4, 46, 49);
  byte tCut2Port = map (tCutWire2, 1, 4, 46, 49);
  unsigned long iTime=millis();
  unsigned long battMillis = millis();
  unsigned long aTime;
  int largoTono = 50;
  runLoop();
  //MAIN LOOP
  while(1){
    runLoop();
    //If you fail disarm. 
    if(endGame){
      explodeSplash();
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
    //Check  battery
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

    lcd.setCursor(0,0);
    lcd.print(F(" DETONATION IN "));

    lcd.setCursor(3,1);

    ////////HERE ARE THE TWO OPTIONS THAT ENDS THE GAME///////////

    ////TIME PASED AWAY AND THE BOMB EXPLODES
    if(minutos-aTime/60000==0 && 59-((aTime/1000)%60)==0)// Check if game ends
    {
      explodeSplash();
      start = true;
    }
    //print time
    printTime(minutos, aTime, true);
    

    if (digitalRead(detonationPort) == HIGH) {
      endGame = true;
      explodeSplash();
   }
   if (digitalRead(tCut1Port) == HIGH and tCutWire1Status == 0) {
      tCutWire1Status = 1;
      minutos = minutos/2;
      Bbipp(500);
      //halfTimeWS();
      
   }
   if (digitalRead(tCut2Port) == HIGH and tCutWire2Status == 0) {
      tCutWire2Status = 1;
      minutos = minutos/2;
      Bbipp(500);
      //halfTimeWS();
      
   }
   if (digitalRead(defusePort) == HIGH) {
      bipp(500);
      endGame = true;
      disarmedSplash();
   }


    
    //// SECOND OPTION: YOU PRESS DISARMING BUTTON  

    //IF IS A PASSWORD GAME 

    if('#' == keypad.getKey() && passwordEnable){

      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(F("DISARMING BOMB"));
      Bbipp(500);
      delay(500);//a little delay to think in the password

      lcd.clear();
      lcd.setCursor(3,0);
      lcd.print(F("Enter Code"));

      setCode();// we need to set the compare variable first

      //then compare :D

      if(comparePassword()){
        disarmedSplash();
      }        
      lcd.clear();
      lcd.setCursor(3,0);
      lcd.print(F("Code Error!"));
      bipp(200); //tone(tonepin,errorTone,200);
      delay(500);
      cls();
    }
  }
}