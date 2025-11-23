#include "app_web.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main(void) {
    app_web_init();
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
