#include <Adafruit_TCA8418.h>
#include <Wire.h>

#define TCA8418_ADDR            0x34    // 默认I2C地址（左移1位后为0x68）

class Keyboard {
public:
    Keyboard() {

    }

    // 初始化键盘
    bool begin();

    void readKey();

private:
    Adafruit_TCA8418 tca8418;
};