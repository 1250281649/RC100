#include <TFT_eSPI.h>
#include "io_def.h"
#include "lvgl.h"

#define BATTERY_WIDTH         40      // 电池图标总宽度
#define BATTERY_HEIGHT        20      // 电池图标总高度
#define BATTERY_BODY_WIDTH    34      // 电池主体宽度
#define BATTERY_KNOB_WIDTH    4       // 电池正极宽度
#define BATTERY_KNOB_HEIGHT   10      // 电池正极高度
#define BATTERY_PADDING       2       // 电池内部填充边距

#define BG_COLOR              lv_color_hex(0x87CEEB)  // 天空蓝
#define BATTERY_OUTLINE_COLOR lv_color_hex(0xEFEFEF)  // 电池外框白色
#define BATTERY_FULL_COLOR    lv_palette_main(LV_PALETTE_GREEN)  // 电量充足绿色
#define BATTERY_LOW_COLOR     lv_palette_main(LV_PALETTE_RED)  // 低电量红色
#define BATTERY_WARN_COLOR    lv_palette_main(LV_PALETTE_YELLOW)  // 电量警告黄色
#define BATTERY_LABEL_COLOR   BATTERY_OUTLINE_COLOR

void LCDInit();

class LCDScreen {
public:
    LCDScreen() {
        
    }

    void begin() {
        LCDInit();
        batteryIndicator(screen, 10, 10); /* 初始电量100% */
    }

    void clearScreen();

    void batteryIndicator(lv_obj_t *parent, lv_coord_t x, lv_coord_t y);
    void update_battery_display(uint8_t level);

private:
    lv_obj_t* screen = lv_scr_act(); /* 获取当前活跃的屏幕对象 */
    /* 全局对象指针 */
    lv_obj_t *battery_outline;   // 电池外框
    lv_obj_t *battery_knob;      // 电池正极
    lv_obj_t *battery_level;     // 电量填充条
    lv_obj_t *battery_label;     // 电量百分比标签

    lv_style_t batteryStyle;
    lv_style_t knobStyle;
    lv_style_t levelStyle;
    lv_style_t lableStyle;
};
