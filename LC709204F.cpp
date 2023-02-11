/**
 * @file LC709204F.cpp
 * @author Razvan Mocanu <razvan@mocanu.biz>
 * @version 1.0.1
 * @details LC709204F Battery Monitor library
 * @copyright MIT (see license.txt)
 */

#include "Arduino.h"
#include "LC709204F.h"

/**
 * LC709204F class
 */
LC709204F::LC709204F(TwoWire *theWire) {
    _wire = theWire;
}

LC709204F::~LC709204F(void) {}

/**
 * LC709204F initialization.
 *
 * @param  wire - The Wire object to be used for I2C connections.
 * @return True if initialization was successful, otherwise false.
 */

bool LC709204F::init(lc709204f_apa_adjustment_t APAAdjustment, lc709204f_battery_profile_t batteryProfile, TwoWire *wire) {
    _wire->begin();

    if (!setAPA(APAAdjustment))
        return false;

    if (!setChangeOfTheParameter(batteryProfile))
        return false;

    if (!setICPowerMode(LC709204F_POWER_MODE_OPERATE))
        return false;

    if (!setBatteryStatus(0x0040))
        return false;

    return true;
}

/**
 * Get TimeToEmpty (0x03)
 *
 * Displays estimated time to empty.
 * Range: 0x0000 to 0xFFFF
 * Unit: minutes
 * Initial value: 0xFFFF
 *
 * @return 16-bit value read from LC709204F_REG_TIME_TO_EMPTY register
 */
uint16_t LC709204F::getTimeToEmpty(void) {
    uint16_t val = 0;
    readWord(LC709204F_REG_TIME_TO_EMPTY, &val);
    return val;
}

/**
 * Set BeforeRSOC (0x04)
 *
 * Optional Command, especially for obtaining the voltage with intentional timing after power on reset.
 * Range:
 * - 0xAA55: 1st sampling
 * - 0xAA56: 2nd sampling
 * - 0xAA57: 3rd sampling
 * - 0xAA58: 4th sampling
 *
 * @param beforeRSOC 16-bit value to write to LC709204F_REG_BEFORE_RSOC register
 * @return True on I2C command success
 */
bool LC709204F::setBeforeRSOC(lc709204f_before_rsoc_t beforeRSOC) {
    return writeWord(LC709204F_REG_BEFORE_RSOC, beforeRSOC);
}

/**
 * Get TimeToFull (0x05)
 *
 * Displays estimated time to full.
 * Range: 0x0000 to 0xFFFF
 * Unit: minutes
 * Initial value: 0xFFFF
 *
 * @return 16-bit value read from LC709204F_REG_TIME_TO_FULL register
 */
uint16_t LC709204F::getTimeToFull(void) {
    uint16_t val = 0;
    readWord(LC709204F_REG_TIME_TO_FULL, &val);
    return val;
}

/**
 * Get TSENSE1ThermistorB (0x06)
 *
 * Gets B−constant of the TSENSE1 thermistor.
 * Range: 0x0000 to 0xFFFF
 * Unit: K
 * Initial value: 0x0D34 (3380K)
 *
 * @return 16-bit value read from LC709204F_REG_TSENSE1_THERMISTOR_B register
 */
uint16_t LC709204F::getTSENSE1ThermistorB(void) {
    uint16_t val = 0;
    readWord(LC709204F_REG_TSENSE1_THERMISTOR_B, &val);
    return val;
}

/**
 * Set TSENSE1ThermistorB (0x06)
 *
 * Sets B−constant of the TSENSE1 thermistor.
 * Range: 0x0000 to 0xFFFF
 * Unit: K
 * Initial value: 0x0D34 (3380K)
 *
 * @param b The value to set it to
 * @return True on successful I2C write
 */
bool LC709204F::setTSENSE1ThermistorB(uint16_t b) {
    return writeWord(LC709204F_REG_TSENSE1_THERMISTOR_B, b);
}

/**
 * Set Initial RSOC (0x07)
 *
 * Initialize RSOC with current voltage when 0xAA55.
 * Range: 0xAA55
 *
 * @return True on successful I2C write
 */
bool LC709204F::setInitialRSOC(void) {
    return writeWord(LC709204F_REG_INITIAL_RSOC, LC709204_INITIALIZE_RSOC_PARAM);
}

/**
 * Get CellTemperatureTSENSE1 (0x08)
 *
 * Displays Cell Temperature.
 * Range: 0x0980 to 0x0DCC (-30°C to 80°C)
 * Unit: 0.1K
 * Initial value: 0x0BA6 (25°C)
 *
 * @return 16-bit value read from LC709204F_REG_CELL_TEMPERATURE_TSENSE1 register
 */
uint16_t LC709204F::getCellTemperatureTSENSE1(void) {
    uint16_t val = 0;
    readWord(LC709204F_REG_CELL_TEMPERATURE_TSENSE1, &val);
    return val;
}

/**
 * Get CellTemperature (0x08)
 *
 * Displays Cell Temperature in °C.
 * Range: -30°C to 80°C
 * Unit: °C
 * Initial value: 25°C
 *
 * @return Floating point value from -30°C to 80°C
 */
float LC709204F::getCellTemperature(void) {
    uint16_t temp = getCellTemperatureTSENSE1();
    float temperature = map(temp, 0x980, 0xDCC, -300, 800);
    return temperature / 10.0;
}

/**
 * Set CellTemperatureTSENSE1 (0x08)
 *
 * Sets Cell Temperature when working in I2C mode.
 * Range: 0x0980 to 0x0DCC (-30°C to 80°C)
 * Unit: 0.1K
 * Initial value: 0x0BA6 (25°C)
 *
 * @param b The value to set it to
 * @return True on successful I2C write
 */
bool LC709204F::setCellTemperatureTSENSE1(uint16_t b) {
    return writeWord(LC709204F_REG_CELL_TEMPERATURE_TSENSE1, b);
}

/**
 * Set CellTemperature(0x08)
 *
 * Sets Cell Temperature in °C when working in I2C mode.
 * Range: -30°C to 80°C
 * Unit: °C
 * Initial value: 25°C
 *
 * @param b The value to set it to
 * @return True on successful I2C write
 */
bool LC709204F::setCellTemperature(float temperature) {
    uint16_t intTemp = (uint16_t)(temperature * 10);
    uint16_t temp = map(intTemp, -300, 800, 0x980, 0xDCC);
    return writeWord(LC709204F_REG_CELL_TEMPERATURE_TSENSE1, temp);
}

/**
 * Get CellVoltage (0x09)
 *
 * Displays Cell Voltage.
 * Range: 0x09C4 to 0x1388 (2500mV to 5000mV)
 * Unit: mV
 *
 * @return 16-bit value read from LC709204F_REG_CELL_VOLTAGE register
 */
uint16_t LC709204F::getCellVoltage(void) {
    uint16_t val = 0;
    readWord(LC709204F_REG_CELL_VOLTAGE, &val);
    return val;
}

/**
 * Get CurrentDirection (0x0A)
 *
 * Selects Auto/Charge/Discharge mode.
 * Range:
 * - 0x0000: Auto mode
 * - 0x0001: Charge mode
 * - 0xFFFF: Discharge mode
 * Initial value: 0x0000
 *
 * @return 16-bit value read from LC709204F_REG_CURRENT_DIRECTION register
 */
uint16_t LC709204F::getCurrentDirection(void) {
    uint16_t val = 0;
    readWord(LC709204F_REG_CURRENT_DIRECTION, &val);
    return val;
}

/**
 * Set CurrentDirection (0x0A)
 *
 * Selects Auto/Charge/Discharge mode.
 * Range:
 * - 0x0000: Auto mode
 * - 0x0001: Charge mode
 * - 0xFFFF: Discharge mode
 * Initial value: 0x0000
 *
 * @param b 16-bit value to write to LC709204F_REG_CURRENT_DIRECTION register
 * @return True on I2C command success
 */
bool LC709204F::setCurrentDirection(lc709204f_current_direction_t currentDirection) {
    return writeWord(LC709204F_REG_BEFORE_RSOC, currentDirection);
}

/**
 * Get APA - Adjustment Pack Application (0x0B)
 *
 * Gets Adjustment parameter.
 * Range: 0x0000 - 0xFFFF
 *
 * @return 16-bit value read from LC709204F_REG_APA register
 */
uint16_t LC709204F::getAPA(void) {
    uint16_t val = 0;
    readWord(LC709204F_REG_APA, &val);
    return val;
}

/**
 * Set APA - Adjustment Pack Application (0x0B)
 *
 * Sets Adjustment parameter.
 * Range: 0x0000 - 0xFFFF
 *
 * @param b 16-bit value to write to LC709204F_REG_APA register
 * @return True on I2C command success
 */
bool LC709204F::setAPA(lc709204f_apa_adjustment_t APAAdjustment) {
    return writeWord(LC709204F_REG_APA, APAAdjustment);
}

/**
 * Get APT - Adjustment Pack Thermistor (0x0C)
 *
 * Get the value to adjust temperature measurement delay timing.
 * Range: 0x0000 - 0xFFFF
 * Initial value: 0x001E
 *
 * @return 16-bit value read from LC709204F_REG_APT register
 */
uint16_t LC709204F::getAPT(void) {
    uint16_t val = 0;
    readWord(LC709204F_REG_APT, &val);
    return val;
}

/**
 * Set APT - Adjustment Pack Thermistor (0x0C)
 *
 * Sets a value to adjust temperature measurement delay timing.
 * Range: 0x0000 - 0xFFFF
 * Initial value: 0x001E
 *
 * @param b 16-bit value to write to LC709204F_REG_APT register
 * @return True on I2C command success
 */
bool LC709204F::setAPT(uint16_t b) {
    return writeWord(LC709204F_REG_APT, b);
}

/**
 * Get RSOC - Relative State Of Charge (0x0D)
 *
 * Displays RSOC value based on a 0−100 scale.
 * Range: 0x0000 - 0x0064
 * Unit: %
 *
 * @return 16-bit value read from LC709204F_REG_RSOC register
 */
uint16_t LC709204F::getRSOC(void) {
    uint16_t val = 0;
    readWord(LC709204F_REG_RSOC, &val);
    return val;
}

/**
 * Set RSOC - Relative State Of Charge (0x0D)
 *
 * Sets RSOC value based on a 0−100 scale.
 * Range: 0x0000 - 0x0064
 * Unit: %
 *
 * @param b 16-bit value to write to LC709204F_REG_RSOC register
 * @return True on I2C command success
 */
bool LC709204F::setRSOC(uint16_t b) {
    return writeWord(LC709204F_REG_RSOC, b);
}

/**
 * Get TSENSE2ThermistorB (0x0E)
 *
 * Gets B−constant of the TSENSE2 thermistor.
 * Range: 0x0000 to 0xFFFF
 * Unit: K
 * Initial value: 0x0D34 (3380K)
 *
 * @return 16-bit value read from LC709204F_REG_TSENSE2_THERMISTOR_B register
 */
uint16_t LC709204F::getTSENSE2ThermistorB(void) {
    uint16_t val = 0;
    readWord(LC709204F_REG_TSENSE2_THERMISTOR_B, &val);
    return val;
}

/**
 * Set TSENSE2ThermistorB (0x0E)
 *
 * Sets B−constant of the TSENSE2 thermistor.
 * Range: 0x0000 to 0xFFFF
 * Unit: K
 * Initial value: 0x0D34 (3380K)
 *
 * @param b The value to set it to
 * @return True on successful I2C write
 */
bool LC709204F::setTSENSE2ThermistorB(uint16_t b) {
    return writeWord(LC709204F_REG_TSENSE2_THERMISTOR_B, b);
}

/**
 * Get ITE - Indicator to Empty (0x0F)
 *
 * Displays RSOC value based on a 0−1000 scale.
 * Range: 0x0000 to 0x03E8 (0.0% - 100.0%)
 * Unit: 0.1%
 *
 * @return Floating point value from 0.0% to 100.0%
 */
float LC709204F::getITE(void) {
    uint16_t val = 0;
    readWord(LC709204F_REG_ITE, &val);
    return val / 10.0;
}

/**
 * Get ICVersion (0x11)
 *
 * Displays an internal management code.
 * Range: 0x0000 to 0xFFFF
 *
 * @return 16-bit value read from LC709204F_REG_IC_VERSION register
 */
uint16_t LC709204F::getICVersion(void) {
    uint16_t val = 0;
    readWord(LC709204F_REG_IC_VERSION, &val);
    return val;
}

/**
 * Get ChangeOfTheParameter (0x12)
 *
 * Selects a battery profile.
 * Range: 0x0000 to 0x0004
 *
 * @return 16-bit value read from LC709204F_REG_CHANGE_OF_THE_PARAMETER register
 */
uint16_t LC709204F::getChangeOfTheParameter(void) {
    uint16_t val = 0;
    readWord(LC709204F_REG_CHANGE_OF_THE_PARAMETER, &val);
    return val;
}

/**
 * Set ChangeOfTheParameter (0x12)
 *
 * Selects a battery profile.
 * Range: 0x0000 to 0x0004
 * Initial value: 0x0000
 *
 * @param batteryProfile The value to set it to
 * @return True on successful I2C write
 */
bool LC709204F::setChangeOfTheParameter(lc709204f_battery_profile_t batteryProfile) {
    return writeWord(LC709204F_REG_CHANGE_OF_THE_PARAMETER, batteryProfile);
}

/**
 * Get AlarmLowRSOC (0x13)
 *
 * Gets RSOC threshold to generate Alarm signal.
 * Range:
 * - 0x0000: Disable
 * - 0x0001 to 0x0064: Threshold (1% to 100%)
 * Unit: %
 * Initial value: 0x0000
 *
 * @return 16-bit value read from LC709204F_REG_ALARM_LOW_RSOC register
 */
uint16_t LC709204F::getAlarmLowRSOC(void) {
    uint16_t val = 0;
    readWord(LC709204F_REG_ALARM_LOW_RSOC, &val);
    return val;
}

/**
 * Set AlarmLowRSOC (0x13)
 *
 * Sets RSOC threshold to generate Alarm signal.
 * Range:
 * - 0x0000: Disable
 * - 0x0001 to 0x0064: Threshold (1% to 100%)
 * Unit: %
 * Initial value: 0x0000
 *
 * @param b The value to set it to
 * @return True on successful I2C write
 */
bool LC709204F::setAlarmLowRSOC(uint16_t b) {
    return writeWord(LC709204F_REG_ALARM_LOW_RSOC, b);
}

/**
 * Get AlarmLowCellVoltage (0x14)
 *
 * Gets Voltage threshold to generate Low Cell Voltage Alarm signal.
 * Range:
 * - 0x0000: Disable
 * - 0x09C4 to 0x1388: Threshold (2.5V to 5V)
 * Unit: mV
 * Initial value: 0x0000
 *
 * @return 16-bit value read from LC709204F_REG_ALARM_LOW_CELL_VOLTAGE register
 */
uint16_t LC709204F::getAlarmLowCellVoltage(void) {
    uint16_t val = 0;
    readWord(LC709204F_REG_ALARM_LOW_CELL_VOLTAGE, &val);
    return val;
}

/**
 * Set AlarmLowCellVoltage (0x14)
 *
 * Sets Voltage threshold to generate Low Cell Voltage Alarm signal.
 * Range:
 * - 0x0000: Disable
 * - 0x09C4 to 0x1388: Threshold (2.5V to 5V)
 * Unit: mV
 * Initial value: 0x0000
 *
 * @param b The value to set it to
 * @return True on successful I2C write
 */
bool LC709204F::setAlarmLowCellVoltage(uint16_t b) {
    return writeWord(LC709204F_REG_ALARM_LOW_CELL_VOLTAGE, b);
}

/**
 * Get ICPowerMode (0x15)
 *
 * Gets Power mode.
 * Range:
 * - 0x0001: Operational mode
 * - 0x0002: Sleep mode
 * Initial value: 0x0002
 *
 * @return 16-bit value read from LC709204F_REG_IC_POWER_MODE register
 */
uint16_t LC709204F::getICPowerMode(void) {
    uint16_t val = 0;
    readWord(LC709204F_REG_IC_POWER_MODE, &val);
    return val;
}

/**
 * Set ICPowerMode (0x15)
 *
 * Sets Power mode.
 * Range:
 * - 0x0001: Operational mode
 * - 0x0002: Sleep mode
 * Initial value: 0x0002
 *
 * @param powerMode The value to set it to
 * @return True on successful I2C write
 */
bool LC709204F::setICPowerMode(lc709204f_power_mode_t powerMode) {
    return writeWord(LC709204F_REG_IC_POWER_MODE, powerMode);
}

/**
 * Get StatusBit (0x16)
 *
 * Controls TSENSE1 and TSENSE2 thermistors.
 * - BIT0: Controls TSENSE1 thermistor
 * - BIT1: Controls TSENSE2 thermistor
 * Range: 0x0000 - 0x0003
 * Initial value: 0x0000
 *
 * @return 16-bit value read from LC709204F_REG_STATUS_BIT register
 */
uint16_t LC709204F::getStatusBit(void) {
    uint16_t val = 0;
    readWord(LC709204F_REG_STATUS_BIT, &val);
    return val;
}

/**
 * Set StatusBit (0x16)
 *
 * Controls TSENSE1 and TSENSE2 thermistors.
 * - BIT0: Controls TSENSE1 thermistor
 * - BIT1: Controls TSENSE2 thermistor
 * Range: 0x0000 - 0x0003
 * Initial value: 0x0000
 *
 * @param b 16-bit value to write to LC709204F_REG_STATUS_BIT register
 * @return True on I2C command success
 */
bool LC709204F::setStatusBit(uint16_t b) {
    return writeWord(LC709204F_REG_STATUS_BIT, b);
}

/**
 * Set Thermistors (0x16)
 *
 * Sets working mode for TSENSE1 and TSENSE2.
 *
 * Initial value: TSENSE1 and TSENSE2 both in I2C mode
 *
 * @param tsense1 boolean - true for thermistor, false for I2C
 * @param tsense2 boolean - true for thermistor, false for I2C
 * @return True on I2C command success
 */
bool LC709204F::setThermistors(bool tsense1, bool tsense2) {
    uint16_t val = 0;

    if (tsense1)
        val = val | 0x0001;

    if (tsense2)
        val = val | 0x0002;

    return setStatusBit(val);
}

/**
 * Get CycleCount (0x17)
 *
 * Displays cycle count.
 * Range: 0x0000 - 0xFFFF
 * Unit: count
 * Initial value: 0x0000
 *
 * @return 16-bit value read from LC709204F_REG_CYCLE_COUNT register
 */
uint16_t LC709204F::getCycleCount(void) {
    uint16_t val = 0;
    readWord(LC709204F_REG_CYCLE_COUNT, &val);
    return val;
}

/**
 * Get BatteryStatus (0x19)
 *
 * Displays various kinds of alarm and estimated state of the battery.
 * Range: 0x0000 - 0xFFFF
 * Initial value: 0x00C0
 *
 * @return 16-bit value read from LC709204F_REG_BATTERY_STATUS register
 */
uint16_t LC709204F::getBatteryStatus(void) {
    uint16_t val = 0;
    readWord(LC709204F_REG_BATTERY_STATUS, &val);
    return val;
}

/**
 * Get BatteryStatus (0x19)
 *
 * Displays various kinds of alarm and estimated state of the battery.
 * Range: 0x0000 - 0xFFFF
 * Initial value: 0x00C0
 *
 * @param b 16-bit value to write to LC709204F_REG_BATTERY_STATUS register
 * @return True on I2C command success
 */
bool LC709204F::setBatteryStatus(uint16_t b) {
    return writeWord(LC709204F_REG_BATTERY_STATUS, b);
}

/**
 * Get NumberOfTheParameter (0x1A)
 *
 * Displays Battery profile code.
 * Range: 0x0000 - 0xFFFF
 *
 * @return 16-bit value read from LC709204F_REG_NUMBER_OF_THE_PARAMETER register
 */
uint16_t LC709204F::getNumberOfTheParameter(void) {
    uint16_t val = 0;
    readWord(LC709204F_REG_NUMBER_OF_THE_PARAMETER, &val);
    return val;
}

/**
 * Get TerminationCurrentRate (0x1C)
 *
 * Gets termination current rate.
 * Range: 0x0002 - 0x001E
 * Unit: 0.01C
 * Initial value: 0x0002
 *
 * @return 16-bit value read from LC709204F_REG_TERMINATION_CURRENT_RATE register
 */
uint16_t LC709204F::getTerminationCurrentRate(void) {
    uint16_t val = 0;
    readWord(LC709204F_REG_TERMINATION_CURRENT_RATE, &val);
    return val;
}

/**
 * Set TerminationCurrentRate (0x1C)
 *
 * Sets termination current rate.
 * Range: 0x0002 - 0x001E
 * Unit: 0.01C
 * Initial value: 0x0002
 *
 * @param b 16-bit value to write to LC709204F_REG_TERMINATION_CURRENT_RATE register
 * @return True on I2C command success
 */
bool LC709204F::setTerminationCurrentRate(uint16_t b) {
    return writeWord(LC709204F_REG_TERMINATION_CURRENT_RATE, b);
}

/**
 * Get EmptyCellVoltage (0x1D)
 *
 * Gets empty cell voltage.
 * Range:
 * - 0x0000: Disable
 * - 0x09C4 - 0x1388 (2.5V to 5V)
 * Unit: mV
 * Initial value: 0x0000
 *
 * @return 16-bit value read from LC709204F_REG_EMPTY_CELL_VOLTAGE register
 */
uint16_t LC709204F::getEmptyCellVoltage(void) {
    uint16_t val = 0;
    readWord(LC709204F_REG_EMPTY_CELL_VOLTAGE, &val);
    return val;
}

/**
 * Set EmptyCellVoltage (0x1D)
 *
 * Sets empty cell voltage.
 * Range:
 * - 0x0000: Disable
 * - 0x09C4 - 0x1388 (2.5V to 5V)
 * Unit: mV
 * Initial value: 0x0000
 *
 * @param b 16-bit value to write to LC709204F_REG_EMPTY_CELL_VOLTAGE register
 * @return True on I2C command success
 */
bool LC709204F::setEmptyCellVoltage(uint16_t b) {
    return writeWord(LC709204F_REG_EMPTY_CELL_VOLTAGE, b);
}

/**
 * Get ITEOffset (0x1E)
 *
 * Gets ITE so that RSOC is 0%.
 * Range: 0x0000 - 0x03E8 (0.0% to 100.0%)
 * Unit: 0.1%
 * Initial value: 0x0000
 *
 * @return 16-bit value read from LC709204F_REG_ITE_OFFSET register
 */
uint16_t LC709204F::getITEOffset(void) {
    uint16_t val = 0;
    readWord(LC709204F_REG_ITE_OFFSET, &val);
    return val;
}

/**
 * Set ITEOffset (0x1E)
 *
 * Sets ITE so that RSOC is 0%.
 * Range: 0x0000 - 0x03E8 (0.0% to 100.0%)
 * Unit: 0.1%
 * Initial value: 0x0000
 *
 * @param b 16-bit value to write to LC709204F_REG_ITE_OFFSET register
 * @return True on I2C command success
 */
bool LC709204F::setITEOffset(uint16_t b) {
    return writeWord(LC709204F_REG_ITE_OFFSET, b);
}

/**
 * Get AlarmHighCellVoltage (0x1F)
 *
 * Gets Voltage threshold to generate High Cell Voltage Alarm signal.
 * Range:
 * - 0x0000: Disable
 * - 0x09C4 - 0x1388 (2.5V to 5V)
 * Unit: mV
 * Initial value: 0x0000
 *
 * @return 16-bit value read from LC709204F_REG_ALARM_HIGH_CELL_VOLTAGE register
 */
uint16_t LC709204F::getAlarmHighCellVoltage(void) {
    uint16_t val = 0;
    readWord(LC709204F_REG_ALARM_HIGH_CELL_VOLTAGE, &val);
    return val;
}

/**
 * Set AlarmHighCellVoltage (0x1F)
 *
 * Sets Voltage threshold to generate High Cell Voltage Alarm signal.
 * Range:
 * - 0x0000: Disable
 * - 0x09C4 - 0x1388 (2.5V to 5V)
 * Unit: mV
 * Initial value: 0x0000
 *
 * @param b 16-bit value to write to LC709204F_REG_ALARM_HIGH_CELL_VOLTAGE register
 * @return True on I2C command success
 */
bool LC709204F::setAlarmHighCellVoltage(uint16_t b) {
    return writeWord(LC709204F_REG_ALARM_HIGH_CELL_VOLTAGE, b);
}

/**
 * Get AlarmLowTemperature (0x20)
 *
 * Gets Voltage threshold to generate Low Temperature alarm signal.
 * Range:
 * - 0x0000: Disable
 * - 0x0980 - 0x0DCC (-30°C to 80°C)
 * Unit: °C
 * Initial value: 0x0000
 *
 * @return float value read from LC709204F_REG_ALARM_LOW_TEMPERATURE register
 */
float LC709204F::getAlarmLowTemperature(void) {
    uint16_t temp = 0;
    readWord(LC709204F_REG_ALARM_LOW_TEMPERATURE, &temp);
    float temperature = map(temp, 0x980, 0xDCC, -300, 800);
    return temperature / 10.0;
}

/**
 * Set AlarmLowTemperature (0x20)
 *
 * Sets Voltage threshold to generate Low Temperature alarm signal.
 * Range:
 * - 0x0000: Disable
 * - 0x0980 - 0x0DCC (-30°C to 80°C)
 * Unit: °C
 * Initial value: 0x0000
 *
 * @param temp float value to write to LC709204F_REG_ALARM_LOW_TEMPERATURE register
 * @return True on I2C command success
 */
bool LC709204F::setAlarmLowTemperature(float temp) {
    uint16_t intTemp = (uint16_t)(temp * 10);
    uint16_t temperature = map(intTemp, -300, 800, 0x980, 0xDCC);
    return writeWord(LC709204F_REG_ALARM_LOW_TEMPERATURE, temperature);
}

/**
 * Get AlarmHighTemperature (0x21)
 *
 * Gets Voltage threshold to generate High Temperature alarm signal.
 * Range:
 * - 0x0000: Disable
 * - 0x0980 - 0x0DCC (-30°C to 80°C)
 * Unit: °C
 * Initial value: 0x0000
 *
 * @return float value read from LC709204F_REG_ALARM_HIGH_TEMPERATURE register
 */
float LC709204F::getAlarmHighTemperature(void) {
    uint16_t temp = 0;
    readWord(LC709204F_REG_ALARM_HIGH_TEMPERATURE, &temp);
    float temperature = map(temp, 0x980, 0xDCC, -300, 800);
    return temperature / 10.0;
}

/**
 * Set AlarmHighTemperature (0x21)
 *
 * Sets Voltage threshold to generate High Temperature alarm signal.
 * Range:
 * - 0x0000: Disable
 * - 0x0980 - 0x0DCC (-30°C to 80°C)
 * Unit: °C
 * Initial value: 0x0000
 *
 * @param temp float value to write to LC709204F_REG_ALARM_HIGH_TEMPERATURE register
 * @return True on I2C command success
 */
bool LC709204F::setAlarmHighTemperature(float temp) {
    uint16_t intTemp = (uint16_t)(temp * 10);
    uint16_t temperature = map(intTemp, -300, 800, 0x980, 0xDCC);
    return writeWord(LC709204F_REG_ALARM_HIGH_TEMPERATURE, temperature);
}

/**
 * Get TotalRunTime (0x24, 0x25)
 *
 * Gets operating time.
 * 0x24 - Lower 16bit
 * 0x25 - Higher 8bit
 * Range: 0x00000000 - 0x00FFFFFF
 * Unit: minutes
 * Initial value: 0x0000
 *
 * @return 32-bit value read from LC709204F_REG_TOTAL_RUN_TIME_LOWER_16BIT and LC709204F_REG_TOTAL_RUN_TIME_HIGHER_8BIT registers
 */
uint32_t LC709204F::getTotalRunTime(void) {
    uint16_t lower = 0;
    uint16_t higher = 0;
    readWord(LC709204F_REG_TOTAL_RUN_TIME_LOWER_16BIT, &lower);
    readWord(LC709204F_REG_TOTAL_RUN_TIME_HIGHER_8BIT, &higher);
    return higher * 0x10000 + lower;
}

/**
 * Set TotalRunTime (0x24, 0x25)
 *
 * Sets operating time.
 * 0x24 - Lower 16bit
 * 0x25 - Higher 8bit
 * Range: 0x00000000 - 0x00FFFFFF
 * Unit: minutes
 * Initial value: 0x0000
 *
 * @param b 32-bit value to write to LC709204F_REG_TOTAL_RUN_TIME_LOWER_16BIT and LC709204F_REG_TOTAL_RUN_TIME_HIGHER_8BIT registers
 * @return True on I2C command success
 */
bool LC709204F::setTotalRunTime(uint32_t b) {
    uint16_t lower = (uint16_t) b;
    uint16_t higher = (uint16_t)(b >> 16);
    return writeWord(LC709204F_REG_TOTAL_RUN_TIME_LOWER_16BIT, lower) &&
           writeWord(LC709204F_REG_TOTAL_RUN_TIME_HIGHER_8BIT, higher);
}

/**
 * Get AccumulatedTemperature (0x26, 0x27)
 *
 * Gets accumulated temperature.
 * 0x26 - Lower 16bit
 * 0x27 - Higher 16bit
 * Range: 0x00000000 - 0xFFFFFFFF
 * Unit: 2K
 * Initial value: 0x0000
 *
 * @return 32-bit value read from LC709204F_REG_ACCUMULATED_TEMPERATURE_LOWER_16BIT and LC709204F_REG_ACCUMULATED_TEMPERATURE_HIGHER_16BIT registers
 */
uint32_t LC709204F::getAccumulatedTemperature(void) {
    uint16_t lower = 0;
    uint16_t higher = 0;
    readWord(LC709204F_REG_ACCUMULATED_TEMPERATURE_LOWER_16BIT, &lower);
    readWord(LC709204F_REG_ACCUMULATED_TEMPERATURE_HIGHER_16BIT, &higher);
    return higher * 0x10000 + lower;
}

/**
 * Set AccumulatedTemperature (0x26, 0x27)
 *
 * Sets accumulated temperature.
 * 0x26 - Lower 16bit
 * 0x27 - Higher 16bit
 * Range: 0x00000000 - 0xFFFFFFFF
 * Unit: 2K
 * Initial value: 0x0000
 *
 * @param b 32-bit value to write to LC709204F_REG_ACCUMULATED_TEMPERATURE_LOWER_16BIT and LC709204F_REG_ACCUMULATED_TEMPERATURE_HIGHER_16BIT registers
 * @return True on I2C command success
 */
bool LC709204F::setAccumulatedTemperature(uint32_t b) {
    uint16_t lower = (uint16_t) b;
    uint16_t higher = (uint16_t)(b >> 16);
    return writeWord(LC709204F_REG_ACCUMULATED_TEMPERATURE_LOWER_16BIT, lower) &&
           writeWord(LC709204F_REG_ACCUMULATED_TEMPERATURE_HIGHER_16BIT, higher);
}

/**
 * Get AccumulatedRSOC (0x28, 0x29)
 *
 * Gets accumulated RSOC.
 * 0x28 - Lower 16bit
 * 0x29 - Higher 16bit
 * Range: 0x00000000 - 0xFFFFFFFF
 * Unit: % minutes
 * Initial value: 0x0000
 *
 * @return 32-bit value read from LC709204F_REG_ACCUMULATED_RSOC_LOWER_16BIT and LC709204F_REG_ACCUMULATED_RSOC_HIGHER_16BIT registers
 */
uint32_t LC709204F::getAccumulatedRSOC(void) {
    uint16_t lower = 0;
    uint16_t higher = 0;
    readWord(LC709204F_REG_ACCUMULATED_RSOC_LOWER_16BIT, &lower);
    readWord(LC709204F_REG_ACCUMULATED_RSOC_HIGHER_16BIT, &higher);
    return higher * 0x10000 + lower;
}

/**
 * Set AccumulatedRSOC (0x28, 0x29)
 *
 * Sets accumulated RSOC.
 * 0x28 - Lower 16bit
 * 0x29 - Higher 16bit
 * Range: 0x00000000 - 0xFFFFFFFF
 * Unit: % minutes
 * Initial value: 0x0000
 *
 * @param b 32-bit value to write to LC709204F_REG_ACCUMULATED_RSOC_LOWER_16BIT and LC709204F_REG_ACCUMULATED_RSOC_HIGHER_16BIT registers
 * @return True on I2C command success
 */
bool LC709204F::setAccumulatedRSOC(uint32_t b) {
    uint16_t lower = (uint16_t) b;
    uint16_t higher = (uint16_t)(b >> 16);
    return writeWord(LC709204F_REG_ACCUMULATED_RSOC_LOWER_16BIT, lower) &&
           writeWord(LC709204F_REG_ACCUMULATED_RSOC_HIGHER_16BIT, higher);
}

/**
 * Get MaximumCellVoltage (0x2A)
 *
 * Gets the maximum historical Cell Voltage.
 * Range: 0x09C4 - 0x1388 (2.5V to 5V)
 * Unit: mV
 * Initial value: 0x0000
 *
 * @return 16-bit value read from LC709204F_REG_MAXIMUM_CELL_VOLTAGE register
 */
uint16_t LC709204F::getMaximumCellVoltage(void) {
    uint16_t val = 0;
    readWord(LC709204F_REG_MAXIMUM_CELL_VOLTAGE, &val);
    return val;
}

/**
 * Set MaximumCellVoltage (0x2A)
 *
 * Sets the maximum historical Cell Voltage.
 * Range: 0x09C4 - 0x1388 (2.5V to 5V)
 * Unit: mV
 * Initial value: 0x0000
 *
 * @param b 16-bit value to write to LC709204F_REG_MAXIMUM_CELL_VOLTAGE register
 * @return True on I2C command success
 */
bool LC709204F::setMaximumCellVoltage(uint16_t b) {
    return writeWord(LC709204F_REG_MAXIMUM_CELL_VOLTAGE, b);
}

/**
 * Get MinimumCellVoltage (0x2B)
 *
 * Gets the minimum historical Cell Voltage.
 * Range: 0x09C4 - 0x1388 (2.5V to 5V)
 * Unit: mV
 * Initial value: 0x0000
 *
 * @return 16-bit value read from LC709204F_REG_MINIMUM_CELL_VOLTAGE register
 */
uint16_t LC709204F::getMinimumCellVoltage(void) {
    uint16_t val = 0;
    readWord(LC709204F_REG_MINIMUM_CELL_VOLTAGE, &val);
    return val;
}

/**
 * Set MinimumCellVoltage (0x2B)
 *
 * Sets the minimum historical Cell Voltage.
 * Range: 0x09C4 - 0x1388 (2.5V to 5V)
 * Unit: mV
 * Initial value: 0x1388 (5V)
 *
 * @param b 16-bit value to write to LC709204F_REG_MINIMUM_CELL_VOLTAGE register
 * @return True on I2C command success
 */
bool LC709204F::setMinimumCellVoltage(uint16_t b) {
    return writeWord(LC709204F_REG_MINIMUM_CELL_VOLTAGE, b);
}

/**
 * Get MaximumCellTemperatureTSENSE1 (0x2C)
 *
 * Gets  the historical maximum temperature of TSENSE1.
 * Range: 0x0980 to 0x0DCC (-30°C to 80°C)
 * Unit: 0.1K
 * Initial value: 0x0980 (-30°C)
 *
 * @return 16-bit value read from LC709204F_REG_MAXIMUM_CELL_TEMPERATURE_TSENSE1 register
 */
uint16_t LC709204F::getMaximumCellTemperatureTSENSE1(void) {
    uint16_t val = 0;
    readWord(LC709204F_REG_MAXIMUM_CELL_TEMPERATURE_TSENSE1, &val);
    return val;
}

/**
 * Get MaximumCellTemperature (0x2C)
 *
 * Gets  the historical maximum temperature of TSENSE1 in °C.
 * Range: -30°C to 80°C
 * Unit: °C
 * Initial value: -30°C
 *
 * @return Floating point value from -30°C to 80°C
 */
float LC709204F::getMaximumCellTemperature(void) {
    uint16_t temp = getMaximumCellTemperatureTSENSE1();
    float temperature = map(temp, 0x980, 0xDCC, -300, 800);
    return temperature / 10.0;
}

/**
 * Set MaximumCellTemperatureTSENSE1 (0x2C)
 *
 * Sets  the historical maximum temperature of TSENSE1.
 * Range: 0x0980 to 0x0DCC (-30°C to 80°C)
 * Unit: 0.1K
 * Initial value: 0x0980 (-30°C)
 *
 * @param b The value to set it to
 * @return True on successful I2C write
 */
bool LC709204F::setMaximumCellTemperatureTSENSE1(uint16_t b) {
    return writeWord(LC709204F_REG_MAXIMUM_CELL_TEMPERATURE_TSENSE1, b);
}

/**
 * Set MaximumCellTemperature (0x2C)
 *
 * Sets  the historical maximum temperature of TSENSE1 in °C.
 * Range: -30°C to 80°C
 * Unit: °C
 * Initial value: -30°C
 *
 * @param b The value to set it to
 * @return True on successful I2C write
 */
bool LC709204F::setMaximumCellTemperature(float temperature) {
    uint16_t intTemp = (uint16_t)(temperature * 10);
    uint16_t temp = map(intTemp, -300, 800, 0x980, 0xDCC);
    return writeWord(LC709204F_REG_MAXIMUM_CELL_TEMPERATURE_TSENSE1, temp);
}

/**
 * Get MinimumCellTemperatureTSENSE1 (0x2D)
 *
 * Gets the historical minimum temperature of TSENSE1.
 * Range: 0x0980 to 0x0DCC (-30°C to 80°C)
 * Unit: 0.1K
 * Initial value: 0x0DCC (80°C)
 *
 * @return 16-bit value read from LC709204F_REG_MINIMUM_CELL_TEMPERATURE_TSENSE1 register
 */
uint16_t LC709204F::getMinimumCellTemperatureTSENSE1(void) {
    uint16_t val = 0;
    readWord(LC709204F_REG_MINIMUM_CELL_TEMPERATURE_TSENSE1, &val);
    return val;
}

/**
 * Get MinimumCellTemperature (0x2D)
 *
 * Gets  the historical minimum temperature of TSENSE1 in °C.
 * Range: -30°C to 80°C
 * Unit: °C
 * Initial value: 80°C
 *
 * @return Floating point value from -30°C to 80°C
 */
float LC709204F::getMinimumCellTemperature(void) {
    uint16_t temp = getMinimumCellTemperatureTSENSE1();
    float temperature = map(temp, 0x980, 0xDCC, -300, 800);
    return temperature / 10.0;
}

/**
 * Set MinimumCellTemperatureTSENSE1 (0x2D)
 *
 * Sets  the historical minimum temperature of TSENSE1.
 * Range: 0x0980 to 0x0DCC (-30°C to 80°C)
 * Unit: 0.1K
 * Initial value: 0x0DCC (80°C)
 *
 * @param b The value to set it to
 * @return True on successful I2C write
 */
bool LC709204F::setMinimumCellTemperatureTSENSE1(uint16_t b) {
    return writeWord(LC709204F_REG_MINIMUM_CELL_TEMPERATURE_TSENSE1, b);
}

/**
 * Set MinimumCellTemperature (0x2D)
 *
 * Sets the historical minimum temperature of TSENSE1 in °C.
 * Range: -30°C to 80°C
 * Unit: °C
 * Initial value: 80°C
 *
 * @param b The value to set it to
 * @return True on successful I2C write
 */
bool LC709204F::setMinimumCellTemperature(float temperature) {
    uint16_t intTemp = (uint16_t)(temperature * 10);
    uint16_t temp = map(intTemp, -300, 800, 0x980, 0xDCC);
    return writeWord(LC709204F_REG_MINIMUM_CELL_TEMPERATURE_TSENSE1, temp);
}

/**
 * Get AmbientTemperatureTSENSE2 (0x30)
 *
 * Gets the ambient temperature of TSENSE2.
 * Range: 0x0980 to 0x0DCC (-30°C to 80°C)
 * Unit: 0.1K
 * Initial value: 0x0BA6 (25°C)
 *
 * @return 16-bit value read from LC709204F_REG_MINIMUM_CELL_TEMPERATURE_TSENSE1 register
 */
uint16_t LC709204F::getAmbientTemperatureTSENSE2(void) {
    uint16_t val = 0;
    readWord(LC709204F_REG_AMBIENT_TEMPERATURE_TSENSE2, &val);
    return val;
}

/**
 * Get AmbientTemperatureTSENSE2 (0x30)
 *
 * Gets  the ambient temperature of TSENSE2 in °C.
 * Range: -30°C to 80°C
 * Unit: °C
 * Initial value: 25°C
 *
 * @return Floating point value from -30°C to 80°C
 */
float LC709204F::getAmbientTemperature(void) {
    uint16_t temp = getAmbientTemperatureTSENSE2();
    float temperature = map(temp, 0x980, 0xDCC, -300, 800);
    return temperature / 10.0;
}

/**
 * Get StateOfHealth (0x32)
 *
 * Gets  State of Health of a battery on a 0−100 scale.
 * Range: 0x0000 to 0x0064 (0% to 100%)
 * Unit: %
 * Initial value: 0x0064 (100%)
 *
 * @return 16-bit value read from LC709204F_REG_STATE_OF_HEALTH register
 */
uint16_t LC709204F::getStateOfHealth(void) {
    uint16_t val = 0;
    readWord(LC709204F_REG_STATE_OF_HEALTH, &val);
    return val;
}

/**
* Get UserID (0x36, 0x37)
*
* Gets 32bit User ID.
* 0x36 - Lower 16bit
* 0x37 - Higher 16bit
* Range: 0x00000000 - 0xFFFFFFFF
*
* @return 32-bit value read from LC709204F_REG_USER_ID_LOWER_16BIT and LC709204F_REG_USER_ID_HIGHER_16BIT registers
*/
uint32_t LC709204F::getUserId(void) {
    uint16_t lower = 0;
    uint16_t higher = 0;
    readWord(LC709204F_REG_USER_ID_LOWER_16BIT, &lower);
    readWord(LC709204F_REG_USER_ID_HIGHER_16BIT, &higher);
    return higher * 0x10000 + lower;
}

/**
 * readWord
 *
 * Reads 16 bits of CRC data from the chip.
 * Note this function performs a CRC on data that includes the I2C
 * write address, command, read address and 2 bytes of response.
 *
 * @param command The I2C register/command
 * @param data Pointer to uint16_t value to store the response
 * @return True on successful I2C read
 */
bool LC709204F::readWord(uint8_t command, uint16_t *data) {
    uint8_t reply[6];
    reply[0] = LC709204F_I2CADDR * 2;
    reply[1] = command;
    reply[2] = reply[0] | 0x1;

    if (!i2cWriteThenRead(&command, 1, reply + 3, 3)) {
        return false;
    }

    uint8_t crc = crc8(reply, 5);
    // CRC failure?
    if (crc != reply[5])
        return false;

    *data = reply[4];
    *data <<= 8;
    *data |= reply[3];

    return true;
}

/**
 * readWord
 *
 * Writes 16 bits of CRC data to the chip.
 * Note this function performs a CRC on data that includes the I2C
 * write address, command, and 2 bytes of response.
 *
 * @param command The I2C register/command
 * @param data Pointer to uint16_t value to write
 * @return True on successful I2C write
 */
bool LC709204F::writeWord(uint8_t command, uint16_t data) {
    uint8_t send[5];
    send[0] = LC709204F_I2CADDR * 2;
    send[1] = command;
    send[2] = data & 0xFF;
    send[3] = data >> 8;
    send[4] = crc8(send, 4);

    return i2cWrite(send + 1, 4);
}

/**
 * CRC8 calculation.
 *
 * @param data Pointer to the data to use when calculating the CRC8.
 * @param len The number of bytes in 'data'.
 *
 * @return Calculated CRC8 value.
 */
uint8_t LC709204F::crc8(uint8_t *data, int len) {
    const uint8_t POLYNOMIAL(0x07);
    uint8_t crc(0x00);

    for (int j = len; j; --j) {
        crc ^= *data++;

        for (int i = 8; i; --i) {
            crc = (crc & 0x80) ? (crc << 1) ^ POLYNOMIAL : (crc << 1);
        }
    }
    return crc;
}

/**
 * I2C write followed by read.
 *
 * @param write_buffer
 * @param write_len
 * @param read_buffer
 * @param read_len
 * @return True on successful I2C operation
 */
bool LC709204F::i2cWriteThenRead(const uint8_t *write_buffer, size_t write_len, uint8_t *read_buffer, size_t read_len, bool stop) {
    if (!i2cWrite(write_buffer, write_len, stop)) {
        return false;
    }

    return i2cRead(read_buffer, read_len);
}

/**
 * I2C Write.
 *
 * @param buffer
 * @param len
 * @return True on successful I2C operation
 */
bool LC709204F::i2cWrite(const uint8_t *buffer, size_t len, bool stop) {
    _wire->beginTransmission(LC709204F_I2CADDR);

    // Write the data itself
    if (_wire->write(buffer, len) != len) {
        return false;
    }

    if (_wire->endTransmission(stop) == 0) {
        return true;
    } else {
        return false;
    }
}

/**
 * I2C Read.
 * @param buffer
 * @param len
 * @return True on successful I2C operation
 */
bool LC709204F::i2cRead(uint8_t *buffer, size_t len, bool stop) {
    size_t pos = 0;
    while (pos < len) {
        size_t read_len = (len - pos);
        bool read_stop = (pos < (len - read_len)) ? false : stop;
        if (!_i2cRead(buffer + pos, read_len, read_stop))
            return false;
        pos += read_len;
    }
    return true;
}

/**
 * I2C Read helper.
 *
 * @param buffer
 * @param len
 * @return True on successful I2C operation
 */
bool LC709204F::_i2cRead(uint8_t *buffer, size_t len, bool stop) {
#if defined(TinyWireM_h)
    size_t recv = _wire->requestFrom((uint8_t)LC709204F_I2CADDR, (uint8_t)len);
#elif defined(ARDUINO_ARCH_MEGAAVR)
    size_t recv = _wire->requestFrom(LC709204F_I2CADDR, len, stop);
#else
    size_t recv = _wire->requestFrom((uint8_t) LC709204F_I2CADDR, (uint8_t) len, (uint8_t) stop);
#endif

    if (recv != len) {
        // Not enough data available to fulfill our obligation!
        return false;
    }

    for (uint16_t i = 0; i < len; i++) {
        buffer[i] = _wire->read();
    }

    return true;
}
