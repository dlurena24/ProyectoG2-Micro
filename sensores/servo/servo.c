#include "servo.h"
#include "driver/rmt_tx.h"
#include "driver/rmt_encoder.h"
#include "esp_log.h"

static const char *TAG = "SERVO";

#define SERVO_MIN_US 500     // microsegundos 0°
#define SERVO_MAX_US 2400    // microsegundos 180°
#define SERVO_FREQ_HZ 50

static rmt_channel_handle_t tx_channel = NULL;
static rmt_encoder_handle_t pwm_encoder = NULL;
static int servo_gpio_pin = -1;

// convierte grados → microsegundos
static uint32_t angle_to_pulse(float deg)
{
    if (deg < 0) deg = 0;
    if (deg > 180) deg = 180;
    return SERVO_MIN_US + (deg / 180.0f) * (SERVO_MAX_US - SERVO_MIN_US);
}

void servo_init(int gpio)
{
    servo_gpio_pin = gpio;

    rmt_tx_channel_config_t tx_cfg = {
        .gpio_num = gpio,
        .clk_src = RMT_CLK_SRC_DEFAULT,
        .mem_block_symbols = 64,
        .resolution_hz = 1000000, // 1 MHz → 1 tick por microsegundo
        .trans_queue_depth = 4,
    };
    rmt_new_tx_channel(&tx_cfg, &tx_channel);

    rmt_pwm_encoder_config_t enc_cfg = {
        .resolution = 1000000, // 1 MHz
        .freq_hz = SERVO_FREQ_HZ,
        .duty_cycle = 0.05, // placeholder, se cambia dinámicamente
    };

    rmt_new_pwm_encoder(&enc_cfg, &pwm_encoder);

    rmt_enable(tx_channel);

    ESP_LOGI(TAG, "Servo inicializado en GPIO %d", gpio);
}

void servo_set_angle(float degrees)
{
    uint32_t pulse = angle_to_pulse(degrees);

    rmt_pwm_encoder_set_pulse_width(pwm_encoder, pulse);

    rmt_transmit_config_t tx_config = {};
    rmt_transmit(tx_channel, pwm_encoder, NULL, &tx_config);

    ESP_LOGI(TAG, "Ángulo %.1f° → %dus", degrees, pulse);
}

void servo_open(void)
{ 
    servo_set_angle(120);   // placeholder antes de prueba fisica
}

void servo_close(void)      // placeholder antes de prueba   
{ 
    servo_set_angle(0); 
}
