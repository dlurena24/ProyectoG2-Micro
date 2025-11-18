#include "ultrasonico_isr.h"
#include "ultrasonico_timer.h"
#include "ultrasonico.h"
#include "driver/gpio.h"
#include "esp_attr.h"

// variables globales
volatile uint64_t echo_rise_us = 0;
volatile uint64_t echo_fall_us = 0;

// Interrupciones de ECHO
static void IRAM_ATTR ultrasonico_echo_isr(void *arg)
{
    int level = gpio_get_level(GPIO_ECHO);

    if (level == 1) {
        // flanco de subida: ECHO pasó a 1
        echo_rise_us = ultrasonico_timer_get_us();
    } else {
        // flanco de bajada: ECHO pasó a 0
        echo_fall_us = ultrasonico_timer_get_us();
    }
}

void ultrasonico_isr_init(void)
{
    gpio_config_t cfg = {
        .pin_bit_mask = (1ULL << GPIO_ECHO),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = 0,
        .pull_down_en = 0,
        .intr_type = GPIO_INTR_ANYEDGE
    };

    gpio_config(&cfg);

    // instala servicio de ISR si no existe aún
    gpio_install_isr_service(0);

    // registra la ISR para el pin ECHO
    gpio_isr_handler_add(GPIO_ECHO, ultrasonico_echo_isr, NULL);
}