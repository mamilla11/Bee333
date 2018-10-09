/*
 * FreeModbus Libary: RT-Thread Port
 * Copyright (C) 2013 Armink <armink.ztl@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: porttimer.c,v 1.60 2013/08/13 15:07:05 Armink $
 */

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"
#include "Board.h"

#include <ti/drivers/timer/GPTimerCC26XX.h>
#include <xdc/runtime/Types.h>
#include <ti/sysbios/BIOS.h>

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

static uint16_t timeout = 0;
static uint16_t downcounter = 0;

GPTimerCC26XX_Handle hTimer;

void timerCallback(GPTimerCC26XX_Handle handle, GPTimerCC26XX_IntMask interruptMask) {
    if (!--downcounter)
        pxMBPortCBTimerExpired();
}

BOOL xMBPortTimersInit(USHORT usTim1Timerout50us)
{
    GPTimerCC26XX_Params params;
    GPTimerCC26XX_Params_init(&params);
    params.width          = GPT_CONFIG_16BIT;
    params.mode           = GPT_MODE_PERIODIC_UP;
    params.debugStallMode = GPTimerCC26XX_DEBUG_STALL_OFF;
    hTimer = GPTimerCC26XX_open(GPTIMER0A, &params);

    if(hTimer == NULL) {
        return FALSE;
    }

    Types_FreqHz  freq;
    BIOS_getCpuFreq(&freq);
    GPTimerCC26XX_Value loadVal = (freq.lo / 1000000 - 1) * usTim1Timerout50us;
    GPTimerCC26XX_setLoadValue(hTimer, loadVal);
    GPTimerCC26XX_registerInterrupt(hTimer, timerCallback, GPT_INT_TIMEOUT);

	timeout = usTim1Timerout50us;

    return TRUE;
}

void vMBPortTimersEnable()
{
	downcounter = timeout;
	GPTimerCC26XX_start(hTimer);
}

void vMBPortTimersDisable()
{
    GPTimerCC26XX_stop(hTimer);
}
