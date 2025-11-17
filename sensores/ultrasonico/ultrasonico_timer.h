#ifndef ULTRASONICO_TIMER_H
#define ULTRASONICO_TIMER_H

#include <stdint.h>

#ifdef __cplusplus
extern  "C" {
#endif

// Inicializa temporizador
void ultrasonico_timer_init(void);

// Retorna timestamp
uint64_t ultrasonico_timer_get_us(void);

#ifdef __cplusplus
}
#endif

#endif