#include "I2CDevice.h"

bool I2CDevice::begin() {
    // Arduino模式初始化
    Wire.begin(sdaPin, sclPin);
    Wire.setClock(clockFreq);
    return scanDevice();
}

bool I2CDevice::scanDevice(uint8_t targetAddress) {
    deviceDetected = false;
    Wire.beginTransmission(targetAddress);
    uint8_t error = Wire.endTransmission();
    if (error == 0) {
        deviceDetected = true;
        return true;
    } else if (error == 4) {
        lastError = ESP_ERR_TIMEOUT;
    } else {
        lastError = ESP_ERR_INVALID_STATE;
    }
    return deviceDetected;
}

bool I2CDevice::scanDevice() {
    return this->scanDevice(address);
}

std::vector<uint8_t> I2CDevice::scanAllDevices() {
    std::vector<uint8_t> foundDevices;
    for (uint8_t addr = 1; addr < 127; addr++) {
        Wire.beginTransmission(addr);
        if (Wire.endTransmission() == 0) {
            foundDevices.push_back(addr);
        }
    }
    return foundDevices;
}