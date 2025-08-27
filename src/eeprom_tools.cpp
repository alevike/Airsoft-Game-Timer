/*
 * EEprom mapp
 * [0] -> [29] Device defaults
 * [10 -> 11] - GAMEMINUTES
 * [12 -> 13] - BOMBMINUTES
 * [14 -> 15] - ACTIVATESECONDS
 * [16] - SOUND ENABLE
 * [17] - PASSWORD ENABLE (not implemented)
 * [30] -> []Domination Game history
 * [30 - 34] - 1.
 * [35 - 39] - 2.
 * [40 - 44] - 3.
 * [45 - 49] - 4.
 * [50 - 54] - 5.
 * [55 - 59] - 6.
 * [60 - 64] - 7.
 * [65 - 69] - 8.
 * [70 - 74] - 9.
 * [75 - 79] - 10.
 * 
 * [80 - 84] - 1.
 * [85 - 89] - 2.
 * [90 - 94] - 3.
 * [95 - 99] - 4.
 * [100 - 104] - 5.
 * [105 - 109] - 6.
 * [110 - 114] - 7.
 * [115 - 119] - 8.
 * [120 - 124] - 9.
 * [125 - 129] - 10.
 * [130] - EEPROM_LORA_ADDH
 * [131] - EEPROM_LORA_ADDL
 * [132] - EEPROM_LORA_CHAN
 * [133] - EEPROM_LORA_MYID
 * [134] - 
 * [135] - 
 * [136] - LoRa paired ID 1
 * [137] - Lora paired ID 2
 * [138] - LoRa Paired ID 3
 * [139] - LoRa Paired ID 4
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
 * 
 * [300] - 
 * [400] - 
 * [401] - device id - lora_myOwnID
 * [402] - lora last assigned ID (if we are a master)
 */
#include "eeprom_tools.h"
int   RedAddress[] = {30, 35, 40, 45, 50,  55,  60,  65,  70,  75};
int GreenAddress[] = {80, 85, 90, 95, 100, 105, 110, 115, 120, 125};
 
long EEPROMReadlong(long address) {
      //Read the 4 bytes from the eeprom memory.
      long four = EEPROM.read(address);
      long three = EEPROM.read(address + 1);
      long two = EEPROM.read(address + 2);
      long one = EEPROM.read(address + 3);

      //Return the recomposed long by using bitshift.
      return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
}

void writeIntIntoEEPROM(int address, int number)
{ 
  EEPROM.write(address, number >> 8);
  EEPROM.write(address + 1, number & 0xFF);
}

int readIntFromEEPROM(int address)
{
  return (EEPROM.read(address) << 8) + EEPROM.read(address + 1);
}

void readSettingsEEPROM() {
  /*
   * int EEPROM_GAMEMINUTES -> [180 - 181]; int EEPROM_BOMBMINUTES -> [182-183]; int EEPROM_ACTIVATESECONDS -> [184-185]; boolean EEPROM_SOUND[186]; boolean EEPROM_PASSWORD[187]; char EEPROM_saved_password[8];
   *       GAMEMINUTES                             BOMBMINUTES                           ACTIVATESECONDS
   */
  EEPROM_GAMEMINUTES = readIntFromEEPROM(10);
  if (EEPROM_GAMEMINUTES >= 999) EEPROM_GAMEMINUTES = 10;
  EEPROM_BOMBMINUTES = readIntFromEEPROM(12);
  if (EEPROM_BOMBMINUTES >= 999) EEPROM_BOMBMINUTES = 10;
  EEPROM_ACTIVATESECONDS = readIntFromEEPROM(14);
  if (EEPROM_ACTIVATESECONDS >= 999) EEPROM_ACTIVATESECONDS = 5;
  EEPROM_SOUND = EEPROM.read(16); // sound bit
  //Read and handle LoRa ID
  lora_myOwnID = EEPROM.read(401);
  if (lora_myOwnID > 100 or lora_myOwnID < 0) { // fix if an address wasn't defined
    lora_myOwnID = 100;     // unassigned
    loraLastAssignedID = 0;
  }
  // Activate new values
  GAMEMINUTES = EEPROM_GAMEMINUTES;
  if (GAMEMINUTES < 1) GAMEMINUTES = 1;
  BOMBMINUTES = EEPROM_BOMBMINUTES;
  if (BOMBMINUTES < 1) BOMBMINUTES = 1;
  ACTIVATESECONDS = EEPROM_ACTIVATESECONDS;
  if (ACTIVATESECONDS < 1) ACTIVATESECONDS = 1;
  soundEnable = EEPROM_SOUND;

  if (soundEnable == false) {
        strncpy(sound_text, string_off, strlen(string_off) + 1);
  } else {
        strncpy(sound_text, string_on, strlen(string_on) + 1);
  }
  if (EEPROM_SOUND == false) {
        strncpy(EEPROM_sound_text, string_off, strlen(string_off) + 1);
  } else {
        strncpy(EEPROM_sound_text, string_on, strlen(string_on) + 1);
  }
}

void writeSettingsEEPROM() {
  writeIntIntoEEPROM(10, EEPROM_GAMEMINUTES);
  writeIntIntoEEPROM(12, EEPROM_BOMBMINUTES);
  writeIntIntoEEPROM(14, EEPROM_ACTIVATESECONDS);
  EEPROM.write(16, EEPROM_SOUND);

  // Activate new values
  GAMEMINUTES = EEPROM_GAMEMINUTES;
  BOMBMINUTES = EEPROM_BOMBMINUTES;
  ACTIVATESECONDS = EEPROM_ACTIVATESECONDS;
  soundEnable = EEPROM_SOUND;
  if (soundEnable == false) {
        strncpy(sound_text, string_off, strlen(string_off) + 1);
    } else {
        strncpy(sound_text, string_on, strlen(string_on) + 1);
    }
  if (EEPROM_SOUND == false) {
        strncpy(EEPROM_sound_text, string_off, strlen(string_off) + 1);
    } else {
        strncpy(EEPROM_sound_text, string_on, strlen(string_on) + 1);
    }
}

void EEPROMWritelong(int address, long value)
      {
      //Decomposition from a long to 4 bytes by using bitshift.
      //One = Most significant -> Four = Least significant byte
      byte four = (value & 0xFF);
      byte three = ((value >> 8) & 0xFF);
      byte two = ((value >> 16) & 0xFF);
      byte one = ((value >> 24) & 0xFF);

      //Write the 4 bytes into the eeprom memory.
      EEPROM.write(address, four);
      EEPROM.write(address + 1, three);
      EEPROM.write(address + 2, two);
      EEPROM.write(address + 3, one);
      }

void DominationSaveHistory(long redTime, long greenTime) {
   int RedHistSize = sizeof(RedAddress) / sizeof(int);
   int GreenHistSize = sizeof(GreenAddress) / sizeof(int);
   unsigned long RedHistory[RedHistSize];
   unsigned long GreenHistory[GreenHistSize];
   //Create red history array
   for (int i = 0; i < RedHistSize; i++) {
       unsigned long RedActualValue = EEPROMReadlong(RedAddress[i]);
       unsigned long GreenActualValue = EEPROMReadlong(GreenAddress[i]);
       RedHistory[i] = RedActualValue;
       GreenHistory[i] = GreenActualValue;     
   }
   
   // rotate History array
   for (int i = RedHistSize - 1; i > 0; i--) RedHistory[i] = RedHistory[i - 1];
   for (int i = GreenHistSize - 1; i > 0; i--) GreenHistory[i] = GreenHistory[i - 1];
   //Insert new time values 
   GreenHistory[0] = greenTime;
   RedHistory[0] = redTime;
   //Write back 
   for (int i = 0; i < RedHistSize; i++)EEPROMWritelong(RedAddress[i], RedHistory[i]);    
   for (int i = 0; i < GreenHistSize; i++)EEPROMWritelong(GreenAddress[i], GreenHistory[i]);    
}

void DominationShowHistory() {
   cls();
   int RedHistSize = sizeof(RedAddress) / sizeof(int);
   int GreenHistSize = sizeof(GreenAddress) / sizeof(int);
   unsigned long RedHistory[RedHistSize];
   unsigned long GreenHistory[GreenHistSize];
   //Create  history array
   for (int i = 0; i < RedHistSize; i++) {
       unsigned long RedActualValue = EEPROMReadlong(RedAddress[i]);
       unsigned long GreenActualValue = EEPROMReadlong(GreenAddress[i]);
       RedHistory[i] = RedActualValue;
       GreenHistory[i] = GreenActualValue;
   }
    //Print history
   int scrolIndex = 0;
   cls();
        lcd.setCursor(0,0);
        lcd.print(F("=Domination History="));
        lcd.setCursor(0,1);
        lcd.print(scrolIndex); 
        lcd.print("  G-");
        printHistoryTime(GreenHistory[scrolIndex]);
        lcd.print(" R-");
        printHistoryTime(RedHistory[scrolIndex]);
        lcd.setCursor(0,2);
        lcd.print(scrolIndex + 1);
        lcd.print("  G-");
        printHistoryTime(GreenHistory[scrolIndex + 1]);
        lcd.print(" R-");
        printHistoryTime(RedHistory[scrolIndex + 1]);
        lcd.setCursor(0,3);
        lcd.print(scrolIndex + 2);
        lcd.print("  G-");
        printHistoryTime(GreenHistory[scrolIndex + 2]);
        lcd.print(" R-");
        printHistoryTime(RedHistory[scrolIndex + 2]);
   
  
   while(1){
    var = keypad.waitForKey();
    if (var == '2') { //up
      if (scrolIndex > 0) {
        cls();
        scrolIndex--;
        lcd.setCursor(0,0);
        lcd.print(F("=Domination History="));
        lcd.setCursor(0,1);
        lcd.print(scrolIndex); 
        lcd.print(F("  G-"));
        printHistoryTime(GreenHistory[scrolIndex]);
        lcd.print(F(" R-"));
        printHistoryTime(RedHistory[scrolIndex]);
        lcd.setCursor(0,2);
        lcd.print(scrolIndex + 1);
        lcd.print(F("  G-"));
        printHistoryTime(GreenHistory[scrolIndex + 1]);
        lcd.print(F(" R-"));
        printHistoryTime(RedHistory[scrolIndex + 1]);
        lcd.setCursor(0,3);
        lcd.print(scrolIndex + 2);
        lcd.print(F("  G-"));
        printHistoryTime(GreenHistory[scrolIndex + 2]);
        lcd.print(F(" R-"));
        printHistoryTime(RedHistory[scrolIndex + 2]);
      }
    }
    if (var == '8') { //down
      if (scrolIndex < RedHistSize - 3) {
        cls();
        scrolIndex++;
        lcd.setCursor(0,0);
        lcd.print(F("=Domination History="));
        lcd.setCursor(0,1);
        lcd.print(scrolIndex);
        lcd.print(F("  G-"));
        printHistoryTime(GreenHistory[scrolIndex]);
        lcd.print(F(" R-"));
        printHistoryTime(RedHistory[scrolIndex]);
        lcd.setCursor(0,2);
        lcd.print(scrolIndex + 1);
        lcd.print(F("  G-"));
        printHistoryTime(GreenHistory[scrolIndex + 1]);
        lcd.print(F(" R-"));
        printHistoryTime(RedHistory[scrolIndex + 1]);
        lcd.setCursor(0,3);
        lcd.print(scrolIndex + 2);
        lcd.print(F("  G-"));
        printHistoryTime(GreenHistory[scrolIndex + 2]);
        lcd.print(F(" R-"));
        printHistoryTime(RedHistory[scrolIndex + 2]);
      }
    }
    if(var == '#' ){
      Bbipp(30);
      break;
    }
   } 
}



// NFC EEprom tools
void writeStringToEEPROM(int addrOffset, const String &strToWrite)
{
  byte len = strToWrite.length();
  EEPROM.write(addrOffset, len);
  for (int i = 0; i < len; i++)
  {
    EEPROM.write(addrOffset + 1 + i, strToWrite[i]);
  }
}
String readStringFromEEPROM(int addrOffset)
{
  int newStrLen = EEPROM.read(addrOffset);
  char data[newStrLen + 1];
  for (int i = 0; i < newStrLen; i++)
  {
    data[i] = EEPROM.read(addrOffset + 1 + i);
  }
  data[newStrLen] = '\0';
  return String(data);
}