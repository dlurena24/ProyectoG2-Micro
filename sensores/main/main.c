#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "ultrasonico.h"

void app_main(void)
{
    ultrasonico_init();

    while (1) {
        ultrasonico_medicion();
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
