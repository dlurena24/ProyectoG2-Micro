#include "ultrasonico_timer.h"
#include "driver/gptimer.h"

static gptimer_handle_t ultra_timer = NULL;

void ultrasonico_timer_init(void)
{
    // Configurar GPTimer con resolución de 1 MHz = 1 us por tick
    gptimer_config_t config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_COUNT_UP,
        .resolution_hz = 1000000,   // 1 tick = 1 microsegundo
    };

    ESP_ERROR_CHECK(gptimer_new_timer(&config, &ultra_timer));
    ESP_ERROR_CHECK(gptimer_enable(ultra_timer));

    // Poner en cero
    ESP_ERROR_CHECK(gptimer_set_raw_count(ultra_timer, 0));

    // Arrancar
    ESP_ERROR_CHECK(gptimer_start(ultra_timer));
}

uint64_t ultrasonico_timer_get_us(void)
{
    uint64_t value = 0;
    ESP_ERROR_CHECK(gptimer_get_raw_count(ultra_timer, &value));
    return value;   // ya son microsegundos
}

void ultrasonico_timer_reset(void)
{
    // Pausar
    ESP_ERROR_CHECK(gptimer_stop(ultra_timer));

    // Reset a 0
    ESP_ERROR_CHECK(gptimer_set_raw_count(ultra_timer, 0));

    // Reanudar
    ESP_ERROR_CHECK(gptimer_start(ultra_timer));
}