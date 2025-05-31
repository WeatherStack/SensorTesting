#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

extern void setup();
extern void loop();
#ifdef __cplusplus
extern "C"
#endif
void app_main() {
    setup();
    // loop task
    uint32_t ts = pdTICKS_TO_MS(xTaskGetTickCount());
    while (1) {
        loop();
        uint32_t ms = pdTICKS_TO_MS(xTaskGetTickCount());
        if (ms > ts + 200) {
            ms = pdTICKS_TO_MS(xTaskGetTickCount());
            vTaskDelay(5);
        }
    }
}