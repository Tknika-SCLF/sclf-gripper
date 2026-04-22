#pragma once

#include <Arduino.h>

// Forward declaration
class MotorController;

/**
 * @brief Estructura de configuración para persistencia en Flash.
 */
struct DeviceConfig {
    uint32_t magic;           // Valor mágico para validar la estructura (ej. 0x5C1F)
    uint32_t version;         // Versión de la estructura
    
    uint8_t  rs485_id;        // ID del dispositivo en el bus
    uint8_t  reserved1;
    uint16_t reserved2;
    
    // Parámetros FOC
    float zero_electric_angle;
    int32_t sensor_direction; // 1 o -1
    
    // PIDs de Velocidad
    float vel_kp;
    float vel_ki;
    float vel_kd;
    
    // PIDs de Posición
    float pos_kp;
    
    // Límites
    float voltage_limit;
    float current_limit;
    float velocity_limit;
    
    uint32_t crc;             // Checksum para validar integridad
};

class FlashConfig {
public:
    FlashConfig();

    /**
     * @brief Carga la configuración desde la Flash.
     * @return true si los datos son válidos (magic + crc OK).
     */
    bool load();

    /**
     * @brief Guarda la configuración actual en la Flash.
     */
    bool save(const DeviceConfig& config);

    /**
     * @brief Devuelve la configuración cargada.
     */
    const DeviceConfig& getConfig() const { return _currentConfig; }

    /**
     * @brief Aplica la configuración cargada a una instancia de MotorController.
     */
    void applyTo(MotorController& mc);

    /**
     * @brief Actualiza la estructura interna con los valores actuales del MotorController.
     */
    void updateFrom(MotorController& mc);

    /**
     * @brief Carga los valores por defecto.
     */
    void resetToDefaults();

    // Dirección segura en el Banco 1 (última página de los primeros 256KB)
    // Evita problemas de configuración Dual-Bank. 0x08000000 + 256KB - 2KB = 0x0803F800
    static constexpr uint32_t FLASH_ADDR = 0x0803F800;

private:
    DeviceConfig _currentConfig;
    uint32_t calculateCRC(const DeviceConfig& config);
};
