#pragma once
/**
 * FaultManager.h — Gestor de fallos del sistema
 *
 * Responsabilidades:
 *  - Polling del DRV8316 STATUS por SPI cada N ciclos
 *  - Detección de error de encoder (MT6701::isOk() == false)
 *  - Detección de stall (baja velocidad + alta corriente)
 *  - Disparo de safe state en < 1ms (FR-FLT-03)
 *  - Reporte por RS-485 y VCP
 *
 * ⚠️  FASE 6 — Implementar en FaultManager.cpp
 *     El esqueleto está listo para integrar en main.cpp cuando llegue la fase.
 */

#include <Arduino.h>

// Importar forward declarations para no crear dependencias circulares
class MotorController;
class DRV8316;
class MT6701;

enum class FaultCode : uint8_t {
    NONE          = 0,
    DRV_FAULT     = 1,   // DRV8316 reporta fallo en STATUS1/2
    ENCODER_ERROR = 2,   // MT6701 no responde por SPI
    STALL         = 3,   // Motor bloqueado mecánicamente
    OVERCURRENT   = 4,   // Corriente > límite
    OVERVOLTAGE   = 5,   // Bus > límite
};

class FaultManager {
public:
    FaultManager(MotorController& mc);

    /**
     * Llamar en cada iteración de loop().
     * Hace polling del DRV8316 cada DRV_POLL_EVERY ciclos.
     * Si detecta fallo, llama a triggerSafeState().
     */
    void update();

    /**
     * ¿Hay algún fallo activo?
     */
    bool hasFault() const { return _activeFault != FaultCode::NONE; }

    /**
     * Devuelve el fallo activo.
     */
    bool isStalled();

    /**
     * Devuelve el fallo activo.
     */
    FaultCode getActiveFault() const { return _activeFault; }

    /**
     * Deshabilita el motor inmediatamente.
     * ⚠️  Debe ejecutarse en < 1 ms desde la detección (FR-FLT-03).
     * Llamar a motor.disable() desde aquí.
     */
    void triggerSafeState(FaultCode code);

    /**
     * Limpia el fallo y permite reintentar (si el hardware lo permite).
     */
    void clearFault();

    /**
     * Devuelve el nombre del fallo como string (para VCP/RS485).
     */
    static const char* faultName(FaultCode code);

    // Cada cuántos ciclos de loop() se hace polling del DRV8316
    // A 10kHz de loop, 100 = cada 10ms
    static constexpr uint32_t DRV_POLL_EVERY = 100;

    // Umbral de stall: velocidad < STALL_VEL_RAD_S durante > STALL_TIME_MS
    static constexpr float    STALL_VEL_RAD_S  = 0.5f;
    static constexpr uint32_t STALL_TIME_MS    = 1000; // 1 segundo de bloqueo

private:
    MotorController& _mc;

    FaultCode _activeFault  = FaultCode::NONE;
    uint32_t  _pollCounter  = 0;
    uint32_t  _stallSince   = 0;
    uint32_t  _encoderErrorCount = 0;
};
