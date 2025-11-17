#include "ultrasonico_timer.h"
#include "soc/timer_group_struct.h"
#include "soc/timer_group_reg.h"
#include "soc/timer_group_reg.h"
#include "esp_err.h"

// Timer Group 0, Timer 0
#define TIMERG       TIMERG0
#define TIMER_INDEX  0

// El APB clock corre a 80MHz → si dividimos entre 80, obtenemos 1MHz (1us por tick)
#define TIMER_DIVIDER     80

void ultrasonico_timer_init(void)
{
    // 1. Detener timer
    TIMERG.hw_timer[TIMER_INDEX].config.enable = 0;

    // 2. Configurar divisor (80 MHz / 80 = 1 MHz = 1 tick = 1us)
    TIMERG.hw_timer[TIMER_INDEX].config.divider = TIMER_DIVIDER;

    // 3. Contador hacia arriba
    TIMERG.hw_timer[TIMER_INDEX].config.direction = 0;  // 0 = up, 1 = down

    // 4. Modo one-shot = 0 (conteo libre)
    TIMERG.hw_timer[TIMER_INDEX].config.autoreload = 0;

    // 5. Valor inicial = 0
    TIMERG.hw_timer[TIMER_INDEX].load_high = 0;
    TIMERG.hw_timer[TIMER_INDEX].load_low  = 0;
    TIMERG.hw_timer[TIMER_INDEX].reload = 1;

    // 6. Habilitar incremento
    TIMERG.hw_timer[TIMER_INDEX].config.increase = 1;

    // 7. Iniciar conteo
    TIMERG.hw_timer[TIMER_INDEX].config.enable = 1;
}

uint64_t ultrasonico_timer_get_us(void)
{
    // Leer registros de 64 bits: cnt_high y cnt_low
    uint32_t low  = TIMERG.hw_timer[TIMER_INDEX].cnt_low;
    uint32_t high = TIMERG.hw_timer[TIMER_INDEX].cnt_high;

    // Ensamblar 64 bits
    uint64_t value = ((uint64_t)high << 32) | low;

    return value;  // Este valor está en microsegundos
}

void ultrasonico_timer_reset(void)
{
    // Detener el timer
    TIMERG.hw_timer[TIMER_INDEX].config.enable = 0;

    // Escribir 0 en contador
    TIMERG.hw_timer[TIMER_INDEX].load_high = 0;
    TIMERG.hw_timer[TIMER_INDEX].load_low  = 0;
    TIMERG.hw_timer[TIMER_INDEX].reload = 1;

    // Reanudar conteo
    TIMERG.hw_timer[TIMER_INDEX].config.enable = 1;
}
