#pragma once

#include <Arduino.h>
#include <SimpleFOC.h>
#include "motor/DRV8316.h"
#include "config/pins.h"

/**
 * Clase SCLF_CurrentSense
 * Wrapper para InlineCurrentSense de SimpleFOC configurado para el SCLF Gripper.
 */
class SCLF_CurrentSense {
public:
    /**
     * @param drv Referencia al objeto DRV8316 para obtener la ganancia configurada.
     */
    SCLF_CurrentSense(DRV8316& drv);

    /**
     * Inicializa el sensor de corriente calibrando los offsets.
     * @return true si la inicialización es correcta.
     */
    bool begin();

    /**
     * Actualiza las lecturas de corriente. Debe llamarse en el loop.
     */
    void update();

    /**
     * Devuelve la corriente de fase en Amperios.
     */
    float getAmpsA() { return _cs.getPhaseCurrents().a; }
    float getAmpsB() { return _cs.getPhaseCurrents().b; }
    float getAmpsC() { return _cs.getPhaseCurrents().c; }

    /**
     * Devuelve el objeto InlineCurrentSense subyacente (para SimpleFOC).
     */
    InlineCurrentSense& getSimpleFOC() { return _cs; }

private:
    DRV8316& _drv;
    InlineCurrentSense _cs;
};
