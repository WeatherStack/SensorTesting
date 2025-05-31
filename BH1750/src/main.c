#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "bh1750.h"
#include "i2c.h"

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

extern bh1750_handle_t sensor;

extern void setup() {
    // Initialize I2C
    i2c_master_init();
    // Create BH1750 sensor instance
    sensor = bh1750_create(I2C_PORT, BH1750_I2C_ADDRESS_DEFAULT);
    
    // Attempt to power on the sensor
    esp_err_t ret = bh1750_power_on(sensor);
    if (ret != ESP_OK) {
        printf("Failed to power on BH1750 sensor: %d\n", ret);
        return;
    }

    // Set the sensor mode
    bh1750_set_measure_mode(sensor, BH1750_CONTINUE_HALFLX_RES);
}

extern void loop() {
    // Delay by 1 second (1000 ms)
    vTaskDelay(pdMS_TO_TICKS(1000));
    // Fetch the data from the sensor
    const float* data;
    bh1750_get_data(sensor, data);
    // Print the data from the sensor
    printf("Light: %d lux\n", data);
}