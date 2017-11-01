#ifndef UI_USER_IFACE_H_
#define UI_USER_IFACE_H_

#include "../Board.h"

#define USER_IFACE_BUTTON_MASK                          0x0FFC
#define USER_IFACE_ENCODER_1_MASK                       0x0004
#define USER_IFACE_ENCODER_2_MASK                       0x0008
#define USER_IFACE_ENCODER_3_MASK                       0x0010
#define USER_IFACE_ENCODER_4_MASK                       0x0020
#define USER_IFACE_ENCODER_5_MASK                       0x0040
#define USER_IFACE_ENCODER_6_MASK                       0x0080
#define USER_IFACE_ENCODER_7_MASK                       0x0100
#define USER_IFACE_ENCODER_8_MASK                       0x0200
#define USER_IFACE_BUTTON_ENCODER_MASK                  0x0400
#define USER_IFACE_BUTTON_RGB_MASK                      0x0800

#define USER_IFACE_LED_ENDCODER_MASK                    0x0003
#define USER_IFACE_LED_RGB_BUTTON_MASK                  0x8FFF
#define USER_IFACE_LED_RGB_1_MASK                       0xF8FF
#define USER_IFACE_LED_RGB_2_MASK                       0xC7FF
#define USER_IFACE_LED_CIRCLE_MASK                      0xFF00

#define USER_IFACE_LED_CIRCLE_1_MASK                      0x00FE
#define USER_IFACE_LED_CIRCLE_2_MASK                      0x00FD
#define USER_IFACE_LED_CIRCLE_3_MASK                      0x00FB
#define USER_IFACE_LED_CIRCLE_4_MASK                      0x00F7
#define USER_IFACE_LED_CIRCLE_5_MASK                      0x00EF
#define USER_IFACE_LED_CIRCLE_6_MASK                      0x00DF
#define USER_IFACE_LED_CIRCLE_7_MASK                      0x00BF
#define USER_IFACE_LED_CIRCLE_8_MASK                      0x007F
#define USER_IFACE_LED_CIRCLE_ALL_MASK                    0x0000

typedef enum
{
    RGB_WHITE    = 0x0,
    RGB_CYAN     = 0x1,
    RGB_MAGENTA  = 0x2,
    RGB_BLUE     = 0x3,
    RGB_YELLOW   = 0x4,
    RGB_GREEN    = 0x5,
    RGB_RED      = 0x6,
    RGB_NO_COLOR = 0x7,
} RgbColor;

typedef enum
{
    CIRCLE_1,
    CIRCLE_2,
    CIRCLE_3,
    CIRCLE_4,
    CIRCLE_5,
    CIRCLE_6,
    CIRCLE_7,
    CIRCLE_8,
    CIRCLE_ALL,
} CirclePosition;

void UserIface_init(void);
uint16_t UserIface_read_buttons_state(void);
void UserIface_switch_button_encoder_color(void);
void UserIface_switch_button_rgb_color(void);
void UserIface_switch_rgb_1_color(void);
void UserIface_switch_rgb_2_color(void);
void UserIface_set_rgb_1_color(RgbColor color);
void UserIface_set_rgb_2_color(RgbColor color);
void UserIface_switch_circle_position(CirclePosition position);
void UserIface_proc_encoder(uint16_t state);

#endif /* UI_USER_IFACE_H_ */
