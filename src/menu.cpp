//##################MENUS###############################
#include "menu.h"
#include "LoRa_domination.h"  


void menuPrincipal(){   //MAIN MENU
  //fix special charset
  if (customCharset != 0) restore_left_focus();  
  readerDisabled = true;
  digitalWrite(BUTTONLEDGREEN, LOW);
  digitalWrite(BUTTONLEDRED, LOW);
  if (customCharset != 0) restore_left_focus();
  WSclsRGB(); 
  //Draw menu
  wcMenuWizard = false;
  menu_system.change_menu(MainMenu);
  MainMenu.set_focusedLine(1);
  menu_system.update();
  while(1){
    runLoop(); 
    if(redrawFull) {
      menu_system.update();
      redrawFull = false;
    }
    var = keypad.getKey();
    if(var == BT_UP){
      Bbipp(30);// tone(tonepin,2400,30);
      MainMenu.switch_focus(false);
      menu_system.update();
    }
    if(var == BT_DOWN){ //inc last param in case of menu item is added
      Bbipp(30);//tone(tonepin,2400,30);
      MainMenu.switch_focus(true);
      menu_system.update();
    }

    if(var == BT_SEL){
      Bbipp(30); //tone(tonepin,2400,30);
      MainMenu.call_function(1);
    }
    
  }
}


void menuGameconfig() {
  readerDisabled = true;
  if (customCharset != 0) restore_left_focus();
  passwordEnable = false; // reset to default each time
  menu_system.change_menu(ConfigGameMenu);
  ConfigGameMenu.set_focusedLine(1);
  menu_system.update();
  while(1){
    runLoop();
    if(redrawFull) {
      menu_system.update();
      redrawFull = false;
    }
    var = keypad.getKey();
    if(var == BT_UP) {
      Bbipp(30);
      ConfigGameMenu.switch_focus(false);
      menu_system.update();
    }
    if(var == BT_DOWN) {
      Bbipp(30);//tone(tonepin,2400,30);
      ConfigGameMenu.switch_focus(true);
      menu_system.update();
    }
    if(var == BT_RIGHT){
      Bbipp(30);// tone(tonepin,2400,30);
      ConfigGameMenu.call_function(increase);
      menu_system.update();
    }
    if(var == BT_LEFT){ //inc last param in case of menu item is added
      Bbipp(30);//tone(tonepin,2400,30);
      ConfigGameMenu.call_function(decrease);
      menu_system.update();
    }
    if(var == BT_SEL){
      Bbipp(30); //tone(tonepin,2400,30);
      gameLauncher();
    }
    if(var == BT_CANCEL){
      Bbipp(30); //tone(tonepin,2400,30);
      menuPrincipal();
    } 
  }
}


void SetupMenu() {
  readerDisabled = true;
  if (customCharset != 0) restore_left_focus();
  menu_system.change_menu(Setup_Menu);
  Setup_Menu.set_focusedLine(1);
  menu_system.update();
  while(1){
    runLoop();
    if(redrawFull) {
      menu_system.update();
      redrawFull = false;
    }
    var = keypad.getKey();
    //var = keypad.waitForKey();
    if(var == BT_UP) {
      Bbipp(30);
      Setup_Menu.switch_focus(false);
      menu_system.update();
    }
    if(var == BT_DOWN) {
      Bbipp(30);//tone(tonepin,2400,30);
      Setup_Menu.switch_focus(true);
      menu_system.update();
    }
    if(var == BT_RIGHT){
      Bbipp(30);// tone(tonepin,2400,30);
      Setup_Menu.call_function(increase);
      menu_system.update();
    }
    if(var == BT_LEFT){ //inc last param in case of menu item is added
      Bbipp(30);//tone(tonepin,2400,30);
      Setup_Menu.call_function(decrease);
      menu_system.update();
    }
    if(var == BT_SEL){
      Bbipp(30); //tone(tonepin,2400,30);
      writeSettingsEEPROM();
      menuPrincipal();
    }
    if(var == BT_CANCEL){
      Bbipp(30); //tone(tonepin,2400,30);
      menuPrincipal();
    } 
  }
}


void menuConfig_wires() {
  readerDisabled = true;
  if (customCharset != 0) restore_left_focus();
  menu_system.change_menu(ConfigWiresMenu);
  ConfigWiresMenu.set_focusedLine(1);
  menu_system.update();
  while(1){
    runLoop();
    if(redrawFull) {
      menu_system.update();
      redrawFull = false;
    }
    var = keypad.getKey();
    //var = keypad.waitForKey();
    if(var == BT_UP) {
      Bbipp(30);
      ConfigWiresMenu.switch_focus(false);
      menu_system.update();
    }
    if(var == BT_DOWN) {
      Bbipp(30);//tone(tonepin,2400,30);
      ConfigWiresMenu.switch_focus(true);
      menu_system.update();
    }
    if(var == BT_RIGHT){
      Bbipp(30);// tone(tonepin,2400,30);
      ConfigWiresMenu.call_function(increase);
      menu_system.update();
    }
    if(var == BT_LEFT){ //inc last param in case of menu item is added
      Bbipp(30);//tone(tonepin,2400,30);
      ConfigWiresMenu.call_function(decrease);
      menu_system.update();
    }
    if(var == BT_SEL){
      Bbipp(30); //tone(tonepin,2400,30);
      gameLauncher();
    }
    if(var == BT_CANCEL){
      Bbipp(30); //tone(tonepin,2400,30);
      menuPrincipal();
    } 
  }
}


void blankFunction() {
    return;
}


void menu_inc_gametime(){
  GAMEMINUTES++;
}


void menu_inc_EEPROM_gametime() {
  EEPROM_GAMEMINUTES++;
}


void menu_dec_gametime(){
  if (GAMEMINUTES >= 2)GAMEMINUTES--;
}


void menu_dec_EEPROM_gametime() {
  if (EEPROM_GAMEMINUTES >= 2)EEPROM_GAMEMINUTES--;
}


void menu_inc_bombtime(){
  if (BOMBMINUTES < 1000)BOMBMINUTES++;
}


void menu_inc_EEPROM_bombtime() {
  if (EEPROM_BOMBMINUTES < 1000)EEPROM_BOMBMINUTES++;
}


void menu_dec_bombtime(){
  if (BOMBMINUTES >= 2)BOMBMINUTES--;
}


void menu_dec_EEPROM_bombtime() {
  if (EEPROM_BOMBMINUTES >= 2)EEPROM_BOMBMINUTES--;
}


void menu_inc_armtime(){
  if (ACTIVATESECONDS < 1000)ACTIVATESECONDS++;
}


void menu_inc_EEPROM_armtime() {
  if (EEPROM_ACTIVATESECONDS < 1000)EEPROM_ACTIVATESECONDS++;
}


void menu_dec_armtime(){
  if (ACTIVATESECONDS >= 2)ACTIVATESECONDS--;
}


void menu_dec_EEPROM_armtime() {
  if (EEPROM_ACTIVATESECONDS >= 2)EEPROM_ACTIVATESECONDS--;
}

void menu_inc_hotWire(){
  if (detonationWire < 4)detonationWire++;
}
void menu_dec_hotWire(){
  if (detonationWire > 1)detonationWire--;
}


void menu_inc_defuseWire(){
  if (defuseWire < 4)defuseWire++;
  if (defuseWire == detonationWire){
    if (defuseWire < 4)defuseWire++;
    else defuseWire--;
  }
  
}


void menu_dec_defuseWire(){
  if (defuseWire > 1)defuseWire--;
  if (defuseWire == detonationWire) {
    if (defuseWire > 1)defuseWire--;
    else defuseWire++;
  }
}


void switch_autoArm() {
  if (autoArm == false) {
    autoArm = true;
  } else {
    autoArm = false;
  }
}


void blink_autoArm() {
  switch_autoArm();
  if (autoArm == false) {
    strncpy(autoarm_text, string_off, strlen(string_off) + 1);
  } else {
    strncpy(autoarm_text, string_on, strlen(string_on) + 1);
  }
}


void switch_sound() {
  if (soundEnable == false) {
    soundEnable = true;
  } else {
    soundEnable = false;
  }
}


void blink_sound() {
  switch_sound();
  if (soundEnable == false) {
    strncpy(sound_text, string_off, strlen(string_off) + 1);
  } else {
    strncpy(sound_text, string_on, strlen(string_on) + 1);
  }
}


void EEPROM_switch_sound() {
  if (EEPROM_SOUND == false) {
    EEPROM_SOUND = true;
  } else {
    EEPROM_SOUND = false;
  }
}


void EEPROM_blink_sound() {
  EEPROM_switch_sound();
  if (EEPROM_SOUND == false) {
    strncpy(EEPROM_sound_text, string_off, strlen(string_off) + 1);
  } else {
    strncpy(EEPROM_sound_text, string_on, strlen(string_on) + 1);
  }
}


void switch_pass() {
  if (passwordEnable == false) {
    passwordEnable = true;
  } else {
    passwordEnable = false;
  }
}


void blink_pass() {
  switch_pass();
  if (passwordEnable == false) {
    strncpy(pass_text, string_off, strlen(string_off) + 1);
  } else {
    strncpy(pass_text, string_on, strlen(string_on) + 1);
  }
  if (passwordEnable == true) {
    setNewPass();
  }
}


void gameLauncher() {
  if (sdStatus == true) {
    startGameCount();
    search();
  } else if (saStatus == true){
    startGameCount();
    sabotage();
  } else if (doStatus == true) {
    startGameCount();
    domination();
  } else if (doStatus_lora == true){
    if (lora_send(50, GAMEMINUTES, 0) == true and lora_send(51, ACTIVATESECONDS, 0) == true) {
        startGameCount();
        loraCallRemote(13,0);
        LoRa_domination();
    } else {
        cls();
        lcd.print(F("Error 104!!"));
        Bbipp(5000);//tone(tonepin,2400,30);
    }
  } else if (wcStatus == true) {
    if (wcMenuWizard == false) {
      wcMenuWizard = true;
      menuConfig_wires();
    } else {
      distribWires();
      quickCheckWires();
      if (passwordEnable == false) {
        passwordEnable = true;
        setNewPass();
      }
      startGameCount();
      wireCut();
    } 
  } else if (bpStatus == true) { 
  //   startGameCount();
//     pushButton();
  }
}


void sdStart() {
  sdStatus=true;
  saStatus=false;
  doStatus=false;
  doStatus_lora = false;
  wcStatus=false;
  bpStatus=false;
  //configQuickGame();
  menuGameconfig();
}


void saStart() {
  sdStatus=false;
  saStatus=true;
  doStatus=false;
  doStatus_lora = false;
  wcStatus=false;
  bpStatus=false;
  menuGameconfig();
}


void doStart() {
  sdStatus=false;
  saStatus=false;
  doStatus=true;
  doStatus_lora = false;
  wcStatus=false;
  bpStatus=false;
  menuGameconfig();
}


void doStart_lora() {
  sdStatus=false;
  saStatus=false;
  doStatus=false;
  doStatus_lora = true;
  wcStatus=false;
  bpStatus=false;
  menuGameconfig();
}


void wcStart() {
  sdStatus=false;
  saStatus=false;
  doStatus=false;
  doStatus_lora = false;
  wcStatus=true;
  bpStatus=false;
  menuGameconfig();
}


void bpStart() {
  sdStatus=false;
  saStatus=false;
  doStatus=false;
  doStatus_lora = false;
  wcStatus=false;
  bpStatus=true;
  menuGameconfig();
}


void StartUpCheck() {
    cls();
    lcd.setCursor(5, 1);
    lcd.print(F("Startup"));
    lcd.setCursor(8, 2);
    lcd.print(F("check"));
    delay(500);
    cls();


    

    // Check battery
    enableBattery = batteryManager.startupEnable_battery(); // Set battery enabled status 
    // Check NFC
    nfcEnabled = isNFCPresent(); // Set NFC enabled status and display firmware version
    // Check LoRa
    loraEnabled = detectLoRaModule(); // Set LoRa enabled status and display configuration

    // Final check
    lcd.setCursor(0, 3);
    if (loraEnabled && nfcEnabled && enableBattery) {
        // All checks passed - no attention needed
        lcd.setCursor(9, 3);
        lcd.print(F("Starting..."));
        delay(500);
    } else {
        lcd.print(F("     Press any key.."));
        // Wait until a key is pressed
        byte key = keypad.getKey();
        while (key == NO_KEY) {
            key = keypad.getKey();
            runLoop();
        }
    } 
}


void batteryPage() {
  batteryManager.displayBatteryPage();
}