#ifndef ARDUINOGLUE_H
#define ARDUINOGLUE_H


//============ Includes ====================
#include <Battery.h>
#include <Wire.h>
#include <Keypad.h>
#include <FastLED.h>
#include <LiquidCrystal.h>
#include <LiquidMenu.h>
#include <SimpleTimer.h>
#include <EEPROM.h>
#include <BigFont02.h>
#include <Adafruit_PN532.h>
#include <LoRa_E32.h>
#include <BatteryManager.h>
#include <LoRa.h>
#include <nfc_utils.h>
#include <zutils.h>


//============ Refactored code ====================
//--------- Power management Defines and Config Macros ----------
extern volatile uint8_t soft_reboot_flag;
extern volatile bool powerButtonEvent;
extern volatile bool powerButtonReleased;
extern volatile unsigned long powerPressedTime;
extern volatile unsigned long powerReleasedTime;

//--------- BatteryManager Defines and Config Macros ----------
#define KEEP_POWER_PIN      42 // Output - to keep power after start and shut down from controller 
#define POWER_BUTTON_SENSE  28 // Soft power off if pressed - PE6 is used on the PCB - no match on arduino boards.
// Battery sensing configuration
// ENABLE_PIN* are used to enable the measurement for the voltage of the battery cells, 
// voltage dividers ar connected via mosfets, these pins are used to enable the mosfets
// SENSE_PIN* are used to measure the voltage of the battery cells, these pins are connected to the voltage dividers
// The voltage dividers are used to reduce the voltage of the battery cells to a level that can be measured by the microcontroller
#define SENSE_PIN1          A2
#define ENABLE_PIN1         A6
#define SENSE_PIN2          A1
#define ENABLE_PIN2         A5
#define SENSE_PIN3          A0
#define ENABLE_PIN3         A4
extern BatteryManager       batteryManager;
extern volatile bool        enableBattery;                     		
//-----------LoRa Defines and Config Macros ----------
extern byte            loraLastAssignedID;                		//-- from AirsoftTimer_V1.4_softPower
extern byte            lora_myOwnID;
extern LiquidLine      loraResetPair;                     		//-- from AirsoftTimer_V1.4_softPower
extern const byte      loraWaitTime;                      		//-- from AirsoftTimer_V1.4_softPower
extern boolean         lora_inSync;                       		//-- from AirsoftTimer_V1.4_softPower
extern volatile bool   loraEnabled;                       		//-- from AirsoftTimer_V1.4_softPower


//============ EO Refactored code ====================
//============Imported Defines & Macros from *.ino ==============
#define PN532_IRQ   (18)
#define PN532_RESET (19)
#define wire1 49
#define wire2 48
#define wire3 47
#define wire4 46
#define wireN 45
#define DATA_PIN 23
#define CLOCK_PIN 24
#define wsPWRon 22
#define NUM_LEDS 32
#define BRIGHTNESS  40
#define COOLING  75 // fire effect
#define SPARKING 120 // fire effect
#define FRAMES_PER_SECOND 60 // fire effect
#define USE_TIMER5
#define POWER_SHORT_PRESS_TIME 500 // 500 milliseconds
#define POWER_LONG_PRESS_TIME  3000 // 3000 milliseconds
#define PIN_RX 16
#define PIN_TX 17
#define PIN_M0 A10
#define PIN_M1 A9
#define PIN_AUX A8

//============ Structs, Unions & Enums ============
//-- from AirsoftTimer_V1.4_softPower.ino
enum FunctionTypes {
  increase = 1,
  decrease = 2,
};

//-- from AirsoftTimer_V1.4_softPower.ino
extern struct Message {
    byte myID;
    byte remoteID;
    byte actionRequested; 
    int actionValue;
    unsigned long greenTime;
    unsigned long redTime;
    //char type[5];
    //char message[8];
    //int temperature;
} message;

//============ Extern Variables ============
extern boolean         softBoot; 
extern int             ACTIVATESECONDS;                   		//-- from AirsoftTimer_V1.4_softPower
extern int             BOMBMINUTES;                       		//-- from AirsoftTimer_V1.4_softPower
extern char            BT_CANCEL;                         		//-- from AirsoftTimer_V1.4_softPower
extern char            BT_DEFUSER;                        		//-- from AirsoftTimer_V1.4_softPower
extern char            BT_DOWN;                           		//-- from AirsoftTimer_V1.4_softPower
extern char            BT_LEFT;                           		//-- from AirsoftTimer_V1.4_softPower
extern char            BT_RIGHT;                          		//-- from AirsoftTimer_V1.4_softPower
extern char            BT_SEL;                            		//-- from AirsoftTimer_V1.4_softPower
extern char            BT_UP;                             		//-- from AirsoftTimer_V1.4_softPower
extern const int       BUTTONLEDGREEN;                    		//-- from AirsoftTimer_V1.4_softPower
extern const int       BUTTONLEDRED;                      		//-- from AirsoftTimer_V1.4_softPower
extern LiquidLine      BatteryInfo;                       		//-- from AirsoftTimer_V1.4_softPower
extern const byte      COLS;                              		//-- from AirsoftTimer_V1.4_softPower
extern LiquidLine      ConfigArmTime_seconds;             		//-- from AirsoftTimer_V1.4_softPower
extern LiquidMenu      ConfigGameMenu;                    		//-- from AirsoftTimer_V1.4_softPower
extern LiquidLine      ConfigGameTime_minutes;            		//-- from AirsoftTimer_V1.4_softPower
extern LiquidLine      ConfigPassword;                    		//-- from AirsoftTimer_V1.4_softPower
extern LiquidLine      ConfigWire_hot;                    		//-- from AirsoftTimer_V1.4_softPower
extern LiquidMenu      ConfigWiresMenu;                   		//-- from AirsoftTimer_V1.4_softPower
extern LiquidLine      ConfigWires_arm;                   		//-- from AirsoftTimer_V1.4_softPower
extern int             EEPROM_ACTIVATESECONDS;            		//-- from AirsoftTimer_V1.4_softPower
extern int             EEPROM_BOMBMINUTES;                		//-- from AirsoftTimer_V1.4_softPower
extern int             EEPROM_GAMEMINUTES;                		//-- from AirsoftTimer_V1.4_softPower
extern byte            EEPROM_LORA_ADDH;                  		//-- from AirsoftTimer_V1.4_softPower
extern byte            EEPROM_LORA_ADDL;                  		//-- from AirsoftTimer_V1.4_softPower
extern byte            EEPROM_LORA_CHAN;                  		//-- from AirsoftTimer_V1.4_softPower
extern byte            EEPROM_LORA_MYID;                  		//-- from AirsoftTimer_V1.4_softPower
extern boolean         EEPROM_PASSWORD;                   		//-- from AirsoftTimer_V1.4_softPower
extern boolean         EEPROM_SOUND;                      		//-- from AirsoftTimer_V1.4_softPower
extern char            EEPROM_pass_text[4];               		//-- from AirsoftTimer_V1.4_softPower
extern char            EEPROM_saved_password[8];          		//-- from AirsoftTimer_V1.4_softPower
extern char            EEPROM_sound_text[4];              		//-- from AirsoftTimer_V1.4_softPower
extern int             GAMEHOURS;                         		//-- from AirsoftTimer_V1.4_softPower
extern int             GAMEMINUTES;                       		//-- from AirsoftTimer_V1.4_softPower
extern const  int      LCD_NB_COLUMNS;                    		//-- from AirsoftTimer_V1.4_softPower
extern const  int      LCD_NB_ROWS;                       		//-- from AirsoftTimer_V1.4_softPower
extern LiquidMenu      MainMenu;                          		//-- from AirsoftTimer_V1.4_softPower
extern const byte      ROWS;                              		//-- from AirsoftTimer_V1.4_softPower
extern LiquidMenu      Setup_Menu;                        		//-- from AirsoftTimer_V1.4_softPower
extern boolean         TimeUp;                            		//-- from AirsoftTimer_V1.4_softPower
extern byte            action;                            		//-- from AirsoftTimer_V1.4_softPower
extern boolean         autoArm;                           		//-- from AirsoftTimer_V1.4_softPower
extern char            autoarm_text[4];                   		//-- from AirsoftTimer_V1.4_softPower
extern BigFont02       big;                               		//-- from AirsoftTimer_V1.4_softPower
extern volatile boolean bipping;                           		//-- from AirsoftTimer_V1.4_softPower
extern boolean         bpStatus;                          		//-- from AirsoftTimer_V1.4_softPower
extern boolean         cancelando;                        		//-- from AirsoftTimer_V1.4_softPower
extern boolean         currentState;                      		//-- from AirsoftTimer_V1.4_softPower
extern const int       d4;                                		//-- from AirsoftTimer_V1.4_softPower
extern const int       d5;                                		//-- from AirsoftTimer_V1.4_softPower
extern const int       d6;                                		//-- from AirsoftTimer_V1.4_softPower
extern const int       d7;                                		//-- from AirsoftTimer_V1.4_softPower
extern int             defuseWire;                        		//-- from AirsoftTimer_V1.4_softPower
extern boolean         defuseando;                        		//-- from AirsoftTimer_V1.4_softPower
extern int             detonationWire;                    		//-- from AirsoftTimer_V1.4_softPower
extern byte            doAction;                          		//-- from AirsoftTimer_V1.4_softPower
extern boolean         doStatus;                          		//-- from AirsoftTimer_V1.4_softPower
extern boolean         doStatus_lora;                     		//-- from AirsoftTimer_V1.4_softPower
extern LoRa_E32        e32ttl;                            		//-- from AirsoftTimer_V1.4_softPower
extern const int       en;                                		//-- from AirsoftTimer_V1.4_softPower
extern boolean         endGame;                           		//-- from AirsoftTimer_V1.4_softPower
extern char            enteredText[8];                    		//-- from AirsoftTimer_V1.4_softPower
extern bool            gReverseDirection;                 		//-- from AirsoftTimer_V1.4_softPower
extern byte            gameSelector;                      		//-- from AirsoftTimer_V1.4_softPower
extern boolean         inGame;                            		//-- from AirsoftTimer_V1.4_softPower
extern int             irqCurr;                           		//-- from AirsoftTimer_V1.4_softPower
extern int             irqPrev;                           		//-- from AirsoftTimer_V1.4_softPower
extern int             key;                               		//-- from AirsoftTimer_V1.4_softPower
extern char            lastKey;                           		//-- from AirsoftTimer_V1.4_softPower
extern boolean         lastState;                         		//-- from AirsoftTimer_V1.4_softPower
extern LiquidCrystal   lcd;                               		//-- from AirsoftTimer_V1.4_softPower
extern CRGB            leds[NUM_LEDS];                    		//-- from AirsoftTimer_V1.4_softPower
extern LiquidLine      line0;                             		//-- from AirsoftTimer_V1.4_softPower
extern LiquidLine      line10;                            		//-- from AirsoftTimer_V1.4_softPower
extern LiquidLine      line2;                             		//-- from AirsoftTimer_V1.4_softPower
extern LiquidLine      line4;                             		//-- from AirsoftTimer_V1.4_softPower
extern LiquidLine      line6;                             		//-- from AirsoftTimer_V1.4_softPower
extern LiquidLine      line8;                             		//-- from AirsoftTimer_V1.4_softPower
extern LiquidScreen    mainScreen;                        		//-- from AirsoftTimer_V1.4_softPower

//NFC global vars
extern Adafruit_PN532  nfc;                               		//-- from AirsoftTimer_V1.4_softPower
extern LiquidLine      nfcInfoLine;                       		//-- from AirsoftTimer_V1.4_softPower
extern volatile bool   nfcEnabled;                       		  //-- from AirsoftTimer_V1.4_softPower
extern boolean         readerDisabled;                    		//-- from AirsoftTimer_V1.4_softPower
extern boolean         redCardDetected;                   		//-- from AirsoftTimer_V1.4_softPower
extern boolean         greenCardDetected;                 		//-- from AirsoftTimer_V1.4_softPower

//Password globals
extern char            pass_text[4];                      		//-- from AirsoftTimer_V1.4_softPower
extern boolean         passwordEnable;                    		//-- from AirsoftTimer_V1.4_softPower
extern char            password[8];                       		//-- from AirsoftTimer_V1.4_softPower
extern long            pressDuration;                     		//-- from AirsoftTimer_V1.4_softPower

extern const int       rs;                                		//-- from AirsoftTimer_V1.4_softPower
extern boolean         saStatus;                          		//-- from AirsoftTimer_V1.4_softPower
extern LiquidScreen    screen_ConfigWires;                		//-- from AirsoftTimer_V1.4_softPower
extern LiquidScreen    screen_SetupMenu;                  		//-- from AirsoftTimer_V1.4_softPower
extern LiquidScreen    screen_configGameTime;             		//-- from AirsoftTimer_V1.4_softPower
extern boolean         sdStatus;                          		//-- from AirsoftTimer_V1.4_softPower
extern LiquidLine      setupArmTime_seconds;              		//-- from AirsoftTimer_V1.4_softPower
extern LiquidLine      setupGameTime_minutes;             		//-- from AirsoftTimer_V1.4_softPower
extern LiquidLine      setupPassword;                     		//-- from AirsoftTimer_V1.4_softPower
extern boolean         soundEnable;                       		//-- from AirsoftTimer_V1.4_softPower
extern bool            soundIsOn;                         		//-- from AirsoftTimer_V1.4_softPower
extern char            sound_text[4];                     		//-- from AirsoftTimer_V1.4_softPower
extern boolean         start;                             		//-- from AirsoftTimer_V1.4_softPower
extern const char      string_off[];                      		//-- from AirsoftTimer_V1.4_softPower
extern const char      string_on[];                       		//-- from AirsoftTimer_V1.4_softPower
extern int             tCutWire1;                         		//-- from AirsoftTimer_V1.4_softPower
extern byte            tCutWire1Status;                   		//-- from AirsoftTimer_V1.4_softPower
extern int             tCutWire2;                         		//-- from AirsoftTimer_V1.4_softPower
extern byte            tCutWire2Status;                   		//-- from AirsoftTimer_V1.4_softPower
extern byte            team;                              		//-- from AirsoftTimer_V1.4_softPower
extern long            timeLastCardRead;                  		//-- from AirsoftTimer_V1.4_softPower
extern SimpleTimer     timer;                             		//-- from AirsoftTimer_V1.4_softPower
extern int             tonepin;                           		//-- from AirsoftTimer_V1.4_softPower
extern char            var;                               		//-- from AirsoftTimer_V1.4_softPower
extern boolean         wcMenuWizard;                      		//-- from AirsoftTimer_V1.4_softPower
extern boolean         wcStatus;                          		//-- from AirsoftTimer_V1.4_softPower
extern uint32_t        Card;                              		//-- from nfc_utils
extern boolean         save;                              		//-- from nfc_utils
extern byte            teamID;                            		//-- from nfc_utils
extern uint8_t         uidLength;                         		//-- from nfc_utils
extern uint8_t         uidLength_1;                       		//-- from nfc_utils
extern unsigned long   redTime;
extern unsigned long   greenTime;
extern unsigned long   iZoneTime;
extern boolean         redrawFull;
extern Keypad          keypad;
      
extern byte            customCharset;
extern unsigned long   loraTimeNow;
extern unsigned long   aTime;
extern unsigned long   iTime;
extern unsigned long   timeCalcVar;
extern LiquidSystem    menu_system;
extern byte            eepromAddresses[];
extern const int       DELAY_BETWEEN_CARDS; // 500 milliseconds
extern byte            START_DIV_0_OF_1[];
extern byte            START_DIV_1_OF_1[];
extern byte            DIV_0_OF_2[];
extern byte            DIV_1_OF_2[];
extern byte            DIV_2_OF_2[];
extern byte            END_DIV_0_OF_1[];
extern byte            END_DIV_1_OF_1[];



//============ Function Prototypes =========
//-- from AirsoftTimer_V1.4_softPower.ino -----------
int freeRam();                                              
void keypadEvent(KeypadEvent key);                          
void disarmedSplash();                                      
void explodeSplash();                                       

//-- from buzzer.ino -----------
void clearBuzzer(void);                                     
void bipp(int duration);                                    
void Bbipp(int duration);                                   
//-- from domination.ino -----------
void domination();                                          
void gameOver();                                            


//-- from eeprom_tools.ino -----------
long EEPROMReadlong(long address);                          
void writeIntIntoEEPROM(int address, int number);           
int readIntFromEEPROM(int address);                         
void readSettingsEEPROM();                                  
void writeSettingsEEPROM();                                 
void EEPROMWritelong(int address, long value);              
void DominationSaveHistory(long redTime, long greenTime);   
void DominationShowHistory();                               
void writeStringToEEPROM(int addrOffset, const String &strToWrite);
String readStringFromEEPROM(int addrOffset);                
//-- from keys.ino -----------
boolean isPressed(char key);                                
boolean comparePassword();                                  
void setCode();                                             
void setPass();                                             
void setNewPass();                                          
char getNumber();                                           
//-- from LoRa.ino -----------
void restoreLoraConfig();                                   
void ParametersPrint();                                     
void loraCallRemote(byte actionToSend, byte actionValueToSend);
void loraSyncCapture(byte receivedActionID);                
void loraReceiveBroadcast();                                
void lora_sendACK(byte actionToACK);                        
boolean lora_send(byte actionToSend, int actionValueToSend, byte destinationID);
void lora_syncDomResults();                                 
void lora_sendResults();                                    
void loraMasterHandlePair();                                
void loraStartPair();                                       
void loraRestPair();                                        
//-- from menu.ino -----------
void menuPrincipal();                                       
void menuGameconfig();                                      
void SetupMenu();                                           
void menuConfig_wires();                                    
void blankFunction();                                       
void menu_inc_gametime();                                   
void menu_inc_EEPROM_gametime();                            
void menu_dec_gametime();                                   
void menu_dec_EEPROM_gametime();                            
void menu_inc_bombtime();                                   
void menu_inc_EEPROM_bombtime();                            
void menu_dec_bombtime();                                   
void menu_dec_EEPROM_bombtime();                            
void menu_inc_armtime();                                    
void menu_inc_EEPROM_armtime();                             
void menu_dec_armtime();                                    
void menu_dec_EEPROM_armtime();                             
void menu_inc_hotWire();                                    
void menu_dec_hotWire();                                    
void menu_inc_defuseWire();                                 
void menu_dec_defuseWire();                                 
void switch_autoArm();                                      
void blink_autoArm();                                       
void switch_sound();                                        
void blink_sound();                                         
void EEPROM_switch_sound();                                 
void EEPROM_blink_sound();                                  
void switch_pass();                                         
void blink_pass();                                          
void gameLauncher();                                        
void sdStart();                                             
void saStart();                                             
void doStart();                                             
void doStart_lora();                                        
void wcStart();                                             
void bpStart();                                             
void StartUpCheck();
void batteryPage();                                                                 
//-- from sabotage.ino -----------
void sabotage();                                            
void destroySabotage();                                     
//-- from searchdestroy.ino -----------
void search();                                              
void destroy();                                             
//-- from wireCut.ino -----------
void wireCut();                                             
void destroy2();                                            
//-- from wireCutUtils.ino -----------
void distribWires();                                        
void checkConnectivityDetailed();                           
void quickCheckWires();                                     
//-- from WS_light_effects.ino -----------
void WSwriteProcent (int procent, int team, int action);    
void WSgreenRGB();                                          
void WSredRGB();                                            
void WSblueRGB();                                           
void WSclsRGB();                                            
void WSfireEffect();                                        
void pacifica_loop();                                       
void pacifica_one_layer( CRGBPalette16& p, uint16_t cistart, uint16_t wavescale, uint8_t bri, uint16_t ioff);
void pacifica_add_whitecaps();                              
void pacifica_deepen_colors();                              
void halfTimeWS();                                          
//-- from zutils.ino -----------
void drawPorcent(byte percent);                             
void setup_progressbar();                                   
void restore_left_focus();                                  
void cls();                                                 
void printTime(unsigned long minutos, unsigned long aTiempo, boolean showMillis);
void printBigTime(unsigned long minutos, unsigned long aTiempo, boolean showMillis);
void printTimeDom(unsigned long aTiempo, boolean showMillis);
void printTeamTime(byte team);                              
void printHistoryTime(unsigned long timeValue);             
void startGameCount();                                      
void runLoop();                                             
void setBigChars();                                         
void software_Reset();                                      

#endif // ARDUINOGLUE_H
