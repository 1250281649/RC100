#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define LED_PIN                     GPIO_NUM_22

void vTaskBlink(void *pvParameters) {
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);

    while (1) {
        gpio_set_level(LED_PIN, 1);
        vTaskDelay(pdMS_TO_TICKS(500));
        gpio_set_level(LED_PIN, 0);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void setup()
{
    Serial.begin(115200);
    Serial.println("RM Mini Car");
    xTaskCreate(vTaskBlink, "Blink_Task",
        2048, //任务栈大小
        NULL, //任务参数指针
        1, //任务优先级
        NULL //任务任务句柄指针
    );
}


void loop() {
    vTaskDelay(1000);
}
