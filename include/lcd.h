#include <TFT_eSPI.h>
#include "io_def.h"
#include "lvgl.h"
#include "ui.h"

#define SCREEN_WIDTH            240
#define SCREEN_HEIGHT           240

#define BATTERY_WIDTH           40      // 电池图标总宽度
#define BATTERY_HEIGHT          20      // 电池图标总高度
#define BATTERY_BODY_WIDTH      34      // 电池主体宽度
#define BATTERY_KNOB_WIDTH      4       // 电池正极宽度
#define BATTERY_KNOB_HEIGHT     10      // 电池正极高度
#define BATTERY_PADDING         2       // 电池内部填充边距

#define BG_COLOR                lv_color_hex(0x87CEEB)  // 天空蓝
#define BATTERY_OUTLINE_COLOR   lv_palette_main(LV_PALETTE_CYAN)
#define BATTERY_FULL_COLOR      lv_palette_main(LV_PALETTE_GREEN)  // 电量充足绿色
#define BATTERY_LOW_COLOR       lv_palette_main(LV_PALETTE_RED)  // 低电量红色
#define BATTERY_WARN_COLOR      lv_palette_main(LV_PALETTE_YELLOW)  // 电量警告黄色
#define BATTERY_LABEL_COLOR     BATTERY_OUTLINE_COLOR

#define SIGNAL_ICON_WIDTH       20  // 信号图标宽度
#define SIGNAL_BAR_WIDTH        3   // 每个竖条的宽度
#define SIGNAL_BAR_SPACING      2   // 信号条间距
#define SIGNAL_BAR_HEIGHT       4  // 信号图标高度

#define COLOR_NO_SIGNAL         lv_color_hex(0X808080)  // 灰色
#define COLOR_WEAK_SIGNAL       lv_color_hex(0xFF0000)  // 红色
#define COLOR_MEDIUM_SIGNAL     lv_color_hex(0xFFFF00)  // 黄色
#define COLOR_STRONG_SIGNAL     lv_color_hex(0x00FF00)  // 绿色

void LCDInit();

class LCDScreen {
public:
    LCDScreen() {
        
    }

    void begin() {
        LCDInit();
        ui_init();

        ScreenExcavatorInit();
    }

    void clearScreen();

    void batteryIndicator(lv_obj_t *parent, lv_coord_t x, lv_coord_t y);
    void update_battery_display(uint8_t level);

    void signalIconCreate(lv_obj_t *parent, lv_coord_t x, lv_coord_t y);
    void signalIconUpdate(int strength);

    void appLogo(const char* text, lv_coord_t x, lv_coord_t y, lv_color_t color);
    void statusBarCreate(lv_obj_t *parent, lv_coord_t x, lv_coord_t y);

    void ScreenExcavatorInit();

private:
    lv_obj_t* screen = lv_scr_act(); /* 获取当前活跃的屏幕对象 */
    /* 全局对象指针 */
    lv_obj_t *battery_outline;   // 电池外框
    lv_obj_t *battery_knob;      // 电池正极
    lv_obj_t *battery_level;     // 电量填充条
    lv_obj_t *battery_percent;     // 电量百分比标签
    lv_obj_t *battery_label;
    const int BATTERY_LABEL_WIDTH = 32;

    lv_style_t batteryStyle;
    lv_style_t knobStyle;
    lv_style_t levelStyle;
    lv_style_t lableStyle;

    lv_obj_t *signal_bars[4]; // 信号强度竖条
    lv_obj_t *status_bar_outline;
    lv_style_t status_bar_style;
};
