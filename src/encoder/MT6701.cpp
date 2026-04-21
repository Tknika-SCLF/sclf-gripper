/**
 * MT6701.cpp — Implementación del driver MT6701QT
 *
 * ⚠️  FASE 1.1 — Pendiente implementar
 *     El esqueleto está listo. Implementar _readRaw() con el protocolo SPI
 *     correcto del MT6701 (ver datasheet sección 6.1: SSI frame format).
 *
 * Protocolo SPI del MT6701:
 *   - Modo SPI: MODE0 (CPOL=0, CPHA=0)
 *   - Velocidad máxima: 10 MHz (usar 5 MHz para margen)
 *   - Frame: 16 bits, MSB first
 *   - Bits [15:2] = ángulo 14-bit
 *   - Bits [1:0]  = bits de estado (ignorar para ángulo)
 */

#include "encoder/MT6701.h"

MT6701::MT6701()
    : _spiSettings(1'000'000, MSBFIRST, SPI_MODE1),
      _spi(&SPI)  // por defecto
      ,
      _ok(false) {}

bool MT6701::begin(SPIClass* spi_ptr) {
    if (spi_ptr != nullptr) {
        _spi = spi_ptr;
    }

    pinMode(PIN_ENC_CS, OUTPUT);
    digitalWrite(PIN_ENC_CS, HIGH);  

    pinMode(PIN_ENC_CLK, OUTPUT);
    digitalWrite(PIN_ENC_CLK, LOW);

    pinMode(PIN_ENC_SDO, INPUT);

    // Asumimos que el SPI ya fue inicializado (begin) en main.cpp,
    // pero por si acaso podríamos hacer _spi->begin() si quisiéramos.
    // Dejamos que main.cpp haga SPI_ENC.begin().

    // Leer una vez para verificar que no devuelve 0x0000 ni 0xFFFF
    _lastAngleRad = getAngleRad();
    if (_ok) {
        initZero();
    }

    return _ok;
}

float MT6701::getAngleRad() {
    uint16_t raw = _readRaw();
    if (!_ok)
        return -1.0f;

    // Extraer los 14 bits de ángulo (bits [15:2]) y convertir a radianes
    uint16_t counts = (raw >> 2) & 0x3FFF;
    return static_cast<float>(counts) * RAD_PER_COUNT;
}

float MT6701::getSensorAngle() {
    return getAngleRad();
}

void MT6701::update() {
    // CRÍTICO: Llamar a la clase base para que SimpleFOC actualice angle_prev,
    // full_rotations y shaft_velocity. Esta llamada internamente invoca getSensorAngle().
    Sensor::update();

    // Sincronizar nuestro estado interno con el de la clase base para que
    // getCumulativeAngleRad() y el tracking de vueltas propio sigan funcionando.
    _lastAngleRad = angle_prev;
}

float MT6701::getCumulativeAngleRad() {
    // Usar el estado de la clase base de SimpleFOC (full_rotations + angle_prev)
    // que siempre está actualizado desde update().
    return (float)full_rotations * TWO_PI + angle_prev - _offsetRad;
}

float MT6701::getCumulativeAngleDeg() {
    return getCumulativeAngleRad() * 180.0f / PI;
}

void MT6701::initZero() {
    update();  // Asegurar que _lastAngleRad está actualizado
    _rotations = 0;
    _offsetRad = _lastAngleRad;
}

uint16_t MT6701::getRawCounts() {
    uint16_t raw = _readRaw();
    if (!_ok)
        return 0xFFFF;
    return (raw >> 2) & 0x3FFF;
}

// ── Privado ──────────────────────────────────────────────────────────────────

uint16_t MT6701::_readRaw() {
    uint16_t raw = 0;

    // CS activo (LOW)
    digitalWrite(PIN_ENC_CS, LOW);
    delayMicroseconds(1); // t_CSn_Clk setup

    // Bit-banging de 16 bits (Lectura manual)
    // El MT6701 envía datos en el flanco descendente del reloj (SPI Mode 1)
    // o simplemente leemos después de poner el reloj en HIGH.
    for (int i = 0; i < 16; i++) {
        digitalWrite(PIN_ENC_CLK, HIGH);
        delayMicroseconds(1);
        
        raw <<= 1;
        if (digitalRead(PIN_ENC_SDO)) {
            raw |= 0x0001;
        }

        digitalWrite(PIN_ENC_CLK, LOW);
        delayMicroseconds(1);
    }

    // CS inactivo (HIGH)
    digitalWrite(PIN_ENC_CS, HIGH);

    // Validación básica: si no hay cambios o todo son 1s/0s, marcamos error
    _ok = (raw != 0x0000) && (raw != 0xFFFF);

    return raw;
}
