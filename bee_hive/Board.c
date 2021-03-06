#include <Board.h>
#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <ti/sysbios/family/arm/m3/Hwi.h>

#include <ti/devices/cc13x0/driverlib/ioc.h>
#include <ti/devices/cc13x0/driverlib/udma.h>
#include <ti/devices/cc13x0/inc/hw_ints.h>
#include <ti/devices/cc13x0/inc/hw_memmap.h>

#include <ti/drivers/PIN.h>
#include <ti/drivers/pin/PINCC26XX.h>

const PIN_Config BoardGpioInitTable[] = {
    PIN_TERMINATE
};

const PINCC26XX_HWAttrs PINCC26XX_hwAttrs = {
    .intPriority = ~0,
    .swiPriority = 0
};

/*
 *  =============================== GPIO ===============================
 */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/gpio/GPIOCC26XX.h>

GPIO_PinConfig gpioPinConfigs[] =
{
 /* Input pins */
 GPIOCC26XX_DIO_21 | GPIO_CFG_INPUT  | GPIO_CFG_IN_INT_NONE,
 /* Output pins */
 GPIOCC26XX_DIO_18 | GPIO_CFG_OUTPUT | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_LOW,
 GPIOCC26XX_DIO_19 | GPIO_CFG_OUTPUT | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_LOW,
 GPIOCC26XX_DIO_20 | GPIO_CFG_OUTPUT | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_LOW,
};

GPIO_CallbackFxn gpioCallbackFunctions[] = {
    NULL,
};

const GPIOCC26XX_Config GPIOCC26XX_config = {
    .pinConfigs = (GPIO_PinConfig *)gpioPinConfigs,
    .callbacks = (GPIO_CallbackFxn *)gpioCallbackFunctions,
    .numberOfPinConfigs = GPIOCOUNT,
    .numberOfCallbacks  = sizeof(gpioCallbackFunctions)/sizeof(GPIO_CallbackFxn),
    .intPriority = (~0)
};

/*
 *  =============================== Power ===============================
 */
#include <ti/drivers/Power.h>
#include <ti/drivers/power/PowerCC26XX.h>

const PowerCC26XX_Config PowerCC26XX_config = {
    .policyInitFxn      = NULL,
    .policyFxn          = &PowerCC26XX_standbyPolicy,
    .calibrateFxn       = &PowerCC26XX_calibrate,
    .enablePolicy       = true,
    .calibrateRCOSC_LF  = true,
    .calibrateRCOSC_HF  = true,
};

/*
 *  =============================== RF Driver ===============================
 */
#include <ti/drivers/rf/RF.h>

const RFCC26XX_HWAttrs RFCC26XX_hwAttrs = {
    .hwiCpe0Priority = ~0,
    .hwiHwPriority   = ~0,
    .swiCpe0Priority =  0,
    .swiHwPriority   =  0,
};

/*
 *  =============================== SPI DMA ===============================
 */
#include <ti/drivers/SPI.h>
#include <ti/drivers/spi/SPICC26XXDMA.h>

SPICC26XXDMA_Object spiCC26XXDMAObjects[SPICOUNT];

const SPICC26XXDMA_HWAttrsV1 spiCC26XXDMAHWAttrs[SPICOUNT] = {
    {
        .baseAddr           = SSI0_BASE,
        .intNum             = INT_SSI0_COMB,
        .intPriority        = ~0,
        .swiPriority        = 0,
        .powerMngrId        = PowerCC26XX_PERIPH_SSI0,
        .defaultTxBufValue  = 0,
        .rxChannelBitMask   = 1<<UDMA_CHAN_SSI0_RX,
        .txChannelBitMask   = 1<<UDMA_CHAN_SSI0_TX,
        .mosiPin            = SPI0_MOSI,
        .misoPin            = SPI0_MISO,
        .clkPin             = SPI0_CLK,
        .csnPin             = SPI0_CSN,
        .minDmaTransferSize = 10
    },
};

const SPI_Config SPI_config[SPICOUNT] = {
    {
         .fxnTablePtr = &SPICC26XXDMA_fxnTable,
         .object      = &spiCC26XXDMAObjects[SPI0],
         .hwAttrs     = &spiCC26XXDMAHWAttrs[SPI0]
    },
};

const uint_least8_t SPI_count = SPICOUNT;

/*
 *  =============================== UART ===============================
 */
#include <ti/drivers/UART.h>
#include <ti/drivers/uart/UARTCC26XX.h>

UARTCC26XX_Object uartCC26XXObjects[UARTCOUNT];

const UARTCC26XX_HWAttrsV2 uartCC26XXHWAttrs[UARTCOUNT] = {
    {
        .baseAddr       = UART0_BASE,
        .powerMngrId    = PowerCC26XX_PERIPH_UART0,
        .intNum         = INT_UART0_COMB,
        .intPriority    = ~0,
        .swiPriority    = 0,
        .txPin          = UART_TX,
        .rxPin          = UART_RX,
        .ctsPin         = PIN_UNASSIGNED,
        .rtsPin         = PIN_UNASSIGNED
    }
};

const UART_Config UART_config[UARTCOUNT] = {
    {
        .fxnTablePtr = &UARTCC26XX_fxnTable,
        .object      = &uartCC26XXObjects[UART0],
        .hwAttrs     = &uartCC26XXHWAttrs[UART0]
    },
};

const uint_least8_t UART_count = UARTCOUNT;

/*
 *  =============================== I2C ===============================
 */
#include <ti/drivers/I2C.h>
#include <ti/drivers/i2c/I2CCC26XX.h>

I2CCC26XX_Object i2cCC26xxObjects[I2CCOUNT];

const I2CCC26XX_HWAttrsV1 i2cCC26xxHWAttrs[I2CCOUNT] = {
    {
        .baseAddr    = I2C0_BASE,
        .powerMngrId = PowerCC26XX_PERIPH_I2C0,
        .intNum      = INT_I2C_IRQ,
        .intPriority = ~0,
        .swiPriority = 0,
        .sdaPin      = I2C0_SDA0,
        .sclPin      = I2C0_SCL0,
    },
};

const I2C_Config I2C_config[I2CCOUNT] = {
    {
        .fxnTablePtr = &I2CCC26XX_fxnTable,
        .object      = &i2cCC26xxObjects[I2C0],
        .hwAttrs     = &i2cCC26xxHWAttrs[I2C0]
    },
};

const uint_least8_t I2C_count = I2CCOUNT;

/*
 *  ============================ GPTimer =================================
 *  Remove unused entries to reduce flash usage both in Board.c and Board.h
 */

#include <ti/drivers/timer/GPTimerCC26XX.h>

GPTimerCC26XX_Object gptimerCC26XXObjects[GPTIMERCOUNT];

const GPTimerCC26XX_HWAttrs gptimerCC26xxHWAttrs[GPTIMERPARTSCOUNT] = {
    { .baseAddr = GPT0_BASE, .intNum = INT_GPT0A, .intPriority = (~0), .powerMngrId = PowerCC26XX_PERIPH_GPT0, .pinMux = GPT_PIN_0A, },
    { .baseAddr = GPT0_BASE, .intNum = INT_GPT0B, .intPriority = (~0), .powerMngrId = PowerCC26XX_PERIPH_GPT0, .pinMux = GPT_PIN_0B, },
};

const GPTimerCC26XX_Config GPTimerCC26XX_config[GPTIMERPARTSCOUNT] = {
    { &gptimerCC26XXObjects[0], &gptimerCC26xxHWAttrs[0], GPT_A },
    { &gptimerCC26XXObjects[0], &gptimerCC26xxHWAttrs[1], GPT_B },
};


/*
 *  =============================== UDMA ===============================
 */
#include <ti/drivers/dma/UDMACC26XX.h>

UDMACC26XX_Object udmaObjects[UDMACOUNT];

const UDMACC26XX_HWAttrs udmaHWAttrs[UDMACOUNT] = {
    {
        .baseAddr    = UDMA0_BASE,
        .powerMngrId = PowerCC26XX_PERIPH_UDMA,
        .intNum      = INT_DMA_ERR,
        .intPriority = ~0
    }
};

const UDMACC26XX_Config UDMACC26XX_config[UDMACOUNT] = {
    {
         .object  = &udmaObjects[UDMA0],
         .hwAttrs = &udmaHWAttrs[UDMA0]
    },
};

/*
 *  ======== CC1310_LAUNCHXL_initGeneral ========
 */
void board_init(void)
{
    Power_init();

    if (PIN_init(BoardGpioInitTable) != PIN_SUCCESS) {
        while (1);
    }

    UART_init();
    SPI_init();
    GPIO_init();
    I2C_init();
}





