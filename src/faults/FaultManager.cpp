#include "faults/FaultManager.h"
#include "motor/MotorController.h"
#include "motor/DRV8316.h"
#include "encoder/MT6701.h"

FaultManager::FaultManager(MotorController& mc)
    : _mc(mc)
{}

void FaultManager::update() {
    // Si ya hay un fallo activo, mantener el estado seguro
    if (hasFault()) {
        if (_mc.getMotor().enabled) {
            _mc.getMotor().disable();
            Serial.println("\n[SAFETY] Motor disabled. Clear fault first!");
        }
        return;
    }

    // Reset del timer de STALL en cambio de modo (evita falsos positivos en transiciones)
    MotionControlType currentMode = _mc.getMotor().controller;
    if (currentMode != _lastMode) {
        _stallSince = 0;
        _lastMode = currentMode;
    }

    _pollCounter++;

    // 1. Monitorización del Driver DRV8316 (cada N ciclos)
    if (_pollCounter >= DRV_POLL_EVERY) {
        _pollCounter = 0;
        if (_mc.getDrv().hasFault()) {
            triggerSafeState(FaultCode::DRV_FAULT);
            return;
        }
    }

    // 2. Monitorización del Encoder MT6701 (con filtro antiruido)
    if (!_mc.getEncoder().isOk()) {
        _encoderErrorCount++;
        if (_encoderErrorCount > 10) {
            triggerSafeState(FaultCode::ENCODER_ERROR);
            return;
        }
    } else {
        _encoderErrorCount = 0;
    }

    // 3. Detección de Bloqueo (Stall)
    if (_mc.getMotor().enabled && _stallEnabled) {
        bool isTryingToMove = false;

        if (_mc.getMotor().controller == MotionControlType::angle ||
            _mc.getMotor().controller == MotionControlType::angle_openloop) {
            if (abs(_mc.getMotor().target - _mc.getMotor().shaft_angle) > 0.1f) {
                isTryingToMove = true;
            }
        } else {
            if (abs(_mc.getMotor().target) > 0.1f) {
                isTryingToMove = true;
            }
        }

        if (isTryingToMove) {
            // Un STALL real es: motor no se mueve Y el controlador está haciendo mucho esfuerzo.
            // Si el PID es bajo y el esfuerzo (voltaje) es pequeño, simplemente no tiene fuerza, no es un STALL.
            float effort = abs(_mc.getMotor().voltage.q) + abs(_mc.getMotor().voltage.d);
            bool highEffort = effort > (_mc.getMotor().voltage_limit * 0.5f); // Más del 50% del límite

            if (abs(_mc.getMotor().shaft_velocity) < STALL_VEL_RAD_S && highEffort) {
                if (_stallSince == 0) {
                    _stallSince = millis();
                } else if (millis() - _stallSince > STALL_TIME_MS) {
                    triggerSafeState(FaultCode::STALL);
                    return;
                }
            } else {
                _stallSince = 0;
            }
        } else {
            _stallSince = 0;
        }
    } else {
        _stallSince = 0;
    }
}

void FaultManager::triggerSafeState(FaultCode code) {
    _activeFault = code;
    
    // Acción inmediata: Desactivar motor
    _mc.getMotor().disable();

    // Reportar por serie
    Serial.print("\n[SAFETY] CRITICAL FAULT DETECTED: ");
    Serial.println(faultName(code));
    Serial.println("[SAFETY] Motor DISABLED for protection.");
}

void FaultManager::clearFault() {
    if (_activeFault == FaultCode::DRV_FAULT) {
        _mc.getDrv().clearFaults();
    }
    _activeFault = FaultCode::NONE;
    _stallSince = 0;
    Serial.println("[SAFETY] Fault cleared. Manual restart required.");
}

bool FaultManager::isStalled() {
    return _activeFault == FaultCode::STALL;
}

const char* FaultManager::faultName(FaultCode code) {
    switch (code) {
        case FaultCode::NONE:          return "NONE";
        case FaultCode::DRV_FAULT:     return "DRV_FAULT (DRV8316)";
        case FaultCode::ENCODER_ERROR: return "ENCODER_ERROR (MT6701)";
        case FaultCode::STALL:         return "STALL (Mechanical Block)";
        case FaultCode::OVERCURRENT:   return "OVERCURRENT";
        case FaultCode::OVERVOLTAGE:   return "OVERVOLTAGE";
        default:                       return "UNKNOWN";
    }
}
