#ifndef ACELEROMETRO_I2C_H
#define ACELEROMETRO_I2C_H

#include <stdint.h>
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

// direccon para I2C
#define MPU6050_ADDR    0x68

// pines para I2C
#define I2C_SDA_PIN     21
#define I2C_SCL_PIN     47

// puerto
#define I2C_PORT        I2C_NUM_0

// frecuencia del bus
#define I2C_FREC        400000

// inicializa el bus
void acelerometro_i2c_init(void);

// escibe un byte a un registro del acelerometro
esp_err_t i2c_write_byte(uint8_t reg_addr, uint8_t data);

// lee un byte de un registro del acelerometro
esp_err_t i2c_read_byte(uint8_t reg_addr, uint8_t *data_out);

// lee varios bytes consecutivos
esp_err_t i2c_read_bytes(uint8_t start_reg, uint8_t *data_out, size_t len);

#ifdef __cplusplus
}
#endif
#endif
