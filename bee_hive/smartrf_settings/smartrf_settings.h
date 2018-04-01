#ifndef _SMARTRF_SETTINGS_H_
#define _SMARTRF_SETTINGS_H_

//*********************************************************************************
// Generated by SmartRF Studio version 2.7.0 (build #23)
// Tested for SimpleLink SDK version: CC13x0 SDK 1.30.xx.xx
// Device: CC1310 Rev. 2.1 (Rev. B)
//
//*********************************************************************************

#ifdef DEVICE_FAMILY
    #undef DEVICE_FAMILY_PATH
    #define DEVICE_FAMILY_PATH(x) <ti/devices/DEVICE_FAMILY/x>
#else
    #error "You must define DEVICE_FAMILY at the project level as one of cc26x0, cc26x0r2, cc13x0, etc."
#endif


#include DEVICE_FAMILY_PATH(driverlib/rf_mailbox.h)
#include DEVICE_FAMILY_PATH(driverlib/rf_common_cmd.h)
#include DEVICE_FAMILY_PATH(driverlib/rf_prop_cmd.h)
#include <ti/drivers/rf/RF.h>


// TI-RTOS RF Mode Object
extern RF_Mode RF_prop;


// RF Core API commands
extern rfc_CMD_PROP_RADIO_DIV_SETUP_t RF_cmdPropRadioDivSetup;
extern rfc_CMD_FS_t RF_cmdFs;
extern rfc_CMD_PROP_TX_t RF_cmdPropTx;
extern rfc_CMD_PROP_RX_t RF_cmdPropRx;


#endif // _SMARTRF_SETTINGS_H_

