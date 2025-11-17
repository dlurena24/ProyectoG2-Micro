#ifndef ULTRASONICO_TIMER_H
#define ULTRASONICO_TIMER_H

#include <stdint.h>

// Inicializa Timer Group 0, Timer 0 como contador libre a ~1 MHz (1 us por tick)
void ultrasonico_timer_init(void);

// Devuelve el valor actual del timer en microsegundos
uint64_t ultrasonico_timer_get_us(void);

// Pone el contador en cero
void ultrasonico_timer_reset(void);

#endif // ULTRASONICO_TIMER_H
