#include <stdio.h>
#include <string.h>
#include <math.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "ultrasonico.h"
#include "acelerometro.h"
#include "servo.h"

// DOOR PARAMS
static const int distanceToOpen = 30;                 // cm
static const uint64_t timeToWaitForClose = 3000;      // ms
static const float impactThreshold = 2.5f;            // m/s^2

// DOOR STATE
typedef enum {
    STOPPED,
    OPENING,
    CLOSING
} DoorState;

static DoorState doorState = STOPPED;
static uint64_t lastPresenceTime = 0;
static float baselineAccel = 9.8f;

uint64_t millis()
{
    return (uint64_t)xTaskGetTickCount() * portTICK_PERIOD_MS;
}

// EVENT SYSTEM
static void emitEvent(const char* eventType, const char* message)
{
    ESP_LOGI("EVENT", "{ \"event\": \"%s\", \"message\": \"%s\", \"timestamp\": %llu }",
             eventType, message, millis());
}

// ACCELEROMETER CALIBRATION
static void calibrate_accelerometer(void)
{
    ESP_LOGI("CALIB", "Calibrating accelerometer...");

    float sum = 0;
    const int samples = 60;

    for (int i = 0; i < samples; i++) {
        float ax, ay, az;
        esp_err_t ret = acelerometro_leer_g(&ax, &ay, &az);

        if (ret == ESP_OK) {
            float mag = acelerometro_magnitud(ax, ay, az) * 9.80665f;
            sum += mag;
        }

        vTaskDelay(pdMS_TO_TICKS(20));
    }

    baselineAccel = sum / samples;

    ESP_LOGI("CALIB", "Baseline acceleration = %.3f m/s²", baselineAccel);
}

// SENSOR READING
static int readDistance()
{
    float pulse_us = ultrasonico_medicion_raw();
    if (pulse_us == 0) return 9999;  // no detection

    float distance = pulse_us * 0.01715f; // cm

    char buffer[64];
    snprintf(buffer, sizeof(buffer), "Distance read: %f cm", distance);
    emitEvent("distance", buffer);

    return (int)distance;
}

static float readAccelerometer(void)
{
    float ax, ay, az;

    // Leer valores del acelerómetro en G
    esp_err_t ret = acelerometro_leer_g(&ax, &ay, &az);
    if (ret != ESP_OK) {
        emitEvent("acceleration_error", "I2C read failed");
        return baselineAccel;
    }

    // Magnitud en G
    float mag_g = acelerometro_magnitud(ax, ay, az);

    // Convertir G → m/s^2
    float mag_ms2 = mag_g * 9.80665f;

    char buffer[64];
    snprintf(buffer, sizeof(buffer),
             "Accel: %.2f m/s^2", mag_ms2);
    emitEvent("acceleration", buffer);

    return mag_ms2;
}

// DOOR ACTIONS
static void openDoor(void)
{
    if (doorState == OPENING) return;

    emitEvent("door_action", "Opening door");
    
    servo_open();

    doorState = OPENING;
}

static void closeDoor(void)
{
    if (doorState == CLOSING) return;

    emitEvent("door_action", "Closing door");
    
    servo_close();

    doorState = CLOSING;
}

static void stopDoor(void)
{
    emitEvent("door_action", "Door stopped");

    doorState = STOPPED;
}

static void forceOpenDoor(void)
{
    emitEvent("door_action", "Force opening door");

    servo_open();
    
    doorState = OPENING;
}

static void handleImpact(void)
{
    if (doorState == CLOSING) {
        emitEvent("safety_stop", "Impact detected while closing. Reopening door.");
        forceOpenDoor();
    } else {
        emitEvent("safety_stop", "Impact detected, stopping door.");
        stopDoor();
    }
}

// MAIN CONTROL TASK
static void door_task(void *arg)
{
    while (1) {

        // ---- READ ACCELEROMETER ----
        float accel = readAccelerometer();
        float delta = fabsf(accel - baselineAccel);

        if (delta >= impactThreshold) {
            emitEvent("impact", "Impact detected");
            handleImpact();
        }

        // ---- READ ULTRASONIC ----
        int distance = readDistance();
        if (distance <= distanceToOpen) {
            openDoor();
            lastPresenceTime = millis();
        }

        // ---- CHECK FOR CLOSING ----
        if (doorState == OPENING &&
            (millis() - lastPresenceTime > timeToWaitForClose))
        {
            closeDoor();
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void app_main(void)
{
    ESP_LOGI("SYSTEM", "Automatic door system started.");

    servo_init(4); 
    ultrasonico_init();
    acelerometro_init();

    calibrate_accelerometer();

    xTaskCreate(door_task, "door_task", 4096, NULL, 5, NULL);
}
