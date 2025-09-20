#include <Arduino.h>
#include "io_def.h"

class Buzzer {
public:
    Buzzer() {

    }

    void begin() {
        pinMode(BUZZER_PIN, OUTPUT);
        digitalWrite(BUZZER_PIN, HIGH); // 初始确保蜂鸣器关闭
    }

    void on() {
        digitalWrite(BUZZER_PIN, LOW);
    }

    void off() {
        digitalWrite(BUZZER_PIN, HIGH);
    }
    
    void beep(uint32_t duration_ms) {
        on();
        delay(duration_ms);
        off();
    }

private:
    // 定义“滴”和“停”的持续时间（毫秒）
    const int beepDuration = 200; // 单次“滴”声的时长
    const int shortPauseDuration = 200; // 两次“滴”之间的短间隔
    const int longPauseDuration = 500; // 完成一次“滴滴”后的长间隔

    int beepState = 0; // 用于状态机状态切换
};