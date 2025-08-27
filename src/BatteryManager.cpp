#include "BatteryManager.h"
#include "arduinoGlue.h"

extern LiquidCrystal lcd; // Use the existing LCD object
extern void cls();        // Clear screen function
extern void Bbipp(int duration); // Buzzer function
extern void restore_left_focus(); // Restore focus function

// Default constructor
BatteryManager::BatteryManager()
    : batt1(3000, 4200, SENSE_PIN1), // Initialize Battery objects
      batt2(3000, 4200, SENSE_PIN2),
      batt3(3000, 4200, SENSE_PIN3),
      customCharset(0),
      minVoltage(2700),               // Default minimum voltage (in millivolts)
      maxVoltage(4200),               // Default maximum voltage (in millivolts)
      startAlarmPercent(15),          // Default alarm percentage
      minStateOfChargePercent(5),     // Default power-off percentage
      powerOffPin(KEEP_POWER_PIN),    // Default power-off pin
      powerOffLogic(LOW) {}           // Default power-off logic

// Parameterized constructor
BatteryManager::BatteryManager(int minVoltage, int maxVoltage, byte startAlarmPercent, byte minStateOfChargePercent, int powerOffPin, bool powerOffLogic)
    : batt1(minVoltage, maxVoltage, SENSE_PIN1),
      batt2(minVoltage, maxVoltage, SENSE_PIN2),
      batt3(minVoltage, maxVoltage, SENSE_PIN3),
      customCharset(0),
      minVoltage(minVoltage),
      maxVoltage(maxVoltage),
      startAlarmPercent(startAlarmPercent),
      minStateOfChargePercent(minStateOfChargePercent),
      powerOffPin(powerOffPin),
      powerOffLogic(powerOffLogic) {}

void BatteryManager::begin() {
    // Configure the battery objects
    batt1.onDemand(ENABLE_PIN1, HIGH);
    batt2.onDemand(ENABLE_PIN2, HIGH);
    batt3.onDemand(ENABLE_PIN3, HIGH);
    batt1.begin(5000, 2.96, &sigmoidal);
    batt2.begin(5000, 2.96, &sigmoidal);
    batt3.begin(5000, 2.96, &sigmoidal);

    // Configure the power-off pin
    pinMode(powerOffPin, OUTPUT);
    digitalWrite(powerOffPin, !powerOffLogic); // Set to the opposite of power-off logic
}

void BatteryManager::checkBattery() {
    byte batType = 0;
    cls();
    int Cell1_voltage = batt1.voltage();
    int Cell2_voltage = batt2.voltage();
    int Cell3_voltage = batt3.voltage();

    if (Cell1_voltage > minVoltage && Cell2_voltage <= minVoltage && Cell3_voltage <= minVoltage) {
        batType = 1;
        byte Cell1_percent = map(Cell1_voltage, minVoltage, maxVoltage, 0, 100);
        Cell1_percent = constrain(Cell1_percent, 0, 100);
        lcd.setCursor(0, 0);
        lcd.print("B.Type:");
        lcd.print(batType);
        lcd.print("S - ");
        lcd.print(Cell1_percent);
        lcd.print("%");
        lcd.setCursor(0, 1);
        drawPercentage(Cell1_percent, 1, 0, 10);
        enableBattery = true;
    } else if (Cell1_voltage > minVoltage && Cell2_voltage > minVoltage && Cell3_voltage <= minVoltage) {
        batType = 2;
        Cell2_voltage -= Cell1_voltage;
        byte Cell1_percent = map(Cell1_voltage, minVoltage, maxVoltage, 0, 100);
        byte Cell2_percent = map(Cell2_voltage, minVoltage, maxVoltage, 0, 100);
        Cell1_percent = constrain(Cell1_percent, 0, 100);
        Cell2_percent = constrain(Cell2_percent, 0, 100);
        byte batPercent = (Cell1_percent + Cell2_percent) / 2;
        lcd.print(F("B.Type:"));
        lcd.print(batType);
        lcd.print("S - ");
        lcd.print(batPercent);
        lcd.print("%");
        lcd.setCursor(0, 1);
        drawPercentage(batPercent, 1, 0, 10);
        enableBattery = true;
    } else if (Cell1_voltage > minVoltage && Cell2_voltage > minVoltage && Cell3_voltage > minVoltage) {
        batType = 3;
        Cell3_voltage -= Cell2_voltage;
        Cell2_voltage -= Cell1_voltage;
        byte Cell1_percent = map(Cell1_voltage, minVoltage, maxVoltage, 0, 100);
        byte Cell2_percent = map(Cell2_voltage, minVoltage, maxVoltage, 0, 100);
        byte Cell3_percent = map(Cell3_voltage, minVoltage, maxVoltage, 0, 100);
        Cell1_percent = constrain(Cell1_percent, 0, 100);
        Cell2_percent = constrain(Cell2_percent, 0, 100);
        Cell3_percent = constrain(Cell3_percent, 0, 100);
        byte batPercent = (Cell1_percent + Cell2_percent + Cell3_percent) / 3;
        lcd.print(F("B.type:"));
        lcd.print(batType);
        lcd.print(F("S - "));
        lcd.print(batPercent);
        lcd.print(F("%"));
        lcd.setCursor(0, 1);
        drawPercentage(batPercent, 1, 0, 10);
        enableBattery = true;
    } else {
        lcd.setCursor(0, 0);
        lcd.print(F("Batt. read error"));
        lcd.setCursor(0, 1);
        lcd.print(F("Monitoring OFF!"));
        enableBattery = false;
    }
    delay(2000);
}

bool BatteryManager::isBatteryEnabled() {
    bool enBattery = (batt1.voltage() > 100);
    return enBattery;
}

bool BatteryManager::startupEnable_battery() {
    bool enabled = false;
    lcd.setCursor(0, 0);
    lcd.print(F("Battery "));
    if (batteryManager.isBatteryEnabled()) { // Check if the battery is enabled
        // batteryManager.quickCheck(); // Perform a quick battery check
        lcd.setCursor(16, 0);
        lcd.print(F("| OK"));
        enabled = true;
    } else {
        lcd.print(F("N/A!!"));
        lcd.setCursor(16, 0);
        lcd.print(F("|Err"));
        enabled = false;    
    }
    return enabled;
}

void BatteryManager::quickCheck() {
    if (enableBattery) {
        byte batPercent = 0;
        int Cell1_voltage = batt1.voltage();
        int Cell2_voltage = batt2.voltage();
        int Cell3_voltage = batt3.voltage();

        if (Cell1_voltage > minVoltage && Cell2_voltage <= minVoltage && Cell3_voltage <= minVoltage) {
            byte Cell1_percent = map(Cell1_voltage, minVoltage, maxVoltage, 0, 100);
            Cell1_percent = constrain(Cell1_percent, 0, 100);
            batPercent = Cell1_percent;
        } else if (Cell1_voltage > minVoltage && Cell2_voltage > minVoltage && Cell3_voltage <= minVoltage) {
            Cell2_voltage -= Cell1_voltage;
            byte Cell1_percent = map(Cell1_voltage, minVoltage, maxVoltage, 0, 100);
            byte Cell2_percent = map(Cell2_voltage, minVoltage, maxVoltage, 0, 100);
            Cell1_percent = constrain(Cell1_percent, 0, 100);
            Cell2_percent = constrain(Cell2_percent, 0, 100);
            batPercent = (Cell1_percent + Cell2_percent) / 2;
        } else if (Cell1_voltage > minVoltage && Cell2_voltage > minVoltage && Cell3_voltage > minVoltage) {
            Cell3_voltage -= Cell2_voltage;
            Cell2_voltage -= Cell1_voltage;
            byte Cell1_percent = map(Cell1_voltage, minVoltage, maxVoltage, 0, 100);
            byte Cell2_percent = map(Cell2_voltage, minVoltage, maxVoltage, 0, 100);
            byte Cell3_percent = map(Cell3_voltage, minVoltage, maxVoltage, 0, 100);
            Cell1_percent = constrain(Cell1_percent, 0, 100);
            Cell2_percent = constrain(Cell2_percent, 0, 100);
            Cell3_percent = constrain(Cell3_percent, 0, 100);
            batPercent = (Cell1_percent + Cell2_percent + Cell3_percent) / 3;
        }

        if (batPercent < minStateOfChargePercent) {
            cls();
            WSclsRGB(); // Clear the RGB LEDs
            lcd.setCursor(0, 0);
            lcd.print   ("####################");
            lcd.setCursor(0, 1);
            lcd.print (F("# Battery too low! #"));
            lcd.setCursor(0, 2);
            lcd.print   ("#       ");
            lcd.print(batPercent);
                      lcd.print(F("%         #"));
            lcd.setCursor(0, 3);
            lcd.print   ("# Power off in 20s #");
            for (int i = 0; i < 20; i++) {
                Bbipp(100);
                delay(33);
                Bbipp(100);
                delay(33); 
                Bbipp(100);
                delay(33);
                Bbipp(100);
                delay(500);
            }
            digitalWrite(powerOffPin, powerOffLogic); //Turn off the device
        } else if (batPercent < startAlarmPercent) { 
            cls();
            lcd.setCursor(0, 0);
            lcd.print       ("####################");
            lcd.setCursor(0, 1);
            lcd.print(F     ("#   Battery low!   #"));
            lcd.setCursor(0, 2);
            lcd.print       ("#       ");
            lcd.print(batPercent);
            lcd.print(F("%"));
            lcd.setCursor(0, 3);
            lcd.print       ("####################");
            for (int i = 0; i < 5; i++) {
                Bbipp(100);
                delay(30);
            }
            delay(1000);
            cls();
        }
    }
}

void BatteryManager::displayBatteryPage() {
    byte batType = 0;
    cls(); // Clear the screen

    int Cell1_voltage = batt1.voltage();
    int Cell2_voltage = batt2.voltage();
    int Cell3_voltage = batt3.voltage();

    if (Cell1_voltage > minVoltage && Cell2_voltage > minVoltage && Cell3_voltage <= minVoltage) {
        batType = 2; // 2S type battery
        Cell2_voltage -= Cell1_voltage;
        byte Cell1_percent = map(Cell1_voltage, minVoltage, maxVoltage, 0, 100);
        byte Cell2_percent = map(Cell2_voltage, minVoltage, maxVoltage, 0, 100);
        Cell1_percent = constrain(Cell1_percent, 0, 100);
        Cell2_percent = constrain(Cell2_percent, 0, 100);
        byte batPercent = (Cell1_percent + Cell2_percent) / 2;

        lcd.setCursor(0, 0);
        lcd.print(F("Type: "));
        lcd.print(batType);
        lcd.print(F("S "));
        lcd.print(batPercent);
        lcd.print(F("%"));
        drawPercentage(batPercent, 0, 13, 7);

        lcd.setCursor(0, 1);
        lcd.print(F("C1-"));
        lcd.print(Cell1_percent);
        lcd.print(F("%-"));
        lcd.print(Cell1_voltage);
        lcd.print(F("mV"));
        drawPercentage(Cell1_percent, 1, 15, 5);

        lcd.setCursor(0, 2);
        lcd.print(F("C2-"));
        lcd.print(Cell2_percent);
        lcd.print(F("%-"));
        lcd.print(Cell2_voltage);
        lcd.print(F("mV"));
        drawPercentage(Cell2_percent, 2, 15, 5);

        lcd.setCursor(0, 3);
        lcd.print(F("Press any key..."));
    } else if (Cell1_voltage > minVoltage && Cell2_voltage > minVoltage && Cell3_voltage > minVoltage) {
        batType = 3; // 3S type battery
        Cell3_voltage -= Cell2_voltage;
        Cell2_voltage -= Cell1_voltage;
        byte Cell1_percent = map(Cell1_voltage, minVoltage, maxVoltage, 0, 100);
        byte Cell2_percent = map(Cell2_voltage, minVoltage, maxVoltage, 0, 100);
        byte Cell3_percent = map(Cell3_voltage, minVoltage, maxVoltage, 0, 100);
        Cell1_percent = constrain(Cell1_percent, 0, 100);
        Cell2_percent = constrain(Cell2_percent, 0, 100);
        Cell3_percent = constrain(Cell3_percent, 0, 100);
        byte batPercent = (Cell1_percent + Cell2_percent + Cell3_percent) / 3;

        lcd.setCursor(0, 0);
        lcd.print(F("Type: "));
        lcd.print(batType);
        lcd.print(F("S "));
        lcd.print(batPercent);
        lcd.print(F("%"));
        drawPercentage(batPercent, 0, 13, 7);

        lcd.setCursor(0, 1);
        lcd.print(F("C1-"));
        lcd.print(Cell1_percent);
        lcd.print(F("%-"));
        lcd.print(Cell1_voltage);
        lcd.print(F("mV"));
        drawPercentage(Cell1_percent, 1, 15, 5);

        lcd.setCursor(0, 2);
        lcd.print(F("C2-"));
        lcd.print(Cell2_percent);
        lcd.print(F("%-"));
        lcd.print(Cell2_voltage);
        lcd.print(F("mV"));
        drawPercentage(Cell2_percent, 2, 15, 5);

        lcd.setCursor(0, 3);
        lcd.print(F("C3-"));
        lcd.print(Cell3_percent);
        lcd.print(F("%-"));
        lcd.print(Cell3_voltage);
        lcd.print(F("mV"));
        drawPercentage(Cell3_percent, 3, 15, 5);
    } else {
        lcd.setCursor(0, 0);
        lcd.print(F("Battery Error"));
        lcd.setCursor(0, 1);
        lcd.print(F("Check Connections"));
    }

    // Wait for a key press
    byte key = keypad.getKey();
    while (key == NO_KEY) {
        key = keypad.getKey();
    }

    restore_left_focus(); // Restore focus after exiting the battery page
}

void BatteryManager::drawPercentage(byte percent, byte row, byte column, byte barSize) {
    if (customCharset != 1) setup_progressbar();
    lcd.setCursor(column, row);
    byte nb_columns = map(percent, 0, 100, 0, barSize * 2 - 2);

    for (byte i = 0; i < barSize; ++i) {
        if (i == 0) {
            if (nb_columns > 0) {
                lcd.write(1);
                nb_columns -= 1;
            } else {
                lcd.write((byte)0);
            }
        } else if (i == barSize - 1) {
            if (nb_columns > 0) {
                lcd.write(6);
            } else {
                lcd.write(5);
            }
        } else {
            if (nb_columns >= 2) {
                lcd.write(4);
                nb_columns -= 2;
            } else if (nb_columns == 1) {
                lcd.write(3);
                nb_columns -= 1;
            } else {
                lcd.write(2);
            }
        }
    }
}
