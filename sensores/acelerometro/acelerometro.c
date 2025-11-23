#include "acelerometro.h"
#include "acelerometro_i2c.h"

#include "esp_log.h"
#include "math.h"

static const char *TAG = "ACELEROMETRO";

// Registros
#define MPU_REG_PWR_MGMT_1        0x6B
#define MPU_REG_ACCEL_XOUT_H      0x3B

// Sensibilidad
#define MPU_ACEL_SENSIBILIDAD     16384.0f

// Inicializa el acelerometro
void acelerometro_init(void)
{
    acelerometro_i2c_init();

    esp_err_t ret = i2c_write_byte(MPU_REG_PWR_MGMT_1, 0x00);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Error al despertar el MPU6050");
    } else {
        ESP_LOGI(TAG, "MPU6050 inicializado correctamente");
    }
}

// Leer los valores del acelerómetro
esp_err_t acelerometro_leer_raw(int16_t *ax, int16_t *ay, int16_t *az)
{
    uint8_t data[6];

    esp_err_t ret = i2c_read_bytes(MPU_REG_ACCEL_XOUT_H, data, 6);
    if (ret != ESP_OK) return ret;

    *ax = (int16_t)((data[0] << 8) | data[1]);
    *ay = (int16_t)((data[2] << 8) | data[3]);
    *az = (int16_t)((data[4] << 8) | data[5]);

    return ESP_OK;
}

// Convertir valores a unidades de g
esp_err_t acelerometro_leer_g(float *ax, float *ay, float *az)
{
    int16_t raw_x, raw_y, raw_z;

    esp_err_t ret = acelerometro_leer_raw(&raw_x, &raw_y, &raw_z);
    if (ret != ESP_OK) return ret;

    const float scale = 1.0f / MPU_ACEL_SENSIBILIDAD;

    *ax = raw_x * scale;
    *ay = raw_y * scale;
    *az = raw_z * scale;

    return ESP_OK;
}

// Calcular magnitud de la aceleración
float acelerometro_magnitud(float ax, float ay, float az)
{
    return sqrtf(ax*ax + ay*ay + az*az);
}
