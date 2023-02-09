/**
 * @file LC709204F.cpp
 * @author Razvan Mocanu <razvan@mocanu.biz>
 * @version 1.0.0
 * @details LC709204F Battery Monitor library
 * @copyright MIT (see LICENSE.md)
 */

#ifndef _LC709204F_H
#define _LC709204F_H

#include "Arduino.h"
#include "Wire.h"

/// See datasheet for details: https://www.onsemi.com/download/data-sheet/pdf/lc709204f-d.pdf
#define LC709204F_I2CADDR 0x0B /// LC709204F default i2c address

#define LC709204F_REG_TIME_TO_EMPTY                        0x03 /// R - Displays estimated time to empty.
#define LC709204F_REG_BEFORE_RSOC                          0x04 /// W - Optional Command, especially for obtaining the voltage with intentional timing after power on reset.
#define LC709204F_REG_TIME_TO_FULL                         0x05 /// R - Displays estimated time to full.
#define LC709204F_REG_TSENSE1_THERMISTOR_B                 0x06 /// R/W - Sets B−constant of the TSENSE1 thermistor.
#define LC709204F_REG_INITIAL_RSOC                         0x07 /// W - Initialize RSOC with current voltage when 0xAA55 is set.
#define LC709204F_REG_CELL_TEMPERATURE_TSENSE1             0x08 /// R - Displays Cell Temperature / W - Sets Cell Temperature in I2C mode.
#define LC709204F_REG_CELL_VOLTAGE                         0x09 /// R - Displays Cell Voltage.
#define LC709204F_REG_CURRENT_DIRECTION                    0x0A /// R/W - Selects Auto/Charge/Discharge mode.
#define LC709204F_REG_APA                                  0x0B /// R/W - Sets Adjustment parameter.
#define LC709204F_REG_APT                                  0x0C /// R/W - Sets a value to adjust temperature measurement delay timing.
#define LC709204F_REG_RSOC                                 0x0D /// R/W - Displays RSOC value based on a 0−100 scale.
#define LC709204F_REG_TSENSE2_THERMISTOR_B                 0x0E /// R/W - Sets B−constant of the TSENSE2 thermistor.
#define LC709204F_REG_ITE                                  0x0F /// R - Displays RSOC value based on a 0−1000 scale.
#define LC709204F_REG_IC_VERSION                           0x11 /// R - Displays an internal management code.
#define LC709204F_REG_CHANGE_OF_THE_PARAMETER              0x12 /// R/W - Selects a battery profile.
#define LC709204F_REG_ALARM_LOW_RSOC                       0x13 /// R/W - Sets RSOC threshold to generate Alarm signal.
#define LC709204F_REG_ALARM_LOW_CELL_VOLTAGE               0x14 /// R/W - Sets voltage threshold to generate Low Cell Voltage Alarm signal.
#define LC709204F_REG_IC_POWER_MODE                        0x15 /// R/W - Selects Power mode.
#define LC709204F_REG_STATUS_BIT                           0x16 /// R/W - Controls TSENSE1 and TSENSE2 thermistors.
#define LC709204F_REG_CYCLE_COUNT                          0x17 /// R - Displays cycle count.
#define LC709204F_REG_BATTERY_STATUS                       0x19 /// R/W - Displays various kinds of alarm and estimated state of the battery.
#define LC709204F_REG_NUMBER_OF_THE_PARAMETER              0x1A /// R - Displays Battery profile code.
#define LC709204F_REG_TERMINATION_CURRENT_RATE             0x1C /// R/W - Sets termination current rate.
#define LC709204F_REG_EMPTY_CELL_VOLTAGE                   0x1D /// R/W - Sets empty cell voltage.
#define LC709204F_REG_ITE_OFFSET                           0x1E /// R/W - Sets ITE so that RSOC is 0%.
#define LC709204F_REG_ALARM_HIGH_CELL_VOLTAGE              0x1F /// R/W - Sets Voltage threshold to generate High Cell Voltage Alarm signal.
#define LC709204F_REG_ALARM_LOW_TEMPERATURE                0x20 /// R/W - Sets Voltage threshold to generate Low Temperature alarm signal.
#define LC709204F_REG_ALARM_HIGH_TEMPERATURE               0x21 /// R/W - Sets Voltage threshold to generate High Temperature alarm signal.
#define LC709204F_REG_TOTAL_RUN_TIME_LOWER_16BIT           0x24 /// R/W - Displays operating time (lower 16bit).
#define LC709204F_REG_TOTAL_RUN_TIME_HIGHER_8BIT           0x25 /// R/W - Displays operating time (higher 8bit)
#define LC709204F_REG_ACCUMULATED_TEMPERATURE_LOWER_16BIT  0x26 /// R/W - Displays accumulated temperature (lower 16bit)
#define LC709204F_REG_ACCUMULATED_TEMPERATURE_HIGHER_16BIT 0x27 /// R/W - Displays accumulated temperature (higher 16bit)
#define LC709204F_REG_ACCUMULATED_RSOC_LOWER_16BIT         0x28 /// R/W - Displays accumulated RSOCC (lower 16bit)
#define LC709204F_REG_ACCUMULATED_RSOC_HIGHER_16BIT        0x29 /// R/W - Displays accumulated RSOCC (higher 16bit)
#define LC709204F_REG_MAXIMUM_CELL_VOLTAGE                 0x2A /// R/W - Displays the maximum historical Cell Voltage.
#define LC709204F_REG_MINIMUM_CELL_VOLTAGE                 0x2B /// R/W - Displays the minimum historical Cell Voltage.
#define LC709204F_REG_MAXIMUM_CELL_TEMPERATURE_TSENSE1     0x2C /// R/W - Displays the historical maximum temperature of TSENSE1.
#define LC709204F_REG_MINIMUM_CELL_TEMPERATURE_TSENSE1     0x2D /// R/W - Displays the historical minimum temperature of TSENSE1.
#define LC709204F_REG_AMBIENT_TEMPERATURE_TSENSE2          0x30 /// R - Displays Ambient Temperature.
#define LC709204F_REG_STATE_OF_HEALTH                      0x32 /// R - Displays State of Health of a battery on a 0−100 scale.
#define LC709204F_REG_USER_ID_LOWER_16BIT                  0x36 /// R - Displays 32bit User Id (lower 16bit).
#define LC709204F_REG_USER_ID_HIGHER_16BIT                 0x37 /// R - Displays 32bit User Id (higher 16bit).

/**
 * Value to initialize RSOC
 */
#define LC709204_INITIALIZE_RSOC_PARAM 0xAA55

/**
 * BeforeRSOC values
 */
typedef enum {
    LC709204F_BEFORE_RSOC_FIRST_SAMPLING = 0xAA55,
    LC709204F_BEFORE_RSOC_SECOND_SAMPLING = 0xAA56,
    LC709204F_BEFORE_RSOC_THIRD_SAMPLING = 0xAA57,
    LC709204F_BEFORE_RSOC_FOURTH_SAMPLING = 0xAA58,
} lc709204f_before_rsoc_t;

/**
 * CurrentDirection values
 */
typedef enum {
    LC709204F_CURRENT_DIRECTION_AUTO_MODE = 0x0000,
    LC709204F_CURRENT_DIRECTION_CHARGE_MODE = 0x0001,
    LC709204F_CURRENT_DIRECTION_DISCHARGE_MODE = 0xFFFF,
} lc709204f_current_direction_t;

/**
 * Approx battery pack size
 **/
typedef enum {
    LC709204F_APA_50MAH = 0x1313,
    LC709204F_APA_100MAH = 0x1515,
    LC709204F_APA_200MAH = 0x1818,
    LC709204F_APA_500MAH = 0x2121,
    LC709204F_APA_1000MAH = 0x2D2D,
    LC709204F_APA_2000MAH = 0x3A3A,
    LC709204F_APA_3000MAH = 0x3F3F,
    LC709204F_APA_4000MAH = 0x4242,
    LC709204F_APA_5000MAH = 0x4444,
    LC709204F_APA_6000MAH = 0x4545,
} lc709204f_apa_adjustment_t;

/**
 * Battery profile
 */
typedef enum {
    LC709204F_BATTERY_PROFILE_3_7_V = 0x00,
    LC709204F_BATTERY_PROFILE_UR18650ZY_PANASONIC = 0x01,
    LC709204F_BATTERY_PROFILE_ICR18650_26H_SAMSUNG = 0x02,
    LC709204F_BATTERY_PROFILE_3_8_V = 0x03,
    LC709204F_BATTERY_PROFILE_3_85_V = 0x04,
} lc709204f_battery_profile_t;

/**
 * IC Power Mode
 */
typedef enum {
    LC709204F_POWER_MODE_OPERATE = 0x0001,
    LC709204F_POWER_MODE_SLEEP = 0x0002,
} lc709204f_power_mode_t;

/**
 * LC709204F I2C battery monitor
 */
class LC709204F {
public:
    LC709204F(TwoWire *theWire = &Wire);

    ~LC709204F();

    bool init(lc709204f_apa_adjustment_t APAAdjustment, lc709204f_battery_profile_t batteryProfile, TwoWire *wire = &Wire);

    uint16_t getTimeToEmpty(void);

    bool setBeforeRSOC(lc709204f_before_rsoc_t beforeRSOC);

    uint16_t getTimeToFull(void);

    uint16_t getTSENSE1ThermistorB(void);

    bool setTSENSE1ThermistorB(uint16_t b);

    bool setInitialRSOC(void);

    uint16_t getCellTemperatureTSENSE1(void);

    float getCellTemperature(void);

    bool setCellTemperatureTSENSE1(uint16_t b);

    bool setCellTemperature(float temperature);

    uint16_t getCellVoltage(void);

    uint16_t getCurrentDirection(void);

    bool setCurrentDirection(lc709204f_current_direction_t currentDirection);

    uint16_t getAPA(void);

    bool setAPA(lc709204f_apa_adjustment_t APAAdjustment);

    uint16_t getAPT(void);

    bool setAPT(uint16_t b);

    uint16_t getRSOC(void);

    bool setRSOC(uint16_t b);

    uint16_t getTSENSE2ThermistorB(void);

    bool setTSENSE2ThermistorB(uint16_t b);

    float getITE(void);

    uint16_t getICVersion(void);

    uint16_t getChangeOfTheParameter(void);

    bool setChangeOfTheParameter(lc709204f_battery_profile_t batteryProfile);

    uint16_t getAlarmLowRSOC(void);

    bool setAlarmLowRSOC(uint16_t b);

    uint16_t getAlarmLowCellVoltage(void);

    bool setAlarmLowCellVoltage(uint16_t b);

    uint16_t getICPowerMode(void);

    bool setICPowerMode(lc709204f_power_mode_t powerMode);

    uint16_t getStatusBit(void);

    bool setStatusBit(uint16_t b);

    bool setThermistors(bool tsense1, bool tsense2);

    uint16_t getCycleCount(void);

    uint16_t getBatteryStatus(void);

    bool setBatteryStatus(uint16_t b);

    uint16_t getNumberOfTheParameter(void);

    uint16_t getTerminationCurrentRate(void);

    bool setTerminationCurrentRate(uint16_t b);

    uint16_t getEmptyCellVoltage(void);

    bool setEmptyCellVoltage(uint16_t b);

    uint16_t getITEOffset(void);

    bool setITEOffset(uint16_t b);

    uint16_t getAlarmHighCellVoltage(void);

    bool setAlarmHighCellVoltage(uint16_t b);

    float getAlarmLowTemperature(void);

    bool setAlarmLowTemperature(float temp);

    float getAlarmHighTemperature(void);

    bool setAlarmHighTemperature(float temp);

    uint32_t getTotalRunTime(void);

    bool setTotalRunTime(uint32_t b);

    uint32_t getAccumulatedTemperature(void);

    bool setAccumulatedTemperature(uint32_t b);

    uint32_t getAccumulatedRSOC(void);

    bool setAccumulatedRSOC(uint32_t b);

    uint16_t getMaximumCellVoltage(void);

    bool setMaximumCellVoltage(uint16_t b);

    uint16_t getMinimumCellVoltage(void);

    bool setMinimumCellVoltage(uint16_t b);

    uint16_t getMaximumCellTemperatureTSENSE1(void);

    float getMaximumCellTemperature(void);

    bool setMaximumCellTemperatureTSENSE1(uint16_t b);

    bool setMaximumCellTemperature(float temperature);

    uint16_t getMinimumCellTemperatureTSENSE1(void);

    float getMinimumCellTemperature(void);

    bool setMinimumCellTemperatureTSENSE1(uint16_t b);

    bool setMinimumCellTemperature(float temperature);

    uint16_t getAmbientTemperatureTSENSE2(void);

    float getAmbientTemperature(void);

    uint16_t getStateOfHealth(void);

    uint32_t getUserId(void);

private:
    TwoWire *_wire;

protected:
    bool readWord(uint8_t address, uint16_t *data);

    bool writeWord(uint8_t command, uint16_t data);

    bool writeThenRead(const uint8_t *write_buffer, size_t write_len, uint8_t *read_buffer, size_t read_len);

    bool i2cWrite(const uint8_t *buffer, size_t len);

    bool i2cRead(uint8_t *buffer, size_t len);

    bool _i2cRead(uint8_t *buffer, size_t len);

    uint8_t crc8(uint8_t *data, int len);
};

#endif
