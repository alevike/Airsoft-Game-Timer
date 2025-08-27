#ifndef BATTERY_MANAGER_H
#define BATTERY_MANAGER_H

#include <Battery.h>
#include <LiquidCrystal.h>

class BatteryManager {
private:
    Battery batt1; // Battery object for cell 1
    Battery batt2; // Battery object for cell 2
    Battery batt3; // Battery object for cell 3

    byte customCharset;

    int minVoltage;               // Minimum voltage for a battery cell (in millivolts)
    int maxVoltage;               // Maximum voltage for a battery cell (in millivolts)
    byte startAlarmPercent;       // Percentage to start warning messages
    byte minStateOfChargePercent; // Percentage to trigger power off
    int powerOffPin;              // Pin used for power off
    bool powerOffLogic;           // Logic level for power off (HIGH or LOW)

    void drawPercentage(byte percent, byte row, byte column, byte barSize);

public:
    BatteryManager();
    BatteryManager(int minVoltage, int maxVoltage, byte startAlarmPercent, byte minStateOfChargePercent, int powerOffPin, bool powerOffLogic);

    void begin(); // Initialize the battery objects
    void checkBattery();
    bool isBatteryEnabled();
    bool startupEnable_battery();
    void quickCheck();
    void displayBatteryPage();
};

#endif // BATTERY_MANAGER_H