#include "UserIface.h"

#include <ti/drivers/I2C.h>

#include "drv/Max7313.h"

#define MAX7313_0_ADDR                       0x20
#define MAX7313_1_ADDR                       0x24

#define MAX7313_0_CONFIG                     0x08
#define MAX7313_1_CONFIG                     0x08

#define MAX7313_0_PORT_CONFIG                0x0000
#define MAX7313_1_PORT_CONFIG                0x0FFC

#define MAX7313_0_PHASE_INITIAL              0x36FF
#define MAX7313_1_PHASE_INITIAL              0x6001

I2C_Handle  i2c;
I2C_Params  i2cParams;

void i2c_transfer(void *wbuf, size_t wlen, void *rbuf, size_t rlen, uint_least8_t addr);

void UserIface_init(void)
{
    I2C_Params_init(&i2cParams);
    i2cParams.bitRate = I2C_400kHz;
    i2c = I2C_open(I2C0, &i2cParams);

    MAX7313_init(i2c_transfer);
    /*GENERAL CONFIG*/
    MAX7313_WriteConfiguration(MAX7313_0_ADDR, MAX7313_0_CONFIG);
    MAX7313_WriteConfiguration(MAX7313_1_ADDR, MAX7313_1_CONFIG);
    /*PORT CONFIG*/
    MAX7313_WritePortConfig(MAX7313_0_ADDR, MAX7313_0_PORT_CONFIG);
    MAX7313_WritePortConfig(MAX7313_1_ADDR, MAX7313_1_PORT_CONFIG);
    /*LEDS CONFIG*/
    MAX7313_WritePhase0(MAX7313_0_ADDR, MAX7313_0_PHASE_INITIAL);
    MAX7313_WritePhase0(MAX7313_1_ADDR, MAX7313_1_PHASE_INITIAL);
    /*MASTER INTENSITY*/
    MAX7313_WriteMasterIntensity(MAX7313_0_ADDR, MAX7313_MAX_MASTER_INTENSITY);
    MAX7313_WriteMasterIntensity(MAX7313_1_ADDR, MAX7313_MAX_MASTER_INTENSITY);
    /*LEDS INTENSITY*/
    MAX7313_WriteIntensity(MAX7313_0_ADDR, MAX7313_LED_INTENSITY_16_16);
    MAX7313_WriteIntensity(MAX7313_1_ADDR, MAX7313_LED_INTENSITY_16_16);

    uint16_t init_state = UserIface_read_buttons_state();
    UserIface_proc_encoder(init_state);
}

uint16_t UserIface_read_buttons_state(void)
{
    return MAX7313_ReadInput(MAX7313_1_ADDR);
}

void UserIface_switch_button_encoder_color(void)
{
    uint16_t ledstate = MAX7313_ReadPhase0(MAX7313_1_ADDR);
    ledstate ^= USER_IFACE_LED_ENDCODER_MASK;
    MAX7313_WritePhase0(MAX7313_1_ADDR, ledstate);
}

void UserIface_switch_button_rgb_color(void)
{
    static RgbColor color = RGB_WHITE;

    uint16_t ledstate = MAX7313_ReadPhase0(MAX7313_1_ADDR);

    ledstate &= USER_IFACE_LED_RGB_BUTTON_MASK;
    ledstate |= (color << 12);

    MAX7313_WritePhase0(MAX7313_1_ADDR, ledstate);

    color++;
    if (color == RGB_NO_COLOR)
        color = RGB_WHITE;
}

void UserIface_switch_rgb_1_color(void)
{
    static RgbColor color = RGB_WHITE;

    UserIface_set_rgb_1_color(color);

    color++;
    if (color == RGB_NO_COLOR)
        color = RGB_WHITE;
}

void UserIface_switch_rgb_2_color(void)
{
    static RgbColor color = RGB_WHITE;

    UserIface_set_rgb_2_color(color);

    color++;
    if (color == RGB_NO_COLOR)
        color = RGB_WHITE;
}

void UserIface_set_rgb_1_color(RgbColor color)
{
    uint16_t ledstate = MAX7313_ReadPhase0(MAX7313_0_ADDR);

    ledstate &= USER_IFACE_LED_RGB_1_MASK;
    ledstate |= (color << 8);

    MAX7313_WritePhase0(MAX7313_0_ADDR, ledstate);
}

void UserIface_set_rgb_2_color(RgbColor color)
{
    uint16_t ledstate = MAX7313_ReadPhase0(MAX7313_0_ADDR);

    ledstate &= USER_IFACE_LED_RGB_2_MASK;
    ledstate |= (color << 11);

    MAX7313_WritePhase0(MAX7313_0_ADDR, ledstate);
}

void UserIface_switch_circle_position(CirclePosition position)
{
    uint16_t ledstate = MAX7313_ReadPhase0(MAX7313_0_ADDR);

    ledstate &= USER_IFACE_LED_CIRCLE_MASK;

    switch (position)
    {
    case CIRCLE_1:   ledstate |=  USER_IFACE_LED_CIRCLE_1_MASK;   break;
    case CIRCLE_2:   ledstate |=  USER_IFACE_LED_CIRCLE_2_MASK;   break;
    case CIRCLE_3:   ledstate |=  USER_IFACE_LED_CIRCLE_3_MASK;   break;
    case CIRCLE_4:   ledstate |=  USER_IFACE_LED_CIRCLE_4_MASK;   break;
    case CIRCLE_5:   ledstate |=  USER_IFACE_LED_CIRCLE_5_MASK;   break;
    case CIRCLE_6:   ledstate |=  USER_IFACE_LED_CIRCLE_6_MASK;   break;
    case CIRCLE_7:   ledstate |=  USER_IFACE_LED_CIRCLE_7_MASK;   break;
    case CIRCLE_8:   ledstate |=  USER_IFACE_LED_CIRCLE_8_MASK;   break;
    case CIRCLE_ALL: ledstate |=  USER_IFACE_LED_CIRCLE_ALL_MASK; break;
    }

    MAX7313_WritePhase0(MAX7313_0_ADDR, ledstate);
}

void UserIface_proc_encoder(uint16_t state)
{
    if (~state & USER_IFACE_ENCODER_1_MASK) {
        UserIface_switch_circle_position(CIRCLE_1);
    }

    else if (~state & USER_IFACE_ENCODER_2_MASK) {
        UserIface_switch_circle_position(CIRCLE_2);
    }

    else if (~state & USER_IFACE_ENCODER_3_MASK) {
        UserIface_switch_circle_position(CIRCLE_3);
    }

    else if (~state & USER_IFACE_ENCODER_4_MASK) {
        UserIface_switch_circle_position(CIRCLE_4);
    }

    else if (~state & USER_IFACE_ENCODER_5_MASK) {
        UserIface_switch_circle_position(CIRCLE_5);
    }

    else if (~state & USER_IFACE_ENCODER_6_MASK) {
        UserIface_switch_circle_position(CIRCLE_6);
    }

    else if (~state & USER_IFACE_ENCODER_7_MASK) {
        UserIface_switch_circle_position(CIRCLE_7);
    }

    else if (~state & USER_IFACE_ENCODER_8_MASK) {
        UserIface_switch_circle_position(CIRCLE_8);
    }

    else {
        UserIface_switch_circle_position(CIRCLE_ALL);
    }
}

void i2c_transfer(void *wbuf, size_t wlen, void *rbuf, size_t rlen, uint_least8_t addr)
{
    I2C_Transaction i2cTransaction;

    i2cTransaction.slaveAddress = addr;
    i2cTransaction.writeBuf     = wbuf;
    i2cTransaction.writeCount   = wlen;
    i2cTransaction.readBuf      = rbuf;
    i2cTransaction.readCount    = rlen;

    while(I2C_transfer(i2c, &i2cTransaction) != true);
}
