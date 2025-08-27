#include "nfc_utils.h"


void startListeningToNFC() {
  // Reset our IRQ indicators
  irqPrev = irqCurr = HIGH;

  //lcd.println("Waiting for an ISO14443A Card ...");
  nfc.startPassiveTargetIDDetection(PN532_MIFARE_ISO14443A);
}


void handleCardDetected() {
  uint8_t success = false;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
/*
 * [140] - NFC CARD 1 ID - GREEN TEAM CARD 1 
 * [150] - NFC CARD 2 ID - GREEN TEAM CARD 2
 * [160] - NFC CARD 3 ID - GREEN TEAM CARD 3
 * [170] - NFC CARD 4 ID - GREEN TEAM CARD 4
 * [180] - NFC CARD 5 ID - GREEN TEAM CARD 5
 * 
 * [190] - NFC CARD 6 ID - RED TEAM CARD 1
 * [200] - NFC CARD 7 ID - RED TEAM CARD 2
 * [210] - NFC CARD 8 ID - RED TEAM CARD 3
 * [220] - NFC CARD 9 ID - RED TEAM CARD 4
 * [230 - 239] NFC CARD 10 ID - RED TEAM CARD 5
 * 
 */

  uint32_t Card;
  byte teamID = 100;

  // read the NFC tag's info
  success = nfc.readDetectedPassiveTargetID(uid, &uidLength);

  if (success) {
    if (uidLength == 4)
    {
      //have a Mifare Classic card ...
      uint32_t cardid = uid[0];
      cardid <<= 8;
      cardid |= uid[1];
      cardid <<= 8;
      cardid |= uid[2];
      cardid <<= 8;
      cardid |= uid[3];

      for (int i = 0; i <= 9; i++) {
            if (EEPROM.get(eepromAddresses[i], Card) == cardid)teamID = i;
          }

      /* if domination is runnig */
      if (doStatus == true) {  
          if(team==1){ 
            greenTime+=millis()-iZoneTime;
            iZoneTime=0; 
          }
          if(team==2){ 
            redTime+=millis()-iZoneTime;
            iZoneTime=0; 
          }
          team=0;
          if (teamID < 5) {   // Green card detected
            lcd.setCursor(0,0);
            lcd.print(F("#GREEN CARD CAPTURE#"));
            team=1;
            iZoneTime=millis();
          } 
          else if (teamID >= 5 and teamID < 10) {  //Red card detected
            lcd.setCursor(0,0);
            lcd.print(F("##RED CARD CAPTURE##"));
            team=2;
            iZoneTime=millis();
          }
          //delay(1000);
          Bbipp(1000);
          redrawFull = true;
      }

      /* If sabotage is runnnig */
      else if (saStatus == true) {
        if (team == 0 and teamID >= 5 and teamID < 10) {  //arm and use only red cards
          team = 2;
          destroySabotage();
        }
        else if (team == 2 and teamID < 5) { // disarm and use only green crads
          team = 0;
          sabotage();
        }
      }

      /* if Search and destroy is running*/
      else if (sdStatus == true) {
        if (team == 0 and teamID >= 5 and teamID < 10) {  //arm and use only red cards
          team = 2;
          destroy();
        }
        else if (team == 2 and teamID < 5) { // disarm and use only green crads
          disarmedSplash();
        }
        
      }
      /* if Cut the wire is running */
      else if (wcStatus == true) {
        
      }
      /* if LoraDomination is running */
      else if (doStatus_lora == true) {
       // if(team==1){ 
       //     greenTime+=millis()-iZoneTime;
       //     iZoneTime=0; 
       //   }
       //   if(team==2){ 
       //     redTime+=millis()-iZoneTime;
       //     iZoneTime=0; 
       //   }
          if (teamID < 5) {   // Green card detected
            greenCardDetected = true;
            if (!lora_inSync) {
              loraCallRemote(11, 0);
            }
            //lcd.setCursor(0,0);
            //lcd.print(F("#GREEN CARD CAPTURE#"));
            //team=1;
            //iZoneTime=millis();
          } 
          else if (teamID >= 5 and teamID < 10) {  //Red card detected
            redCardDetected = true;
            if (!lora_inSync) {
                loraCallRemote(12, 0);
            }
            //lcd.setCursor(0,0);
            //lcd.print(F("##RED CARD CAPTURE##"));
            //team=2;
            //iZoneTime=millis();
          }
          //delay(1000);
          Bbipp(100);
          cls();
          redrawFull = true;
      }
    }
    // lcd.println("");

    timeLastCardRead = millis();
  }

  // The reader will be enabled again after DELAY_BETWEEN_CARDS ms will pass.
  readerDisabled = true;
}


void checkCard() {
  if (nfcEnabled) {
      if (readerDisabled) {
          if (millis() - timeLastCardRead > DELAY_BETWEEN_CARDS) {
              readerDisabled = false;
              startListeningToNFC();
          }
      } else {
          irqCurr = digitalRead(PN532_IRQ);

          // When the IRQ is pulled low - the reader has got something for us.
          if (irqCurr == LOW && irqPrev == HIGH) {
              lcd.setCursor(19,0);
              lcd.print("*");
              
              handleCardDetected();
              
          }
          irqPrev = irqCurr;
    }
  }
}


void(* resetFunc) (void) = 0;


void NFCCardSetupWizard() {
  uint32_t cardid1;
  uint32_t Card;

  uint8_t success_1;
  uint8_t uid_1[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength_1;          // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
/*
 * [140] - NFC CARD 1 ID - GREEN TEAM CARD 1 
 * [150] - NFC CARD 2 ID - GREEN TEAM CARD 2
 * [160] - NFC CARD 3 ID - GREEN TEAM CARD 3
 * [170] - NFC CARD 4 ID - GREEN TEAM CARD 4
 * [180] - NFC CARD 5 ID - GREEN TEAM CARD 5
 * 
 * [190] - NFC CARD 6 ID - RED TEAM CARD 1
 * [200] - NFC CARD 7 ID - RED TEAM CARD 2
 * [210] - NFC CARD 8 ID - RED TEAM CARD 3
 * [220] - NFC CARD 9 ID - RED TEAM CARD 4
 * [230 - 239] NFC CARD 10 ID - RED TEAM CARD 5
 * 
 */
  
  byte eepromAddresses[] = {140, 150, 160, 170, 180, 190, 200, 210, 220, 230};
  boolean save = false;

  for (int i = 0; i < 10; i++) {
    cls();
    lcd.setCursor(0, 0);
    if (i <= 4) {
      lcd.print(F("Green Team - Card "));
      lcd.print(i + 1);
      lcd.setCursor(0, 1);
      lcd.print(F("ID:"));
      EEPROM.get(eepromAddresses[i], Card);
      if (Card == 4294967295) {
        lcd.print(F("None"));
      } else {
        lcd.print(Card);
      }

    } else {
      lcd.print(F("Red Team - Card "));
      lcd.print(i - 4);
      lcd.setCursor(0, 1);
      lcd.print(F("ID:"));
      EEPROM.get(eepromAddresses[i], Card);
      if (Card == 4294967295) {
        lcd.print(F("None"));
      } else {
        lcd.print(Card);
      }
    }
    lcd.setCursor(0, 2);
    lcd.print(F("[D]Delete"));
    lcd.setCursor(0, 3);
    lcd.print(F("[*]Next      Edit[#]"));
    while (1)
    {
      var = keypad.waitForKey();
      if (var == 'D' ) {
        EEPROM.put(eepromAddresses[i], 4294967295);
        lcd.setCursor(3,1);
        lcd.print(F("Card Deleted!"));
      }
      if (var == '#' ) {
        Bbipp(30);//tone(tonepin,2400,30);
        cls();
        lcd.setCursor(2, 0);
        save = false;
        if (i <= 4) {
          lcd.print(F("Add Green card "));
          lcd.print(i + 1);
        } else {
          lcd.print(F("Add Red card "));
          lcd.print(i - 4);
        }
        lcd.setCursor(4, 1);
        lcd.print(F("Scan now ..."));
        while (save == false) {
          success_1 = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid_1, &uidLength_1);
          if (uidLength_1 == 4) {
            // We probably have a Mifare Classic card ...
            cardid1 = uid_1[0];
            cardid1 <<= 8;
            cardid1 |= uid_1[1];
            cardid1 <<= 8;
            cardid1 |= uid_1[2];
            cardid1 <<= 8;
            cardid1 |= uid_1[3];
            lcd.setCursor(0, 2);
            lcd.print(F("ID: "));
            lcd.print(cardid1);
          }
          save = true;
          for (int i = 0; i <= 9; i++) {
            if (EEPROM.get(eepromAddresses[i], Card) == cardid1) {
              save = false;
            }
          }
          if (save == false) {
            lcd.setCursor(0, 3);
            lcd.print(F("Already exists!!!"));
            delay(2000);
            lcd.setCursor(0, 2);
            lcd.print(F("                   "));
            lcd.setCursor(0, 3);
            lcd.print(F("                   "));
          } else {
            EEPROM.put(eepromAddresses[i], cardid1);
            lcd.setCursor(10, 3);
            lcd.print(F("OK. Saved!"));
            delay(2000);
            save = true;
          }
        }
        break;
        
      }
      if (var == '*' ) {
        Bbipp(30);//tone(tonepin,2400,30);
        break;
      }
    }
    startListeningToNFC();
    
  }
  SetupMenu();
}


void NFCPrintInfo() {
  cls();
  uint32_t versiondata = nfc.getFirmwareVersion();
  lcd.setCursor(0,0);
  lcd.print(F("### NFC HW Info ###"));
  if (! versiondata) {
    lcd.setCursor(0,1);
    lcd.print(F("PN53x board not found"));
    //while (1); // halt
    delay (5000);
  } else {
      lcd.setCursor(2,1);
      lcd.print(F("Board id: ")); lcd.print((versiondata>>24) & 0xFF, HEX); 
      lcd.setCursor(2,2);
      lcd.print(F("Firmware: ")); lcd.print((versiondata>>16) & 0xFF, DEC); lcd.print('.'); lcd.print((versiondata>>8) & 0xFF, DEC);
      lcd.setCursor(0,3); lcd.print(F("Press any key..."));
  }
  byte key = keypad.getKey();
  while(key == NO_KEY) {
    key = keypad.getKey();
  }
}


bool isNFCPresent() {
  uint32_t versiondata = nfc.getFirmwareVersion(); // Check for NFC module firmware
  lcd.setCursor(0, 1);
  lcd.print(F("NFC "));
  // NFC module not found - display error message durning startup - set NFC enabled status to false
  if (!versiondata) {
    lcd.print(F("not found!"));
    lcd.setCursor(16, 1);
    lcd.print(F("|Err"));
    return false;
  }
  // NFC module is present - display firmware version and ok during startup - set NFC enabled status
  lcd.print(F("Fw: "));
  lcd.print((versiondata >> 16) & 0xFF, DEC);
  lcd.print(F("."));
  lcd.print((versiondata >> 8) & 0xFF, DEC);
  lcd.setCursor(16, 1);
  lcd.print(F("| OK"));
  return true;
}