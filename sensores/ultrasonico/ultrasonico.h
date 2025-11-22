#ifndef ULTRASONICO_H
#define ULTRASONICO_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// pines
#define GPIO_TRIG 5
#define GPIO_ECHO 18

// inicializa el modulo
void ultrasonico_init(void);

// mide la distancia
void ultrasonico_medicion(void);

// mide duracion del pulso
uint64_t ultrasonico_medicion_raw(void);

#ifdef __cplusplus
}
#endif

#endif // Ultrasonico_H