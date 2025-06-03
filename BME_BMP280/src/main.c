#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
// Sensor header
#include "i2c.h"
#include "driver/gpio.h"

#define LED_PIN     GPIO_NUM_10

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

// Sensor definition
static int fail = 0;
static int fails = 0;

extern void setup() {
    if(gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT) != ESP_OK) {
        printf("GPIO PIN %d failed to turn to direction %d\n", LED_PIN, GPIO_MODE_OUTPUT);
    } else {
        printf("GPIO PIN %d successfully turned to direction %d\n", LED_PIN, GPIO_MODE_OUTPUT);
    }
    if(gpio_set_level(LED_PIN, 1) != ESP_OK) {
        printf("GPIO PIN %d failed to set level to %d\n", LED_PIN, 1);
    } else {
        printf("GPIO PIN %d successfully set level to %d\n", LED_PIN, 1);
    }

    // Initialize I2C
    i2c_master_init();

    // Sensor initialisation
}

static int on = 0;

extern void loop() {
    // Delay by 1 second (1000 ms)
    vTaskDelay(pdMS_TO_TICKS(1000));
    if(on == 0) {
        on = 1;
        gpio_set_level(LED_PIN, 0);
    } else {
        on = 0;
        gpio_set_level(LED_PIN, 1);
    }
    if(fail == 1 || fails > 3) return;
    
    // Fetch data from sensor
}