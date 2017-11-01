#ifndef SENSORS_DRV_HDC1080_H_
#define SENSORS_DRV_HDC1080_H_

#include <Board.h>

#define HDC1080_REG_TEMPERATURE                            0x00
#define HDC1080_REG_HUMIDITY                               0x01
#define HDC1080_REG_CONFIG                                 0x02
#define HDC1080_REG_SERIAL_ID_1                            0xFB
#define HDC1080_REG_SERIAL_ID_2                            0xFC
#define HDC1080_REG_SERIAL_ID_3                            0xFD
#define HDC1080_REG_MANUFACTURER_ID                        0xFE
#define HDC1080_REG_DIE_ID                                 0xFF

#define HDC1080_RESET                                      0x8000
#define HDC1080_HEAT                                       0x2000
#define HDC1080_MODE                                       0x1000
#define HDC1080_BTST                                       0x0800
#define HDC1080_TRES                                       0x0400
#define HDC1080_HRES                                       0x0300

#define HDC1080_MANUFACTURER_ID                            0x5449
#define HDC1080_DIE_ID                                     0x1050

#define HDC1080_MEASURE_DIVIDER                            65536
#define HDC1080_TEMPERATURE_COEFF_1                        165
#define HDC1080_TEMPERATURE_COEFF_2                        40
#define HDC1080_HUMIDITY_COEFF_1                           100

typedef enum {
    HDC1080_DATA_SEQUENCED,
    HDC1080_DATA_SEPARATED,
}HDC1080_Mode;

typedef enum {
    HDC1080_TRES_14_BIT,
    HDC1080_TRES_11_BIT,
} HDC1080_TempResolution;

typedef enum {
    HDC1080_HRES_14_BIT = (uint16_t)0x0000,
    HDC1080_HRES_11_BIT = (uint16_t)0x0100,
    HDC1080_HRES_08_BIT = (uint16_t)0x0200,
} HDC1080_HumiResolution;


typedef int8_t (*HDC1080_transfer)(void *, size_t, void *, size_t, uint_least8_t);

void HDC1080_init(uint8_t address, HDC1080_transfer callback);
bool HDC1080_is_present();
void HDC1080_reset();
void HDC1080_heat_enable();
void HDC1080_heat_disable();
void HDC1080_set_mode(HDC1080_Mode mode);
void HDC1080_set_temp_resolution(HDC1080_TempResolution resolution);
void HDC1080_set_humi_resolution(HDC1080_HumiResolution resolution);
float HDC1080_get_temperature_in_C();
float HDC1080_get_humidity();
uint16_t HDC1080_get_serial_ID_1();
uint16_t HDC1080_get_serial_ID_2();
uint16_t HDC1080_get_serial_ID_3();
uint16_t HDC1080_get_manufacturer_ID();
uint16_t HDC1080_get_die_ID();

#endif /* SENSORS_DRV_HDC1080_H_ */
