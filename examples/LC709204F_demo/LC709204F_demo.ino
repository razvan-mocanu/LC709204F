/**
 * @file LC709204F_demo.ino
 * @author Razvan Mocanu <razvan@mocanu.biz>
 * @version 1.0.0
 * @details LC709204F Battery Monitor Example
 * @copyright MIT (see LICENSE.md)
 */

#include "LC709204F.h"

LC709204F batteryMonitor;

void setup() {
    Serial.begin(115200);
    delay(10);
    Serial.println("\nLC709204F battery monitor demo.");

    if (!batteryMonitor.init(
            lc709204f_apa_adjustment_t::LC709204F_APA_1000MAH, // 1000 mAh cell
            lc709204f_battery_profile_t::LC709204F_BATTERY_PROFILE_3_7_V) // 3.7V cell, charging at 4.2V
            ) {
        Serial.println("Couldn't find LC709204F battery monitor!");
        while (1) delay(1000);
    }

    Serial.println("Found LC709204F.");

    /*
    The temperature measurement settings are specific to the RedPill+ (ESP32-S3) board,
    which contains an onboard thermistor for measuring the ESP32 temperature
    and is powered by a battery with internal thermistor.

    Remove the thermistors setting if you have no thermistors in your setup.
    */
    Serial.println("Setting ESP32 thermistor.");
    if (!batteryMonitor.setTSENSE1ThermistorB(3950)) {
        Serial.println("Couldn't set ThermistorB for TSENSE1!");
        while (1) delay(1000);
    }

    if (!batteryMonitor.setTSENSE2ThermistorB(3380)) {
        Serial.println("Couldn't set ThermistorB for TSENSE2!");
        while (1) delay(1000);
    }

    if (!batteryMonitor.setThermistors(true, true)) {
        Serial.println("Couldn't activate thermistors!");
        while (1) delay(1000);
    }

    Serial.print("BatteryStatus: ");
    Serial.println(batteryMonitor.getBatteryStatus(), HEX);

    Serial.print("ICVersion: ");
    Serial.println(batteryMonitor.getICVersion(), HEX);

    Serial.print("StateOfHealth: ");
    Serial.println(batteryMonitor.getStateOfHealth(), HEX);
}

void loop() {

    int16_t bstatus = batteryMonitor.getBatteryStatus();

    if ((bstatus & 0x80) == 0x80) {
        Serial.println("Resetting...");
        if (!batteryMonitor.init(lc709204f_apa_adjustment_t::LC709204F_APA_1000MAH, lc709204f_battery_profile_t::LC709204F_BATTERY_PROFILE_3_7_V)) {
            Serial.println("Couldn't reset LC709204F battery monitor!");
            while (1) delay(1000);
        }
    }

    Serial.print("CellVoltage: ");
    Serial.print(batteryMonitor.getCellVoltage());
    Serial.print("mV\t");

    Serial.print("RSOC: "); // Relative State Of Charge
    Serial.print(batteryMonitor.getRSOC());
    Serial.print("%\t");

    /*
    TimeToEmpty returns 0xFFFF until the discharge time can be estimated.
    This requires the device to work on battery power until the battery gets
    discharged by 10% (Eg: From 90% to 80%).
    */
    Serial.print("TimeToEmpty: ");
    Serial.print(batteryMonitor.getTimeToEmpty());
    Serial.print("min.\t");

    // Remove temperature readings if you have no thermistors in your setup.
    Serial.print("CellTemperature: ");
    Serial.print(batteryMonitor.getCellTemperature());
    Serial.print("°C\t");

    Serial.print("AmbientTemperature / RedPill+ temp.: ");
    Serial.print(batteryMonitor.getAmbientTemperature());
    Serial.println("°C");

    delay(5000); // Read every 5 seconds.
}
