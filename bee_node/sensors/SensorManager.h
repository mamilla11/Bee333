#ifndef SENSORS_SENSOR_MANAGER_H_
#define SENSORS_SENSOR_MANAGER_H_

#include <Board.h>

typedef void (*sleep)(uint32_t t_ms);

void sm_init(sleep sleep_callback);
void sm_trigger_measurement();
uint16_t sm_get_current();
uint16_t sm_get_voltage();
uint16_t sm_get_power();
uint16_t sm_get_light();
uint16_t sm_get_temp1();
uint16_t sm_get_humi1();
uint16_t sm_get_temp2();
uint16_t sm_get_humi2();
uint16_t sm_get_pressure();

uint16_t sm_get_adc_value();

uint16_t sm_get_status();

#endif
