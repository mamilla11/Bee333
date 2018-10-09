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
#include "mb.h"
#include "port.h"

#define CONCENTRATOR_TASK_STACK_SIZE                    1024
#define CONCENTRATOR_TASK_PRIORITY                      3
#define CONCENTRATOR_EVENT_ALL                          0xFFFFFFFF
#define CONCENTRATOR_EVENT_NEW_SENSOR_DATA              (uint32_t)(1 << 0)
#define CONCENTRATOR_MAX_NODES                          10

static Task_Params concentratorTaskParams;
Task_Struct concentratorTask;
static uint8_t concentratorTaskStack[CONCENTRATOR_TASK_STACK_SIZE];
Event_Struct concentratorEvent;
static Event_Handle concentratorEventHandle;
uint8_t knownSensorNodes[CONCENTRATOR_MAX_NODES] = {0};
struct NodeData lastNodeData;

static PIN_Handle pin_handle;
static PIN_State pin_state;

PIN_Config pinTable[] = {
   LED_BEE       | PIN_GPIO_OUTPUT_EN  | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
   PIN_TERMINATE
};

/***** Prototypes *****/
static void concentratorTaskFunction(UArg arg0, UArg arg1);
static void packetReceivedCallback(struct SensorPacket* packet, int8_t rssi);
void updateNode(struct NodeData* node);
void updateRegs(uint8_t start_address);
bool initModbus();

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
    pin_handle = PIN_open(&pin_state, pinTable);
    PIN_setOutputValue(pin_handle, LED_BEE, 0);

    initModbus();
    ConcentratorRadioTask_registerPacketReceivedCallback(CONCENTRATOR, packetReceivedCallback);

    while(1)
    {
        eMBPoll();
        uint32_t events = Event_pend(concentratorEventHandle, 0, CONCENTRATOR_EVENT_ALL, 10);

        if(events & CONCENTRATOR_EVENT_NEW_SENSOR_DATA)
        {
            updateNode(&lastNodeData);

            PIN_setOutputValue(pin_handle, LED_BEE, 1);
            Task_sleep(500);
            PIN_setOutputValue(pin_handle, LED_BEE, 0);
        }
    }
}


bool initModbus()
{
    if( MB_ENOERR != eMBInit(MB_ASCII, 0x80, 0, 19200, MB_PAR_NONE)) {
        return false;
    }

    if( MB_ENOERR != eMBEnable()) {
        return false;
    }

    return true;
}


static void packetReceivedCallback(struct SensorPacket* packet, int8_t rssi)
{
    if(packet->header.packetType == SENSOR_PACKET)
    {

        lastNodeData.address = packet->header.sourceAddress;
        lastNodeData.data    = packet->data;
        if (rssi < 0) {
            lastNodeData.rssi = (uint16_t)(rssi * -1) | 0x8000;
        }
        else {
            lastNodeData.rssi    = rssi;
        }

        Event_post(concentratorEventHandle, CONCENTRATOR_EVENT_NEW_SENSOR_DATA);
    }
}

void updateNode(struct NodeData* node)
{
    uint8_t i;
    for (i = 0; i < CONCENTRATOR_MAX_NODES; i++)
    {
        if (knownSensorNodes[i] == node->address)
        {
            updateRegs(NodeDataSize * i);
            break;
        }

        if (knownSensorNodes[i] == 0)
        {
            knownSensorNodes[i] = node->address;
            updateRegs(NodeDataSize * i);
            break;
        }
    }
}

void updateRegs(uint8_t start_address)
{
    set_single_ao(start_address + ADDRESS,     lastNodeData.address);
    set_single_ao(start_address + STATUS,      lastNodeData.data.status);
    set_single_ao(start_address + CURRENT,     lastNodeData.data.current);
    set_single_ao(start_address + POWER,       lastNodeData.data.power);
    set_single_ao(start_address + VOLTAGE,     lastNodeData.data.voltage);
    set_single_ao(start_address + LIGHT,       lastNodeData.data.light);
    set_single_ao(start_address + TEMP_1,      lastNodeData.data.temp1);
    set_single_ao(start_address + HUMI_1,      lastNodeData.data.humi1);
    set_single_ao(start_address + TEMP_2,      lastNodeData.data.temp2);
    set_single_ao(start_address + HUMI_2,      lastNodeData.data.humi2);
    set_single_ao(start_address + PRESSURE,    lastNodeData.data.pressure);
    set_single_ao(start_address + ACC_VOLTAGE, lastNodeData.data.acc_voltage);
    set_single_ao(start_address + RSSI,        lastNodeData.rssi);
}
