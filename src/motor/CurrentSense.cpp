#include "motor/CurrentSense.h"

SCLF_CurrentSense::SCLF_CurrentSense(DRV8316& drv)
    : _drv(drv),
      // Inicializamos con valores temporales, se reconfiguran en begin()
      _cs(1.0f, 0.3f, PIN_CURA, PIN_CURB, PIN_CURC) {
}

bool SCLF_CurrentSense::begin() {
    // 1. Obtener la ganancia configurada en el DRV8316
    float sensitivity = _drv.getCSAGainV_A();
    
    // 2. Configurar InlineCurrentSense
    // I = V / (R * G) -> R*G = sensitivity
    // Usamos R=1.0 y G=sensitivity
    _cs.gain_a = sensitivity;
    _cs.gain_b = sensitivity;
    _cs.gain_c = sensitivity;

    // 3. Inicializar hardware (ADC) y calibrar offsets
    // init() llama internamente a calibrateOffsets()
    return _cs.init() == 1;
}

void SCLF_CurrentSense::update() {
    // Lectura síncrona de los ADCs
    // En el futuro FOC real, esto se sincroniza con el PWM
    _cs.getPhaseCurrents();
}
