#include "Opt3001.h"

static uint8_t this_address;

OPT3001_transfer opt3001_transfer;

#define OPT3001_RBUF_LEN  2
#define OPT3001_WBUF_LEN  3

static uint8_t wbuf[OPT3001_WBUF_LEN];
static uint8_t rbuf[OPT3001_RBUF_LEN];

uint16_t OPT3001_read_register(uint8_t reg);
void OPT3001_write_register(uint8_t reg, uint16_t value);

void OPT3001_init(uint8_t address, OPT3001_transfer callback)
{
    this_address = address;
    opt3001_transfer = callback;
}

bool OPT3001_is_present()
{
    uint16_t id = OPT3001_get_device_ID();

    if (id == OPT3001_DEVICE_ID)
        return true;
    return false;
}

void OPT3001_set_mode(OPT3001_Mode mode)
{
    uint16_t buf = OPT3001_read_register(OPT3001_REG_CONFIG);

    switch(mode)
    {
        case MODE_SHUTDOWN     :
            buf = (buf & OPT3001_CONVERSION_MODE_MASK) |
            OPT3001_CONVERSION_MODE_SHUTDOWN;
            break;
        case MODE_SINGLE_SHOT  :
            buf = (buf & OPT3001_CONVERSION_MODE_MASK) |
            OPT3001_CONVERSION_MODE_SINGLE;
            break;
        case MODE_CONTINUOUS   :
            buf = (buf & OPT3001_CONVERSION_MODE_MASK) |
            OPT3001_CONVERSION_MODE_CONTINUOUS;
            break;
    }

    OPT3001_write_register(OPT3001_REG_CONFIG, buf);
}

void OPT3001_set_range(OPT3001_Range range)
{
    uint16_t buf = OPT3001_read_register(OPT3001_REG_CONFIG);

    switch(range)
    {
        case RANGE_40_95     :
            buf = (buf & OPT3001_FULLSCALE_RANGE_MASK) |
            OPT3001_FULLSCALE_RANGE_40_95;
            break;
        case RANGE_81_90     :
            buf = (buf & OPT3001_FULLSCALE_RANGE_MASK) |
            OPT3001_FULLSCALE_RANGE_81_90;
            break;
        case RANGE_163_80    :
            buf = (buf & OPT3001_FULLSCALE_RANGE_MASK) |
            OPT3001_FULLSCALE_RANGE_163_80;
            break;
        case RANGE_327_60    :
            buf = (buf & OPT3001_FULLSCALE_RANGE_MASK) |
            OPT3001_FULLSCALE_RANGE_327_60;
            break;
        case RANGE_655_20    :
            buf = (buf & OPT3001_FULLSCALE_RANGE_MASK) |
            OPT3001_FULLSCALE_RANGE_655_20;
            break;
        case RANGE_1310_40   :
            buf = (buf & OPT3001_FULLSCALE_RANGE_MASK) |
            OPT3001_FULLSCALE_RANGE_1310_40;
            break;
        case RANGE_2620_80   :
            buf = (buf & OPT3001_FULLSCALE_RANGE_MASK) |
            OPT3001_FULLSCALE_RANGE_2620_80;
            break;
        case RANGE_5241_60   :
            buf = (buf & OPT3001_FULLSCALE_RANGE_MASK) |
            OPT3001_FULLSCALE_RANGE_5241_60;
            break;
        case RANGE_10483_20  :
            buf = (buf & OPT3001_FULLSCALE_RANGE_MASK) |
            OPT3001_FULLSCALE_RANGE_10483_20;
            break;
        case RANGE_20966_40  :
            buf = (buf & OPT3001_FULLSCALE_RANGE_MASK) |
            OPT3001_FULLSCALE_RANGE_20966_40;
            break;
        case RANGE_41932_80  :
            buf = (buf & OPT3001_FULLSCALE_RANGE_MASK) |
            OPT3001_FULLSCALE_RANGE_41932_80;
            break;
        case RANGE_83865_60  :
            buf = (buf & OPT3001_FULLSCALE_RANGE_MASK) |
            OPT3001_FULLSCALE_RANGE_83865_60;
            break;
        case RANGE_AUTO      :
            buf = (buf & OPT3001_FULLSCALE_RANGE_MASK) |
            OPT3001_FULLSCALE_RANGE_AUTO;
            break;
    }

    OPT3001_write_register(OPT3001_REG_CONFIG, buf);
}

void OPT3001_set_conversion_time(OPT3001_ConvTime time)
{
    uint16_t buf = OPT3001_read_register(OPT3001_REG_CONFIG);

    switch(time)
    {
        case CONVTIME_100MS     :
            buf = (buf & OPT3001_CONVERSION_TIME_MASK) |
            OPT3001_CONVERSION_TIME_100;
            break;
        case CONVTIME_800MS     :
            buf = (buf & OPT3001_CONVERSION_TIME_MASK) |
            OPT3001_CONVERSION_TIME_800;
            break;
    }

    OPT3001_write_register(OPT3001_REG_CONFIG, buf);
}

void OPT3001_set_fault_mode(OPT3001_FaultMode mode)
{
    uint16_t buf = OPT3001_read_register(OPT3001_REG_CONFIG);

    switch(mode)
    {
        case FLTMODE_HYSTERESIS     :
            buf = (buf & OPT3001_FAULT_MODE_MASK) |
            OPT3001_FAULT_MODE_HYSTERESIS;
            break;
        case FLTMODE_WINDOW_LATCH   :
            buf = (buf & OPT3001_FAULT_MODE_MASK) |
            OPT3001_FAULT_MODE_WINDOW_LATCH;
            break;
    }

    OPT3001_write_register(OPT3001_REG_CONFIG, buf);
}

void OPT3001_set_fault_count(OPT3001_FaultCount count)
{
    uint16_t buf = OPT3001_read_register(OPT3001_REG_CONFIG);

    switch(count)
    {
        case FLTCOUNT_1     :
            buf = (buf & OPT3001_FAULT_COUNT_MASK) |
            OPT3001_FAULT_COUNT_1;
            break;
        case FLTCOUNT_2     :
            buf = (buf & OPT3001_FAULT_COUNT_MASK) |
            OPT3001_FAULT_COUNT_2;
            break;
        case FLTCOUNT_4     :
            buf = (buf & OPT3001_FAULT_COUNT_MASK) |
            OPT3001_FAULT_COUNT_4;
            break;
        case FLTCOUNT_8     :
            buf = (buf & OPT3001_FAULT_COUNT_MASK) |
            OPT3001_FAULT_COUNT_8;
            break;
    }

    OPT3001_write_register(OPT3001_REG_CONFIG, buf);
}

void OPT3001_set_int_polarity(OPT3001_Polarity polarity)
{
    uint16_t buf = OPT3001_read_register(OPT3001_REG_CONFIG);

    switch(polarity)
    {
        case LOW      :
            buf = (buf & OPT3001_INT_POLARITY_MASK) |
            OPT3001_INT_POLARITY_NORMAL;
            break;
        case HIGH      :
            buf = (buf & OPT3001_INT_POLARITY_MASK) |
            OPT3001_INT_POLARITY_REVERSE;
            break;
    }

    OPT3001_write_register(OPT3001_REG_CONFIG, buf);
}

float OPT3001_get_light_in_lux()
{
    uint16_t buf = OPT3001_read_register(OPT3001_REG_RESULT);
    return 0.01F * powf(2.0, (float)(buf >> OPT3001_RESULT_EXPONENT_OFFSET)) * \
          (float)(buf & OPT3001_RESULT_MANTISSA_MASK);
}

void OPT3001_set_limits_in_lux(float lo_limit, float hi_limit)
{

}

uint16_t OPT3001_get_device_ID()
{
    return OPT3001_read_register(OPT3001_REG_DEVICE_ID);
}

uint16_t OPT3001_get_manufacturer_ID()
{
    return OPT3001_read_register(OPT3001_REG_MANUFACTURER_ID);
}

uint16_t OPT3001_read_register(uint8_t reg)
{
    wbuf[0] = reg;
    opt3001_transfer(wbuf, 1, rbuf, 2, this_address);
    return (((uint16_t)rbuf[0] << 8) | rbuf[1]);

}

void OPT3001_write_register(uint8_t reg, uint16_t value)
{
    wbuf[0] = reg;
    wbuf[1] = (uint8_t)((value >> 8) & 0xFF);
    wbuf[2] = (uint8_t)((value >> 0) & 0xFF);

    opt3001_transfer(wbuf, 3, rbuf, 0, this_address);
}

