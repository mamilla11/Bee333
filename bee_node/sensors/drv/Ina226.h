#ifndef SENSORS_DRV_INA226_H_
#define SENSORS_DRV_INA226_H_

#include <math.h>
#include <Board.h>

#define INA226_REG_CONFIG                                 0x00
#define INA226_REG_SHUNT_VOLTAGE                          0x01
#define INA226_REG_BUS_VOLTAGE                            0x02
#define INA226_REG_POWER                                  0x03
#define INA226_REG_CURRENT                                0x04
#define INA226_REG_CALIBRATION                            0x05
#define INA226_REG_MASK_ENABLE                            0x06
#define INA226_REG_ALERT_LIMIT                            0x07
#define INA226_REG_MANUFACTURER_ID                        0xFE
#define INA226_REG_DIE_ID                                 0xFF

#define INA226_RESET_SYSTEM                              0x8000
#define INA226_RESET_SYSTEM_MASK                         0x7FFF

#define INA226_AVERAGING_MODE_1                          0x0000
#define INA226_AVERAGING_MODE_4                          0x0200
#define INA226_AVERAGING_MODE_16                         0x0400
#define INA226_AVERAGING_MODE_64                         0x0600
#define INA226_AVERAGING_MODE_128                        0x0800
#define INA226_AVERAGING_MODE_256                        0x0A00
#define INA226_AVERAGING_MODE_512                        0x0C00
#define INA226_AVERAGING_MODE_1024                       0x0E00
#define INA226_AVERAGING_MODE_MASK                       0xF1FF

#define INA226_BUS_VOLTAGE_CONVERSION_TIME_140_US        0x0000
#define INA226_BUS_VOLTAGE_CONVERSION_TIME_204_US        0x0040
#define INA226_BUS_VOLTAGE_CONVERSION_TIME_332_US        0x0080
#define INA226_BUS_VOLTAGE_CONVERSION_TIME_588_US        0x00C0
#define INA226_BUS_VOLTAGE_CONVERSION_TIME_1_100_MS      0x0100
#define INA226_BUS_VOLTAGE_CONVERSION_TIME_2_116_MS      0x0140
#define INA226_BUS_VOLTAGE_CONVERSION_TIME_4_156_MS      0x0180
#define INA226_BUS_VOLTAGE_CONVERSION_TIME_8_244_MS      0x01C0
#define INA226_BUS_VOLTAGE_CONVERSION_TIME_MASK          0xFE3F

#define INA226_SHUNT_VOLTAGE_CONVERSION_TIME_140_US      0x0000
#define INA226_SHUNT_VOLTAGE_CONVERSION_TIME_204_US      0x0008
#define INA226_SHUNT_VOLTAGE_CONVERSION_TIME_332_US      0x0010
#define INA226_SHUNT_VOLTAGE_CONVERSION_TIME_588_US      0x0018
#define INA226_SHUNT_VOLTAGE_CONVERSION_TIME_1_100_MS    0x0020
#define INA226_SHUNT_VOLTAGE_CONVERSION_TIME_2_116_MS    0x0028
#define INA226_SHUNT_VOLTAGE_CONVERSION_TIME_4_156_MS    0x0030
#define INA226_SHUNT_VOLTAGE_CONVERSION_TIME_8_244_MS    0x0038
#define INA226_SHUNT_VOLTAGE_CONVERSION_TIME_MASK        0xFFC7

#define INA226_MODE_TRIG_SHUTDOWN                        0x0000
#define INA226_MODE_TRIG_SHUNT_VOLTAGE                   0x0001
#define INA226_MODE_TRIG_BUS_VOLTAGE                     0x0002
#define INA226_MODE_TRIG_SHUNT_AND_BUS_VOLTAGE           0x0003
#define INA226_MODE_CONT_SHUTDOWN                        0x0004
#define INA226_MODE_CONT_SHUNT_VOLTAGE                   0x0005
#define INA226_MODE_CONT_BUS_VOLTAGE                     0x0006
#define INA226_MODE_CONT_SHUNT_AND_BUS_VOLTAGE           0x0007
#define INA226_MODE_MASK                                 0xFFF8

#define INA226_CALIBRATION_FACTOR                        0.00512F
#define INA226_SHUNT_VOLTAGE_FULL_SCALE_RANGE            0.08192F
#define INA226_SHUNT_VOLTAGE_LSB                         0.0000025F
#define INA226_BUS_VOLTAGE_LSB                           0.00125F
#define INA226_MANUFACTURER_ID                           0x5449
#define INA226_DIE_ID                                    0x2260

typedef enum {
    SAMPLES_1,
    SAMPLES_4,
    SAMPLES_16,
    SAMPLES_64,
    SAMPLES_128,
    SAMPLES_256,
    SAMPLES_512,
    SAMPLES_1024
}INA226_AveragingMode;

typedef enum {
    CONVTIME_140_US,
    CONVTIME_204_US,
    CONVTIME_332_US,
    CONVTIME_588_US,
    CONVTIME_1_100_MS,
    CONVTIME_2_116_MS,
    CONVTIME_4_156_MS,
    CONVTIME_8_244_MS
}INA226_ConversionTime;

typedef enum {
    TRIG_SHUTDOWN,
    TRIG_SHUNT_VOLTAGE,
    TRIG_BUS_VOLTAGE,
    TRIG_SHUNT_AND_BUS_VOLTAGE,
    CONT_SHUTDOWN,
    CONT_SHUNT_VOLTAGE,
    CONT_BUS_VOLTAGE,
    CONT_SHUNT_AND_BUS_VOLTAGE
}INA226_OperatingMode;

typedef int8_t (*INA226_transfer)(void *, size_t, void *, size_t, uint_least8_t);

void INA226_init(uint8_t address, INA226_transfer callback);
bool INA226_is_present();
void INA226_reset();
void INA226_set_averaging_mode(INA226_AveragingMode mode);
void INA226_set_bus_voltage_conversion_time(INA226_ConversionTime time);
void INA226_set_shunt_voltage_conversion_time(INA226_ConversionTime time);
void INA226_set_mode(INA226_OperatingMode mode);
void INA226_set_calibration(float shunt_resistance_in_Om);
float INA226_get_shunt_voltage_in_V();
float INA226_get_bus_voltage_in_V();
float INA226_get_power_in_W();
float INA226_get_current_in_A();
uint16_t INA226_get_manufacturer_ID();
uint16_t INA226_get_die_ID();

#endif /* SENSORS_DRV_INA226_H_ */
