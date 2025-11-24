#include "acelerometro_i2c.h"
#include "driver/i2c.h"
#include "esp_log.h"

static const char *TAG = "ACEL_I2C";

/**
 * @brief Este metodo se encarga de incializar la comunicacion del acelerometro
 * 
 */
void acelerometro_i2c_init(void)
{
    i2c_config_t cfg = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_SDA_PIN,
        .scl_io_num = I2C_SCL_PIN,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_FREC
    };

    ESP_ERROR_CHECK(i2c_param_config(I2C_PORT, &cfg));
    ESP_ERROR_CHECK(i2c_driver_install(I2C_PORT, I2C_MODE_MASTER, 0, 0, 0));

    ESP_LOGI(TAG, "I2C inicializado en SDA=%d SCL=%d @ %d Hz",
             I2C_SDA_PIN, I2C_SCL_PIN, I2C_FREC);
}

/**
 * @brief Este metodo se encarga de escribir un byte a un registro del acelerometro
 * 
 * @param reg_addr La ubicacion del registro
 * @param data La informacion que se envia al registro
 * @return esp_err_t 
 */
esp_err_t i2c_write_byte(uint8_t reg_addr, uint8_t data)
{
    uint8_t buffer[2] = { reg_addr, data };

    return i2c_master_write_to_device(
                I2C_PORT,
                MPU6050_ADDR,
                buffer,
                sizeof(buffer),
                pdMS_TO_TICKS(10));
}

/**
 * @brief Este metodo se encarga de leer un byte de un registro del acelerometro
 * 
 * @param reg_addr El registro al que se desea leer
 * @param data_out El valor del registro que se leyó
 * @return esp_err_t 
 */
esp_err_t i2c_read_byte(uint8_t reg_addr, uint8_t *data_out)
{
    return i2c_master_write_read_device(
                I2C_PORT,
                MPU6050_ADDR,
                &reg_addr,
                1,
                data_out,
                1,
                pdMS_TO_TICKS(10));
}

/**
 * @brief Este metodo se encarga de leer varios bytes consecutivos
 * 
 * @param start_reg 
 * @param data_out 
 * @param len 
 * @return esp_err_t 
 */
esp_err_t i2c_read_bytes(uint8_t start_reg, uint8_t *data_out, size_t len)
{
    return i2c_master_write_read_device(
                I2C_PORT,
                MPU6050_ADDR,
                &start_reg,
                1,
                data_out,
                len,
                pdMS_TO_TICKS(10));
}
