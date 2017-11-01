#ifndef SENSORS_SENSOR_MANAGER_H_
#define SENSORS_SENSOR_MANAGER_H_

#include <Board.h>

typedef void (*sleep)(uint32_t t_ms);

void sm_init(sleep sleep_callback);
void sm_trigger_measurement();
float sm_get_current_in_A();
float sm_get_current_in_uA();
float sm_get_voltage_in_V();
float sm_get_voltage_in_mV();
float sm_get_power_in_W();
float sm_get_power_in_uW();
float sm_get_light_in_lux();
float sm_get_temp1_in_C();
float sm_get_humi1_in_rH();
float sm_get_temp2_in_C();
float sm_get_humi2_in_rH();
float sm_get_pressure_in_hPa();
int32_t sm_get_gas();
uint16_t sm_get_adc_value();

#endif
