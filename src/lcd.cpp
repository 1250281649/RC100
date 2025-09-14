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

void LCDScreen::clearScreen() {
    tft.fillScreen(TFT_BLACK);
}

void LCDScreen::batteryIndicator(lv_obj_t *parent, lv_coord_t x, lv_coord_t y) {
     /* 创建电池外框 */
    battery_outline = lv_obj_create(parent);
    lv_obj_remove_style_all(battery_outline); // 移除所有默认样式
    lv_obj_set_size(battery_outline, BATTERY_BODY_WIDTH, BATTERY_HEIGHT);
    lv_obj_set_pos(battery_outline, x, y);
    
    lv_style_init(&batteryStyle);
    lv_style_set_border_color(&batteryStyle, BATTERY_OUTLINE_COLOR);
    lv_style_set_border_width(&batteryStyle, 2);
    lv_style_set_radius(&batteryStyle, 0);
    // lv_style_set_bg_color(&batteryStyle, BATTERY_OUTLINE_COLOR);
    lv_style_set_bg_opa(&batteryStyle, LV_OPA_TRANSP);    // 设置背景透明
    lv_obj_add_style(battery_outline, &batteryStyle, LV_PART_MAIN);

    /* 创建电池正极 */
    battery_knob = lv_obj_create(parent);
    lv_obj_set_size(battery_knob, BATTERY_KNOB_WIDTH, BATTERY_KNOB_HEIGHT);
    lv_obj_set_pos(battery_knob, x + BATTERY_BODY_WIDTH, y + (BATTERY_HEIGHT - BATTERY_KNOB_HEIGHT) / 2);
    lv_style_init(&knobStyle);
    lv_style_set_radius(&knobStyle, 1);
    lv_style_set_bg_color(&knobStyle, BATTERY_OUTLINE_COLOR);
    lv_style_set_border_width(&knobStyle, 0);
    lv_obj_add_style(battery_knob, &knobStyle, LV_PART_MAIN);

    /* 创建电量填充条 */
    battery_level = lv_obj_create(parent);
    lv_obj_set_size(battery_level, BATTERY_BODY_WIDTH - BATTERY_PADDING * 2, 
                    BATTERY_HEIGHT - BATTERY_PADDING * 2);
    lv_obj_set_pos(battery_level, x + BATTERY_PADDING, y + BATTERY_PADDING);
    lv_style_init(&levelStyle);
    lv_style_set_radius(&levelStyle, 1);
    lv_style_set_bg_color(&levelStyle, BATTERY_FULL_COLOR);
    lv_style_set_border_width(&levelStyle, 0);
    lv_obj_add_style(battery_level, &levelStyle, LV_PART_MAIN);

    /* 创建电量百分比标签 */
    battery_label = lv_label_create(parent);
    lv_obj_set_pos(battery_label, x + 1, y + 2);
    lv_label_set_text(battery_label, "100%");
    lv_obj_set_style_text_color(battery_label, BATTERY_LABEL_COLOR, LV_PART_MAIN);
}

/**
 * @brief 更新电池电量显示
 * @param level 电量百分比 (0-100)
 */
void LCDScreen::update_battery_display(uint8_t level)
{
    /* 确保电量在0-100范围内 */
    if(level > 100) level = 100;
    
    /* 计算填充条的新宽度 */
    lv_coord_t max_width = BATTERY_BODY_WIDTH - BATTERY_PADDING * 2;
    lv_coord_t new_width = (max_width * level) / 100;
    lv_obj_set_width(battery_level, new_width);
    
    /* 根据电量改变颜色 */
    if(level > 50) {
        lv_obj_set_style_bg_color(battery_level, BATTERY_FULL_COLOR, LV_PART_MAIN);
    } else if(level > 20) {
        lv_obj_set_style_bg_color(battery_level, BATTERY_WARN_COLOR, LV_PART_MAIN);
    } else {
        lv_obj_set_style_bg_color(battery_level, BATTERY_LOW_COLOR, LV_PART_MAIN);
    }
    
    /* 更新百分比文本 */
    static char buf[8];
    lv_snprintf(buf, sizeof(buf), "%d%%", level);
    lv_label_set_text(battery_label, buf);
}
