#include "LC709204F.h"

LC709204F batteryMonitor;

void setup() {
    Serial.begin(115200);
    delay(10);
    Serial.println("\nLC709204F battery monitor demo.");

    if (!batteryMonitor.init(lc709204f_apa_adjustment_t::LC709204F_APA_1000MAH, lc709204f_battery_profile_t::LC709204F_BATTERY_PROFILE_3_7_V)) {
        Serial.println("Couldn't find LC709204F battery monitor!");
        while (1) delay(1000);
    }

    Serial.println("Found LC709204F.");

    /*
    This setting is specific to the RedPill+ (ESP32-S3) board,
    which contains on board thermistor for measuring the ESP32 temperature
    and is powered by a battery with internal thermistor.

    Remove the thermistors setting if you have no thermistors in your setup.
    */
    Serial.println("Setting ESP32 thermistor.");
    if (!batteryMonitor.setThermistors(true, true)) {
        Serial.println("Failed to set thermistor!");
    }

    Serial.print("BatteryStatus:");
    Serial.println(batteryMonitor.getBatteryStatus(), HEX);

    Serial.print("ICVersion:");
    Serial.println(batteryMonitor.getICVersion(), HEX);

    Serial.print("StateOfHealth:");
    Serial.println(batteryMonitor.getStateOfHealth(), HEX);
}

void loop() {
    Serial.print("CellVoltage:");
    Serial.print(batteryMonitor.getCellVoltage());
    Serial.print("mV\t");

    Serial.print("RSOC:"); // Relative State Of Charge
    Serial.print(batteryMonitor.getRSOC());
    Serial.print("%\t");

    /*
    TimeToEmpty returns 0xFFFF until the discharge time can be estimated.
    This requires the device to work on battery power until the battery gets
    discharged by 10% (Eg: From 90% to 80%).
    */
    Serial.print("TimeToEmpty:");
    Serial.print(batteryMonitor.getTimeToEmpty());
    Serial.print("minutes\t");

    // Remove temperature readings if you have no thermistors in your setup.
    Serial.print("CellTemperature:");
    Serial.print(batteryMonitor.getCellTemperature());
    Serial.print("°C\t");

    Serial.print("AmbientTemperature / ESP32 temperature for RedPill+:");
    Serial.print("°C");
    Serial.println(batteryMonitor.getAmbientTemperature());

    delay(5000); // Read every 5 seconds.
}
