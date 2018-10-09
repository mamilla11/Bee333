#include <xdc/std.h>
#include <xdc/runtime/System.h>
//tirtos_
#include <ti/sysbios/BIOS.h>

#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Event.h>
#include <ti/sysbios/knl/Clock.h>

#include <cstring>
#include <cstdio>
#include "DisplayTask.h"
#include "ConcentratorRadioTask.h"
#include <RadioProtocol.h>
#include <paper/EPD_drive.h>
#include <ti/drivers/SPI.h>

/***** Defines *****/
#define DISPLAY_TASK_STACK_SIZE                   512
#define DISPLAY_TASK_PRIORITY                     3
#define DISPLAY_EVENT_ALL                         0xFFFFFFFF
#define DISPLAY_EVENT_UPDATE                      (uint32_t)(1 << 0)
#define DISPLAY_EVENT_NEW_SENSOR_DATA             (uint32_t)(1 << 1)
#define UPDATE_INTERVAL                           6000
#define DATA_LINES                                9

/***** Type declarations *****/
struct SensorData {
    uint8_t address;
    struct Data data;
    uint8_t button;
    int8_t latestRssi;
};

/***** Variable declarations *****/
Event_Struct displayEvent;
Task_Struct  displayTask;
Clock_Struct updateTimeoutClock;
static Event_Handle displayEventHandle;
static Task_Params displayTaskParams;
static uint8_t displayTaskStack[DISPLAY_TASK_STACK_SIZE];
static Clock_Handle updateTimeoutClockHandle;
static struct SensorData sensor_data;
SPI_Handle handle;
SPI_Params params;

/***** Prototypes *****/
static void displayTaskFunction(UArg arg0, UArg arg1);
static void packetReceivedCallback(struct SensorPacket* packet, int8_t rssi);
void updateTimeoutCallback(UArg arg0);
void update(void);
void init(void);
void driver_delay_xms(unsigned long xms);
void SPI_Write(unsigned char value);

void DisplayTask_init(void)
{
    Event_Params eventParam;
    Event_Params_init(&eventParam);
    Event_construct(&displayEvent, &eventParam);
    displayEventHandle = Event_handle(&displayEvent);

    /* Create clock object which is used for fast report timeout */
    Clock_Params clkParams;
    clkParams.period = 0;
    clkParams.startFlag = FALSE;
    Clock_construct(&updateTimeoutClock, updateTimeoutCallback, 1, &clkParams);
    updateTimeoutClockHandle = Clock_handle(&updateTimeoutClock);

    Task_Params_init(&displayTaskParams);
    displayTaskParams.stackSize = DISPLAY_TASK_STACK_SIZE;
    displayTaskParams.priority = DISPLAY_TASK_PRIORITY;
    displayTaskParams.stack = &displayTaskStack;
    Task_construct(&displayTask, displayTaskFunction, &displayTaskParams, NULL);
}

static void displayTaskFunction(UArg arg0, UArg arg1)
{
    SPI_Params_init(&params);
    handle = SPI_open(SPI0, &params);

    ConcentratorRadioTask_registerPacketReceivedCallback(DISPLAY, packetReceivedCallback);

    Dis_init(driver_delay_xms, SPI_Write);
    Dis_Clear_full();
    Dis_Clear_part();
    init();

    Clock_setTimeout(updateTimeoutClockHandle, UPDATE_INTERVAL * 1000 / Clock_tickPeriod);

    while(1)
    {
        uint32_t events = Event_pend(displayEventHandle, 0, DISPLAY_EVENT_ALL, BIOS_WAIT_FOREVER);

        if(events & DISPLAY_EVENT_UPDATE) {
            update();
            Clock_start(updateTimeoutClockHandle);
        }
    }
}

void update(void)
{
    char buffer[10];

    memset(buffer, ' ', 10);
    sprintf(buffer, "%d",  (int32_t)sensor_data.data.current);
    buffer[strlen(buffer)] = ' ';
    buffer[9] = '\0';
    Dis_String(88, 42, buffer, 16);

    memset(buffer, ' ', 10);
    sprintf(buffer, "%d",  (int32_t)sensor_data.data.power);
    buffer[strlen(buffer)] = ' ';
    buffer[9] = '\0';
    Dis_String(88, 58, buffer, 16);

    memset(buffer, ' ', 10);
    sprintf(buffer, "%d",  (int32_t)sensor_data.data.voltage);
    buffer[strlen(buffer)] = ' ';
    buffer[9] = '\0';
    Dis_String(88, 74, buffer, 16);

    memset(buffer, ' ', 10);
    sprintf(buffer, "%d", (int32_t)sensor_data.data.light);
    buffer[strlen(buffer)] = ' ';
    buffer[9] = '\0';
    Dis_String(88, 90, buffer, 16);

    memset(buffer, ' ', 10);
    sprintf(buffer, "%d / %d", (int32_t)sensor_data.data.temp1, (int32_t)sensor_data.data.temp2);
    buffer[strlen(buffer)] = ' ';
    buffer[9] = '\0';
    Dis_String(88, 106, buffer, 16);

    memset(buffer, ' ', 10);
    sprintf(buffer, "%d / %d",   (int32_t)sensor_data.data.humi1, (int32_t)sensor_data.data.humi2);
    buffer[strlen(buffer)] = ' ';
    buffer[9] = '\0';
    Dis_String(88, 122, buffer, 16);

    memset(buffer, ' ', 10);
    sprintf(buffer, "%d",   (int32_t)sensor_data.data.pressure);
    buffer[strlen(buffer)] = ' ';
    buffer[9] = '\0';
    Dis_String(88, 138, buffer, 16);

    memset(buffer, ' ', 10);
    sprintf(buffer, "%d",     sensor_data.data.acc_voltage);
    buffer[strlen(buffer)] = ' ';
    buffer[9] = '\0';
    Dis_String(88, 154, buffer, 16);

    memset(buffer, ' ', 10);
    sprintf(buffer, "%04d", sensor_data.latestRssi);
    buffer[strlen(buffer)] = ' ';
    buffer[9] = '\0';
    Dis_String(88, 170, buffer, 16);
}

void init(void)
{
    Dis_String(8, 10,  "BEE 333", 16);
    Dis_String(8, 26,  "-----------------------", 16);
    Dis_String(8, 42,  "Current:            uA",  16);
    Dis_String(8, 58,  "Power:              uW",  16);
    Dis_String(8, 74,  "Voltage:            mV",  16);
    Dis_String(8, 90,  "Light:              lux", 16);
    Dis_String(8, 106, "Temp:               C",   16);
    Dis_String(8, 122, "Humidity:           %rH", 16);
    Dis_String(8, 138, "Pressure:           hPa", 16);
    Dis_String(8, 154, "Battery:            mV",  16);
    Dis_String(8, 170, "RSSI:                 ",  16);
}

void updateTimeoutCallback(UArg arg0)
{
    Event_post(displayEventHandle, DISPLAY_EVENT_UPDATE);
}

static void packetReceivedCallback(struct SensorPacket* packet, int8_t rssi)
{
    static bool first_event = true;

    if(packet->header.packetType == SENSOR_PACKET)
    {
        /* Save the values */
        sensor_data.address = packet->header.sourceAddress;
        sensor_data.data = packet->data;
        sensor_data.latestRssi = rssi;

        if (first_event)
        {
            first_event = false;
            Event_post(displayEventHandle, DISPLAY_EVENT_UPDATE);
        }
    }
}

void SPI_Write(unsigned char value)
{
     SPI_Transaction transaction;
     transaction.count = 1;
     transaction.txBuf = &value;
     transaction.arg   = NULL;
     transaction.rxBuf = NULL;
     SPI_transfer(handle, &transaction);
}

void driver_delay_xms(unsigned long xms)
{
    unsigned long i;
    for(i = 0; i<xms; i++)
        Task_sleep(100);
}
