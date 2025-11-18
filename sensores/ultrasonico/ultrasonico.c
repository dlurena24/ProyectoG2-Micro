#include "ultrasonico.h"
#include "ultrasonico_timer.h"
#include "ultrasonico_isr.h"

#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_rom_sys.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// debug
static const char *TAG = "ULTRASONICO";

// tiempo max entre pulsos
#define ULTRA_TIMEOUT_US 30000

// inicializa el componente
void ultrasonico_init(void)
{
    // salida en TRIG
    gpio_config_t trig_cfg = {
        .pin_bit_mask = (1ULL << GPIO_TRIG),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = 0,
        .pull_down_en = 0,
        .intr_type = GPIO_INTR_DISABLE
    };

    gpio_config(&trig_cfg);

    gpio_set_level(GPIO_TRIG, 0);

    // init timer
    ultrasonico_timer_init();

    // init ISR
    ultrasonico_isr_init();

    // debug
    ESP_LOGI(TAG, "ultrasonico init");
}

// mide tiempo entre pulsos
uint64_t ultrasonico_medicion_raw(void)
{
    echo_rise_us = 0;
    echo_fall_us = 0;

    // genera pulso en TRIG
    gpio_set_level(GPIO_TRIG, 1);
    ets_delay_us(10);
    gpio_set_level(GPIO_TRIG, 0);

    // espera ECHO en el rango definido
    uint64_t start_wait = ultrasonico_timer_get_us();

    while (echo_fall_us == 0) {
        // timeout para evitar freeze
        if (ultrasonico_timer_get_us() - start_wait > ULTRA_TIMEOUT_US) {
            ESP_LOGW(TAG, "Timeout esperando pulso ECHO");
            return 0;
        }
    }

    // duracion del pulso
    return echo_fall_us - echo_rise_us;
}

// convierte duracion de pulso a distancia
void ultrasonico_medicion(void)
{
    uint64_t duracion = ultrasonico_medicion_raw();

    if (duracion == 0) {
        ESP_LOGW(TAG, "Error de lectura");
        return;
    }

    // distancia  = tiempo * 0.01715 (343 m/s -> 0.01715 cm/us)
    float distancia_cm = (float)duracion * 0.01715f;

    ESP_LOGI(TAG, "Distancia: %.2f cm", distancia_cm);
}
