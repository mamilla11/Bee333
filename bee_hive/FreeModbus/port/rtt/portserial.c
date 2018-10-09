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
 * File: $Id: portserial.c,v 1.60 2013/08/13 15:07:05 Armink $
 */

#include "port.h"
#include "Board.h"
#include "stdio.h"
#include <ti/drivers/UART.h>
#include <ti/drivers/uart/UARTCC26XX.h>

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

UART_Handle uart;
UART_Params uartParams;
uint8_t rx_buffer[1];

static void rx_callback(UART_Handle handle, void *buf, size_t count);
static void tx_callback(UART_Handle handle, void *buf, size_t count);


/* ----------------------- Start implementation -----------------------------*/
BOOL xMBPortSerialInit(UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity)
{
	BOOL bStatus = TRUE;

	UART_Params_init(&uartParams);

	uartParams.readMode       = UART_MODE_CALLBACK;
	uartParams.writeMode      = UART_MODE_CALLBACK;
	uartParams.readCallback   = rx_callback;
	uartParams.writeCallback  = tx_callback;
	uartParams.readReturnMode = UART_RETURN_FULL;
	uartParams.readDataMode   = UART_DATA_BINARY;
	uartParams.writeDataMode  = UART_DATA_BINARY;
	uartParams.readEcho       = UART_ECHO_OFF;
	uartParams.baudRate       = ulBaudRate;
	uartParams.dataLength     = UART_LEN_8;
	uartParams.stopBits       = UART_STOP_ONE;

	switch (eParity)
	{
	case MB_PAR_NONE:
	    uartParams.parityType = UART_PAR_NONE;
		break;
	case MB_PAR_ODD:
	    uartParams.parityType = UART_PAR_ODD;
		break;
	case MB_PAR_EVEN:
	    uartParams.parityType = UART_PAR_EVEN;
		break;
	default:
		bStatus = FALSE;
		break;
	}

	uart = UART_open(UART0, &uartParams);

    if (uart == NULL)
        bStatus = FALSE;

	return bStatus;
}

void UART_poll() {
    UART_read(uart, rx_buffer, 1);
}

void vMBPortSerialEnable(BOOL xRxEnable, BOOL xTxEnable)
{
    if (xTxEnable) {
        pxMBFrameCBTransmitterEmpty();
    }
}

void vMBPortClose(void)
{
    UART_close(uart);
}

BOOL xMBPortSerialPutByte(CHAR ucByte)
{
    UART_write(uart, &ucByte, 1);
    return TRUE;
}

BOOL xMBPortSerialGetByte(CHAR * pucByte)
{
	*pucByte = rx_buffer[0];
	return TRUE;
}

void rx_callback(UART_Handle handle, void *buf, size_t count)
{
    int i;

    for (i = 0; i < count; i++) {
        pxMBFrameCBByteReceived();
    }
}

void tx_callback(UART_Handle handle, void *buf, size_t count)
{
    pxMBFrameCBTransmitterEmpty();
}

