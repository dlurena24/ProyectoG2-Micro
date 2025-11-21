#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "ultrasonico.h"
#include "acelerometro.h"

// =======================
// DOOR PARAMS
// =======================

static const int distanceToOpen = 30;                 // cm
static const uint64_t timeToWaitForClose = 3000;      // ms
static const float impactThreshold = 2.5f;            // m/s^2

// =======================
// DOOR STATE
// =======================

typedef enum {
    STOPPED,
    OPENING,
    CLOSING
} DoorState;

static DoorState doorState = STOPPED;
static uint64_t lastPresenceTime = 0;

uint64_t millis()
{
    return (uint64_t)xTaskGetTickCount() * portTICK_PERIOD_MS;
}

// =======================
// EVENT SYSTEM
// =======================

static void emitEvent(const char* eventType, const char* message)
{
    ESP_LOGI("EVENT", "{ \"event\": \"%s\", \"message\": \"%s\", \"timestamp\": %llu }",
             eventType, message, millis());
}

// =======================
// SENSOR READING
// =======================

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
    // Reemplazar con lectura real I2C
    float accel = 0.5f;

    char buffer[64];
    snprintf(buffer, sizeof(buffer), "Acceleration: %.2f m/s^2", accel);
    emitEvent("acceleration", buffer);

    return accel;
}

// =======================
// DOOR ACTIONS
// =======================

static void openDoor(void)
{
    if (doorState == OPENING) return;

    emitEvent("door_action", "Opening door");
    // TODO mover servo

    doorState = OPENING;
}

static void closeDoor(void)
{
    if (doorState == CLOSING) return;

    emitEvent("door_action", "Closing door");
    // TODO mover servo

    doorState = CLOSING;
}

static void stopDoor(void)
{
    emitEvent("door_action", "Door stopped");
    // TODO parar motor

    doorState = STOPPED;
}

static void forceOpenDoor(void)
{
    emitEvent("door_action", "Force opening door");
    // TODO mover servo en apertura

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

// =======================
// MAIN CONTROL TASK
// =======================

static void door_task(void *arg)
{
    while (1) {

        // ---- READ ACCELEROMETER ----
        float accel = readAccelerometer();
        if (accel >= impactThreshold) {
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

    ultrasonico_init();

    xTaskCreate(door_task, "door_task", 4096, NULL, 5, NULL);
}
