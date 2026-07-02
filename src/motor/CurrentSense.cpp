#include "motor/CurrentSense.h"

SCLF_CurrentSense::SCLF_CurrentSense(DRV8316& drv)
    : _drv(drv) {
}

bool SCLF_CurrentSense::begin() {
    return init() == 1;
}

int SCLF_CurrentSense::init() {
    // ── Desactivar OPAMP1 para liberar PA2 al ADC ──────────────────────────────
    // En STM32G474, PA2 es OPAMP1_VINM. Si OPAMP1 está activo, reclama el pin
    // y analogRead() devuelve 0. Desactivamos OPAMP1 directamente en su registro
    // de control (OPAMP1_CSR, dirección base 0x40010300, bit 0 = OPAEN).
    // Ref: STM32G474 RM0440, sección 23.4.1.
    volatile uint32_t* OPAMP1_CSR = reinterpret_cast<volatile uint32_t*>(0x40010300UL);
    *OPAMP1_CSR &= ~(1U << 0);  // Clear OPAEN bit → desactiva OPAMP1

    // ── Configurar los tres pines como INPUT_ANALOG ────────────────────────────
    // INPUT_ANALOG es el modo STM32-específico que desconecta el pin de cualquier
    // periférico digital (UART, SPI, etc.) y lo entrega exclusivamente al ADC.
    pinMode(PIN_CURA, INPUT_ANALOG);
    pinMode(PIN_CURB, INPUT_ANALOG);
    pinMode(PIN_CURC, INPUT_ANALOG);

    // Forzar resolución a 12 bits (0-4095)
    analogReadResolution(12);

    float sensitivity = _drv.getCSAGainV_A();
    _volts_to_amps = 1.0f / sensitivity;

    // ── Calibración de offsets (promedio 1000 muestras sin corriente) ──────────
    long sum_a = 0, sum_b = 0, sum_c = 0;
    for (int i = 0; i < 1000; i++) {
        sum_a += analogRead(PIN_CURA);
        sum_b += analogRead(PIN_CURB);
        sum_c += analogRead(PIN_CURC);
        delayMicroseconds(100);
    }

    // Convertir cuentas ADC a Voltios (0-3.3V)
    offset_ia = (sum_a / 1000.0f) * (3.3f / 4096.0f);
    offset_ib = (sum_b / 1000.0f) * (3.3f / 4096.0f);
    offset_ic = (sum_c / 1000.0f) * (3.3f / 4096.0f);

    this->initialized = true;
    return 1;
}

PhaseCurrent_s SCLF_CurrentSense::getPhaseCurrents() {
    // Lectura síncrona manual (bypasseando el DMA/trigger de SimpleFOC en STM32G4)
    float va = analogRead(PIN_CURA) * (3.3f / 4096.0f);
    float vb = analogRead(PIN_CURB) * (3.3f / 4096.0f);
    float vc = analogRead(PIN_CURC) * (3.3f / 4096.0f);

    float a = (va - offset_ia) * _volts_to_amps;
    float b = (vb - offset_ib) * _volts_to_amps;

    // Si PA2 sigue sin funcionar tras el DeInit, Kirchhoff como fallback
    float c = (offset_ic > 0.5f) ? (vc - offset_ic) * _volts_to_amps : (-a - b);

    return {a, b, c};
}
