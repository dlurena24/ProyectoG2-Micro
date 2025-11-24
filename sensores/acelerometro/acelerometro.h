#ifndef ACELEROMETRO_H
#define ACELEROMETRO_H

#include <stdint.h>
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Este metodo se encarga de incializar el acelerometro
 * 
 */
void acelerometro_init(void);

/**
 * @brief Este metodo se encarga de leer los datos directos del acelerometrio
 * 
 * @param ax 
 * @param ay 
 * @param az 
 * @return esp_err_t 
 */
esp_err_t acelerometro_leer_raw(int16_t *ax, int16_t *ay, int16_t *az);

/**
 * @brief Este metodo se encarga de leer los valores g del acelerometro
 * 
 * @param ax 
 * @param ay 
 * @param az 
 * @return esp_err_t 
 */
esp_err_t acelerometro_leer_g(float *ax, float *ay, float *az);

/**
 * @brief Este metodo se encarga de calcular la magnitud total de la aceleracion
 * 
 * @param ax 
 * @param ay 
 * @param az 
 * @return float 
 */
float acelerometro_magnitud(float ax, float ay, float az);

#ifdef __cplusplus
}
#endif

#endif
