#include <Board.h>
#include <cstring>
#include <cstdio>

#include <xdc/std.h>
#include <xdc/runtime/System.h>

#include <ti/sysbios/BIOS.h>

#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Event.h>
#include <ti/sysbios/knl/Clock.h>

#include "UserIfaceTask.h"
#include "ui/UserIface.h"

/***** Defines *****/
#define USERIFACE_TASK_STACK_SIZE                   512
#define USERIFACE_TASK_PRIORITY                     3
#define USERIFACE_EVENT_ALL                         0xFFFFFFFF
#define USERIFACE_EVENT_BUTTON_PRESSED              (uint32_t)(1 << 0)
#define USERIFACE_EVENT_RGB_CHANGE_COLOR            (uint32_t)(1 << 1)
#define USERIFACE_RGB_INTERVAL                      500

/***** Variable declarations *****/
Event_Struct userifaceEvent;
Task_Struct  userifaceTask;
Clock_Struct rgbTimeoutClock;
static Event_Handle userifaceEventHandle;
static Task_Params userifaceTaskParams;
static uint8_t userifaceTaskStack[USERIFACE_TASK_STACK_SIZE];
static Clock_Handle rgbTimeoutClockHandle;
static PIN_Handle buttonPinHandle;
static PIN_State buttonPinState;

PIN_Config buttonPinTable[] = {
    MAX7313_IRQ | PIN_INPUT_EN | PIN_PULLUP | PIN_IRQ_NEGEDGE,
    PIN_TERMINATE
};

/***** Prototypes *****/
static void userifaceTaskFunction(UArg arg0, UArg arg1);
void rgbTimeoutCallback(UArg arg0);
void buttonCallback(PIN_Handle handle, PIN_Id pinId);

void UserIfaceTask_init(void)
{
    Event_Params eventParam;
    Event_Params_init(&eventParam);
    Event_construct(&userifaceEvent, &eventParam);
    userifaceEventHandle = Event_handle(&userifaceEvent);

    Clock_Params clkParams;
    clkParams.period = 0;
    clkParams.startFlag = FALSE;
    Clock_construct(&rgbTimeoutClock, rgbTimeoutCallback, 1, &clkParams);
    rgbTimeoutClockHandle = Clock_handle(&rgbTimeoutClock);

    Task_Params_init(&userifaceTaskParams);
    userifaceTaskParams.stackSize = USERIFACE_TASK_STACK_SIZE;
    userifaceTaskParams.priority = USERIFACE_TASK_PRIORITY;
    userifaceTaskParams.stack = &userifaceTaskStack;
    Task_construct(&userifaceTask, userifaceTaskFunction, &userifaceTaskParams, NULL);
}

static void userifaceTaskFunction(UArg arg0, UArg arg1)
{
    buttonPinHandle = PIN_open(&buttonPinState, buttonPinTable);
    PIN_registerIntCb(buttonPinHandle, &buttonCallback);

    UserIface_init();

    Clock_setTimeout(rgbTimeoutClockHandle, USERIFACE_RGB_INTERVAL * 1000 / Clock_tickPeriod);
    Clock_start(rgbTimeoutClockHandle);

    bool rgb_on = true;

    while(1)
    {

        uint32_t events = Event_pend(userifaceEventHandle, 0, USERIFACE_EVENT_ALL, BIOS_WAIT_FOREVER);

        if(events & USERIFACE_EVENT_BUTTON_PRESSED)
        {
            uint16_t state = UserIface_read_buttons_state();

            if (~state & USER_IFACE_BUTTON_RGB_MASK) {
                UserIface_switch_button_rgb_color();
            }

            if (~state & USER_IFACE_BUTTON_ENCODER_MASK) {
                UserIface_switch_button_encoder_color();
                rgb_on = !rgb_on;
                if (rgb_on)
                {
                    Clock_start(rgbTimeoutClockHandle);
                }

                else
                {
                    Clock_stop(rgbTimeoutClockHandle);
                    UserIface_set_rgb_1_color(RGB_NO_COLOR);
                    UserIface_set_rgb_2_color(RGB_NO_COLOR);
                }
            }

            UserIface_proc_encoder(state);
        }

        if (events & USERIFACE_EVENT_RGB_CHANGE_COLOR)
        {
            UserIface_switch_rgb_1_color();
            UserIface_switch_rgb_2_color();
            Clock_start(rgbTimeoutClockHandle);
        }
    }
}

void rgbTimeoutCallback(UArg arg0)
{
    Event_post(userifaceEventHandle, USERIFACE_EVENT_RGB_CHANGE_COLOR);
}

void buttonCallback(PIN_Handle handle, PIN_Id pinId)
{
    if (PIN_getInputValue(MAX7313_IRQ) != 0)
        return;

    Event_post(userifaceEventHandle, USERIFACE_EVENT_BUTTON_PRESSED);
}
