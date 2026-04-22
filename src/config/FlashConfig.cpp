#include "config/FlashConfig.h"
#include "motor/MotorController.h"
#include <stm32g4xx_hal.h>

FlashConfig::FlashConfig() {
    resetToDefaults();
}

void FlashConfig::resetToDefaults() {
    _currentConfig.magic = 0x5C1F; // "SCLF"
    _currentConfig.version = 1;
    _currentConfig.rs485_id = 1;
    _currentConfig.zero_electric_angle = 0.0f;
    _currentConfig.sensor_direction = 1;
    _currentConfig.vel_kp = 0.1f;
    _currentConfig.vel_ki = 2.0f;
    _currentConfig.vel_kd = 0.0f;
    _currentConfig.pos_kp = 20.0f;
    _currentConfig.voltage_limit = 2.0f; // Seguro para pruebas
    _currentConfig.current_limit = 1.0f;
    _currentConfig.velocity_limit = 20.0f;
    _currentConfig.crc = 0;
}

bool FlashConfig::load() {
    DeviceConfig temp;
    memcpy(&temp, (void*)FLASH_ADDR, sizeof(DeviceConfig));

    if (temp.magic != 0x5C1F) return false;

    uint32_t computedCrc = calculateCRC(temp);
    if (computedCrc != temp.crc) return false;

    _currentConfig = temp;
    return true;
}

bool FlashConfig::save(const DeviceConfig& config) {
    DeviceConfig toSave = config;
    toSave.crc = calculateCRC(toSave);

    HAL_FLASH_Unlock();

    // 1. Borrar la página
    FLASH_EraseInitTypeDef eraseInit;
    eraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
    eraseInit.Banks = FLASH_BANK_1; 
    eraseInit.Page = (FLASH_ADDR - 0x08000000) / FLASH_PAGE_SIZE;
    eraseInit.NbPages = 1;

    uint32_t pageError = 0;
    if (HAL_FLASHEx_Erase(&eraseInit, &pageError) != HAL_OK) {
        uint32_t err = HAL_FLASH_GetError();
        Serial.print("[FLASH] Erase Error: ");
        Serial.println(err);
        HAL_FLASH_Lock();
        return false;
    }

    // 2. Escribir los datos (DoubleWord = 64 bits = 8 bytes)
    uint64_t* dataPtr = (uint64_t*)&toSave;
    uint32_t addr = FLASH_ADDR;
    uint32_t iterations = sizeof(DeviceConfig) / 8;
    if (sizeof(DeviceConfig) % 8 != 0) iterations++;

    for (uint32_t i = 0; i < iterations; i++) {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, addr, dataPtr[i]) != HAL_OK) {
            uint32_t err = HAL_FLASH_GetError();
            Serial.print("[FLASH] Write Error at ");
            Serial.print(addr, HEX);
            Serial.print(": ");
            Serial.println(err);
            HAL_FLASH_Lock();
            return false;
        }
        addr += 8;
    }

    HAL_FLASH_Lock();
    _currentConfig = toSave;
    return true;
}

uint32_t FlashConfig::calculateCRC(const DeviceConfig& config) {
    uint32_t crc = 0;
    uint32_t* ptr = (uint32_t*)&config;
    uint32_t len = (sizeof(DeviceConfig) - sizeof(uint32_t)) / 4;

    for (uint32_t i = 0; i < len; i++) {
        crc ^= ptr[i];
    }
    return crc;
}

void FlashConfig::applyTo(MotorController& mc) {
    BLDCMotor& motor = mc.getMotor();
    
    motor.zero_electric_angle = _currentConfig.zero_electric_angle;
    motor.sensor_direction = (Direction)_currentConfig.sensor_direction;
    
    motor.PID_velocity.P = _currentConfig.vel_kp;
    motor.PID_velocity.I = _currentConfig.vel_ki;
    motor.PID_velocity.D = _currentConfig.vel_kd;
    
    motor.P_angle.P = _currentConfig.pos_kp;
    
    motor.voltage_limit = _currentConfig.voltage_limit;
    motor.current_limit = _currentConfig.current_limit;
    motor.velocity_limit = _currentConfig.velocity_limit;
}

void FlashConfig::updateFrom(MotorController& mc) {
    BLDCMotor& motor = mc.getMotor();
    
    _currentConfig.zero_electric_angle = motor.zero_electric_angle;
    _currentConfig.sensor_direction = (int32_t)motor.sensor_direction;
    
    _currentConfig.vel_kp = motor.PID_velocity.P;
    _currentConfig.vel_ki = motor.PID_velocity.I;
    _currentConfig.vel_kd = motor.PID_velocity.D;
    
    _currentConfig.pos_kp = motor.P_angle.P;
    
    _currentConfig.voltage_limit = motor.voltage_limit;
    _currentConfig.current_limit = motor.current_limit;
    _currentConfig.velocity_limit = motor.velocity_limit;
}
