#include "Max7313.h"

MAX7313_transfer max7313_transfer;

#define MAX7313_RBUF_LEN  2
#define MAX7313_WBUF_LEN  9

static uint8_t wbuf[MAX7313_WBUF_LEN];
static uint8_t rbuf[MAX7313_RBUF_LEN];

uint8_t MAX7313_read_register(uint8_t address, uint8_t reg);
void MAX7313_write_register(uint8_t address, uint8_t count);

void MAX7313_init(MAX7313_transfer callback)
{
    max7313_transfer = callback;
}

void MAX7313_WriteConfiguration(uint8_t max7313_addr, uint8_t config)
{
    wbuf[0] = MAX7313_CONFIGURATION_ADDR;
    wbuf[1] = config;
    MAX7313_write_register(max7313_addr, 2);
}

void MAX7313_WritePortConfig(uint8_t max7313_addr, uint16_t portmask)
{
    wbuf[0] = MAX7313_PORT_CONFIG_P7_P0_ADDR;
    wbuf[1] = (uint8_t)portmask;
    wbuf[2] = (uint8_t)(portmask >> 8);
    MAX7313_write_register(max7313_addr, 3);
}

void MAX7313_WritePhase0(uint8_t max7313_addr, uint16_t phasemask)
{
    wbuf[0] = MAX7313_PHASE0_P7_P0_ADDR;
    wbuf[1] = (uint8_t)phasemask;
    wbuf[2] = (uint8_t)(phasemask >> 8);
    MAX7313_write_register(max7313_addr, 3);
}

void MAX7313_WritePhase0_P7_P0(uint8_t max7313_addr, uint8_t phasemask)
{
    wbuf[0] = MAX7313_PHASE0_P7_P0_ADDR;
    wbuf[1] = phasemask;
    MAX7313_write_register(max7313_addr, 2);
}

void MAX7313_WritePhase0_P15_P8(uint8_t max7313_addr, uint8_t phasemask)
{
    wbuf[0] = MAX7313_PHASE0_P15_P8_ADDR;
    wbuf[1] = phasemask;
    MAX7313_write_register(max7313_addr, 2);
}

void MAX7313_WriteIntensity(uint8_t max7313_addr, uint8_t intensity)
{
    wbuf[0] = MAX7313_OUTPUT_INTENSITY_P1_P0_ADDR;
    wbuf[1] = intensity;
    wbuf[2] = intensity;
    wbuf[3] = intensity;
    wbuf[4] = intensity;
    wbuf[5] = intensity;
    wbuf[6] = intensity;
    wbuf[7] = intensity;
    wbuf[8] = intensity;
    MAX7313_write_register(max7313_addr, 9);
}

void MAX7313_WriteMasterIntensity(uint8_t max7313_addr, uint8_t intensity)
{
    wbuf[0] = MAX7313_MASTER_INTENSITY_ADDR;
    wbuf[1] = intensity;
    MAX7313_write_register(max7313_addr, 2);
}

uint16_t MAX7313_ReadPhase0(uint8_t max7313_addr)
{
    uint8_t p7p0 = MAX7313_read_register(max7313_addr, MAX7313_PHASE0_P7_P0_ADDR);
    uint8_t p15p8 = MAX7313_read_register(max7313_addr, MAX7313_PHASE0_P15_P8_ADDR);

    return ((p15p8 << 8) | p7p0);
}

uint8_t MAX7313_ReadPhase0_P7_P0(uint8_t max7313_addr)
{
    return MAX7313_read_register(max7313_addr, MAX7313_PHASE0_P7_P0_ADDR);
}

uint16_t MAX7313_ReadPortConfig(uint8_t max7313_addr)
{
    uint8_t port_l = MAX7313_read_register(max7313_addr, MAX7313_PORT_CONFIG_P7_P0_ADDR);
    uint8_t port_h = MAX7313_read_register(max7313_addr, MAX7313_PORT_CONFIG_P15_P8_ADDR);
    return (port_h << 8) | port_l;
}

uint8_t MAX7313_ReadConfiguration(uint8_t max7313_addr)
{
    return MAX7313_read_register(max7313_addr, MAX7313_CONFIGURATION_ADDR);
}

uint8_t MAX7313_ReadGlobalIntensity(uint8_t max7313_addr)
{
    return MAX7313_read_register(max7313_addr, MAX7313_MASTER_INTENSITY_ADDR);
}

uint16_t MAX7313_ReadInput(uint8_t max7313_addr)
{
    uint8_t p7p0 = MAX7313_read_register(max7313_addr, MAX7313_READ_INPUT_P7_P0_ADDR);
    uint8_t p15p8 = MAX7313_read_register(max7313_addr, MAX7313_READ_INPUT_P15_P8_ADDR);

    return ((p15p8 << 8) | p7p0);
}

uint8_t MAX7313_read_register(uint8_t address, uint8_t reg)
{
    wbuf[0] = reg;
    max7313_transfer(wbuf, 1, rbuf, 1, address);
    return rbuf[0];
}

void MAX7313_write_register(uint8_t address, uint8_t count)
{
    max7313_transfer(wbuf, count, rbuf, 0, address);
}
