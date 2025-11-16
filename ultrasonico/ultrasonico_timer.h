#ifndef ULTRASONICO_TIMER_H
#define ULTRASONICO_TIMER_H

#include <stdint.h>

// Inicializa Timer Group 0, Timer 0 como contador libre a 1 MHz (1us por tick)
void ultrasonico_timer_init(void);

// Retorna el tiempo actual del timer en microsegundos
uint64_t ultrasonico_timer_get_us(void);

// Reinicia el contador a cero
void ultrasonico_timer_reset(void);

#endif
