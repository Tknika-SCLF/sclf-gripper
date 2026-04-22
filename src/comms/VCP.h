#pragma once

#include <Arduino.h>
#include <SimpleFOC.h>
#include "motor/MotorController.h"

/**
 * @brief VCPManager — Gestiona la comunicación serie interactiva vía USB (Virtual COM Port).
 * 
 * Implementa el Commander de SimpleFOC y añade funciones de telemetría personalizada
 * sin bloquear el bucle de control principal.
 */
class VCPManager {
public:
    VCPManager(Stream& serial = Serial);

    /**
     * @brief Configura el Commander y habilita el debug de SimpleFOC.
     */
    void begin();

    /**
     * @brief Registra el motor en el sistema de comandos.
     * @param mc Referencia al controlador del motor.
     */
    void registerMotor(MotorController& mc);

    /**
     * @brief Procesa los comandos entrantes. Debe llamarse en el loop().
     */
    void update();

    /**
     * @brief Activa o desactiva la telemetría periódica.
     */
    void setTelemetry(bool enabled) { _telemetryEnabled = enabled; }

private:
    Stream& _serial;
    Commander _commander;
    MotorController* _mc = nullptr;
    bool _telemetryEnabled = false;
    uint32_t _lastTelemetryTime = 0;

    // Callbacks estáticos para el Commander
    static void _onMotor(char* cmd);
    static void _onTelemetry(char* cmd);
    
    // Puntero estático para poder usar callbacks desde C
    static VCPManager* _instance;
};
