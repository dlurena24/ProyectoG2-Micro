#ifndef ACELEROMETRO_H
#define ACELEROMETRO_H

#include <stdint.h>
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

// Inicializa el acelerometro
void acelerometro_init(void);

// Lee valores del acelerometro
esp_err_t acelerometro_leer_raw(int16_t *ax, int16_t *ay, int16_t *az);

// Lee valores convertidos a unidades de g
esp_err_t acelerometro_leer_g(float *ax, float *ay, float *az);

// Calcula la magnitud total
float acelerometro_magnitud(float ax, float ay, float az);

#ifdef __cplusplus
}
#endif

#endif
