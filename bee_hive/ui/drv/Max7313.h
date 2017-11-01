#ifndef UI_DRV_MAX7313_H_
#define UI_DRV_MAX7313_H_

#include "../../Board.h"

#define MAX7313_READ_INPUT_P7_P0_ADDR           0x00
#define MAX7313_READ_INPUT_P15_P8_ADDR          0x01
#define MAX7313_PHASE0_P7_P0_ADDR               0x02
#define MAX7313_PHASE0_P15_P8_ADDR              0x03
#define MAX7313_PORT_CONFIG_P7_P0_ADDR          0x06
#define MAX7313_PORT_CONFIG_P15_P8_ADDR         0x07
#define MAX7313_MASTER_INTENSITY_ADDR           0x0E
#define MAX7313_CONFIGURATION_ADDR              0x0F
#define MAX7313_OUTPUT_INTENSITY_P1_P0_ADDR     0x10
#define MAX7313_OUTPUT_INTENSITY_P3_P2_ADDR     0x11
#define MAX7313_OUTPUT_INTENSITY_P5_P4_ADDR     0x12
#define MAX7313_OUTPUT_INTENSITY_P7_P6_ADDR     0x13
#define MAX7313_OUTPUT_INTENSITY_P9_P8_ADDR     0x14
#define MAX7313_OUTPUT_INTENSITY_P11_P10_ADDR   0x15
#define MAX7313_OUTPUT_INTENSITY_P13_P12_ADDR   0x16
#define MAX7313_OUTPUT_INTENSITY_P15_P14_ADDR   0x17

#define MAX7313_LED_INTENSITY_1_16              0x00
#define MAX7313_LED_INTENSITY_2_16              0x11
#define MAX7313_LED_INTENSITY_3_16              0x22
#define MAX7313_LED_INTENSITY_4_16              0x33
#define MAX7313_LED_INTENSITY_5_16              0x44
#define MAX7313_LED_INTENSITY_6_16              0x55
#define MAX7313_LED_INTENSITY_7_16              0x66
#define MAX7313_LED_INTENSITY_8_16              0x77
#define MAX7313_LED_INTENSITY_9_16              0x88
#define MAX7313_LED_INTENSITY_10_16             0x99
#define MAX7313_LED_INTENSITY_11_16             0xAA
#define MAX7313_LED_INTENSITY_12_16             0xBB
#define MAX7313_LED_INTENSITY_13_16             0xCC
#define MAX7313_LED_INTENSITY_14_16             0xDD
#define MAX7313_LED_INTENSITY_15_16             0xEE
#define MAX7313_LED_INTENSITY_16_16             0xFF

#define MAX7313_LED_ON_MASK                     0
#define MAX7313_LED_OFF_MASK                    0xFFFF
#define MAX7313_MAX_MASTER_INTENSITY            0xFF

typedef void (*MAX7313_transfer)(void *, size_t, void *, size_t, uint_least8_t);

void MAX7313_init(MAX7313_transfer callback);
void MAX7313_WriteConfiguration(uint8_t max7313_addr, uint8_t config);
void MAX7313_WritePortConfig(uint8_t max7313_addr, uint16_t portmask);
void MAX7313_WritePhase0(uint8_t max7313_addr, uint16_t phasemask);
void MAX7313_WritePhase0_P7_P0(uint8_t max7313_addr, uint8_t phasemask);
void MAX7313_WritePhase0_P15_P8(uint8_t max7313_addr, uint8_t phasemask);
void MAX7313_WriteIntensity(uint8_t max7313_addr, uint8_t intensity);
void MAX7313_WriteMasterIntensity(uint8_t max7313_addr, uint8_t intensity);

uint16_t MAX7313_ReadPhase0(uint8_t max7313_addr);
uint8_t MAX7313_ReadPhase0_P7_P0(uint8_t max7313_addr);
uint8_t MAX7313_ReadConfiguration(uint8_t max7313_addr);
uint16_t MAX7313_ReadPortConfig(uint8_t max7313_addr);
uint8_t MAX7313_ReadGlobalIntensity(uint8_t max7313_addr);
uint16_t MAX7313_ReadInput(uint8_t max7313_addr);

#endif /* UI_DRV_MAX7313_H_ */
