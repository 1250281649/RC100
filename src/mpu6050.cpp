#include "mpu6050.h"

MPU6050 mpu(Wire);

void setupSensor() {
    if(mpu.begin()) {
        Serial.println("Failed to find MPU6050 chip");  
        while (1) {
            vTaskDelay(pdMS_TO_TICKS(100));
        }
    }
    printf("MPU6050 初始化成功!\n");

    // mpu.calcAccOffsets(); // 计算并设置加速度计和陀螺仪偏移
}
