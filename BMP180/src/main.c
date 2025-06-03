#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "bmp180.h"
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

static bmp180_t sensor;
static int fail = 0;
static int fails = 0;

void setup() {
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

    // Create BMP180 sensor instance
    2c_lowlevel_config config;
    config.port = I2C_NUM_0;
    config.pin_sda = GPIO_NUM_21;
    config.pin_scl = GPIO_NUM_22;
    &sensor = bmp180_init(&config, DEVICE_I2C_ADDRESS, BMP180_MODE_HIGH_RESOLUTION);
}

static int on = 0;

void loop() {
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

    float temperature;
    uint32_t pressure; 
    if(bmp180_measure(ctx, &temperature, &pressure)) {
        printf("Temperature: %.2f Celsius, Pressure: %u hPA", temperature, pressure);
    } else {
        printf("Failed to fetch data from BMP180 sensor")
        ++fails;
        return;
    }
}