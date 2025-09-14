#include "lcd.h"
#include <TFT_eSPI.h>
#include "lvgl.h"
#include "lv_port_disp.h"

TFT_eSPI tft = TFT_eSPI();


void LCDInit() {
    tft.begin();
    lv_init();  //初始化lvgl库
    lv_port_disp_init();
    LV_FONT_DECLARE(lv_font_simsun_16_cjk); // 声明字体
    
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
    lv_obj_set_pos(battery_outline, x + BATTERY_LABEL_WIDTH, y);
    
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
    lv_obj_set_pos(battery_knob, x + BATTERY_LABEL_WIDTH + BATTERY_BODY_WIDTH, y + (BATTERY_HEIGHT - BATTERY_KNOB_HEIGHT) / 2);
    lv_style_init(&knobStyle);
    lv_style_set_radius(&knobStyle, 0);
    lv_style_set_bg_color(&knobStyle, BATTERY_OUTLINE_COLOR);
    lv_style_set_border_width(&knobStyle, 0);
    lv_obj_add_style(battery_knob, &knobStyle, LV_PART_MAIN);

    /* 创建电量填充条 */
    battery_level = lv_obj_create(parent);
    lv_obj_set_size(battery_level, BATTERY_BODY_WIDTH - BATTERY_PADDING * 2, 
                    BATTERY_HEIGHT - BATTERY_PADDING * 2);
    lv_obj_set_pos(battery_level, x + BATTERY_LABEL_WIDTH + BATTERY_PADDING, y + BATTERY_PADDING);
    lv_style_init(&levelStyle);
    lv_style_set_radius(&levelStyle, 1);
    lv_style_set_bg_color(&levelStyle, BATTERY_FULL_COLOR);
    lv_style_set_border_width(&levelStyle, 0);
    lv_obj_add_style(battery_level, &levelStyle, LV_PART_MAIN);

    /* 创建电量百分比标签 */
    battery_percent = lv_label_create(parent);
    lv_obj_set_pos(battery_percent, x + BATTERY_LABEL_WIDTH + 1, y + 2);
    lv_label_set_text(battery_percent, "100%");
    lv_obj_set_style_text_color(battery_percent, BATTERY_LABEL_COLOR, LV_PART_MAIN);

    battery_label = lv_label_create(parent);
    lv_obj_set_pos(battery_label, x, y);
    lv_label_set_text(battery_label, "电量");
    lv_obj_set_style_text_color(battery_label, BATTERY_LABEL_COLOR, LV_PART_MAIN);
    lv_obj_set_style_text_font(battery_label, &lv_font_simsun_16_cjk, LV_STATE_DEFAULT);
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
    lv_label_set_text(battery_percent, buf);
}

/**
 * @brief 创建信号强度图标
 * @param parent 父对象（例如状态栏容器）
 * @param x 图标在父对象中的x坐标
 * @param y 图标在父对象中的y坐标
 */
void LCDScreen::signalIconCreate(lv_obj_t *parent, lv_coord_t x, lv_coord_t y)
{
    lv_coord_t start_x = x + int16_t((SIGNAL_ICON_WIDTH - (SIGNAL_BAR_WIDTH * 4 + SIGNAL_BAR_SPACING * 3)) / 2.0);

    /* 创建4个竖条，从下往上，高度递增 */
    for (int16_t i = 0; i < 4; i++) {
        signal_bars[i] = lv_obj_create(parent);
        int16_t bar_height = SIGNAL_BAR_HEIGHT * (i + 1);
        lv_obj_set_size(signal_bars[i], SIGNAL_BAR_WIDTH, bar_height); // 高度递增
        /* 对齐：底部对齐，x坐标依次增加 */
        lv_obj_set_pos(signal_bars[i], start_x + i * (SIGNAL_BAR_WIDTH + SIGNAL_BAR_SPACING), y + (SIGNAL_ICON_WIDTH - bar_height));
        lv_obj_set_style_radius(signal_bars[i], 0, LV_PART_MAIN); // 矩形直角
        lv_obj_set_style_border_width(signal_bars[i], 0, LV_PART_MAIN); // 无边框
        /* 初始状态：全部置为无信号颜色 */
        lv_obj_set_style_bg_color(signal_bars[i], COLOR_NO_SIGNAL, LV_PART_MAIN);
    }
}

void LCDScreen::signalIconUpdate(int strength)
{
    /* strength范围：0-4 */
    if(strength < 0) strength = 0;
    if(strength > 4) strength = 4;

    /* 更新每个竖条的颜色 */
    for (int i = 0; i < 4; i++) {
        if(i < strength) {
            /* 根据信号强度设置颜色 */
            if(strength == 1) {
                lv_obj_set_style_bg_color(signal_bars[i], COLOR_WEAK_SIGNAL, LV_PART_MAIN);
            } else if(strength == 2) {
                lv_obj_set_style_bg_color(signal_bars[i], COLOR_MEDIUM_SIGNAL, LV_PART_MAIN);
            } else {
                lv_obj_set_style_bg_color(signal_bars[i], COLOR_STRONG_SIGNAL, LV_PART_MAIN);
            }
        } else {
            lv_obj_set_style_bg_color(signal_bars[i], COLOR_NO_SIGNAL, LV_PART_MAIN);
        }
    }
}
