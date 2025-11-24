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
#define I2C_SDA_PIN     5
#define I2C_SCL_PIN     4

// puerto
#define I2C_PORT        I2C_NUM_0

// frecuencia del bus
#define I2C_FREC        400000

/**
 * @brief Este metodo se encarga de incializar la comunicacion del acelerometro
 * 
 */
void acelerometro_i2c_init(void);

/**
 * @brief Este metodo se encarga de escribir un byte a un registro del acelerometro
 * 
 * @param reg_addr La ubicacion del registro
 * @param data La informacion que se envia al registro
 * @return esp_err_t 
 */
esp_err_t i2c_write_byte(uint8_t reg_addr, uint8_t data);

/**
 * @brief Este metodo se encarga de leer un byte de un registro del acelerometro
 * 
 * @param reg_addr El registro al que se desea leer
 * @param data_out El valor del registro que se leyó
 * @return esp_err_t 
 */
esp_err_t i2c_read_byte(uint8_t reg_addr, uint8_t *data_out);

/**
 * @brief Este metodo se encarga de leer varios bytes consecutivos
 * 
 * @param start_reg 
 * @param data_out 
 * @param len 
 * @return esp_err_t 
 */
esp_err_t i2c_read_bytes(uint8_t start_reg, uint8_t *data_out, size_t len);

#ifdef __cplusplus
}
#endif
#endif
