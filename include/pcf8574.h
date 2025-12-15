#include <Adafruit_PCF8574.h>
#ifndef PCF8574_H
#define PCF8574_H

#define PCF8574_ADDR 0x21 // 根据A0、A1、A2引脚连接情况设置地址

#define BUZZER_PIN  4 // PCF8574上的引脚0连接蜂鸣器
#define BAT_IN_PIN  3 // PCF8574上的引脚1连接电池电压检测
#define LCD_BL_PIN  5 // PCF8574上的引脚2连接LCD背光控制

extern Adafruit_PCF8574 pcf;

void PCF8574_Init(Adafruit_PCF8574 &pcf);

#endif //PCF8574_H
