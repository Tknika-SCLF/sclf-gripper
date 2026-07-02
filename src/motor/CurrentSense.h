#pragma once

#include <Arduino.h>
#include <SimpleFOC.h>
#include "motor/DRV8316.h"
#include "config/pins.h"

/**
 * Clase SCLF_CurrentSense
 * Wrapper para InlineCurrentSense de SimpleFOC configurado para el SCLF Gripper.
 */
class SCLF_CurrentSense : public CurrentSense {
public:
    SCLF_CurrentSense(DRV8316& drv);

    bool begin();
    void update() {} // No se usa en esta implementación directa

    // Override SimpleFOC CurrentSense virtual methods
    int init() override;
    PhaseCurrent_s getPhaseCurrents() override;
    int driverAlign(float align_voltage) override { return 1; }

    CurrentSense& getSimpleFOC() { return *this; }

    // Offsets públicos para diagnóstico
    float offset_ia = 0.0f;
    float offset_ib = 0.0f;
    float offset_ic = 0.0f;

private:
    DRV8316& _drv;
    float _volts_to_amps = 0.0f;
};
