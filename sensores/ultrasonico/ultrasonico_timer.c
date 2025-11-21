#include "ultrasonico_timer.h"
#include "driver/timer.h"

#define ULTRA_TIMER_GROUP   TIMER_GROUP_0
#define ULTRA_TIMER_INDEX   TIMER_0

// 80 MHz
#define ULTRA_TIMER_DIVIDER 80

void ultrasonico_timer_init(void)
{
    timer_config_t config = {
        .divider = ULTRA_TIMER_DIVIDER,
        .counter_dir = TIMER_COUNT_UP,
        .counter_en = TIMER_PAUSE,
        .alarm_en = TIMER_ALARM_DIS,
        .auto_reload = false,
    };

    // configurar timer
    timer_init(ULTRA_TIMER_GROUP, ULTRA_TIMER_INDEX, &config);

    // contador en 0
    timer_set_counter_value(ULTRA_TIMER_GROUP, ULTRA_TIMER_INDEX, 0);

    // iniciar conteo
    timer_start(ULTRA_TIMER_GROUP, ULTRA_TIMER_INDEX);
}

uint64_t ultrasonico_timer_get_us(void)
{
    uint64_t value = 0;
    timer_get_counter_value(ULTRA_TIMER_GROUP, ULTRA_TIMER_INDEX, &value);
    // Con divisor 80, cada tick = 1 us
    return value;
}

void ultrasonico_timer_reset(void)
{
    // Pausar
    timer_pause(ULTRA_TIMER_GROUP, ULTRA_TIMER_INDEX);

    // Poner contador en 0
    timer_set_counter_value(ULTRA_TIMER_GROUP, ULTRA_TIMER_INDEX, 0);

    // Reanudar
    timer_start(ULTRA_TIMER_GROUP, ULTRA_TIMER_INDEX);
}
