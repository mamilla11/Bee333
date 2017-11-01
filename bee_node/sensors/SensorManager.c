#include "SensorManager.h"

#include <ti/drivers/I2C.h>
#include <ti/drivers/ADC.h>

#include "drv/Ina226.h"
#include "drv/Opt3001.h"
#include "drv/Hdc1080.h"
#include "drv/bme680/bme680.h"
#include "drv/bme680/bme680_calculations.h"

#define OPT3001_ADDRESS         (uint8_t)0x44
#define INA226_ADDRESS          (uint8_t)0x45
#define HDC1080_ADDRESS         (uint8_t)0x40

#define INA226_SHUNT_RESISTANCE (float)1.0F
#define U_FACTOR                (float)1000000.0F
#define M_FACTOR                (float)1000.0F

static struct bme680_t               bme680_config;
static struct bme680_sens_conf       bme680_sens_config;
static struct bme680_heater_conf     bme680_gas_config;
static struct bme680_comp_field_data bme680_data;

static bool HDC1080_present = false;
static bool OPT3001_present = false;
static bool INA226_present  = false;
static bool BME680_present  = false;

I2C_Handle  i2c;
ADC_Handle  adc;

I2C_Params i2cParams;
ADC_Params adcParams;

int8_t i2c_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data_ptr, uint8_t data_len);
int8_t i2c_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data_ptr, uint8_t data_len);
int8_t i2c_transfer(void *wbuf, size_t wlen, void *rbuf, size_t rlen, uint_least8_t addr);

void sm_init(sleep sleep_callback)
{
    ADC_init();
    ADC_Params_init(&adcParams);
    adc = ADC_open(ADC0, &adcParams);

    I2C_init();
    I2C_Params_init(&i2cParams);
    i2cParams.bitRate = I2C_100kHz;
    i2c = I2C_open(I2C0, &i2cParams);

    HDC1080_init(HDC1080_ADDRESS, i2c_transfer);
    HDC1080_present = HDC1080_is_present();
    if (HDC1080_present)
    {
        HDC1080_set_mode(HDC1080_DATA_SEPARATED);
        HDC1080_set_temp_resolution(HDC1080_TRES_14_BIT);
        HDC1080_set_humi_resolution(HDC1080_HRES_14_BIT);
    }

    OPT3001_init(OPT3001_ADDRESS, i2c_transfer);
    OPT3001_present = OPT3001_is_present();
    if (OPT3001_present)
    {
        OPT3001_set_range(RANGE_AUTO);
        OPT3001_set_conversion_time(CONVTIME_800MS);
        OPT3001_set_mode(MODE_CONTINUOUS);
    }

    INA226_init(INA226_ADDRESS, i2c_transfer);
    INA226_present = INA226_is_present();
    if (INA226_present)
    {
        INA226_set_calibration(INA226_SHUNT_RESISTANCE);
    }

    bme680_config.dev_addr         = BME680_I2C_ADDR_PRIMARY;
    bme680_config.interface        = BME680_I2C_INTERFACE;
    bme680_config.bme680_bus_write = i2c_write;
    bme680_config.bme680_bus_read  = i2c_read;
    bme680_config.delay_msec       = sleep_callback;

    bme680_sens_config.heatr_ctrl  = BME680_HEATR_CTRL_DISABLE;
    bme680_sens_config.odr         = BME680_ODR_20MS;
    bme680_sens_config.run_gas     = BME680_RUN_GAS_DISABLE;
    bme680_sens_config.nb_conv     = 0x00;
    bme680_sens_config.osrs_hum    = BME680_OSRS_1X;
    bme680_sens_config.osrs_pres   = BME680_OSRS_1X;
    bme680_sens_config.osrs_temp   = BME680_OSRS_1X;
    bme680_sens_config.filter      = BME680_FILTER_COEFF_1;
    bme680_sens_config.spi_3w      = BME680_SPI_3W_DISABLE;
    bme680_sens_config.intr        = BME680_SPI_3W_INTR_DISABLE;

    bme680_gas_config.heater_temp[0] = 300;
    bme680_gas_config.heatr_dur[0]   = 100;
    bme680_gas_config.profile_cnt    = 1;

    if (bme680_init(&bme680_config) == BME680_COMM_RES_OK)
        BME680_present = true;
}

void sm_trigger_measurement()
{
    struct bme680_uncomp_field_data  bme680_uncomp_data;

    if (!BME680_present)
        return;

    bme680_set_power_mode(1, &bme680_config);
    bme680_set_sensor_config(&bme680_sens_config, &bme680_config);
    //bme680_set_gas_heater_config(&bme680_gas_config, &bme680_config);
    bme680_get_uncomp_data(&bme680_uncomp_data, 1, BME680_ALL, &bme680_config);
    bme680_compensate_data(&bme680_uncomp_data, &bme680_data, 1, BME680_ALL, &bme680_config);
}

float sm_get_current_in_A()
{
    if (INA226_present)
        return INA226_get_current_in_A();
    return 0.0F;
}

float sm_get_current_in_uA()
{
    if (INA226_present)
        return INA226_get_current_in_A() * U_FACTOR;
    return 0.0F;
}

float sm_get_voltage_in_V()
{
    if (INA226_present)
        return INA226_get_bus_voltage_in_V();
    return 0.0F;
}

float sm_get_voltage_in_mV()
{
    if (INA226_present)
        return INA226_get_bus_voltage_in_V() * M_FACTOR;
    return 0.0F;
}

float sm_get_power_in_W()
{
    if (INA226_present)
        return INA226_get_power_in_W();
    return 0.0F;
}

float sm_get_power_in_uW()
{
    if (INA226_present)
        return INA226_get_power_in_W() * U_FACTOR;
    return 0.0F;
}

float sm_get_light_in_lux()
{
    if (OPT3001_present)
        return (OPT3001_get_light_in_lux());
    return 0.0F;
}

float sm_get_temp1_in_C()
{
    if (HDC1080_present)
        return HDC1080_get_temperature_in_C();
    return 0.0F;
}

float sm_get_humi1_in_rH()
{
    if (HDC1080_present)
        return HDC1080_get_humidity();
    return 0.0F;
}

float sm_get_temp2_in_C()
{
    if (BME680_present)
        return bme680_data.comp_temperature1 * 0.01F;
    return 0.0F;
}

float sm_get_humi2_in_rH()
{
    if (BME680_present)
        return bme680_data.comp_humidity / 1024.0F;
    return 0.0F;
}

float sm_get_pressure_in_hPa()
{
    if (BME680_present)
        return bme680_data.comp_pressure * 0.01F;
    return 0.0F;
}

int32_t sm_get_gas()
{
    if (BME680_present)
        return bme680_data.comp_gas;
    return 0;
}

uint16_t sm_get_adc_value()
{
    int i;
    uint16_t value = 0;
    uint32_t value_r = 0;
    uint16_t sum = 0;
    for (i = 0; i < 10; i++)
    {
        int_fast16_t res = ADC_convert(adc, &value);

        if (res != ADC_STATUS_SUCCESS) {
            return 0;
        }

        sum += value;
    }
    value_r = ADC_convertRawToMicroVolts(adc, (sum/10));
    value = (value_r/1000)*2.07;
    return value;
}


int8_t i2c_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data_ptr, uint8_t data_len)
{
    static uint8_t w_buf[10];
    w_buf[0] = reg_addr;
    memcpy(&w_buf[1], reg_data_ptr, data_len);
    return i2c_transfer(w_buf, (data_len + 1), (void*)0, 0, dev_addr);
}

int8_t i2c_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data_ptr, uint8_t data_len)
{
    return i2c_transfer(&reg_addr, 1, reg_data_ptr, data_len, dev_addr);
}

int8_t i2c_transfer(void *wbuf, size_t wlen, void *rbuf, size_t rlen, uint_least8_t addr)
{
    I2C_Transaction i2cTransaction;

    i2cTransaction.slaveAddress = addr;
    i2cTransaction.writeBuf     = wbuf;
    i2cTransaction.writeCount   = wlen;
    i2cTransaction.readBuf      = rbuf;
    i2cTransaction.readCount    = rlen;

    if (I2C_transfer(i2c, &i2cTransaction) == true)
        return 0;
    return -1;
}
