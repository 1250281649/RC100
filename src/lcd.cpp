#include "lcd.h"
#include <TFT_eSPI.h>
#include "lvgl.h"
#include "lv_port_disp.h"

TFT_eSPI tft = TFT_eSPI();


void LCDInit() {
    tft.begin();
    lv_init();  //初始化lvgl库
    lv_port_disp_init();
    
    gpio_reset_pin(LCD_BL);
    gpio_set_direction(LCD_BL, GPIO_MODE_OUTPUT);
    gpio_set_level(LCD_BL, 1);
}
