#include "keyboard.h"

bool Keyboard::begin()
{
    if (!tca8418.begin(TCA8418_ADDR, &Wire)) {
        Serial.println("Keyboard initialization failed!");
        return false;
    } else {
        Serial.println("Keyboard initialized successfully.");
    }

    tca8418.matrix(KEY_ROWS, KEY_COLS); // 配置为4x4矩阵键盘
    tca8418.flush();  // 清空中断

    return true;
}

void Keyboard::readKey()
{
    if (tca8418.available() > 0) {        
        //  datasheet page 15 - Table 1
        int k = tca8418.getEvent();
        bool pressed = k & 0x80;
        k &= 0x7F;
        k--;
        uint8_t row = k / 10;
        uint8_t col = k % 10;

        if (pressed) 
            Serial.print("PRESS\tR: ");
        else 
            Serial.print("RELEASE\tR: ");
        Serial.print(row);
        Serial.print("\tC: ");
        Serial.print(col);
        printf("  %c\n", keymap[row][col]);
    }
}