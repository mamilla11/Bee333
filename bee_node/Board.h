#ifndef __CC1310_LAUNCHXL_BOARD_H__
#define __CC1310_LAUNCHXL_BOARD_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <ti/drivers/PIN.h>
#include <ti/devices/cc13x0/driverlib/ioc.h>

/* Externs */
extern const PIN_Config BoardGpioInitTable[];

/* Defines */
#define CC1310_LAUNCHXL

#define LED_BEE               IOID_1
#define LED_RED               IOID_6
#define LED_GREEN             IOID_7
#define LED_BLUE              IOID_25
#define BUTTON_USER           IOID_13
#define SENSORS_POWER         IOID_19
#define I2C0_SCL0             IOID_4
#define I2C0_SDA0             IOID_5

#define ANALOG_0              IOID_23
#define ANALOG_1              PIN_UNASSIGNED
#define ANALOG_2              PIN_UNASSIGNED
#define ANALOG_3              PIN_UNASSIGNED
#define ANALOG_4              PIN_UNASSIGNED
#define ANALOG_5              PIN_UNASSIGNED
#define ANALOG_6              PIN_UNASSIGNED
#define ANALOG_7              PIN_UNASSIGNED

void board_init(void);

typedef enum I2CName {
    I2C0 = 0,

    I2CCOUNT
} I2CName;

typedef enum ADCName {
    ADC0 = 0,
    ADC1,
    ADC2,
    ADC3,
    ADC4,
    ADC5,
    ADC6,
    ADC7,
    ADCDCOUPL,
    ADCVSS,
    ADCVDDS,

    ADCCOUNT
} ADCName;

typedef enum CC1310_LAUNCHXL_ADCBufName {
    ADCBUF0 = 0,

    ADCBUFCOUNT
} CC1310_LAUNCHXL_ADCBufName;

typedef enum ADCBuf0ChannelName {
    ADCBUF0CHANNEL0 = 0,
    ADCBUF0CHANNEL1,
    ADCBUF0CHANNEL2,
    ADCBUF0CHANNEL3,
    ADCBUF0CHANNEL4,
    ADCBUF0CHANNEL5,
    ADCBUF0CHANNEL6,
    ADCBUF0CHANNEL7,
    ADCBUF0CHANNELVDDS,
    ADCBUF0CHANNELDCOUPL,
    ADCBUF0CHANNELVSS,

    ADCBUF0CHANNELCOUNT
} ADCBuf0ChannelName;

typedef enum UDMAName {
    UDMA0 = 0,

    UDMACOUNT
} UDMAName;

#ifdef __cplusplus
}
#endif

#endif /* __CC1310_LAUNCHXL_BOARD_H__ */
