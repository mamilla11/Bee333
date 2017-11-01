/*
 * Copyright (c) 2015-2017, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/***** Includes *****/
/* XDCtools Header files */ 
#include <Board.h>
#include <xdc/std.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */ 
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Event.h>

/* TI-RTOS Header files */
#include <ti/drivers/PIN.h>
#include <ti/display/Display.h>
#include <ti/display/DisplayExt.h>

/* Board Header files */
#include "ConcentratorRadioTask.h"
#include "ConcentratorTask.h"
#include "RadioProtocol.h"


/***** Defines *****/
#define CONCENTRATOR_TASK_STACK_SIZE                    1024
#define CONCENTRATOR_TASK_PRIORITY                      3
#define CONCENTRATOR_EVENT_ALL                          0xFFFFFFFF
#define CONCENTRATOR_EVENT_NEW_SENSOR_DATA              (uint32_t)(1 << 0)
#define CONCENTRATOR_MAX_NODES                          7

/***** Type declarations *****/
struct SensorDataNode {
    uint8_t address;
    struct Data data;
    uint8_t button;
    int8_t latestRssi;
};


/***** Variable declarations *****/
static Task_Params concentratorTaskParams;
Task_Struct concentratorTask;    /* not static so you can see in ROV */
static uint8_t concentratorTaskStack[CONCENTRATOR_TASK_STACK_SIZE];
Event_Struct concentratorEvent;  /* not static so you can see in ROV */
static Event_Handle concentratorEventHandle;
static struct SensorDataNode latestActiveAdcSensorNode;
struct SensorDataNode knownSensorNodes[CONCENTRATOR_MAX_NODES];
static struct SensorDataNode* lastAddedSensorNode = knownSensorNodes;
static Display_Handle hDisplaySerial;


/***** Prototypes *****/
static void concentratorTaskFunction(UArg arg0, UArg arg1);
static void packetReceivedCallback(struct SensorPacket* packet, int8_t rssi);
static void updateLcd(void);
static void addNewNode(struct SensorDataNode* node);
static void updateNode(struct SensorDataNode* node);
static uint8_t isKnownNodeAddress(uint8_t address);


/***** Function definitions *****/
void ConcentratorTask_init(void) {

    /* Create event used internally for state changes */
    Event_Params eventParam;
    Event_Params_init(&eventParam);
    Event_construct(&concentratorEvent, &eventParam);
    concentratorEventHandle = Event_handle(&concentratorEvent);

    /* Create the concentrator radio protocol task */
    Task_Params_init(&concentratorTaskParams);
    concentratorTaskParams.stackSize = CONCENTRATOR_TASK_STACK_SIZE;
    concentratorTaskParams.priority = CONCENTRATOR_TASK_PRIORITY;
    concentratorTaskParams.stack = &concentratorTaskStack;
    Task_construct(&concentratorTask, concentratorTaskFunction, &concentratorTaskParams, NULL);
}

static void concentratorTaskFunction(UArg arg0, UArg arg1)
{
    /* Initialize display and try to open both UART and LCD types of display. */
    Display_Params params;
    Display_Params_init(&params);
    params.lineClearMode = DISPLAY_CLEAR_BOTH;

    /* Open both an available LCD display and an UART display.
     * Whether the open call is successful depends on what is present in the
     * Display_config[] array of the board file.
     *
     * Note that for SensorTag evaluation boards combined with the SHARP96x96
     * Watch DevPack, there is a pin conflict with UART such that one must be
     * excluded, and UART is preferred by default. To display on the Watch
     * DevPack, add the precompiler define BOARD_DISPLAY_EXCLUDE_UART.
     */
    hDisplaySerial = Display_open(Display_Type_UART, &params);

    /* Check if the selected Display type was found and successfully opened */
    if (hDisplaySerial)
    {
        Display_printf(hDisplaySerial, 0, 0, "Waiting for nodes...");
    }

    /* Register a packet received callback with the radio task */
    ConcentratorRadioTask_registerPacketReceivedCallback(CONCENTRATOR, packetReceivedCallback);

    /* Enter main task loop */
    while(1) {
        /* Wait for event */
        uint32_t events = Event_pend(concentratorEventHandle, 0, CONCENTRATOR_EVENT_ALL, BIOS_WAIT_FOREVER);

        /* If we got a new ADC sensor value */
        if(events & CONCENTRATOR_EVENT_NEW_SENSOR_DATA) {
            /* If we knew this node from before, update the value */
            if(isKnownNodeAddress(latestActiveAdcSensorNode.address)) {
                updateNode(&latestActiveAdcSensorNode);
            }
            else {
                /* Else add it */
                addNewNode(&latestActiveAdcSensorNode);
            }

            /* Update the values on the LCD */
            updateLcd();
        }
    }
}

static void packetReceivedCallback(struct SensorPacket* packet, int8_t rssi)
{
    if(packet->header.packetType == SENSOR_PACKET)
    {

        /* Save the values */
        latestActiveAdcSensorNode.address = packet->header.sourceAddress;
        latestActiveAdcSensorNode.data = packet->data;
        latestActiveAdcSensorNode.button = packet->button;
        latestActiveAdcSensorNode.latestRssi = rssi;

        Event_post(concentratorEventHandle, CONCENTRATOR_EVENT_NEW_SENSOR_DATA);
    }
}

static uint8_t isKnownNodeAddress(uint8_t address) {
    uint8_t found = 0;
    uint8_t i;
    for (i = 0; i < CONCENTRATOR_MAX_NODES; i++)
    {
        if (knownSensorNodes[i].address == address)
        {
            found = 1;
            break;
        }
    }
    return found;
}

static void updateNode(struct SensorDataNode* node) {
    uint8_t i;
    for (i = 0; i < CONCENTRATOR_MAX_NODES; i++) {
        if (knownSensorNodes[i].address == node->address)
        {
            knownSensorNodes[i].data = node->data;
            knownSensorNodes[i].latestRssi = node->latestRssi;
            knownSensorNodes[i].button = node->button;
            break;
        }
    }
}

static void addNewNode(struct SensorDataNode* node) {
    *lastAddedSensorNode = *node;

    /* Increment and wrap */
    lastAddedSensorNode++;
    if (lastAddedSensorNode > &knownSensorNodes[CONCENTRATOR_MAX_NODES-1])
    {
        lastAddedSensorNode = knownSensorNodes;
    }
}

static void updateLcd(void) {
    struct SensorDataNode* nodePointer = knownSensorNodes;

    while ((nodePointer < &knownSensorNodes[CONCENTRATOR_MAX_NODES]) &&
          (nodePointer->address != 0))
    {
        Display_printf(hDisplaySerial, 0, 0, "SENSOR DATA");
        Display_printf(hDisplaySerial, 0, 0, "address:     0x%02x", (int32_t)nodePointer->address);
        Display_printf(hDisplaySerial, 0, 0, "current:     %d uA",  (int32_t)nodePointer->data.current);
        Display_printf(hDisplaySerial, 0, 0, "power:       %d uW",  (int32_t)nodePointer->data.power);
        Display_printf(hDisplaySerial, 0, 0, "voltage:     %d mV",  (int32_t)nodePointer->data.voltage);
        Display_printf(hDisplaySerial, 0, 0, "light:       %d lux", (int32_t)nodePointer->data.light);
        Display_printf(hDisplaySerial, 0, 0, "temp1:       %d C",   (int32_t)nodePointer->data.temp1);
        Display_printf(hDisplaySerial, 0, 0, "temp2:       %d C",   (int32_t)nodePointer->data.temp2);
        Display_printf(hDisplaySerial, 0, 0, "humi1:       %d %rH", (int32_t)nodePointer->data.humi1);
        Display_printf(hDisplaySerial, 0, 0, "humi2:       %d %rH", (int32_t)nodePointer->data.humi2);
        Display_printf(hDisplaySerial, 0, 0, "pressure:    %d hPa", (int32_t)nodePointer->data.pressure);
        Display_printf(hDisplaySerial, 0, 0, "acc voltage: %d V",   nodePointer->data.acc_voltage);
        Display_printf(hDisplaySerial, 0, 0, "button:      %d",     nodePointer->button);
        Display_printf(hDisplaySerial, 0, 0, "RSSI:        %04d\n", nodePointer->latestRssi);

        nodePointer++;
    }
}
