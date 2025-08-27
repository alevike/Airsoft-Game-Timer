#include "arduinoGlue.h"
#include "LoRa_domination.h"

bool detectLoRaModule() {
  // Attempt to get the configuration from the LoRa module
  ResponseStructContainer c = e32ttl.getConfiguration();

  // Check if the response is valid
  if (c.status.code == 1) { // Status code 1 indicates success
      Configuration configuration = *(Configuration*)c.data;
      c.close(); // Free memory
      lcd.setCursor(0, 2);
      lcd.print(F("LoRa "));
      lcd.print(F("- Ch "));
      lcd.print(configuration.CHAN, DEC);
      lcd.setCursor(16, 2);
      lcd.print(F("| OK"));
      return true;
  } else {
      c.close();
      lcd.setCursor(0, 2);
      lcd.print(F("LoRa "));
      lcd.print(F("N/A!!"));
      lcd.setCursor(16, 2);
      lcd.print(F("|Err"));
      return false;
  }
}


void restoreLoraConfig() {
  ResponseStructContainer c;
  c = e32ttl.getConfiguration();
  Configuration configuration = *(Configuration*) c.data;
  cls();
  lcd.setCursor(0,0);
  lcd.print("Restoring Config....");
  delay (500);
  configuration.HEAD = 0xC0;
  configuration.ADDL = 0x01;
  configuration.ADDH = 0x00;
  configuration.CHAN = 0x02;
  configuration.OPTION.transmissionPower = POWER_20;
  configuration.OPTION.fixedTransmission = FT_FIXED_TRANSMISSION;
  configuration.SPED.airDataRate = AIR_DATA_RATE_011_48;
  ResponseStatus rs = e32ttl.setConfiguration(configuration, WRITE_CFG_PWR_DWN_SAVE);
  lcd.setCursor(0,1);
  lcd.print("H:");lcd.print(configuration.HEAD, DEC);lcd.print("|ADDH");lcd.print(configuration.ADDH, BIN);lcd.print("|ADDL:");lcd.print(configuration.ADDL, BIN);
  lcd.setCursor(0,2);
  lcd.setCursor(0,3);
  lcd.print(rs.getResponseDescription());
  c.close();
  delay(2000);
  ParametersPrint();
}


void ParametersPrint() {
  //Serial.println("----------------------------------------");
  ResponseStructContainer c;
  c = e32ttl.getConfiguration();
  Configuration configuration = *(Configuration*) c.data;
  cls();
  lcd.setCursor(0,0);
  lcd.print(F("HEAD:"));  
  //Serial.print(configuration.HEAD, BIN);
  //Theoretical default: DEC: 192 HEX: C0
  //AddH: 0; AddL: 1
  //Chan 1; Freq 412Mz
  //P: 0 -> 20dBm
  lcd.print(" "); lcd.print(configuration.HEAD, DEC);lcd.print(" ");lcd.print(configuration.HEAD, HEX); lcd.print(F("  ID-")); lcd.print(lora_myOwnID);
  lcd.setCursor(0,1);
  lcd.print(F("AddH:")); lcd.print(configuration.ADDH, BIN);
  lcd.print(F("| AddL:")); lcd.print(configuration.ADDL, BIN);
  lcd.setCursor(0,2);
  lcd.print(F("Chan:")); lcd.print(configuration.ADDL, BIN); lcd.print(F(" Freq: ")); lcd.print(configuration.getChannelDescription());
  lcd.setCursor(0,3);
  lcd.print(F("P:")); lcd.print(configuration.OPTION.transmissionPower, BIN); lcd.print("->"); lcd.print(configuration.OPTION.getTransmissionPowerDescription());
  if (configuration.SPED.airDataRate != AIR_DATA_RATE_011_48) { 
    configuration.SPED.airDataRate = AIR_DATA_RATE_011_48;
    e32ttl.setConfiguration(configuration);
  }
  c.close();
/*
  Serial.println(F(" "));
  Serial.print(F("AddH : "));  Serial.println(configuration.ADDH, BIN);
  Serial.print(F("AddL : "));  Serial.println(configuration.ADDL, BIN);
  Serial.print(F("Chan : "));  Serial.print(configuration.CHAN, DEC); Serial.print(" -> "); Serial.println(configuration.getChannelDescription());
  Serial.println(F(" "));
  Serial.print(F("SpeedParityBit     : "));  Serial.print(configuration.SPED.uartParity, BIN);Serial.print(" -> "); Serial.println(configuration.SPED.getUARTParityDescription());
  Serial.print(F("SpeedUARTDatte  : "));  Serial.print(configuration.SPED.uartBaudRate, BIN);Serial.print(" -> "); Serial.println(configuration.SPED.getUARTBaudRate());
  Serial.print(F("SpeedAirDataRate   : "));  Serial.print(configuration.SPED.airDataRate, BIN);Serial.print(" -> "); Serial.println(configuration.SPED.getAirDataRate());

  Serial.print(F("OptionTrans        : "));  Serial.print(configuration.OPTION.fixedTransmission, BIN);Serial.print(" -> "); Serial.println(configuration.OPTION.getFixedTransmissionDescription());
  Serial.print(F("OptionPullup       : "));  Serial.print(configuration.OPTION.ioDriveMode, BIN);Serial.print(" -> "); Serial.println(configuration.OPTION.getIODroveModeDescription());
  Serial.print(F("OptionWakeup       : "));  Serial.print(configuration.OPTION.wirelessWakeupTime, BIN);Serial.print(" -> "); Serial.println(configuration.OPTION.getWirelessWakeUPTimeDescription());
  Serial.print(F("OptionFEC          : "));  Serial.print(configuration.OPTION.fec, BIN);Serial.print(" -> "); Serial.println(configuration.OPTION.getFECDescription());
  Serial.print(F("OptionPower        : "));  Serial.print(configuration.OPTION.transmissionPower, BIN);Serial.print(" -> "); Serial.println(configuration.OPTION.getTransmissionPowerDescription());

  Serial.println("----------------------------------------");
*/
  byte key = keypad.getKey();
  while(key == NO_KEY) {
    key = keypad.getKey();
  }
  
}

  /* Notes: 
   * struct Message {
        byte myID; = sender ID
        byte remoteID; = destination ID
        byte actionRequested; 
        byte actionValue;
    } message;

    
   */
   /*
    * Lora action schema
    * 0     - send pairing request
    * 10    - Domination - capture to Neutral - loraCallRemote(10, 0);
    * 11    - Domination - capture to Green - loraCallRemote(11,0);
    * 12    - Domination - capture to Red - loraCallRemote(12,0);
    * 13    - Domination - Jumpstart - loraCallRemote(13,0);
    * 14    - Domination - Stop game and Send win team loraCallRemote(14,0);
    *  
    *  
    *  
    * 21    - Domination - sync response - capture to Green success
    * 22    - Domination - sync response - capture to Red success
    * 
    * 
    * 50    - Domination - send game time 
    * 51    - Domination - send arm time
    * 
    * 
    * 
    * 200   - pairing offer/response from a master
    * 201   - pairing
    * 202   - reset pair remotely 
    * 203   - softreset
    * 
    * 35000   - ACK message value
    */


//lora_send(byte actionToSend, byte actionValueToSend, byte destinationID)


void loraCallRemote(byte actionToSend, byte actionValueToSend) {
  if (lora_myOwnID != 100) {
    if (customCharset != 2)setBigChars();
    loraTimeNow = millis()/1000;
    byte prevSec = 0;
    byte sec = 5;
    lora_inSync = true; // nfc set insinc
      if (lora_send(actionToSend, actionValueToSend, 0)) {
          //ResponseStatus rs = e32ttl.sendBroadcastFixedMessage(0x02,&message, sizeof(Message));
          //rs.getResponseDescription();
          // Wait for response in special cases
          switch (actionToSend) {
            case 10:
              lora_inSync = false; // instant neutral -= we have to stop symc mode..
              break;
            case 11:
                    cls();
                    lcd.setCursor(5,0);
                    lcd.print(F("Syncing..."));
                    lcd.setCursor(1,1);
                    lcd.print(F("Green    |"));
                    lcd.setCursor(1,2);
                    lcd.print(F("Team     |"));
                    lcd.setCursor(0,3);
                    lcd.print(F("Waiting for remote.."));     
                    TimeUp = false;
                    while(!TimeUp) {                    
                        checkCard();
                        if (millis()/1000 >= loraWaitTime + loraTimeNow) {
                              // time is up and sync failed
                              cls();
                              lora_inSync = false;
                              break;
                        } else {
                              sec = millis()/1000-loraTimeNow;
                              if (sec != prevSec) {   //fix flickering
                                  prevSec = sec;
                                  big.writeint(1,12, loraWaitTime - sec, 2, true);
                                  digitalWrite(BUTTONLEDGREEN, !digitalRead(BUTTONLEDGREEN));
                              }
                              if (e32ttl.available() >= sizeof(Message)){
                                  ResponseStructContainer rsc = e32ttl.receiveMessage(sizeof(Message));
                                  struct Message message = *(Message*) rsc.data;
                                  if (message.actionRequested == 21) {   // remote side button pusshed
                                        lora_sendACK(message.actionRequested);
                                        if(team==1){ 
                                            greenTime+=millis()-iZoneTime;
                                            iZoneTime=0; 
                                        }
                                        if(team==2){ 
                                            redTime+=millis()-iZoneTime;
                                            iZoneTime=0; 
                                        }
                                        iZoneTime=millis();
                                        team=1;
                                        TimeUp = true;
                                        lora_inSync = false;
                                        cls();
                                        lcd.print(F("===================="));
                                        lcd.setCursor(3,1);
                                        lcd.print(F("Sync complete!"));
                                        lcd.setCursor(0,2);
                                        lcd.print(F("===================="));
                                        delay(1000);
                                        cls();
                                        break;
                                  }
                              }
                      }
                  }
                  break;
                        
            case 12:
                    cls();
                    lcd.setCursor(5,0);
                    lcd.print(F("Syncing..."));
                    lcd.setCursor(1,1);
                    lcd.print(F("Red      |"));
                    lcd.setCursor(1,2);
                    lcd.print(F("Team     |"));
                    lcd.setCursor(0,3);
                    lcd.print(F("Waiting for remote.."));     
                    TimeUp = false;
                    while(!TimeUp) {
                        checkCard();
                        if (millis()/1000 >= loraWaitTime + loraTimeNow) {
                              // time is up and sync failed
                              cls();
                              lora_inSync = false;
                              break;
                        } else {
                              sec = millis()/1000-loraTimeNow;
                              if (sec != prevSec) {   //fix flickering
                                  prevSec = sec;
                                  big.writeint(1,12, loraWaitTime - sec, 2, true);
                                  digitalWrite(BUTTONLEDRED, !digitalRead(BUTTONLEDRED));
                              }
                              if (e32ttl.available() >= sizeof(Message)){
                                  ResponseStructContainer rsc = e32ttl.receiveMessage(sizeof(Message));
                                  struct Message message = *(Message*) rsc.data;
                                  if (message.actionRequested == 22) {   // remote side button pusshed
                                        lora_sendACK(message.actionRequested);
                                        if(team==1){ 
                                            greenTime+=millis()-iZoneTime;
                                            iZoneTime=0; 
                                        }
                                        if(team==2){ 
                                            redTime+=millis()-iZoneTime;
                                            iZoneTime=0; 
                                        }
                                        iZoneTime=millis();
                                        team=2;
                                        TimeUp = true;
                                        lora_inSync = false;
                                        cls();
                                        lcd.print(F("===================="));
                                        lcd.setCursor(3,1);
                                        lcd.print(F("Sync complete!"));
                                        lcd.setCursor(0,2);
                                        lcd.print(F("===================="));
                                        delay(1000);
                                        cls();
                                      break;
                                  }
                              }
                       }
                  }
                  break;
        }
      } else {
        cls();
        lcd.print (F("RF send error"));
        lcd.setCursor(2,1);
        lcd.print(F("Error - 101"));  
        lora_inSync = false;           
      }
   }
} 
 

void loraSyncCapture(byte receivedActionID) {
  lora_sendACK(receivedActionID);
  loraTimeNow = millis()/1000;
  TimeUp = false;
  byte prevSec = 0;
  byte sec = 5;
  lora_inSync = true;
  greenCardDetected = false;
  redCardDetected = false;
  switch (receivedActionID) {
    case 11:
        
        /*    12345678901234567890
         *    Capturing |
         *    Green     |
         *    /         |
         *    /         |
         */
        cls();
        lcd.setCursor(5,0);
        lcd.print(F("Syncing..."));
        lcd.setCursor(1,1);
        lcd.print(F("Green    |"));
        lcd.setCursor(1,2);
        lcd.print(F("Team     |"));
        lcd.setCursor(0,3);
        lcd.print(F("Press your button..."));
        if (customCharset != 2)setBigChars();
        while(!TimeUp) {
          checkCard();
          var = keypad.getKey();
          if (millis()/1000 >= loraWaitTime + loraTimeNow) {
            // time is up
            redCardDetected = false;
            lora_inSync = false;
            cls();
            break;
          } else {
            sec = millis()/1000-loraTimeNow;
            if (sec != prevSec) {   //fix flickering
                prevSec = sec;
                big.writeint(1,12, loraWaitTime - sec, 2, true);
                digitalWrite(BUTTONLEDGREEN, !digitalRead(BUTTONLEDGREEN));
            }
            if (var == BT_CANCEL or greenCardDetected == true) {   //Green button ?
                //Lora Send success capture
                if (lora_send(21, 0, 0)) {
                      //e32ttl.sendBroadcastFixedMessage(0x02,&message, sizeof(Message));
                      cls();
                      //lcd.print(rs.getResponseDescription());
                      if(team==1){ 
                            greenTime+=millis()-iZoneTime;
                            iZoneTime=0; 
                      }
                      if(team==2){ 
                            redTime+=millis()-iZoneTime;
                            iZoneTime=0; 
                      }
                      iZoneTime=millis();
                      team=1;
                      cls();
                      lcd.print(F("====================")); lcd.setCursor(3,1); lcd.print(F("Sync complete!")); lcd.setCursor(0,2); lcd.print(F("===================="));
                      lora_inSync = false;
                      greenCardDetected = false;
                      TimeUp = true;
                      delay(1000);
                      cls();
                      break;
                } else {
                     lora_inSync = false;
                     cls();
                     lcd.print(F("Sync ERROR - try again"));
                     delay(1000);
                     break;
                }
            }
          }          
        }             
      break;
    case 12:
    //Capturing red team
        cls();
        lcd.setCursor(5,0);
        lcd.print(F("Syncing..."));
        lcd.setCursor(1,1);
        lcd.print(F("RED      |"));
        lcd.setCursor(1,2);
        lcd.print(F("Team     |"));
        lcd.setCursor(0,3);
        lcd.print(F("Press your button..."));
        if (customCharset != 2)setBigChars();
        while(!TimeUp) {
          checkCard();
          var = keypad.getKey();
          if (millis()/1000 >= loraWaitTime + loraTimeNow) {
            // time is up
            cls();
            lora_inSync = false;
            redCardDetected = false;
            break;
          } else {
            sec = millis()/1000-loraTimeNow;
            if (sec != prevSec) {   //fix flickering
                prevSec = sec;
                big.writeint(1,12, loraWaitTime - sec, 2, true);
                digitalWrite(BUTTONLEDRED, !digitalRead(BUTTONLEDRED));
            }
            if (var == BT_SEL or redCardDetected == true) {   //Green button ?
                //Lora Send success capture
                if (lora_send(22, 0, 0)) {
                    //e32ttl.sendBroadcastFixedMessage(0x02,&message, sizeof(Message));
                    //lcd.print(rs.getResponseDescription());
                    if(team==1){ 
                          greenTime+=millis()-iZoneTime;
                          iZoneTime=0; 
                    }
                    if(team==2){ 
                          redTime+=millis()-iZoneTime;
                          iZoneTime=0; 
                    }
                    iZoneTime=millis();
                    team=2;
                    cls();
                    lcd.print(F("===================="));
                    lcd.setCursor(3,1);
                    lcd.print(F("Sync complete!"));
                    lcd.setCursor(0,2);
                    lcd.print(F("===================="));
                    redCardDetected = false;
                    lora_inSync = false;
                    TimeUp = true;
                    delay(1000);
                    cls();
                    break;
                } else {
                    cls();
                    lora_inSync = false;
                    lcd.print(F("Sync ERROR - try again"));
                    delay(1000);
                    break;
                }
            }
          }          
        }             
      break;
    case 13:
      break;
      
  }
}
   

void loraReceiveBroadcast() {
  if (e32ttl.available() >= sizeof(Message)){
    ResponseStructContainer rsc = e32ttl.receiveMessage(sizeof(Message));
    struct Message message = *(Message*) rsc.data;
    switch (message.actionRequested) {
      case 0:
        if (message.myID == 100 and lora_myOwnID == 1) {
          loraMasterHandlePair();
        }
        break;
      case 10: // Domination - capture to Neutral
                            greenCardDetected = false;
                            redCardDetected = false;
                            lora_sendACK(message.actionRequested);
                            if(team==1){ 
                                greenTime+=millis()-iZoneTime;
                                iZoneTime=0; 
                            }
                            if(team==2){ 
                                redTime+=millis()-iZoneTime;
                                iZoneTime=0; 
                            }
                            team=0;
                            lora_inSync = false;
                            lcd.setCursor(0,0);
                            lcd.print(F("====NEUTRAL ZONE===="));
                        break;
      case 11: // Domination - capture to Green                         
                          loraSyncCapture(11);
                       break;
      case 12: //Domination - capture to RED
                          loraSyncCapture(12);
                   break;
      case 13:
            lora_sendACK(message.actionRequested);
            sdStatus=false;
            saStatus=false;
            doStatus=false;
            doStatus_lora = true;
            wcStatus=false;
            bpStatus=false;
            LoRa_domination();  //Start Domination game
            break;
      case 14:
            break;
      case 15:
            break;
      case 16:
            break;
      case 17:
            break;
      case 18:
            break;
      case 21:
            lcd.setCursor(0,3);
            lcd.print("Case 21 - Error!");
            delay (2000);
            break;
      case 22:
            lcd.setCursor(0,3);
            lcd.print("Case 22 - Error");
            delay (2000);
            break;
      case 50: // receive game time from remote
            GAMEMINUTES = message.actionValue;
            lora_sendACK(message.actionRequested);
            break;
      case 51:  // receive arm time from remote
            ACTIVATESECONDS = message.actionValue;
            lora_sendACK(message.actionRequested);
            break;
      case 202:  //Pairing reset broadcast 
            if (message.actionValue == 0) {
                lora_sendACK(202);
                cls();
                lora_myOwnID = 100;
                EEPROM.write(401, lora_myOwnID); 
                loraLastAssignedID = 0;
                EEPROM.write(402, loraLastAssignedID);
                lcd.print(F("===================="));
                lcd.setCursor(2,1);
                lcd.print(F("Pair reset done!"));
                lcd.setCursor(0,2);
                lcd.print(F("===================="));        
                delay(1000);
                break;
            }
            break;
      case 203:
          if (message.actionValue == 0) {
              lora_sendACK(message.actionRequested);
              software_Reset();
          }
          break;
    }
    free(rsc.data);
  }
}


void lora_sendACK(byte actionToACK) {
  message.actionRequested = actionToACK;
  message.actionValue = 30000;
  e32ttl.sendBroadcastFixedMessage(0x02,&message, sizeof(Message));
}


boolean lora_send(byte actionToSend, int actionValueToSend, byte destinationID) {
  boolean ack = false;
  unsigned long ack_millis;
  byte maxRetry = 3; // retry time 
  byte retry = 1;    // retry counter

  //Prepare data
  message.myID = lora_myOwnID;
  message.remoteID = destinationID;
  message.actionRequested = actionToSend;
  message.actionValue = actionValueToSend;
  message.greenTime = greenTime;
  message.redTime = redTime;
 //Broadcast message
  e32ttl.sendBroadcastFixedMessage(0x02,&message, sizeof(Message));
  ack_millis = millis();
  while (!ack) {
    if (e32ttl.available() >= sizeof(Message)){
      ResponseStructContainer rsc = e32ttl.receiveMessage(sizeof(Message));
      struct Message message = *(Message*) rsc.data;
      if (message.actionRequested == actionToSend and message.actionValue == 30000){ 
        lcd.setCursor(17,0); lcd.print(("ACK"));
        ack = true;
        return true;
        break; 
      }  
    }
    
    if (ack_millis+1000 < millis() and retry <= maxRetry) {
      retry++;  // increase retry nr
      ack_millis = millis();
      e32ttl.sendBroadcastFixedMessage(0x02,&message, sizeof(Message));
      lcd.setCursor(0,3); lcd.print(retry);
      delay(100);
    } else if (ack_millis+1000 < millis() and retry > maxRetry) { 
      cls();
      lcd.print(F("===================="));
      lcd.setCursor(2,1);
      lcd.print(F("Error: RF failed!"));
      lcd.setCursor(2,2);
      lcd.print(F("Repeat last action!"));
      lcd.setCursor(0,3);
      lcd.print(F("===================="));
      Bbipp(1000);
      return false;
      break;
      
    }
  }
  return false; // Ensure a boolean value is returned in all cases
} 

void lora_syncDomResults() {
  boolean synced = false;
  lcd.print(F("===================="));
  lcd.setCursor(2,1);
  lcd.print(F("Waiting for master"));
  lcd.setCursor(0,2);
  lcd.print(F("===================="));
  unsigned long startTime = millis(); // Start timeout timer
  while (!synced) {
    if (e32ttl.available() >= sizeof(Message)) {
      ResponseStructContainer rsc = e32ttl.receiveMessage(sizeof(Message));
      if (rsc.data != nullptr) { // Ensure data is valid
        struct Message message = *(Message*) rsc.data;
        greenTime = message.greenTime;
        redTime = message.redTime;
        synced = true;
      }
    }

    // Timeout after 10 seconds
    /* if (millis() - startTime > 10000) {
      lcd.setCursor(2,3);
      lcd.print(F("Timeout!"));
      delay(1000);
      break;
    } */
  }
}

void lora_sendResults() {
  message.actionRequested = 14;
  message.greenTime = greenTime;
  message.redTime = redTime;
  e32ttl.sendBroadcastFixedMessage(0x02,&message, sizeof(Message));
}

void loraMasterHandlePair() {
  message.myID = lora_myOwnID;
  loraLastAssignedID = loraLastAssignedID + 1;
  message.remoteID = loraLastAssignedID;
  EEPROM.write(402, loraLastAssignedID); 
  message.actionRequested = 200;
  e32ttl.sendBroadcastFixedMessage(0x02,&message, sizeof(Message));
  //ResponseStatus rs = e32ttl.sendBroadcastFixedMessage(0x02,&message, sizeof(Message));
  //lcd.print(rs.getResponseDescription());
  //delay(500);
  //byte key = keypad.getKey();
  //while(key == NO_KEY) {
  //        key = keypad.getKey();
  // } 
  
  //cls();
}

void loraStartPair() {
  //Sending a broadcast message
  if (lora_myOwnID == 100) {
    message.myID = lora_myOwnID;
    message.remoteID = 0;
    message.actionRequested = 0; // 0 = pair request
    e32ttl.sendBroadcastFixedMessage(0x02,&message, sizeof(Message));
    delay(1000);
    if (e32ttl.available() >= sizeof(Message)){
      ResponseStructContainer rsc = e32ttl.receiveMessage(sizeof(Message));
      struct Message message = *(Message*) rsc.data;
      if (message.myID == 1 and message.actionRequested == 200) {       // we've got a response from a defined master - proceeding received data integration
        lora_myOwnID = message.remoteID;                               // remote id sent from a master will be our new device id - offered by master/counted by the master - master id will be always 1
        EEPROM.write(401, lora_myOwnID);
      }

      //Pair info
      cls();
      lcd.print(F("RX-from a master |OK"));
      lcd.print(F("Our new slave id |")); lcd.print(lora_myOwnID);
      free(rsc.data);
      byte key = keypad.getKey();
      while(key == NO_KEY) {
            key = keypad.getKey();
      } 
    } else {
      //no response received from remote side in the wait time - we are the masters
      cls();
      lcd.print(F("No master found...."));
      lcd.setCursor(0,1);
      lcd.print(F("We are a Master!"));
      lcd.setCursor(0,3);
      lcd.print(F("Press any key..."));
      lora_myOwnID = 1;
      loraLastAssignedID = 1;
      EEPROM.write(401, lora_myOwnID); 
      EEPROM.write(402, loraLastAssignedID);   
      byte key = keypad.getKey();
      while(key == NO_KEY) {
            key = keypad.getKey();
      }
    } 
  } else {
    cls();
      lcd.print(F("Already paired"));
      lcd.setCursor(0,1);
      lcd.print(F("Device ID: ")); lcd.print(lora_myOwnID); if(lora_myOwnID == 1) {lcd.print(F("-Master"));} else {lcd.print(F("-Slave"));}
      lcd.setCursor(0,2);
      byte key = keypad.getKey();
      while(key == NO_KEY) {
            key = keypad.getKey();
      }
  }
}


void loraRestPair() {
  restoreLoraConfig();
  cls();
  lora_myOwnID = 100;
  EEPROM.write(401, lora_myOwnID); 
  loraLastAssignedID = 0;
  EEPROM.write(402, loraLastAssignedID);
  lcd.print(F("--------------------"));
  lcd.setCursor(2,1);
  lcd.print(F("Pair reset done!"));
  lcd.setCursor(0,2);
  lcd.print(F("--------------------"));
  lora_send(202, 0, 0);
  //message.actionRequested = 202;
  //e32ttl.sendBroadcastFixedMessage(0x02,&message, sizeof(Message));
  delay(1000);
}