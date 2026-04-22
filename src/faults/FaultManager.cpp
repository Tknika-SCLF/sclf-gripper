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
        // Prevenir que el usuario reactive el motor sin limpiar el fallo
        if (_mc.getMotor().enabled) {
            _mc.getMotor().disable();
            Serial.println("\n[SAFETY] Motor disabled. Clear fault first!");
        }
        return;
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
        // Si falla durante 10 ciclos consecutivos (~1ms), es un error real
        if (_encoderErrorCount > 10) {
            triggerSafeState(FaultCode::ENCODER_ERROR);
            return;
        }
    } else {
        _encoderErrorCount = 0;
    }

    // 3. Detección de Bloqueo (Stall)
    // Solo detectamos stall si el motor está habilitado y tiene un objetivo > 0.1
    if (_mc.getMotor().enabled && abs(_mc.getMotor().target) > 0.1f) {
        if (abs(_mc.getMotor().shaft_velocity) < STALL_VEL_RAD_S) {
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
