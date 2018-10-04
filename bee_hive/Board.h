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
#define SPI0_MISO             PIN_UNASSIGNED
#define SPI0_MOSI             IOID_9
#define SPI0_CLK              IOID_10
#define SPI0_CSN              PIN_UNASSIGNED
#define UART_RX               IOID_2
#define UART_TX               IOID_3
#define MAX7313_IRQ           IOID_16

#define I2C0_SCL0             IOID_4
#define I2C0_SDA0             IOID_5

void board_init(void);

typedef enum SPIName {
    SPI0 = 0,
    SPICOUNT
} SPIName;

typedef enum UARTName {
    UART0 = 0,
    UARTCOUNT
} UARTName;

typedef enum I2CName {
    I2C0 = 0,
    I2CCOUNT
} I2CName;

typedef enum GPIOName {
    PAPER_BUSY = 0,
    PAPER_CS,
    PAPER_DC,
    PAPER_RST,

    GPIOCOUNT
} GPIOName;

typedef enum UDMAName {
    UDMA0 = 0,
    UDMACOUNT
} UDMAName;

typedef enum GPTimerName
{
    GPTIMER0A = 0,
    GPTIMER0B,
    GPTIMERPARTSCOUNT
} GPTimerName;

typedef enum GPTimers
{
    GPTIMER0 = 0,
    GPTIMERCOUNT
} GPTimers;

#ifdef __cplusplus
}
#endif

#endif /* __CC1310_LAUNCHXL_BOARD_H__ */
