#include "Ina226.h"

static uint8_t this_address;
static float _current_lsb;
static float _power_lsb;

INA226_transfer ina226_transfer;

#define INA226_RBUF_LEN  2
#define INA226_WBUF_LEN  3

static uint8_t wbuf[INA226_WBUF_LEN];
static uint8_t rbuf[INA226_RBUF_LEN];

uint16_t INA226_read_register(uint8_t reg);
void INA226_write_register(uint8_t reg, uint16_t value);

void INA226_init(uint8_t address, INA226_transfer callback)
{
    this_address = address;
    ina226_transfer = callback;
}

bool INA226_is_present()
{
    uint16_t id = INA226_get_die_ID();

    if (id == INA226_DIE_ID)
        return true;
    return false;
}

void INA226_reset()
{
    uint16_t buf = INA226_read_register(INA226_REG_CONFIG);
    buf = (buf & INA226_RESET_SYSTEM_MASK) | INA226_RESET_SYSTEM;
    INA226_write_register(INA226_REG_CONFIG, buf);
}

void INA226_set_averaging_mode(INA226_AveragingMode mode)
{
    uint16_t buf = INA226_read_register(INA226_REG_CONFIG);

    switch (mode)
    {
        case SAMPLES_1     :
            buf = (buf & INA226_AVERAGING_MODE_MASK) |
                         INA226_AVERAGING_MODE_1;
            break;
        case SAMPLES_4     :
            buf = (buf & INA226_AVERAGING_MODE_MASK) |
                         INA226_AVERAGING_MODE_4;
            break;
        case SAMPLES_16    :
            buf = (buf & INA226_AVERAGING_MODE_MASK) |
                         INA226_AVERAGING_MODE_16;
            break;
        case SAMPLES_64    :
            buf = (buf & INA226_AVERAGING_MODE_MASK) |
                         INA226_AVERAGING_MODE_64;
            break;
        case SAMPLES_128   :
            buf = (buf & INA226_AVERAGING_MODE_MASK) |
                         INA226_AVERAGING_MODE_128;
            break;
        case SAMPLES_256   :
            buf = (buf & INA226_AVERAGING_MODE_MASK) |
                         INA226_AVERAGING_MODE_256;
            break;
        case SAMPLES_512   :
            buf = (buf & INA226_AVERAGING_MODE_MASK) |
                         INA226_AVERAGING_MODE_512;
            break;
        case SAMPLES_1024  :
            buf = (buf & INA226_AVERAGING_MODE_MASK) |
                         INA226_AVERAGING_MODE_1024;
            break;
    }

    INA226_write_register(INA226_REG_CONFIG, buf);
}

void INA226_set_bus_voltage_conversion_time(INA226_ConversionTime time)
{
    uint16_t buf = INA226_read_register(INA226_REG_CONFIG);

    switch (time)
    {
        case CONVTIME_140_US    :
            buf = (buf & INA226_BUS_VOLTAGE_CONVERSION_TIME_MASK) |
                         INA226_BUS_VOLTAGE_CONVERSION_TIME_140_US;
            break;
        case CONVTIME_204_US    :
            buf = (buf & INA226_BUS_VOLTAGE_CONVERSION_TIME_MASK) |
                         INA226_BUS_VOLTAGE_CONVERSION_TIME_204_US;
            break;
        case CONVTIME_332_US    :
            buf = (buf & INA226_BUS_VOLTAGE_CONVERSION_TIME_MASK) |
                         INA226_BUS_VOLTAGE_CONVERSION_TIME_332_US;
            break;
        case CONVTIME_588_US    :
            buf = (buf & INA226_BUS_VOLTAGE_CONVERSION_TIME_MASK) |
                         INA226_BUS_VOLTAGE_CONVERSION_TIME_588_US;
            break;
        case CONVTIME_1_100_MS  :
            buf = (buf & INA226_BUS_VOLTAGE_CONVERSION_TIME_MASK) |
                         INA226_BUS_VOLTAGE_CONVERSION_TIME_1_100_MS;
            break;
        case CONVTIME_2_116_MS  :
            buf = (buf & INA226_BUS_VOLTAGE_CONVERSION_TIME_MASK) |
                         INA226_BUS_VOLTAGE_CONVERSION_TIME_2_116_MS;
            break;
        case CONVTIME_4_156_MS  :
            buf = (buf & INA226_BUS_VOLTAGE_CONVERSION_TIME_MASK) |
                         INA226_BUS_VOLTAGE_CONVERSION_TIME_4_156_MS;
            break;
        case CONVTIME_8_244_MS  :
            buf = (buf & INA226_BUS_VOLTAGE_CONVERSION_TIME_MASK) |
                         INA226_BUS_VOLTAGE_CONVERSION_TIME_8_244_MS;
            break;
    }

    INA226_write_register(INA226_REG_CONFIG, buf);
}

void INA226_set_shunt_voltage_conversion_time(INA226_ConversionTime time)
{
    uint16_t buf = INA226_read_register(INA226_REG_CONFIG);

    switch (time)
    {
        case CONVTIME_140_US    :
            buf = (buf & INA226_SHUNT_VOLTAGE_CONVERSION_TIME_MASK) |
                         INA226_SHUNT_VOLTAGE_CONVERSION_TIME_140_US;
            break;
        case CONVTIME_204_US    :
            buf = (buf & INA226_SHUNT_VOLTAGE_CONVERSION_TIME_MASK) |
                         INA226_SHUNT_VOLTAGE_CONVERSION_TIME_204_US;
            break;
        case CONVTIME_332_US    :
            buf = (buf & INA226_SHUNT_VOLTAGE_CONVERSION_TIME_MASK) |
                         INA226_SHUNT_VOLTAGE_CONVERSION_TIME_332_US;
            break;
        case CONVTIME_588_US    :
            buf = (buf & INA226_SHUNT_VOLTAGE_CONVERSION_TIME_MASK) |
                         INA226_SHUNT_VOLTAGE_CONVERSION_TIME_588_US;
            break;
        case CONVTIME_1_100_MS  :
            buf = (buf & INA226_SHUNT_VOLTAGE_CONVERSION_TIME_MASK) |
                         INA226_SHUNT_VOLTAGE_CONVERSION_TIME_1_100_MS;
            break;
        case CONVTIME_2_116_MS  :
            buf = (buf & INA226_SHUNT_VOLTAGE_CONVERSION_TIME_MASK) |
                         INA226_SHUNT_VOLTAGE_CONVERSION_TIME_2_116_MS;
            break;
        case CONVTIME_4_156_MS  :
            buf = (buf & INA226_SHUNT_VOLTAGE_CONVERSION_TIME_MASK) |
                         INA226_SHUNT_VOLTAGE_CONVERSION_TIME_4_156_MS;
            break;
        case CONVTIME_8_244_MS  :
            buf = (buf & INA226_SHUNT_VOLTAGE_CONVERSION_TIME_MASK) |
                         INA226_SHUNT_VOLTAGE_CONVERSION_TIME_8_244_MS;
            break;
    }

    INA226_write_register(INA226_REG_CONFIG, buf);
}

void INA226_set_mode(INA226_OperatingMode mode)
{
    uint16_t buf = INA226_read_register(INA226_REG_CONFIG);

    switch (mode)
    {
        case TRIG_SHUTDOWN               :
            buf = (buf & INA226_MODE_MASK) |
                         INA226_MODE_TRIG_SHUTDOWN;
            break;
        case TRIG_SHUNT_VOLTAGE          :
            buf = (buf & INA226_MODE_MASK) |
                         INA226_MODE_TRIG_SHUNT_VOLTAGE;
            break;
        case TRIG_BUS_VOLTAGE            :
            buf = (buf & INA226_MODE_MASK) |
                         INA226_MODE_TRIG_BUS_VOLTAGE;
            break;
        case TRIG_SHUNT_AND_BUS_VOLTAGE  :
            buf = (buf & INA226_MODE_MASK) |
                         INA226_MODE_TRIG_SHUNT_AND_BUS_VOLTAGE;
            break;
        case CONT_SHUTDOWN               :
            buf = (buf & INA226_MODE_MASK) |
                         INA226_MODE_CONT_SHUTDOWN;
            break;
        case CONT_SHUNT_VOLTAGE          :
            buf = (buf & INA226_MODE_MASK) |
                         INA226_MODE_CONT_SHUNT_VOLTAGE;
            break;
        case CONT_BUS_VOLTAGE            :
            buf = (buf & INA226_MODE_MASK) |
                         INA226_MODE_CONT_BUS_VOLTAGE;
            break;
        case CONT_SHUNT_AND_BUS_VOLTAGE  :
            buf = (buf & INA226_MODE_MASK) |
                         INA226_MODE_CONT_SHUNT_AND_BUS_VOLTAGE;
            break;
    }

    INA226_write_register(INA226_REG_CONFIG, buf);
}

void INA226_set_calibration(float shunt_resistance_in_Om)
{
    float max_expected_current_in_A = (INA226_SHUNT_VOLTAGE_FULL_SCALE_RANGE / shunt_resistance_in_Om);

    _current_lsb = max_expected_current_in_A / (pow(2, 15));
    _power_lsb = _current_lsb * 25;

    uint16_t cal = (uint16_t)(INA226_CALIBRATION_FACTOR / (_current_lsb * shunt_resistance_in_Om));

    INA226_write_register(INA226_REG_CALIBRATION, cal);
}

float INA226_get_shunt_voltage_in_V()
{
    return (float)INA226_read_register(INA226_REG_SHUNT_VOLTAGE) * INA226_SHUNT_VOLTAGE_LSB;
}

float INA226_get_bus_voltage_in_V()
{
    return INA226_read_register(INA226_REG_BUS_VOLTAGE) * INA226_BUS_VOLTAGE_LSB;
}

float INA226_get_power_in_W()
{
    return (INA226_read_register(INA226_REG_POWER)) * _power_lsb;
}

float INA226_get_current_in_A()
{
    uint16_t current = INA226_read_register(INA226_REG_CURRENT);
    return (float)current * _current_lsb;
}

uint16_t INA226_get_manufacturer_ID()
{
    return INA226_read_register(INA226_REG_MANUFACTURER_ID);
}

uint16_t INA226_get_die_ID()
{
    return INA226_read_register(INA226_REG_DIE_ID);
}

uint16_t INA226_read_register(uint8_t reg)
{
    wbuf[0] = reg;
    ina226_transfer(wbuf, 1, rbuf, 2, this_address);
    return (((uint16_t)rbuf[0] << 8) | rbuf[1]);
}

void INA226_write_register(uint8_t reg, uint16_t value)
{
    wbuf[0] = reg;
    wbuf[1] = (uint8_t)((value >> 8) & 0xFF);
    wbuf[2] = (uint8_t)((value >> 0) & 0xFF);
    ina226_transfer(wbuf, 3, rbuf, 0, this_address);
}

