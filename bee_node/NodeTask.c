/*
 * Copyright (c) 2015-2016, Texas Instruments Incorporated
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

#include <xdc/std.h>
#include <xdc/runtime/System.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Event.h>
#include <ti/sysbios/knl/Clock.h>

#include <ti/drivers/PIN.h>

#include "Board.h"

#include <sensors/SensorManager.h>
#include "RadioProtocol.h"
#include "NodeTask.h"
#include "NodeRadioTask.h"


/***** Defines *****/
#define NODE_TASK_STACK_SIZE                            1024
#define NODE_TASK_PRIORITY                              3
#define EVENT_ALL                                       (uint32_t)0xFFFFFFFF
#define EVENT_UPDATE_DATA                               (uint32_t)(1 << 0)
#define UPDATE_DATA_1S_TIMEOUT                          (uint32_t)1000
#define UPDATE_DATA_5S_TIMEOUT                          (uint32_t)5000
#define UPDATE_DATA_10S_TIMEOUT                          (uint32_t)10000
#define UPDATE_DATA_30S_TIMEOUT                          (uint32_t)30000

/***** Variable declarations *****/
static Task_Params nodeTaskParams;
Task_Struct nodeTask;
static uint8_t nodeTaskStack[NODE_TASK_STACK_SIZE];
Event_Struct nodeEvent;
static Event_Handle nodeEventHandle;
Clock_Struct fastReportTimeoutClock;
static Clock_Handle update_data;

static PIN_Handle pin_handle;
static PIN_State pin_state;

PIN_Config pinTable[] = {
   BUTTON_USER   | PIN_INPUT_EN | PIN_PULLUP | PIN_IRQ_NEGEDGE,
   LED_BEE       | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
   LED_RED       | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
   LED_GREEN     | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
   LED_BLUE      | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
   SENSORS_POWER | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
   PIN_TERMINATE
};

typedef enum {RED,GREEN,BLUE,CYAN,MAGENTA,YELLOW,WHITE} Color;
typedef enum {OFF, ON} State;
/***** Prototypes *****/
static void nodeTaskFunction(UArg arg0, UArg arg1);
void update_data_callback(UArg arg0);
void button_callback(PIN_Handle handle, PIN_Id pinId);
void set_update_data_timeout(uint32_t timeout);
void set_led_color(Color color, State state);
void delay_ms(uint32_t t_ms);

static uint32_t update_data_timeout = UPDATE_DATA_10S_TIMEOUT;

void NodeTask_init(void)
{
    Event_Params eventParam;
    Event_Params_init(&eventParam);
    Event_construct(&nodeEvent, &eventParam);
    nodeEventHandle = Event_handle(&nodeEvent);

    Clock_Params clkParams;
    clkParams.period = 0;
    clkParams.startFlag = FALSE;
    Clock_construct(&fastReportTimeoutClock, update_data_callback, 1, &clkParams);
    update_data = Clock_handle(&fastReportTimeoutClock);

    Task_Params_init(&nodeTaskParams);
    nodeTaskParams.stackSize = NODE_TASK_STACK_SIZE;
    nodeTaskParams.priority = NODE_TASK_PRIORITY;
    nodeTaskParams.stack = &nodeTaskStack;
    Task_construct(&nodeTask, nodeTaskFunction, &nodeTaskParams, NULL);
}

static void nodeTaskFunction(UArg arg0, UArg arg1)
{
    pin_handle = PIN_open(&pin_state, pinTable);
    PIN_setOutputValue(pin_handle, LED_BEE, 1);
    PIN_setOutputValue(pin_handle, LED_RED, 0);
    PIN_setOutputValue(pin_handle, SENSORS_POWER, 1);
    PIN_registerIntCb(pin_handle, &button_callback);

    sm_init(delay_ms);

    set_update_data_timeout(update_data_timeout);

    while(1)
    {
        uint32_t events = Event_pend(nodeEventHandle, 0, EVENT_ALL, BIOS_WAIT_FOREVER);

        if (events & EVENT_UPDATE_DATA)
        {
            sm_trigger_measurement();

            struct Data data;
            data.status   = sm_get_status();
            data.current  = sm_get_current();
            data.power    = sm_get_power();
            data.voltage  = sm_get_voltage();
            data.light    = sm_get_light();
            data.temp1    = sm_get_temp1();
            data.humi1    = sm_get_humi1();
            data.temp2    = sm_get_temp2();
            data.humi2    = sm_get_humi2();
            data.pressure = sm_get_pressure();
            data.acc_voltage = sm_get_adc_value();

            PIN_setOutputValue(pin_handle, LED_BEE, 0);
            Task_sleep(500);
            PIN_setOutputValue(pin_handle, LED_BEE, 1);

            NodeRadioTask_sendData(data);

            Clock_start(update_data);
        }
    }
}

void set_led_color(Color color, State state)
{
    if (state == OFF)
    {
        PIN_setOutputValue(pin_handle, LED_RED,   OFF);
        PIN_setOutputValue(pin_handle, LED_GREEN, OFF);
        PIN_setOutputValue(pin_handle, LED_BLUE,  OFF);
        return;
    }

    switch (color)
    {
    case RED:
        PIN_setOutputValue(pin_handle, LED_RED,   ON);
        PIN_setOutputValue(pin_handle, LED_GREEN, OFF);
        PIN_setOutputValue(pin_handle, LED_BLUE,  OFF);
        break;
    case GREEN:
        PIN_setOutputValue(pin_handle, LED_RED,   OFF);
        PIN_setOutputValue(pin_handle, LED_GREEN, ON);
        PIN_setOutputValue(pin_handle, LED_BLUE,  OFF);
        break;
    case BLUE:
        PIN_setOutputValue(pin_handle, LED_RED,   OFF);
        PIN_setOutputValue(pin_handle, LED_GREEN, OFF);
        PIN_setOutputValue(pin_handle, LED_BLUE,  ON);
        break;
    case CYAN:
        PIN_setOutputValue(pin_handle, LED_RED,   OFF);
        PIN_setOutputValue(pin_handle, LED_GREEN, ON);
        PIN_setOutputValue(pin_handle, LED_BLUE,  ON);
        break;
    case MAGENTA:
        PIN_setOutputValue(pin_handle, LED_RED,   ON);
        PIN_setOutputValue(pin_handle, LED_GREEN, OFF);
        PIN_setOutputValue(pin_handle, LED_BLUE,  ON);
        break;
    case YELLOW:
        PIN_setOutputValue(pin_handle, LED_RED,   ON);
        PIN_setOutputValue(pin_handle, LED_GREEN, ON);
        PIN_setOutputValue(pin_handle, LED_BLUE,  OFF);
        break;
    case WHITE:
        PIN_setOutputValue(pin_handle, LED_RED,   ON);
        PIN_setOutputValue(pin_handle, LED_GREEN, ON);
        PIN_setOutputValue(pin_handle, LED_BLUE,  ON);
        break;
    }
}

void set_update_data_timeout(uint32_t timeout)
{
    Clock_stop(update_data);
    Clock_setTimeout(update_data, timeout * 1000 / Clock_tickPeriod);
    Clock_start(update_data);
}

void button_callback(PIN_Handle handle, PIN_Id pinId)
{
    CPUdelay(8000*10);

    if (PIN_getInputValue(BUTTON_USER) != 0)
        return;

    if (update_data_timeout == UPDATE_DATA_1S_TIMEOUT)
        update_data_timeout = UPDATE_DATA_10S_TIMEOUT;
    else
        update_data_timeout = UPDATE_DATA_1S_TIMEOUT;

    set_update_data_timeout(update_data_timeout);
}

void update_data_callback(UArg arg0)
{
    Event_post(nodeEventHandle, EVENT_UPDATE_DATA);
}

void delay_ms(uint32_t t_ms)
{
    uint32_t i;
    for(i = 0; i < t_ms; i++)
        Task_sleep(100);
}
