#ifndef I2CDEVICE_H
#define I2CDEVICE_H

#include <Wire.h>
#include <vector>
#include "io_def.h"

class I2CDevice {
public:
    I2CDevice(uint8_t targetAddr = 0x00, 
            gpio_num_t sda = SDA_PIN,
            gpio_num_t scl = SCL_PIN,
            uint32_t freq = 400000)
    {
        address = targetAddr;
        sdaPin = sda;
        sclPin = scl;
        clockFreq = freq;
        deviceDetected = false;
        lastError = ESP_OK;
    }

    bool begin();

    bool writeRegister(uint8_t reg, uint8_t value) {
        Wire.beginTransmission(address);
        Wire.write(reg);
        Wire.write(value);
        return Wire.endTransmission() == 0; // 返回是否成功
    }

    bool writeRegister(uint8_t reg, const uint8_t *data, size_t len) {
        Wire.beginTransmission(address);
        Wire.write(reg);
        Wire.write(data, len);
        return Wire.endTransmission() == 0; // 返回是否成功
    }

    bool writeRegister(uint8_t reg, const std::vector<uint8_t> data) {
        return writeRegister(reg, data.data(), data.size());
    }

    bool readRegister(uint8_t reg, uint8_t *value, size_t len = 1) {
        Wire.beginTransmission(address);
        Wire.write(reg);
        if (Wire.endTransmission(false) != 0) { // 重启总线
            return false; // 传输失败
        }
        return Wire.requestFrom(address, len) ==  len;
    }

    esp_err_t getLastError() const {
        return lastError;
    }

    bool scanDevice(uint8_t targetAddress);
    bool scanDevice();

    std::vector<uint8_t> scanAllDevices();  // 扫描总线上所有设备

private:
    uint8_t address;          // 目标设备地址（7位格式）
    gpio_num_t sdaPin;        // SDA引脚
    gpio_num_t sclPin;        // SCL引脚
    uint32_t clockFreq;       // 总线频率
    bool deviceDetected = false;
    esp_err_t lastError = ESP_OK;

};

#endif // I2CDEVICE_H