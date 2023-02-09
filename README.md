# LC709204F
Arduino library for the LC709204F battery monitor.

This library was developed and tested on the RedPill+ (ESP32-S3) module, which has an
onboard LC709204F and onboard thermistor.

RedPill+ (ESP32-S3) is available at [munich-labs.com](https://munich-labs.com).
<hr>

## Contents

  - [Installation](#installation)
  - [Usage](#usage)
  - [Functions](#functions)
  - [Credits](#credits)
  - [License](#license)
<hr>

## Installation
To install the library in the Arduino IDE, open `Tools > Manage libraries...`,
type LC709204F in the search box and click Install.
<br>

## Usage
The library comes with and example which is a good starting point.

Include the library in your project and define the battery monitor:

`#include "LC709204F.h"`

`LC709204F batteryMonitor;`

In the setup section initialize the LC709204F according to your setup:

`batteryMonitor.init(lc709204f_apa_adjustment_t::LC709204F_APA_1000MAH, lc709204f_battery_profile_t::LC709204F_BATTERY_PROFILE_3_7_V)`

Use the batteryMonitor to read/write parameter:

`batteryMonitor.getCellVoltage()`

The `init` requires two parameters:

* APA (Adjustment Pack Application): Sets the battery capacity
* Change Of The Parameter: Sets the battery profile/type

There are predefined values for both these parameters.

<details><summary>For APA, chose one of the following values (or the closest one to your battery):</summary>
<p>

* LC709204F_APA_50MAH
* LC709204F_APA_100MAH
* LC709204F_APA_200MAH
* LC709204F_APA_500MAH
* LC709204F_APA_1000MAH
* LC709204F_APA_2000MAH
* LC709204F_APA_3000MAH
* LC709204F_APA_4000MAH
* LC709204F_APA_5000MAH
* LC709204F_APA_6000MAH
</p>
<hr>
</details>

<details><summary>For the battery profile, chose one of the following values:</summary>
<p>

* LC709204F_BATTERY_PROFILE_3_7_V (3.7V charging at 4.2V)
* LC709204F_BATTERY_PROFILE_UR18650ZY_PANASONIC (UR18650ZY (Panasonic))
* LC709204F_BATTERY_PROFILE_ICR18650_26H_SAMSUNG (ICR18650−26H (SAMSUNG))
* LC709204F_BATTERY_PROFILE_3_8_V (3.8V charging at 4.35V)
* LC709204F_BATTERY_PROFILE_3_85_V (3.85V charging at 4.4V)
</p>
<hr>
</details>
<hr>

## Functions

<details><summary>getTimeToEmpty()</summary>
<p>
Displays estimated time to empty.

* Range: 0x0000 to 0xFFFF
* Unit: minutes
* Initial value: 0xFFFF
* Return: 16-bit value read from LC709204F_REG_TIME_TO_EMPTY register.
</p>
<hr>
</details>

<details><summary>setBeforeRSOC(lc709204f_before_rsoc_t beforeRSOC)</summary>
<p>
Optional Command, especially for obtaining the voltage with intentional timing after power on reset.

* Param values:
  * LC709204F_BEFORE_RSOC_FIRST_SAMPLING
  * LC709204F_BEFORE_RSOC_SECOND_SAMPLING
  * LC709204F_BEFORE_RSOC_THIRD_SAMPLING
  * LC709204F_BEFORE_RSOC_FOURTH_SAMPLING
* Param: beforeRSOC 16-bit value to write to LC709204F_REG_BEFORE_RSOC register
* Return: True on I2C command success
</p>
<hr>
</details>

<details><summary>getTimeToFull()</summary>
<p>
Displays estimated time to full. It will require charging 10% for the TimeToFull to be estimated. (Eg: from 20% to 30%)

* Range: 0x0000 to 0xFFFF
* Unit: minutes
* Initial value: 0xFFFF
* Return: 16-bit value read from LC709204F_REG_TIME_TO_FULL register
</p>
<hr>
</details>

<details><summary>getTSENSE1ThermistorB()</summary>
<p>
Gets B−constant of the TSENSE1 thermistor.

* Range: 0x0000 to 0xFFFF
* Unit: K
* Initial value: 0x0D34 (3380K)
* Return: 16-bit value read from LC709204F_REG_TSENSE1_THERMISTOR_B register
</p>
<hr>
</details>

<details><summary>setTSENSE1ThermistorB(uint16_t b)</summary>
<p>
Sets B−constant of the TSENSE1 thermistor.

* Range: 0x0000 to 0xFFFF
* Unit: K
* Initial value: 0x0D34 (3380K)
* Param: b The value to set it to
* Return: True on successful I2C write
</p>
<hr>
</details>

<details><summary>setInitialRSOC()</summary>
<p>
Initialize RSOC with current voltage.

* Return: True on successful I2C write
</p>
<hr>
</details>

<details><summary>getCellTemperatureTSENSE1()</summary>
<p>
Displays Cell Temperature.

* Range: 0x0980 to 0x0DCC (-30°C to 80°C)
* Unit: 0.1K
* Initial value: 0x0BA6 (25°C)
* Return: 16-bit value read from LC709204F_REG_CELL_TEMPERATURE_TSENSE1 register
</p>
<hr>
</details>

<details><summary>getCellTemperature()</summary>
<p>
Displays Cell Temperature in °C.

* Range: -30°C to 80°C
* Unit: °C
* Initial value: 25°C
* Return: Floating point value from -30°C to 80°C
</p>
<hr>
</details>

<details><summary>setCellTemperatureTSENSE1(uint16_t b)</summary>
<p>
Sets Cell Temperature when working in I2C mode.

* Range: 0x0980 to 0x0DCC (-30°C to 80°C)
* Unit: 0.1K
* Initial value: 0x0BA6 (25°C)
* Param: b The value to set it to
* Return: True on successful I2C write
</p>
<hr>
</details>

<details><summary>setCellTemperature(float temperature)</summary>
<p>
Sets Cell Temperature in °C when working in I2C mode.

* Range: -30°C to 80°C
* Unit: °C
* Initial value: 25°C
* Param: b The value to set it to
* Return: True on successful I2C write
</p>
<hr>
</details>

<details><summary>getCellVoltage()</summary>
<p>
Displays Cell Voltage.

* Range: 0x09C4 to 0x1388 (2500mV to 5000mV)
* Unit: mV
* Return: 16-bit value read from LC709204F_REG_CELL_VOLTAGE register
</p>
<hr>
</details>

<details><summary>getCurrentDirection()</summary>
<p>
Selects Auto/Charge/Discharge mode.

* Range:
  * 0x0000: Auto mode
  * 0x0001: Charge mode
  * 0xFFFF: Discharge mode
* Initial value: 0x0000
* Return: 16-bit value read from LC709204F_REG_CURRENT_DIRECTION register
</p>
<hr>
</details>

<details><summary>setCurrentDirection(lc709204f_current_direction_t currentDirection)</summary>
<p>
Selects Auto/Charge/Discharge mode.

* Range:
  * 0x0000: Auto mode
  * 0x0001: Charge mode
  * 0xFFFF: Discharge mode
* Initial value: 0x0000
* Param: b 16-bit value to write to LC709204F_REG_CURRENT_DIRECTION register
  * LC709204F_CURRENT_DIRECTION_AUTO_MODE
  * LC709204F_CURRENT_DIRECTION_CHARGE_MODE
  * LC709204F_CURRENT_DIRECTION_DISCHARGE_MODE
* Return: True on I2C command success
</p>
<hr>
</details>

<details><summary>getAPA()</summary>
<p>
Gets Adjustment parameter.

* Range: 0x0000 - 0xFFFF
* Return: 16-bit value read from LC709204F_REG_APA register
</p>
<hr>
</details>

<details><summary>setAPA(lc709204f_apa_adjustment_t APAAdjustment)</summary>
<p>
Sets Adjustment parameter.

* Range: 0x0000 - 0xFFFF
* Param: b 16-bit value to write to LC709204F_REG_APA register
  * LC709204F_APA_50MAH
  * LC709204F_APA_100MAH
  * LC709204F_APA_200MAH
  * LC709204F_APA_500MAH
  * LC709204F_APA_1000MAH
  * LC709204F_APA_2000MAH
  * LC709204F_APA_3000MAH
  * LC709204F_APA_4000MAH
  * LC709204F_APA_5000MAH
  * LC709204F_APA_6000MAH
* Return: True on I2C command success
</p>
<hr>
</details>

<details><summary>getAPT()</summary>
<p>
Get the value to adjust temperature measurement delay timing.

* Range: 0x0000 - 0xFFFF
* Initial value: 0x001E
* Return: 16-bit value read from LC709204F_REG_APT register
</p>
<hr>
</details>

<details><summary>setAPT(uint16_t b)</summary>
<p>
Sets a value to adjust temperature measurement delay timing.

* Range: 0x0000 - 0xFFFF
* Initial value: 0x001E
* Param: b 16-bit value to write to LC709204F_REG_APT register
* Return: True on I2C command success
</p>
<hr>
</details>

<details><summary>getRSOC()</summary>
<p>
Displays RSOC (Relative State Of Charge) value based on a 0−100 scale.

* Range: 0x0000 - 0x0064
* Unit: %
* Return: 16-bit value read from LC709204F_REG_RSOC register
</p>
<hr>
</details>

<details><summary>setRSOC(uint16_t b)</summary>
<p>
Gets RSOC (Relative State Of Charge) value based on a 0−100 scale.

* Range: 0x0000 - 0x0064
* Unit: %
* Param: b 16-bit value to write to LC709204F_REG_RSOC register
* Return: True on I2C command success
</p>
<hr>
</details>

<details><summary>getTSENSE2ThermistorB()</summary>
<p>
Gets B−constant of the TSENSE2 thermistor.

* Range: 0x0000 to 0xFFFF
* Unit: K
* Initial value: 0x0D34 (3380K)
* Return: 16-bit value read from LC709204F_REG_TSENSE2_THERMISTOR_B register
</p>
<hr>
</details>

<details><summary>setTSENSE2ThermistorB(uint16_t b)</summary>
<p>
Sets B−constant of the TSENSE2 thermistor.

* Range: 0x0000 to 0xFFFF
* Unit: K
* Initial value: 0x0D34 (3380K)
* Param: b The value to set it to
* Return: True on successful I2C write
</p>
<hr>
</details>

<details><summary>getITE()</summary>
<p>
Displays RSOC value based on a 0−1000 scale.

* Range: 0x0000 to 0x03E8 (0.0% - 100.0%)
* Unit: 0.1%
* Return: Floating point value from 0.0% to 100.0%
</p>
<hr>
</details>

<details><summary>getICVersion()</summary>
<p>
Displays an internal management code.

* Range: 0x0000 to 0xFFFF
* Return: 16-bit value read from LC709204F_REG_IC_VERSION register
</p>
<hr>
</details>

<details><summary>getChangeOfTheParameter()</summary>
<p>
Gets the battery profile.

* Range: 0x0000 to 0x0004
* @return 16-bit value read from LC709204F_REG_CHANGE_OF_THE_PARAMETER register
</p>
<hr>
</details>

<details><summary>setChangeOfTheParameter(lc709204f_battery_profile_t batteryProfile)</summary>
<p>
Selects a battery profile.

* Range: 0x0000 to 0x0004
* Initial value: 0x0000
* Param: batteryProfile The value to set it to
  * LC709204F_BATTERY_PROFILE_3_7_V
  * LC709204F_BATTERY_PROFILE_UR18650ZY_PANASONIC
  * LC709204F_BATTERY_PROFILE_ICR18650_26H_SAMSUNG
  * LC709204F_BATTERY_PROFILE_3_8_V
  * LC709204F_BATTERY_PROFILE_3_85_V
* Return: True on successful I2C write
</p>
<hr>
</details>

<details><summary>getAlarmLowRSOC()</summary>
<p>
Gets RSOC threshold to generate Alarm signal.

* Range:
  * 0x0000: Disable
  * 0x0001 to 0x0064: Threshold (1% to 100%)
* Unit: %
* Initial value: 0x0000
* Return: 16-bit value read from LC709204F_REG_ALARM_LOW_RSOC register
</p>
<hr>
</details>

<details><summary>setAlarmLowRSOC(uint16_t b)</summary>
<p>
Sets RSOC threshold to generate Alarm signal.

* Range:
  * 0x0000: Disable
  * 0x0001 to 0x0064: Threshold (1% to 100%)
* Unit: %
* Initial value: 0x0000
* Param: b The value to set it to
* Return: True on successful I2C write
</p>
<hr>
</details>

<details><summary>getAlarmLowCellVoltage()</summary>
<p>
Gets Voltage threshold to generate Low Cell Voltage Alarm signal.

* Range:
  * 0x0000: Disable
  * 0x09C4 to 0x1388: Threshold (2.5V to 5V)
* Unit: mV
* Initial value: 0x0000
* Return: 16-bit value read from LC709204F_REG_ALARM_LOW_CELL_VOLTAGE register
</p>
<hr>
</details>

<details><summary>setAlarmLowCellVoltage(uint16_t b)</summary>
<p>
Sets Voltage threshold to generate Low Cell Voltage Alarm signal.

* Range:
    * 0x0000: Disable
    * 0x09C4 to 0x1388: Threshold (2.5V to 5V)
* Unit: mV
* Initial value: 0x0000
* Param: b The value to set it to
* Return: True on successful I2C write
</p>
<hr>
</details>

<details><summary>getICPowerMode()</summary>
<p>
Gets Power mode.

* Range:
  * 0x0001: Operational mode
  * 0x0002: Sleep mode
* Initial value: 0x0002
* Return: 16-bit value read from LC709204F_REG_IC_POWER_MODE register
</p>
<hr>
</details>

<details><summary>setICPowerMode(lc709204f_power_mode_t powerMode)</summary>
<p>
Sets Power mode.

* Range:
  * LC709204F_POWER_MODE_OPERATE
  * LC709204F_POWER_MODE_SLEEP
* Initial value: LC709204F_POWER_MODE_SLEEP
* Param: powerMode The value to set it to
* Return: True on successful I2C write
</p>
<hr>
</details>

<details><summary>getStatusBit()</summary>
<p>
Gets TSENSE1 and TSENSE2 thermistors.

* Range: 0x0000 - 0x0003
  * BIT0: Controls TSENSE1 thermistor
  * BIT1: Controls TSENSE2 thermistor
* Initial value: 0x0000
* Return: 16-bit value read from LC709204F_REG_STATUS_BIT register
</p>
<hr>
</details>

<details><summary>setStatusBit(uint16_t b)</summary>
<p>
Sets working mode for TSENSE1 and TSENSE2.

* Range: 0x0000 - 0x0003
  * BIT0: Controls TSENSE1 thermistor
  * BIT1: Controls TSENSE2 thermistor
* Initial value: 0x0000
* Param: b 16-bit value to write to LC709204F_REG_STATUS_BIT register
* Return: True on I2C command success
</p>
<hr>
</details>

<details><summary>setThermistors(bool tsense1, bool tsense2)</summary>
<p>
Sets working mode for TSENSE1 and TSENSE2.

* Initial value: TSENSE1 and TSENSE2 both in I2C mode
* Param: tsense1 boolean - true for thermistor, false for I2C
* Param: tsense2 boolean - true for thermistor, false for I2C
* Return: True on I2C command success
</p>
<hr>
</details>

<details><summary>getCycleCount()</summary>
<p>
Displays cycle count.

* Range: 0x0000 - 0xFFFF
* Unit: count
* Initial value: 0x0000
* Return: 16-bit value read from LC709204F_REG_CYCLE_COUNT register
</p>
<hr>
</details>

<details><summary>getBatteryStatus()</summary>
<p>
Displays various kinds of alarm and estimated state of the battery.

* Range: 0x0000 - 0xFFFF
* Initial value: 0x00C0
* Return: 16-bit value read from LC709204F_REG_BATTERY_STATUS register
</p>
<hr>
</details>

<details><summary>setBatteryStatus(uint16_t b)</summary>
<p>
Sets various kinds of alarm and estimated state of the battery.

* Range: 0x0000 - 0xFFFF
* Initial value: 0x00C0
* Param: b 16-bit value to write to LC709204F_REG_BATTERY_STATUS register
* Return: True on I2C command success
</p>
<hr>
</details>

<details><summary>getNumberOfTheParameter()</summary>
<p>
Displays Battery profile code.

* Range: 0x0000 - 0xFFFF
* Return: 16-bit value read from LC709204F_REG_NUMBER_OF_THE_PARAMETER register
</p>
<hr>
</details>

<details><summary>getTerminationCurrentRate()</summary>
<p>
Gets termination current rate.

* Range: 0x0002 - 0x001E
* Unit: 0.01C
* Initial value: 0x0002
* Return: 16-bit value read from LC709204F_REG_TERMINATION_CURRENT_RATE register
</p>
<hr>
</details>

<details><summary>setTerminationCurrentRate(uint16_t b)</summary>
<p>
Sets termination current rate.

* Range: 0x0002 - 0x001E
* Unit: 0.01C
* Initial value: 0x0002
* Param: b 16-bit value to write to LC709204F_REG_TERMINATION_CURRENT_RATE register
* Return: True on I2C command success
</p>
<hr>
</details>

<details><summary>getEmptyCellVoltage()</summary>
<p>
Gets empty cell voltage.

* Range:
  * 0x0000: Disable
  * 0x09C4 - 0x1388 (2.5V to 5V)
* Unit: mV
* Initial value: 0x0000
* Return 16-bit value read from LC709204F_REG_EMPTY_CELL_VOLTAGE register
</p>
<hr>
</details>

<details><summary>setEmptyCellVoltage(uint16_t b)</summary>
<p>
Sets empty cell voltage.

* Range:
  * 0x0000: Disable
  * 0x09C4 - 0x1388 (2.5V to 5V)
* Unit: mV
* Initial value: 0x0000
* Param: b 16-bit value to write to LC709204F_REG_EMPTY_CELL_VOLTAGE register
* Return: True on I2C command success
</p>
<hr>
</details>

<details><summary>getITEOffset()</summary>
<p>
Gets ITE so that RSOC is 0%.

* Range: 0x0000 - 0x03E8 (0.0% to 100.0%)
* Unit: 0.1%
* Initial value: 0x0000
* Return: 16-bit value read from LC709204F_REG_ITE_OFFSET register
</p>
<hr>
</details>

<details><summary>getAlarmHighCellVoltage()</summary>
<p>
Gets Voltage threshold to generate High Cell Voltage Alarm signal.

* Range:
  * 0x0000: Disable
  * 0x09C4 - 0x1388 (2.5V to 5V)
* Unit: mV
* Initial value: 0x0000
* Return: 16-bit value read from LC709204F_REG_ALARM_HIGH_CELL_VOLTAGE register
</p>
<hr>
</details>

<details><summary>setAlarmHighCellVoltage(uint16_t b)</summary>
<p>
Sets Voltage threshold to generate High Cell Voltage Alarm signal.

* Range:
    * 0x0000: Disable
    * 0x09C4 - 0x1388 (2500mV to 5000mV)
* Unit: mV
* Initial value: 0x0000
* Param: b 16-bit value to write to LC709204F_REG_ALARM_HIGH_CELL_VOLTAGE register
* Return: True on I2C command success
</p>
<hr>
</details>

<details><summary>getAlarmLowTemperature()</summary>
<p>
Gets Voltage threshold to generate Low Temperature alarm signal.

* Range:
  * 0x0000: Disable
  * 0x0980 - 0x0DCC (-30°C to 80°C)
* Unit: °C
* Initial value: 0x0000
* Return: float value read from LC709204F_REG_ALARM_LOW_TEMPERATURE register
</p>
<hr>
</details>

<details><summary>setAlarmLowTemperature(float temp)</summary>
<p>
Sets Voltage threshold to generate Low Temperature alarm signal.

* Range:
  * 0x0000: Disable
  * 0x0980 - 0x0DCC (-30°C to 80°C)
* Unit: °C
* Initial value: 0x0000
* Param: temp float value to write to LC709204F_REG_ALARM_LOW_TEMPERATURE register
* Return: True on I2C command success
</p>
<hr>
</details>

<details><summary>getAlarmHighTemperature()</summary>
<p>
Gets Voltage threshold to generate High Temperature alarm signal.

* Range:
  * 0x0000: Disable
  * 0x0980 - 0x0DCC (-30°C to 80°C)
* Unit: °C
* Initial value: 0x0000
* Return: float value read from LC709204F_REG_ALARM_HIGH_TEMPERATURE register
</p>
<hr>
</details>

<details><summary>setAlarmHighTemperature(float temp)</summary>
<p>
Sets Voltage threshold to generate High Temperature alarm signal.

* Range:
    * 0x0000: Disable
    * 0x0980 - 0x0DCC (-30°C to 80°C)
* Unit: °C
* Initial value: 0x0000
* Param: temp float value to write to LC709204F_REG_ALARM_HIGH_TEMPERATURE register
* Return: True on I2C command success
</p>
<hr>
</details>

<details><summary>getTotalRunTime()</summary>
<p>
Gets operating time.

* 0x24 - Lower 16bit
* 0x25 - Higher 8bit
* Range: 0x00000000 - 0x00FFFFFF
* Unit: minutes
* Initial value: 0x0000
* Return: 32-bit value read from LC709204F_REG_TOTAL_RUN_TIME_LOWER_16BIT and LC709204F_REG_TOTAL_RUN_TIME_HIGHER_8BIT registers
</p>
<hr>
</details>

<details><summary>setTotalRunTime(uint32_t b)</summary>
<p>
Sets operating time.

* 0x24 - Lower 16bit
* 0x25 - Higher 8bit
* Range: 0x00000000 - 0x00FFFFFF
* Unit: minutes
* Initial value: 0x0000
* Param: b 32-bit value to write to LC709204F_REG_TOTAL_RUN_TIME_LOWER_16BIT and LC709204F_REG_TOTAL_RUN_TIME_HIGHER_8BIT registers
* Return: True on I2C command success</p>
<hr>
</details>

<details><summary>getAccumulatedTemperature()</summary>
<p>
Gets accumulated temperature.

* 0x26 - Lower 16bit
* 0x27 - Higher 16bit
* Range: 0x00000000 - 0xFFFFFFFF
* Unit: 2K
* Initial value: 0x0000
* Return: 32-bit value read from LC709204F_REG_ACCUMULATED_TEMPERATURE_LOWER_16BIT and LC709204F_REG_ACCUMULATED_TEMPERATURE_HIGHER_16BIT registers
<hr>
</details>

<details><summary>setAccumulatedTemperature(uint32_t b)</summary>
<p>
Sets accumulated temperature.

* 0x26 - Lower 16bit
* 0x27 - Higher 16bit
* Range: 0x00000000 - 0xFFFFFFFF
* Unit: 2K
* Initial value: 0x0000
* Param: b 32-bit value to write to LC709204F_REG_ACCUMULATED_TEMPERATURE_LOWER_16BIT and LC709204F_REG_ACCUMULATED_TEMPERATURE_HIGHER_16BIT registers
* Return: True on I2C command success
<hr>
</details>

<details><summary>getAccumulatedRSOC()</summary>
<p>
Gets accumulated RSOC.

* 0x28 - Lower 16bit
* 0x29 - Higher 16bit
* Range: 0x00000000 - 0xFFFFFFFF
* Unit: % minutes
* Initial value: 0x0000
* Return: 32-bit value read from LC709204F_REG_ACCUMULATED_RSOC_LOWER_16BIT and LC709204F_REG_ACCUMULATED_RSOC_HIGHER_16BIT registers
<hr>
</details>

<details><summary>setAccumulatedRSOC(uint32_t b)</summary>
<p>
Sets accumulated RSOC.

* 0x28 - Lower 16bit
* 0x29 - Higher 16bit
* Range: 0x00000000 - 0xFFFFFFFF
* Unit: % minutes
* Initial value: 0x0000
* Param: b 32-bit value to write to LC709204F_REG_ACCUMULATED_RSOC_LOWER_16BIT and LC709204F_REG_ACCUMULATED_RSOC_HIGHER_16BIT registers
* Return: True on I2C command success
<hr>
</details>

<details><summary>getMaximumCellVoltage()</summary>
<p>
Gets the maximum historical Cell Voltage.

* Range: 0x09C4 - 0x1388 (2.5V to 5V)
* Unit: mV
* Initial value: 0x0000
* Return: 16-bit value read from LC709204F_REG_MAXIMUM_CELL_VOLTAGE register
<hr>
</details>

<details><summary>setMaximumCellVoltage(uint16_t b)</summary>
<p>
Sets the maximum historical Cell Voltage.

* Range: 0x09C4 - 0x1388 (2500mV to 5000mV)
* Unit: mV
* Initial value: 0x0000
* Param: b 16-bit value to write to LC709204F_REG_MAXIMUM_CELL_VOLTAGE register
* Return: True on I2C command success
<hr>
</details>

<details><summary>getMinimumCellVoltage()</summary>
<p>
Gets the minimum historical Cell Voltage.

* Range: 0x09C4 - 0x1388 (2500mV to 5000mV)
* Unit: mV
* Initial value: 0x0000
* Return 16-bit value read from LC709204F_REG_MINIMUM_CELL_VOLTAGE register
<hr>
</details>

<details><summary>setMinimumCellVoltage(uint16_t b)</summary>
<p>
Sets the minimum historical Cell Voltage.

* Range: 0x09C4 - 0x1388 (2500mV to 5000mV)
* Unit: mV
* Initial value: 0x0000
* Param: b 16-bit value to write to LC709204F_REG_MINIMUM_CELL_VOLTAGE register
* Return: True on I2C command success
<hr>
</details>

<details><summary>getMaximumCellTemperatureTSENSE1()</summary>
<p>
Gets  the historical maximum temperature of TSENSE1.

* Range: 0x0980 to 0x0DCC (-30°C to 80°C)
* Unit: 0.1K
* Initial value: 0x0980 (-30°C)
* Return: 16-bit value read from LC709204F_REG_MAXIMUM_CELL_TEMPERATURE_TSENSE1 register
<hr>
</details>

<details><summary>getMaximumCellTemperature()</summary>
<p>
Gets  the historical maximum temperature of TSENSE1 in °C.

* Range: -30°C to 80°C
* Unit: °C
* Initial value: -30°C
* Return: Floating point value from -30°C to 80°C
<hr>
</details>

<details><summary>setMaximumCellTemperatureTSENSE1(uint16_t b)</summary>
<p>
Sets  the historical maximum temperature of TSENSE1 in °C.

* Range: 0x0980 to 0x0DCC (-30°C to 80°C)
* Unit: 0.1K
* Initial value: 0x0980 (-30°C)
* Param: b The value to set it to
* Return: True on successful I2C write
<hr>
</details>

<details><summary>setMaximumCellTemperature(float temperature)</summary>
<p>
Sets  the historical maximum temperature of TSENSE1 in °C.

* Range: -30°C to 80°C
* Unit: °C
* Initial value: -30°C
* Param: b The value to set it to
* Return: True on successful I2C write
<hr>
</details>

<details><summary>getMinimumCellTemperatureTSENSE1()</summary>
<p>
Gets the historical minimum temperature of TSENSE1.

* Range: 0x0980 to 0x0DCC (-30°C to 80°C)
* Unit: 0.1K
* Initial value: 0x0DCC (80°C)
* Return: 16-bit value read from LC709204F_REG_MINIMUM_CELL_TEMPERATURE_TSENSE1 register
<hr>
</details>

<details><summary>getMinimumCellTemperature()</summary>
<p>
Gets  the historical minimum temperature of TSENSE1 in °C.

* Range: -30°C to 80°C
* Unit: °C
* Initial value: 80°C
* Return Floating point value from -30°C to 80°C
<hr>
</details>

<details><summary>setMinimumCellTemperatureTSENSE1(uint16_t b)</summary>
<p>
Sets  the historical minimum temperature of TSENSE1.

* Range: 0x0980 to 0x0DCC (-30°C to 80°C)
* Unit: 0.1K
* Initial value: 0x0DCC (80°C)
* Param: b The value to set it to
* Return: True on successful I2C write
<hr>
</details>

<details><summary>setMinimumCellTemperature(float temperature)</summary>
<p>
Sets the historical minimum temperature of TSENSE1 in °C.

* Range: -30°C to 80°C
* Unit: °C
* Initial value: 80°C
* Param: b The value to set it to
* Return: True on successful I2C write
<hr>
</details>

<details><summary>getAmbientTemperatureTSENSE2()</summary>
<p>
Gets the ambient temperature of TSENSE2.

* Range: 0x0980 to 0x0DCC (-30°C to 80°C)
* Unit: 0.1K
* Initial value: 0x0BA6 (25°C)
* Return: 16-bit value read from LC709204F_REG_MINIMUM_CELL_TEMPERATURE_TSENSE1 register
<hr>
</details>

<details><summary>getAmbientTemperature()</summary>
<p>
Gets  the ambient temperature of TSENSE2 in °C.

* Range: -30°C to 80°C
* Unit: °C
* Initial value: 25°C
* Return: Floating point value from -30°C to 80°C
<hr>
</details>

<details><summary>getStateOfHealth()</summary>
<p>
Gets  State of Health of a battery on a 0−100 scale.

* Range: 0x0000 to 0x0064 (0% to 100%)
* Unit: %
* Initial value: 0x0064 (100%)
* Return: 16-bit value read from LC709204F_REG_STATE_OF_HEALTH register
<hr>
</details>

<details><summary>getUserId()</summary>
<p>
Gets 32bit User ID.

* Range: 0x00000000 - 0xFFFFFFFF
* Return: 32-bit value read from LC709204F_REG_USER_ID_LOWER_16BIT and LC709204F_REG_USER_ID_HIGHER_16BIT registers
<hr>
</details>
<hr>

## Credits

Maintainer: Razvan Mocanu
<hr>

## License

MIT © Razvan Mocanu