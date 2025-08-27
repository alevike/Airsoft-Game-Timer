#include "AirsoftTimer_V1_4_softPower.h"
#include "version.h"
#include "LoRa_E32.h"
#include <SPI.h>
#include <avr/io.h>
#include <avr/interrupt.h>

volatile bool powerButtonPressed = false;
volatile bool powerButtonReleased = false;
volatile unsigned long powerPressedTime = 0;
volatile unsigned long powerReleasedTime = 0;

//Store soft reboot flag 
volatile uint8_t soft_reboot_flag __attribute__((section(".noinit")));
boolean softBoot = false;

//NFC Reader
bool volatile nfcEnabled __attribute__((section(".noinit")));
boolean greenCardDetected = false;
boolean redCardDetected = false;

//
int freeRam() {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

//memory optimization
boolean inGame = LOW;
byte doAction = 0;

// NFC I2C connection:
Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);
const int DELAY_BETWEEN_CARDS = 500;
long timeLastCardRead = 0;
boolean readerDisabled = false;
int irqCurr;
int irqPrev;
unsigned long aTime = 0;

//TIME INITS
int GAMEHOURS = 0;
int GAMEMINUTES = 10;
int BOMBMINUTES = 10;
int ACTIVATESECONDS = 5;
int defuseWire = 0;
int detonationWire = 0;
int tCutWire1 = 0;
int tCutWire2 = 0;
byte tCutWire1Status = 0;
byte tCutWire2Status = 0;

//LCD screen config
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
boolean redrawFull = true; // flicker fix
const  int  LCD_NB_ROWS  =  4 ;
const  int  LCD_NB_COLUMNS  =  20 ;
BigFont02     big(&lcd);
byte customCharset = 15;


bool soundIsOn = false;
char sound_text[4];
char autoarm_text[4];
char pass_text[4];
const char string_on[] = "ON";
const char string_off[] = "OFF";
boolean wcMenuWizard = false;

//Globals for EEPROM
int EEPROM_GAMEMINUTES;
int EEPROM_BOMBMINUTES;
int EEPROM_ACTIVATESECONDS;
boolean EEPROM_SOUND; // EEPROM_sound_text
char EEPROM_sound_text[4];
boolean EEPROM_PASSWORD; // EEPROM_pass_text
char EEPROM_pass_text[4];
char EEPROM_saved_password[8];
byte EEPROM_LORA_ADDH;
byte EEPROM_LORA_ADDL;
byte EEPROM_LORA_CHAN;
byte EEPROM_LORA_MYID;
byte eepromAddresses[] = {140, 150, 160, 170, 180, 190, 200, 210, 220, 230};  // NFC Cards storage

const byte loraWaitTime = 10;


// Keypad Configuration
const byte ROWS = 4;
const byte COLS = 4; 
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {37, 36, 35, 34}; 
byte colPins[COLS] = {33, 32, 31, 30}; 

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
bool gReverseDirection = false; // fire effect

SimpleTimer timer;
CRGB leds[NUM_LEDS];

//Password related vars/consts
char enteredText[8];
char password[8];
int key=-1;
char lastKey;
char var;
boolean passwordEnable=false;
boolean autoArm=false;

char BT_RIGHT = '6';
char BT_UP = '2';
char BT_DOWN = '8';
char BT_LEFT = '4';
char BT_SEL = '#';   
char BT_CANCEL = '*';
char BT_DEFUSER = 'x';   // not implemented

//button leds

const int BUTTONLEDGREEN = 29;
const int BUTTONLEDRED = 28;

byte gameSelector = 0;
boolean endGame = false;
boolean sdStatus = false; //search and destroy game enable used in config
boolean saStatus = false; //same but SAbotaghe
boolean doStatus = false; //for Domination
boolean doStatus_lora = false; //for Domaniation_lora
boolean wcStatus = false; //for wire cut game
boolean bpStatus = false; //for push the button game
boolean start = true;
boolean defuseando;
boolean cancelando;
volatile boolean bipping = 0;
// SOUND TONES
boolean soundEnable = true;
int tonepin = 25;
unsigned long loraTimeNow = 0;
unsigned long iTime;
unsigned long timeCalcVar;
unsigned long redTime;
unsigned long greenTime;
unsigned long iZoneTime;
byte team=0;
byte action;

boolean lastState = LOW;
boolean currentState = LOW;
long pressDuration = 0;

// LoRa E32 Configuration
LoRa_E32 e32ttl(&Serial2, PIN_AUX, PIN_M0, PIN_M1);

// LoRa Globals
byte lora_myOwnID;
byte loraLastAssignedID;
boolean TimeUp;
boolean lora_inSync = false;
volatile bool loraEnabled __attribute__((section(".noinit")));

struct Message message;

//Main Menu definition
  LiquidLine line0(0, 0, "=== Offline game ===");
  LiquidLine line1(2, 1, "1. Search&Destroy");
  LiquidLine line2(2, 2, "2. Sabotage");
  LiquidLine line3(2, 3, "3. Domination");
  LiquidLine line4(2, 4, "4. Cut the wire");
  LiquidLine line5(2, 5, "5. Button push*");
  LiquidLine line6(0, 6, "=== Online games ===");
  LiquidLine line7(2, 7, "1. Domination LoRa");
  LiquidLine line8(0, 8, "====== Utils =======");
  LiquidLine line9(2, 9, "1. Dom. History");
  LiquidLine line10(2, 10, "2. Setup");
  //LiquidLine line11(2, 11, "3. Lora Pair");
  //LiquidLine line12(2, 12, "4. Lora Reset");
  LiquidLine line13(2, 11, "5. SoftReboot");
  
LiquidScreen mainScreen;
LiquidMenu MainMenu(lcd, mainScreen);

//Menu - Config game menu
  LiquidLine ConfigGameTime_header  (0, 0, "---- Game setup ----");
  LiquidLine ConfigGameTime_minutes (1, 1, "Game time     |", GAMEMINUTES, "m");
  LiquidLine ConfigBombTime_minutes (1, 2, "Bomb time     |", BOMBMINUTES, "m");
  LiquidLine ConfigArmTime_seconds  (1, 3, "Arm  time     |", ACTIVATESECONDS, "s");
  LiquidLine ConfigSound            (1, 4, "Sound         |", sound_text);
  LiquidLine ConfigPassword         (1, 5, "Password      |", pass_text);
  LiquidLine Config_nav2            (0, 6, "[*]Back     Start[#]");
LiquidScreen screen_configGameTime;
LiquidMenu ConfigGameMenu(lcd, screen_configGameTime);

//Menu - Config wires
  LiquidLine ConfigWires_header(0, 0, "---- Wire setup ----  ");
  LiquidLine ConfigWire_hot(1, 1, "Hot wire     |", detonationWire);
  LiquidLine ConfigWires_defuse(1, 2, "Defuse wire  |", defuseWire);
  LiquidLine ConfigWires_arm(1, 3, "Auto arm     |", autoarm_text);
  LiquidLine ConfigWires_next(13, 4, "Next[#]");

LiquidScreen screen_ConfigWires;
LiquidMenu ConfigWiresMenu(lcd, screen_ConfigWires);

//Menu - Setup
  LiquidLine setupHeaderLine      (0, 0,  "--- Set Defaults ---");
  LiquidLine setupGameTime_minutes(1, 1,  "Game time     |", EEPROM_GAMEMINUTES, "m");
  LiquidLine setupBombTime_minutes(1, 2,  "Bomb time     |", EEPROM_BOMBMINUTES, "m");
  LiquidLine setupArmTime_seconds (1, 3,  "Arm  time     |", EEPROM_ACTIVATESECONDS, "s");
  LiquidLine setupConfigSound     (1, 4,  "Sound         |", EEPROM_sound_text);
  LiquidLine setupPassword        (1, 5,  "Password      |", EEPROM_pass_text);
  LiquidLine nfcSetupLine         (1, 6,  "NFC Cards     | [6]");
  LiquidLine nfcInfoLine          (1, 7,  "NFC HW Info   | [6]");
  LiquidLine LoraSetupLine        (1, 8,  "LoRa Info     | [6]");
  LiquidLine BatteryInfo          (1, 9,  "Battery info  | [6]");
  LiquidLine loraPair             (1, 10, "Lora Pairing  | [6]");
  LiquidLine loraResetPair        (1, 11, "Lora Reset    | [6]");
  LiquidLine Setup_nav2           (1, 12, "[*]Back     Save[#]");

LiquidScreen screen_SetupMenu;
LiquidMenu Setup_Menu(lcd, screen_SetupMenu);

// Define menu system
LiquidSystem menu_system(MainMenu, ConfigGameMenu, ConfigWiresMenu, Setup_Menu);

 // * Custom characters - Progress bar * / 
byte START_DIV_0_OF_1[8]  = { B01111, B11000, B10000, B10000, B10000, B10000, B11000, B01111}; // Char start 0/1
byte START_DIV_1_OF_1[8]  = { B01111, B11000, B10011, B10111, B10111, B10011, B11000, B01111}; // Char start 1/1
byte DIV_0_OF_2 [8]       = { B11111, B00000, B00000, B00000, B00000, B00000, B00000, B11111}; // Char middle 0/2
byte DIV_1_OF_2 [8]       = { B11111, B00000, B11000, B11000, B11000, B11000, B00000, B11111}; // Middle tank 1/2
byte DIV_2_OF_2 [ 8 ]     = { B11111, B00000, B11011, B11011, B11011, B11011, B00000, B11111}; // Middle tank 2/2
byte END_DIV_0_OF_1 [ 8 ] = { B11110, B00011, B00001, B00001, B00001, B00001, B00011, B11110}; // Char end 0/1
byte END_DIV_1_OF_1 [ 8 ] = { B11110, B00011, B11001, B11101, B11101, B11001, B00011, B11110}; // Char thin 1/1

//Battery class
BatteryManager batteryManager(3100, 4200, 15, 5, KEEP_POWER_PIN, HIGH);
volatile bool enableBattery __attribute__((section(".noinit")));





void setup(){
  Serial.begin(115200);
  Serial.println("Setup start");

  // Power management
  pinMode (POWER_BUTTON_SENSE, INPUT_PULLUP);
  pinMode (KEEP_POWER_PIN, OUTPUT);
  digitalWrite (KEEP_POWER_PIN, HIGH); //Keep power on
  //Power button sense
  // Configure PE6 (INT6) as input with pull-up
  DDRE  &= ~_BV(PE6);
  PORTE |=  _BV(PE6);

  // Interrupt-driven power button (INT6) on both edges
  cli();
  EICRB &= ~_BV(ISC61);     // ISC61:ISC60 = 01 -> any logical change
  EICRB |=  _BV(ISC60);
  EIFR   =  _BV(INTF6);     // clear any pending flag
  EIMSK |=  _BV(INT6);      // enable INT6
  sei();

  // soft reboot detection
  if (soft_reboot_flag == 0xAA) {
    soft_reboot_flag = 0x00;
    // Detected soft reboot
    softBoot = true;
  }

  batteryManager.begin(); // will aslo enable the power keep alive pin
  if (!softBoot) enableBattery = batteryManager.isBatteryEnabled();

  //Reading EEPROM
  Serial.println(F("Reading EEPROM......"));
  readSettingsEEPROM();
  Serial.println(F("Reading EEPROM - OK"));

  //LCD Setup
  Serial.println(F("Initializing LCD...."));
  lcd.begin(20, 4);

  // * Save custom characters in LCD display memory * / 
  lcd.createChar ( 0 , START_DIV_0_OF_1);
  lcd.createChar ( 1 , START_DIV_1_OF_1);
  lcd.createChar ( 2 , DIV_0_OF_2);
  lcd.createChar ( 3 , DIV_1_OF_2);
  lcd.createChar ( 4 , DIV_2_OF_2);
  lcd.createChar ( 5 , END_DIV_0_OF_1);
  lcd.createChar ( 6 , END_DIV_1_OF_1);
  Serial.println(F("Initializing LCD - OK"));


  //FASTLed setup
  Serial.println(F("Initializing FASTLED...."));
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);  // initialize RGB strip
  //FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, BGR>(leds, NUM_LEDS);  // initialize RGB strip
  FastLED.setBrightness( BRIGHTNESS );
  Serial.println(F("Initializing FASTLED - OK"));

// LORA Setup part
  Serial.println(F("Initializing LORA...."));
  e32ttl.begin(); // init module
  Serial.println(F("Initializing LORA - OK"));
// END of LORA Setup

  
// Setup pins leds on and wire setup
  pinMode(wsPWRon, OUTPUT);
  pinMode(tonepin, OUTPUT);
  digitalWrite (wsPWRon, HIGH);
  pinMode (BUTTONLEDGREEN, OUTPUT);
  pinMode (BUTTONLEDRED, OUTPUT);
  pinMode (wire1, INPUT_PULLUP);
  pinMode (wire2, INPUT_PULLUP);
  pinMode (wire3, INPUT_PULLUP);
  pinMode (wire4, INPUT_PULLUP);
  pinMode (wireN, OUTPUT);
  digitalWrite (wireN, LOW);   
  
//Bootup Sequence display
  if (!softBoot) {
      lcd.setCursor(0,0);
      lcd.print(F("     Game Timer"));
      lcd.setCursor(7,2);
      lcd.print(F("SW.Ver.: "));
      lcd.print(RELEASE_VERSION);
      lcd.setCursor(7,3);
      lcd.print(F("HW.Ver.: 1.7"));
      for (int i = 0; i <= 2; i++) {
          digitalWrite (BUTTONLEDGREEN, HIGH);
          digitalWrite (BUTTONLEDRED, HIGH);
          WSblueRGB();
          delay(500);
          digitalWrite (BUTTONLEDGREEN, LOW);
          digitalWrite (BUTTONLEDRED, LOW);
          WSclsRGB();
           delay(500);
      }
      digitalWrite (BUTTONLEDGREEN, HIGH);
      digitalWrite (BUTTONLEDRED, HIGH);
  }
  Serial.println(F("Initializing SoftReboot - OK"));

  
  //Keypad setup
  Serial.println(F("Initializing Keypad...."));
  keypad.setHoldTime(50);
  keypad.setDebounceTime(50);
  keypad.addEventListener(keypadEvent);
  Serial.println(F("Initializing Keypad - OK"));

//NFC setup and precheck
  Serial.println(F("Initializing NFC...."));
  nfc.begin();
  // configure board to read RFID tags
  nfc.SAMConfig();
  Serial.println(F("Initializing NFC - OK"));


// Perform startup checks
  if(!softBoot) {
    Serial.println(F("Hardware initialization done! Performing startup checks..."));
    StartUpCheck();
  }


  Serial.println(F("Initializing MENUS...."));
// main menu setup
  line1.set_focusPosition(Position::LEFT);
  line2.set_focusPosition(Position::LEFT);
  line3.set_focusPosition(Position::LEFT);
  line4.set_focusPosition(Position::LEFT);
  line5.set_focusPosition(Position::LEFT);
  line6.set_focusPosition(Position::LEFT);
  line7.set_focusPosition(Position::LEFT);
  line8.set_focusPosition(Position::LEFT);
  line9.set_focusPosition(Position::LEFT);
  line10.set_focusPosition(Position::LEFT);
  line13.set_focusPosition(Position::LEFT);
 // mainScreen.add_line(line0);
  mainScreen.add_line(line1);
  mainScreen.add_line(line2);
  mainScreen.add_line(line3);
  mainScreen.add_line(line4);
  //mainScreen.add_line(line5);
  mainScreen.add_line(line6);
  mainScreen.add_line(line7); 
  mainScreen.add_line(line8);
  mainScreen.add_line(line9);
  mainScreen.add_line(line10);
  mainScreen.add_line(line13); // soft reboot test

    // Attaching a function to the lines is required for scrolling to work.
  line1.attach_function(1, sdStart);
  line2.attach_function(1, saStart);
  line3.attach_function(1, doStart);
  line4.attach_function(1, wcStart);
  //line5.attach_function(1, bpStart);
  line7.attach_function(1, doStart_lora);
  line9.attach_function(1, DominationShowHistory);
  line10.attach_function(1, SetupMenu);
  line13.attach_function(1, software_Reset);
  mainScreen.set_displayLineCount(4);
  MainMenu.add_screen(mainScreen);

 
  // Config Game menu
  ConfigGameTime_minutes.set_focusPosition(Position::LEFT);
  ConfigBombTime_minutes.set_focusPosition(Position::LEFT);
  ConfigArmTime_seconds.set_focusPosition(Position::LEFT);
  ConfigSound.set_focusPosition(Position::LEFT);
  ConfigPassword.set_focusPosition(Position::LEFT);
  
  screen_configGameTime.add_line(ConfigGameTime_header);
  screen_configGameTime.add_line(ConfigGameTime_minutes);
  screen_configGameTime.add_line(ConfigBombTime_minutes);
  screen_configGameTime.add_line(ConfigArmTime_seconds);
  screen_configGameTime.add_line(ConfigSound);
  screen_configGameTime.add_line(ConfigPassword);
  screen_configGameTime.add_line(Config_nav2);
  
  ConfigGameTime_minutes.attach_function(increase,  menu_inc_gametime);
  ConfigGameTime_minutes.attach_function(decrease,  menu_dec_gametime);
  ConfigBombTime_minutes.attach_function(increase,  menu_inc_bombtime);
  ConfigBombTime_minutes.attach_function(decrease,  menu_dec_bombtime);
  ConfigArmTime_seconds.attach_function(increase,  menu_inc_armtime);
  ConfigArmTime_seconds.attach_function(decrease,  menu_dec_armtime);
  ConfigPassword.attach_function(increase, blink_pass);
  ConfigPassword.attach_function(decrease, blink_pass);
  ConfigSound.attach_function(1, blink_sound);
  ConfigSound.attach_function(2, blink_sound);
    
  screen_configGameTime.set_displayLineCount(4);
  ConfigGameMenu.add_screen(screen_configGameTime);


  //Setup Menu
  setupGameTime_minutes.set_focusPosition(Position::LEFT);
  setupBombTime_minutes.set_focusPosition(Position::LEFT);
  setupArmTime_seconds.set_focusPosition(Position::LEFT);
  setupConfigSound.set_focusPosition(Position::LEFT);
  if (nfcEnabled)    nfcSetupLine.set_focusPosition(Position::LEFT);
  if (loraEnabled)   LoraSetupLine.set_focusPosition(Position::LEFT);
  if (loraEnabled)   loraPair.set_focusPosition(Position::LEFT);
  if (loraEnabled)   loraResetPair.set_focusPosition(Position::LEFT);
  if (enableBattery) BatteryInfo.set_focusPosition(Position::LEFT);
  nfcInfoLine.set_focusPosition(Position::LEFT);
   //setupDefPasswd.set_focusPosition(Position::LEFT);
   
  screen_SetupMenu.add_line(setupHeaderLine);
  screen_SetupMenu.add_line(setupGameTime_minutes);
  screen_SetupMenu.add_line(setupBombTime_minutes);
  screen_SetupMenu.add_line(setupArmTime_seconds);
  screen_SetupMenu.add_line(setupConfigSound);
  if (nfcEnabled) screen_SetupMenu.add_line(nfcSetupLine);
  if (nfcEnabled) screen_SetupMenu.add_line(nfcInfoLine);
  if (loraEnabled) screen_SetupMenu.add_line(LoraSetupLine);
  if (loraEnabled) screen_SetupMenu.add_line(loraPair);
  if (loraEnabled) screen_SetupMenu.add_line(loraResetPair);
  if (enableBattery) screen_SetupMenu.add_line(BatteryInfo);
  screen_SetupMenu.add_line(Setup_nav2);

  setupGameTime_minutes.attach_function(increase,  menu_inc_EEPROM_gametime);
  setupGameTime_minutes.attach_function(decrease,  menu_dec_EEPROM_gametime);
  setupBombTime_minutes.attach_function(increase,  menu_inc_EEPROM_bombtime);
  setupBombTime_minutes.attach_function(decrease,  menu_dec_EEPROM_bombtime);
  setupArmTime_seconds.attach_function(increase,  menu_inc_EEPROM_armtime);
  setupArmTime_seconds.attach_function(decrease,  menu_dec_EEPROM_armtime);
  setupConfigSound.attach_function(1, EEPROM_blink_sound);
  setupConfigSound.attach_function(2, EEPROM_blink_sound);
  if (nfcEnabled)    nfcSetupLine.attach_function(1, NFCCardSetupWizard);
  if (nfcEnabled)    nfcInfoLine.attach_function(1, NFCPrintInfo);
  if (enableBattery) BatteryInfo.attach_function(1, batteryPage);
  if (loraEnabled)   LoraSetupLine.attach_function(1, ParametersPrint);
  if (loraEnabled)   loraPair.attach_function(1, loraStartPair);
  if (loraEnabled)   loraResetPair.attach_function(1, loraRestPair);
  Setup_nav2.attach_function(1, writeSettingsEEPROM);
  screen_SetupMenu.set_displayLineCount(4);
  Setup_Menu.add_screen(screen_SetupMenu);
   
  if (soundEnable == false) {
        strncpy(sound_text, string_off, sizeof(string_off));
  } else {
        strncpy(sound_text, string_on, sizeof(string_on));
  }
  
  if (EEPROM_SOUND == false) {
        strncpy(EEPROM_sound_text, string_off, sizeof(string_off));
  } else {
        strncpy(EEPROM_sound_text, string_on, sizeof(string_on));
  }

// Wire config menu

  ConfigWire_hot.set_focusPosition(Position::LEFT);
  ConfigWires_defuse.set_focusPosition(Position::LEFT);
  ConfigWires_arm.set_focusPosition(Position::LEFT);
  ConfigWires_next.set_focusPosition(Position::LEFT);
    
  screen_ConfigWires.add_line(ConfigWires_header);
  screen_ConfigWires.add_line(ConfigWire_hot);
  screen_ConfigWires.add_line(ConfigWires_defuse);
  screen_ConfigWires.add_line(ConfigWires_arm);
  screen_ConfigWires.add_line(ConfigWires_next);

  ConfigWire_hot.attach_function(increase,  menu_inc_hotWire);
  ConfigWire_hot.attach_function(decrease,  menu_dec_hotWire);
  ConfigWires_defuse.attach_function(increase,  menu_inc_defuseWire);
  ConfigWires_defuse.attach_function(decrease,  menu_dec_defuseWire);
  ConfigWires_arm.attach_function(increase,  blink_autoArm);
  ConfigWires_arm.attach_function(decrease,  blink_autoArm);
  ConfigWires_next.attach_function(1, gameLauncher);
    
  screen_ConfigWires.set_displayLineCount(4);
  ConfigWiresMenu.add_screen(screen_ConfigWires);
  wcMenuWizard = false;
  menu_system.update();
  if (autoArm == false) {
      strncpy(autoarm_text, string_off, sizeof(string_off));
  } else {
      strncpy(autoarm_text, string_on, sizeof(string_on));
  }
  if (passwordEnable == false) {
      strncpy(pass_text, string_off, sizeof(string_off));
  } else {
      strncpy(pass_text, string_on, sizeof(string_on));
  }
  Serial.println(F("Initializing MENUS - OK"));
  Serial.println(F("Setup done."));
}

void loop(){
  
 runLoop();
 menuPrincipal();
 

}

void keypadEvent(KeypadEvent key){
  switch (keypad.getState()){
    case PRESSED:
      switch (key){

      }
    break;
    case RELEASED:
      switch (key){
         case '#': defuseando= false;
         //Serial.println("d Releases");
         break;
         case '*': cancelando=false;
         //Serial.println("c Releases");
         break;
      }
    break;
    case HOLD:
      switch (key){
        case '#': defuseando= true; //red
        action = 2;
        //Serial.println("d hold");
        break;
        case '*': cancelando=true; //green
        action = 1;
        //Serial.println("c hold");
        break;
      }
    break;
    case IDLE: // Handle the IDLE state
      // Add any specific logic for IDLE state if needed
      break;
    default: // Handle any unhandled states
      // Add default behavior if necessary
      break;
  }
}

void disarmedSplash(){
  endGame = false;
  inGame = false;
  WSclsRGB();
  if(sdStatus || saStatus || wcStatus){
    lcd.clear();
    lcd.setCursor(5,0);
    lcd.print(F("GAME OVER"));
    lcd.setCursor(3,1);
    lcd.print(F("DEFENDERS WIN!"));
    lcd.setCursor(4,3);
    lcd.print(F("Press any key..."));
 
    WSclsRGB(); //digitalWrite(GREENLED, LOW); 
  }
  //end code
  byte keyLocal = keypad.getKey();
  while(keyLocal == NO_KEY) {
    keyLocal = keypad.getKey();
    EVERY_N_MILLISECONDS( 20) {
    pacifica_loop();
    FastLED.show();
    runLoop();
    }
  }
  lcd.clear();
  lcd.setCursor(5,1);
  lcd.print(F("Play Again?"));
  lcd.setCursor(0,3);
  lcd.print(F("[*]No         [#]Yes"));
  WSclsRGB(); //digitalWrite(REDLED, LOW);  
              //digitalWrite(GREENLED, LOW); 
  while(1)
  {
    var = keypad.waitForKey();
    if(var == '#' ){
      Bbipp(30);//tone(tonepin,2400,30);
      //
      if(bpStatus){
        //startGameCount();
        //pushButton();
      }
      if(wcStatus){
        quickCheckWires();
        startGameCount();
        wireCut();
      }
      if(sdStatus){
        startGameCount();
        search();
      }
      if(saStatus){
        saStatus=true;
        startGameCount();
        start=true; //
        sabotage();
      }
    }  
    if(var == '*' ){
      restore_left_focus();
      Bbipp(30);
      software_Reset();

    }  
  } 
}

void explodeSplash(){
  WSclsRGB(); 
  cls();
  delay(100);
  endGame = false;
  inGame = false;
  lcd.setCursor(5,0);
    lcd.print(F("GAME OVER"));
    lcd.setCursor(2,1);
    lcd.print(F("Terrorists WIN!"));
    lcd.setCursor(4,3);
    lcd.print(F("Press any key..."));
  WSredRGB();
  for(int i = 100; i>0; i--)// 
  {
    Bbipp(20);
    delay(20);
  }
  byte key = keypad.getKey();
  while(key == NO_KEY) {
    key = keypad.getKey();
    WSfireEffect();
    FastLED.show(); // display this frame
    FastLED.delay(1000 / FRAMES_PER_SECOND);
  }
  
  cls();

  //end code
  lcd.clear();
  lcd.setCursor(5,1);
  lcd.print(F("Play Again?"));
  lcd.setCursor(0,3);
  lcd.print(F("[*]No         [#]Yes"));
  while(1)
  {
    var = keypad.waitForKey();
    if(var == '#' ){
      if(soundEnable)Bbipp(30);//tone(tonepin,2400,30);
      //
      if(bpStatus){
//        startGameCount();
//        pushButton();
      }
      if(wcStatus){
        quickCheckWires();
        startGameCount();
        wireCut();
      }
      if(sdStatus){
        startGameCount();
        search();
      }
      if(saStatus){
        startGameCount();
        start=true; //
        sabotage();
      }
    }  
    if(var == '*' ){
      restore_left_focus();
      if(soundEnable)Bbipp(30);
      software_Reset(); // Memory freeing   
    }  
  } 
}

ISR(INT6_vect) {
  if (!(PINE & _BV(PE6))) { // Button pressed (active low)
    powerPressedTime = millis();
    powerButtonPressed = true;
  } else { // Button released (high)
    powerReleasedTime = millis();
    powerButtonReleased = true;
  }
}