#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "ultrasonico.h"

static const char *TAG = "DOOR_SYSTEM";

static bool doorOpen = false;

static const int distanceToOpen = 30;             // 30 cm
static const uint64_t timeToWaitForClose = 3000;  // 3 seconds

static uint64_t lastPresenceTime = 0;

uint64_t millis()
{
    return (uint64_t)xTaskGetTickCount() * portTICK_PERIOD_MS;
}

// ---------------------------------------------------------
// Función para leer distancia real del sensor ultrasónico
// ---------------------------------------------------------
static int readDistance()
{
    float pulse_us = ultrasonico_medicion_raw();
    if (pulse_us == 0) return 9999;  // no detection

    float distance_cm = pulse_us * 0.01715f;
    return (int)distance_cm;
}

// ---------------------------------------------------------
// Abrir puerta
// ---------------------------------------------------------
static void openDoor()
{
    if (!doorOpen) {
        ESP_LOGI(TAG, "Opening door...");
        // TODO: Código real para mover el servo
        doorOpen = true;
    }
}

// ---------------------------------------------------------
// Cerrar puerta
// ---------------------------------------------------------
static void closeDoor()
{
    if (doorOpen) {
        ESP_LOGI(TAG, "Closing door...");
        // TODO: Código real para mover el servo
        doorOpen = false;
    }
}

// ---------------------------------------------------------
// Lógica principal
// ---------------------------------------------------------
static void door_task(void *arg)
{
    ESP_LOGI(TAG, "Automatic door system started.");

    while (1) {

        int distance = readDistance();

        // Si hay presencia, abrir puerta
        if (distance <= distanceToOpen) {
            openDoor();
            lastPresenceTime = millis(); // ms
        }

        // Si no hay presencia en un tiempo, cerrar
        uint64_t now = millis(); // ms
        if (doorOpen && (now - lastPresenceTime > timeToWaitForClose)) {
            closeDoor();
        }

        vTaskDelay(pdMS_TO_TICKS(500));  // delay(500)
    }
}

void app_main(void)
{
    ultrasonico_init();
    xTaskCreate(door_task, "door_task", 4096, NULL, 5, NULL);

    // while (1) {
    //     ultrasonico_medicion();
    //     vTaskDelay(pdMS_TO_TICKS(500));
    // }
}
