#include "Hdc1080.h"

static uint8_t this_address;

HDC1080_transfer hdc1080_transfer;

#define HDC1080_RBUF_LEN  2
#define HDC1080_WBUF_LEN  3

static uint8_t wbuf[HDC1080_WBUF_LEN];
static uint8_t rbuf[HDC1080_RBUF_LEN];

uint16_t HDC1080_read_register(uint8_t reg);
void HDC1080_write_register(uint8_t reg, uint16_t value);

void HDC1080_init(uint8_t address, HDC1080_transfer callback)
{
    this_address = address;
    hdc1080_transfer = callback;
}

bool HDC1080_is_present()
{
    uint16_t id = HDC1080_get_die_ID();
    if (id == HDC1080_DIE_ID)
        return true;
    return false;
}

void HDC1080_reset()
{
    uint16_t buf = HDC1080_read_register(HDC1080_REG_CONFIG);
    buf |= HDC1080_RESET;
    HDC1080_write_register(HDC1080_REG_CONFIG, buf);
}

void HDC1080_heat_enable()
{
    uint16_t buf = HDC1080_read_register(HDC1080_REG_CONFIG);
    buf |= HDC1080_HEAT;
    HDC1080_write_register(HDC1080_REG_CONFIG, buf);
}

void HDC1080_heat_disable()
{
    uint16_t buf = HDC1080_read_register(HDC1080_REG_CONFIG);
    buf &= ~HDC1080_HEAT;
    HDC1080_write_register(HDC1080_REG_CONFIG, buf);
}

void HDC1080_set_mode(HDC1080_Mode mode)
{
    uint16_t buf = HDC1080_read_register(HDC1080_REG_CONFIG);

    switch (mode)
    {
    case HDC1080_DATA_SEQUENCED: buf |= HDC1080_MODE;  break;
    case HDC1080_DATA_SEPARATED: buf &= ~HDC1080_MODE; break;
    }

    HDC1080_write_register(HDC1080_REG_CONFIG, buf);
}

void HDC1080_set_temp_resolution(HDC1080_TempResolution resolution)
{
    uint16_t buf = HDC1080_read_register(HDC1080_REG_CONFIG);

    switch (resolution)
    {
    case HDC1080_TRES_11_BIT: buf |= HDC1080_TRES;  break;
    case HDC1080_TRES_14_BIT: buf &= ~HDC1080_TRES; break;
    }

    HDC1080_write_register(HDC1080_REG_CONFIG, buf);
}

void HDC1080_set_humi_resolution(HDC1080_HumiResolution resolution)
{
    uint16_t buf = HDC1080_read_register(HDC1080_REG_CONFIG);
    buf &= ~HDC1080_HRES;
    buf |= resolution;
    HDC1080_write_register(HDC1080_REG_CONFIG, buf);
}

float HDC1080_get_temperature_in_C()
{
    int i;
    HDC1080_write_register(HDC1080_REG_TEMPERATURE, 0);
    for (i = 0; i < 50000; i++);
    uint16_t buf = HDC1080_read_register(HDC1080_REG_TEMPERATURE);
    return (((float)buf / HDC1080_MEASURE_DIVIDER) * HDC1080_TEMPERATURE_COEFF_1) - HDC1080_TEMPERATURE_COEFF_2;

}

float HDC1080_get_humidity()
{
    int i;
    HDC1080_write_register(HDC1080_REG_HUMIDITY, 0);
    for (i = 0; i < 50000; i++);
    uint16_t buf = HDC1080_read_register(HDC1080_REG_HUMIDITY);
    return ((float)buf / HDC1080_MEASURE_DIVIDER) * HDC1080_HUMIDITY_COEFF_1;
}

uint16_t HDC1080_get_serial_ID_1()
{
    return HDC1080_read_register(HDC1080_REG_SERIAL_ID_1);
}

uint16_t HDC1080_get_serial_ID_2()
{
    return HDC1080_read_register(HDC1080_REG_SERIAL_ID_2);
}

uint16_t HDC1080_get_serial_ID_3()
{
    return HDC1080_read_register(HDC1080_REG_SERIAL_ID_3);
}

uint16_t HDC1080_get_manufacturer_ID()
{
    return HDC1080_read_register(HDC1080_REG_MANUFACTURER_ID);
}

uint16_t HDC1080_get_die_ID()
{
    return HDC1080_read_register(HDC1080_REG_DIE_ID);
}

uint16_t HDC1080_read_register(uint8_t reg)
{
    if ((reg == HDC1080_REG_TEMPERATURE) || (reg == HDC1080_REG_HUMIDITY))
    {
        hdc1080_transfer(wbuf, 0, rbuf, 2, this_address);
    }
    else
    {
        wbuf[0] = reg;
        hdc1080_transfer(wbuf, 1, rbuf, 2, this_address);
    }
    return (((uint16_t)rbuf[0] << 8) | rbuf[1]);
}
void HDC1080_write_register(uint8_t reg, uint16_t value)
{
    wbuf[0] = reg;
    if ((reg == HDC1080_REG_TEMPERATURE) || (reg == HDC1080_REG_HUMIDITY))
    {
        hdc1080_transfer(wbuf, 1, rbuf, 0, this_address);
    }
    else
    {
        wbuf[1] = (uint8_t)((value >> 8) & 0xFF);
        wbuf[2] = (uint8_t)((value >> 0) & 0xFF);
        hdc1080_transfer(wbuf, 3, rbuf, 0, this_address);
    }
}
