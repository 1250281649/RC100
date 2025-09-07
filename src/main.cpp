#include <Arduino.h>
#include <driver/adc.h>
#include <esp_adc_cal.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include <TFT_eSPI.h>
#include "lvgl.h"
#include "lv_port_disp.h"

#define LED_PIN                     GPIO_NUM_22
#define LCD_BL                      GPIO_NUM_4

// 定义4个ADC通道
#define NUM_CHANNELS 4
const adc1_channel_t channels[NUM_CHANNELS] = {
    ADC1_CHANNEL_0, // GPIO36
    ADC1_CHANNEL_3, // GPIO39
    ADC1_CHANNEL_4, // GPIO32
    ADC1_CHANNEL_5  // GPIO33
};


// 全局变量
esp_adc_cal_characteristics_t *adc_chars;
QueueHandle_t adc_queue;

// static const uint16_t screenWidth = 240;
// static const uint16_t screenHeight = 240;
// static const uint32_t DISP_BUF_SIZE = screenHeight * screenWidth;
// TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight); /* TFT instance */
TFT_eSPI tft = TFT_eSPI();

// ADC任务
void adc_task(void *pvParameters) {
    uint32_t adc_values[NUM_CHANNELS];
    
    while(1) {
        // 读取所有通道
        for (int i = 0; i < NUM_CHANNELS; i++) {
            // 每次采样8次求平均
            adc_values[i] = 0;
            for (int j = 0; j < 8; j++) {
                adc_values[i] += adc1_get_raw(channels[i]);
            }
            adc_values[i] /= 8;
        }
        
        // 发送到队列
        xQueueSend(adc_queue, adc_values, portMAX_DELAY);
        vTaskDelay(pdMS_TO_TICKS(100)); // 每50ms采样一次
    }
}

// 数据处理任务
void data_task(void *pvParameters) {
    uint32_t adc_values[NUM_CHANNELS];
    char channel_names[NUM_CHANNELS][10] = {"L-X", "L-Y", "R-X", "R-Y"};
    
    while(1) {
        if (xQueueReceive(adc_queue, adc_values, portMAX_DELAY)) {
            printf("\n=== ADC读数 ===\n");
            for (int i = 0; i < NUM_CHANNELS; i++) {
                uint32_t voltage = esp_adc_cal_raw_to_voltage(adc_values[i], adc_chars);
                printf("%s: %dmV (原始值: %d)\n", channel_names[i], voltage, adc_values[i]);
            }
        }
    }
}

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
    Serial.println("RC Controller");

    tft.begin();
    lv_init();  //初始化lvgl库
    lv_port_disp_init();
    
    gpio_reset_pin(LCD_BL);
    gpio_set_direction(LCD_BL, GPIO_MODE_OUTPUT);
    gpio_set_level(LCD_BL, 1);

    // 获取当前活跃的屏幕对象
    lv_obj_t * scr = lv_scr_act();
    // 设置屏幕背景为白色
    lv_obj_set_style_bg_color(scr, lv_color_hex(0xFFFFFF), LV_PART_MAIN); // 设置背景颜色为白色
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, LV_PART_MAIN);             // 设置背景完全不透明

    lv_obj_t* label = lv_label_create(scr);
    lv_label_set_text(label, "RC Controller");
    lv_obj_set_style_text_color(label, lv_color_hex(0x000F0F), LV_PART_MAIN); // 设置文本颜色为黑色
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 100);


    // 初始化ADC
    adc1_config_width(ADC_WIDTH_BIT_12);
    for (int i = 0; i < NUM_CHANNELS; i++) {
        adc1_config_channel_atten(channels[i], ADC_ATTEN_DB_11);
    }

    // 初始化校准
    adc_chars = (esp_adc_cal_characteristics_t*)calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, adc_chars);

    // 创建队列
    adc_queue = xQueueCreate(10, sizeof(uint32_t[NUM_CHANNELS]));

    xTaskCreate(vTaskBlink, "Blink_Task",
        2048, //任务栈大小
        NULL, //任务参数指针
        1, //任务优先级
        NULL //任务任务句柄指针
    );

    // 创建任务
    xTaskCreate(adc_task, "ADC Task", 2048, NULL, 5, NULL);
    xTaskCreate(data_task, "Data Task", 2048, NULL, 4, NULL);

    Serial.println("ADC多通道采集系统已启动...\n");
}


void loop() {
    lv_task_handler();
    vTaskDelay(100);
}
