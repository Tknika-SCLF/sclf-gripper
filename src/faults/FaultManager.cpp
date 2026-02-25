/**
 * FaultManager.cpp — Implementación del gestor de fallos
 * ⚠️  FASE 6 — Pendiente integración con motor SimpleFOC
 */

#include "faults/FaultManager.h"
#include "motor/DRV8316.h"
#include "encoder/MT6701.h"

FaultManager::FaultManager(DRV8316& drv, MT6701& enc)
    : _drv(drv), _enc(enc)
{}

void FaultManager::update() {
    _pollCounter++;

    // Polling DRV8316 cada N ciclos (no en cada loop para no saturar SPI)
    if (_pollCounter >= DRV_POLL_EVERY) {
        _pollCounter = 0;

        if (_drv.hasFault()) {
            triggerSafeState(FaultCode::DRV_FAULT);
            return;
        }
    }

    // Encoder siempre monitorizado
    if (!_enc.isOk()) {
        triggerSafeState(FaultCode::ENCODER_ERROR);
        return;
    }

    // TODO (FASE 6): implementar detección de stall
    // Requiere acceso a motor.shaft_velocity — integrar con MotorController
}

void FaultManager::triggerSafeState(FaultCode code) {
    _activeFault = code;
    _motorEnabled = false;

    // TODO (FASE 6): llamar a motor.disable() desde aquí
    // Requiere referencia al MotorController — añadir al constructor

    // Reportar por VCP inmediatamente
    Serial.print("[FAULT] ");
    Serial.println(faultName(code));
}

void FaultManager::clearFault() {
    if (_activeFault == FaultCode::DRV_FAULT) {
        _drv.clearFaults();
    }
    _activeFault = FaultCode::NONE;
}

const char* FaultManager::faultName(FaultCode code) {
    switch (code) {
        case FaultCode::NONE:          return "NONE";
        case FaultCode::DRV_FAULT:     return "DRV_FAULT";
        case FaultCode::ENCODER_ERROR: return "ENCODER_ERROR";
        case FaultCode::STALL:         return "STALL";
        case FaultCode::OVERCURRENT:   return "OVERCURRENT";
        case FaultCode::OVERVOLTAGE:   return "OVERVOLTAGE";
        default:                       return "UNKNOWN";
    }
}
