#include "servo.h"
#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


static const char *TAG = "SERVO";

// CONFIG
#define SERVO_MIN_US 500
#define SERVO_MAX_US 2400
#define SERVO_FREQ_HZ 50

// LEDC
#define SERVO_LEDC_TIMER   LEDC_TIMER_0
#define SERVO_LEDC_MODE    LEDC_LOW_SPEED_MODE
#define SERVO_LEDC_CH      LEDC_CHANNEL_0

static int servo_gpio = -1;

// microsegundos a duty
static uint32_t pulse_to_duty(uint32_t us)
{
    uint32_t period_us = 1000000 / SERVO_FREQ_HZ; // 20000 us
    return (us * ((1 << 13) - 1)) / period_us;
}

void servo_init(int gpio)
{
    servo_gpio = gpio;

    // configurar timer
    ledc_timer_config_t timer = {
        .speed_mode = SERVO_LEDC_MODE,
        .duty_resolution = LEDC_TIMER_13_BIT,
        .timer_num = SERVO_LEDC_TIMER,
        .freq_hz = SERVO_FREQ_HZ,
    };
    ledc_timer_config(&timer);

    // configurar canal
    ledc_channel_config_t ch = {
        .gpio_num = gpio,
        .speed_mode = SERVO_LEDC_MODE,
        .channel = SERVO_LEDC_CH,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = SERVO_LEDC_TIMER,
        .duty = 0,
        .hpoint = 0
    };
    ledc_channel_config(&ch);

    ESP_LOGI(TAG, "Servo inicializado en GPIO %d", gpio);
}

void servo_set_angle(float deg)
{
    if (deg < 0) deg = 0;
    if (deg > 180) deg = 180;

    uint32_t us = SERVO_MIN_US +
                  (deg / 180.0f) * (SERVO_MAX_US - SERVO_MIN_US);

    uint32_t duty = pulse_to_duty(us);

    ledc_set_duty(SERVO_LEDC_MODE, SERVO_LEDC_CH, duty);
    ledc_update_duty(SERVO_LEDC_MODE, SERVO_LEDC_CH);
}

void servo_move_smooth(float start_deg, float end_deg, float step_deg, int delay_ms)
{
    if (step_deg <= 0) step_deg = 1;

    float pos = start_deg;

    if (end_deg > start_deg)
    {
        for (pos = start_deg; pos <= end_deg; pos += step_deg)
        {
            servo_set_angle(pos);
            vTaskDelay(pdMS_TO_TICKS(delay_ms));
        }
    }
    else
    {
        for (pos = start_deg; pos >= end_deg; pos -= step_deg)
        {
            servo_set_angle(pos);
            vTaskDelay(pdMS_TO_TICKS(delay_ms));
        }
    }

    servo_set_angle(end_deg);
}

void servo_open(void)
{
    servo_move_smooth(0, 120, 2, 15);
}

void servo_close(void)
{
    servo_move_smooth(120, 0, 2, 15);
}
