#include <Board.h>
#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <ti/sysbios/family/arm/m3/Hwi.h>

#include <ti/devices/cc13x0/driverlib/ioc.h>
#include <ti/devices/cc13x0/driverlib/udma.h>
#include <ti/devices/cc13x0/inc/hw_ints.h>
#include <ti/devices/cc13x0/inc/hw_memmap.h>

/*
 *  =============================== ADC ===============================
 */
#include <ti/drivers/ADC.h>
#include <ti/drivers/adc/ADCCC26XX.h>

ADCCC26XX_Object adcCC26xxObjects[ADCCOUNT];


const ADCCC26XX_HWAttrs adcCC26xxHWAttrs[ADCCOUNT] = {
    {
        .adcDIO              = ANALOG_0,
        .adcCompBInput       = ADC_COMPB_IN_AUXIO7,
        .refSource           = ADCCC26XX_FIXED_REFERENCE,
        .samplingDuration    = ADCCC26XX_SAMPLING_DURATION_10P6_US,
        .inputScalingEnabled = true,
        .triggerSource       = ADCCC26XX_TRIGGER_MANUAL,
        .returnAdjustedVal   = 0
    },
    {
        .adcDIO              = ANALOG_1,
        .adcCompBInput       = ADC_COMPB_IN_AUXIO6,
        .refSource           = ADCCC26XX_FIXED_REFERENCE,
        .samplingDuration    = ADCCC26XX_SAMPLING_DURATION_2P7_US,
        .inputScalingEnabled = true,
        .triggerSource       = ADCCC26XX_TRIGGER_MANUAL,
        .returnAdjustedVal   = 0
    },
    {
        .adcDIO              = ANALOG_2,
        .adcCompBInput       = ADC_COMPB_IN_AUXIO5,
        .refSource           = ADCCC26XX_FIXED_REFERENCE,
        .samplingDuration    = ADCCC26XX_SAMPLING_DURATION_2P7_US,
        .inputScalingEnabled = true,
        .triggerSource       = ADCCC26XX_TRIGGER_MANUAL,
        .returnAdjustedVal   = 0
    },
    {
        .adcDIO              = ANALOG_3,
        .adcCompBInput       = ADC_COMPB_IN_AUXIO4,
        .refSource           = ADCCC26XX_FIXED_REFERENCE,
        .samplingDuration    = ADCCC26XX_SAMPLING_DURATION_2P7_US,
        .inputScalingEnabled = true,
        .triggerSource       = ADCCC26XX_TRIGGER_MANUAL,
        .returnAdjustedVal   = 0
    },
    {
        .adcDIO              = ANALOG_4,
        .adcCompBInput       = ADC_COMPB_IN_AUXIO3,
        .refSource           = ADCCC26XX_FIXED_REFERENCE,
        .samplingDuration    = ADCCC26XX_SAMPLING_DURATION_2P7_US,
        .inputScalingEnabled = true,
        .triggerSource       = ADCCC26XX_TRIGGER_MANUAL,
        .returnAdjustedVal   = 0
    },
    {
        .adcDIO              = ANALOG_5,
        .adcCompBInput       = ADC_COMPB_IN_AUXIO2,
        .refSource           = ADCCC26XX_FIXED_REFERENCE,
        .samplingDuration    = ADCCC26XX_SAMPLING_DURATION_2P7_US,
        .inputScalingEnabled = true,
        .triggerSource       = ADCCC26XX_TRIGGER_MANUAL,
        .returnAdjustedVal   = 0
    },
    {
        .adcDIO              = ANALOG_6,
        .adcCompBInput       = ADC_COMPB_IN_AUXIO1,
        .refSource           = ADCCC26XX_FIXED_REFERENCE,
        .samplingDuration    = ADCCC26XX_SAMPLING_DURATION_2P7_US,
        .inputScalingEnabled = true,
        .triggerSource       = ADCCC26XX_TRIGGER_MANUAL,
        .returnAdjustedVal   = 0
    },
    {
        .adcDIO              = ANALOG_7,
        .adcCompBInput       = ADC_COMPB_IN_AUXIO0,
        .refSource           = ADCCC26XX_FIXED_REFERENCE,
        .samplingDuration    = ADCCC26XX_SAMPLING_DURATION_10P9_MS,
        .inputScalingEnabled = true,
        .triggerSource       = ADCCC26XX_TRIGGER_MANUAL,
        .returnAdjustedVal   = 0
    },
    {
        .adcDIO              = PIN_UNASSIGNED,
        .adcCompBInput       = ADC_COMPB_IN_DCOUPL,
        .refSource           = ADCCC26XX_FIXED_REFERENCE,
        .samplingDuration    = ADCCC26XX_SAMPLING_DURATION_2P7_US,
        .inputScalingEnabled = true,
        .triggerSource       = ADCCC26XX_TRIGGER_MANUAL,
        .returnAdjustedVal   = 0
    },
    {
        .adcDIO              = PIN_UNASSIGNED,
        .adcCompBInput       = ADC_COMPB_IN_VSS,
        .refSource           = ADCCC26XX_FIXED_REFERENCE,
        .samplingDuration    = ADCCC26XX_SAMPLING_DURATION_2P7_US,
        .inputScalingEnabled = true,
        .triggerSource       = ADCCC26XX_TRIGGER_MANUAL,
        .returnAdjustedVal   = 0
    },
    {
        .adcDIO              = PIN_UNASSIGNED,
        .adcCompBInput       = ADC_COMPB_IN_VDDS,
        .refSource           = ADCCC26XX_FIXED_REFERENCE,
        .samplingDuration    = ADCCC26XX_SAMPLING_DURATION_2P7_US,
        .inputScalingEnabled = true,
        .triggerSource       = ADCCC26XX_TRIGGER_MANUAL,
        .returnAdjustedVal   = 0
    }
};

const ADC_Config ADC_config[ADCCOUNT] = {
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[ADC0], &adcCC26xxHWAttrs[ADC0]},
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[ADC1], &adcCC26xxHWAttrs[ADC1]},
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[ADC2], &adcCC26xxHWAttrs[ADC2]},
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[ADC3], &adcCC26xxHWAttrs[ADC3]},
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[ADC4], &adcCC26xxHWAttrs[ADC4]},
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[ADC5], &adcCC26xxHWAttrs[ADC5]},
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[ADC6], &adcCC26xxHWAttrs[ADC6]},
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[ADC7], &adcCC26xxHWAttrs[ADC7]},
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[ADCDCOUPL], &adcCC26xxHWAttrs[ADCDCOUPL]},
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[ADCVSS], &adcCC26xxHWAttrs[ADCVSS]},
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[ADCVDDS], &adcCC26xxHWAttrs[ADCVDDS]},
};

const uint_least8_t ADC_count = ADCCOUNT;

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
 *  =============================== PIN ===============================
 */
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
 *  =============================== Power ===============================
 */
#include <ti/drivers/Power.h>
#include <ti/drivers/power/PowerCC26XX.h>

const PowerCC26XX_Config PowerCC26XX_config = {
    .policyInitFxn      = NULL,
    .policyFxn          = &PowerCC26XX_standbyPolicy,
    .calibrateFxn       = &PowerCC26XX_calibrate,
    .enablePolicy       = TRUE,
    .calibrateRCOSC_LF  = TRUE,
    .calibrateRCOSC_HF  = TRUE,
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
}
