#ifndef ULTRASONICO_ISR_H
#define ULTRASONICO_ISR_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// timestamps
extern volatile uint64_t echo_rise_us;
extern volatile uint64_t echo_fall_us;

// inicializa ECHO
void ultrasonico_isr_init(void);

#ifdef __cplusplus
}
#endif

#endif