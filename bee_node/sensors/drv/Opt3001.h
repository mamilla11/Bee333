#ifndef SENSORS_DRV_OPT3001_H_
#define SENSORS_DRV_OPT3001_H_

#include <stdint.h>
#include <math.h>
#include <Board.h>

#define OPT3001_REG_RESULT                         0x00
#define OPT3001_REG_CONFIG                         0x01
#define OPT3001_REG_LOW_LIMIT                      0x02
#define OPT3001_REG_HIGH_LIMIT                     0x03
#define OPT3001_REG_MANUFACTURER_ID                0x7E
#define OPT3001_REG_DEVICE_ID                      0x7F

#define OPT3001_FULLSCALE_RANGE_40_95             0x0000
#define OPT3001_FULLSCALE_RANGE_81_90             0x1000
#define OPT3001_FULLSCALE_RANGE_163_80            0x2000
#define OPT3001_FULLSCALE_RANGE_327_60            0x3000
#define OPT3001_FULLSCALE_RANGE_655_20            0x4000
#define OPT3001_FULLSCALE_RANGE_1310_40           0x5000
#define OPT3001_FULLSCALE_RANGE_2620_80           0x6000
#define OPT3001_FULLSCALE_RANGE_5241_60           0x7000
#define OPT3001_FULLSCALE_RANGE_10483_20          0x8000
#define OPT3001_FULLSCALE_RANGE_20966_40          0x9000
#define OPT3001_FULLSCALE_RANGE_41932_80          0xA000
#define OPT3001_FULLSCALE_RANGE_83865_60          0xB000
#define OPT3001_FULLSCALE_RANGE_AUTO              0xC000
#define OPT3001_FULLSCALE_RANGE_MASK              0x0FFF

#define OPT3001_CONVERSION_TIME_100               0x0000
#define OPT3001_CONVERSION_TIME_800               0x0800
#define OPT3001_CONVERSION_TIME_MASK              0xF7FF

#define OPT3001_CONVERSION_MODE_SHUTDOWN          0x0000
#define OPT3001_CONVERSION_MODE_SINGLE            0x0200
#define OPT3001_CONVERSION_MODE_CONTINUOUS        0x0400
#define OPT3001_CONVERSION_MODE_MASK              0xF9FF

#define OPT3001_FAULT_MODE_HYSTERESIS             0x0000
#define OPT3001_FAULT_MODE_WINDOW_LATCH           0x0010
#define OPT3001_FAULT_MODE_MASK                   0xFFEF

#define OPT3001_INT_POLARITY_NORMAL               0x0000
#define OPT3001_INT_POLARITY_REVERSE              0x0008
#define OPT3001_INT_POLARITY_MASK                 0xFFF7

#define OPT3001_FAULT_COUNT_1                     0x0000
#define OPT3001_FAULT_COUNT_2                     0x0001
#define OPT3001_FAULT_COUNT_4                     0x0002
#define OPT3001_FAULT_COUNT_8                     0x0003
#define OPT3001_FAULT_COUNT_MASK                  0xFFFC

#define OPT3001_FLAG_OVERFLOW_MASK                0x0100
#define OPT3001_FLAG_CONVERSION_READY_MASK        0x0080
#define OPT3001_FLAG_HIGH_LIMIT_MASK              0x0040
#define OPT3001_FLAG_LOW_LIMIT_MASK               0x0020

#define OPT3001_MANUFACTURER_ID                   0x5449
#define OPT3001_DEVICE_ID                         0x3001

#define OPT3001_RESULT_MANTISSA_MASK              0x0FFF
#define OPT3001_RESULT_EXPONENT_OFFSET            12

typedef enum {
    MODE_SHUTDOWN,
    MODE_SINGLE_SHOT,
    MODE_CONTINUOUS
} OPT3001_Mode;

typedef enum {
    RANGE_40_95,
    RANGE_81_90,
    RANGE_163_80,
    RANGE_327_60,
    RANGE_655_20,
    RANGE_1310_40,
    RANGE_2620_80,
    RANGE_5241_60,
    RANGE_10483_20,
    RANGE_20966_40,
    RANGE_41932_80,
    RANGE_83865_60,
    RANGE_AUTO
} OPT3001_Range;

typedef enum {
    CONVTIME_100MS,
    CONVTIME_800MS
} OPT3001_ConvTime;

typedef enum {
    FLTMODE_HYSTERESIS,
    FLTMODE_WINDOW_LATCH
} OPT3001_FaultMode;

typedef enum {
    FLTCOUNT_1,
    FLTCOUNT_2,
    FLTCOUNT_4,
    FLTCOUNT_8
} OPT3001_FaultCount;

typedef enum {
    LOW,
    HIGH
} OPT3001_Polarity;

typedef int8_t (*OPT3001_transfer)(void *, size_t, void *, size_t, uint_least8_t);

void OPT3001_init(uint8_t address, OPT3001_transfer callback);
bool OPT3001_is_present();
void OPT3001_set_mode(OPT3001_Mode mode);
void OPT3001_set_range(OPT3001_Range range);
void OPT3001_set_conversion_time(OPT3001_ConvTime time);
void OPT3001_set_fault_mode(OPT3001_FaultMode mode);
void OPT3001_set_fault_count(OPT3001_FaultCount count);
void OPT3001_set_int_polarity(OPT3001_Polarity polarity);
float OPT3001_get_light_in_lux();
void OPT3001_set_limits_in_lux(float lo_limit, float hi_limit);
uint16_t OPT3001_get_device_ID();
uint16_t OPT3001_get_manufacturer_ID();

#endif /* SENSORS_DRV_OPT3001_H_ */
