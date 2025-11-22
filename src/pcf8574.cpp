#include "pcf8574.h"

Adafruit_PCF8574 pcf;

void PCF8574_Init(Adafruit_PCF8574 &pcf) {
    if (!pcf.begin(PCF8574_ADDR)) {
        Serial.println("PCF8574 初始化失败！");
    }
    Serial.println("PCF8574 初始化成功！");
    
    // 设置引脚模式
    pcf.pinMode(BUZZER_PIN, OUTPUT);
    pcf.pinMode(BAT_IN_PIN, INPUT);
    
    // 初始状态
    pcf.digitalWrite(BUZZER_PIN, HIGH); // 关闭蜂鸣器
}
