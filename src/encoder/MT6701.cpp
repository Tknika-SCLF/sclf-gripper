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
    : _spiSettings(5'000'000, MSBFIRST, SPI_MODE0),
      _spi(&SPI)  // por defecto
      ,
      _ok(false) {}

bool MT6701::begin(SPIClass* spi_ptr) {
    if (spi_ptr != nullptr) {
        _spi = spi_ptr;
    }

    pinMode(PIN_ENC_CS, OUTPUT);
    digitalWrite(PIN_ENC_CS, HIGH);  // CS inactivo = HIGH

    // Asumimos que el SPI ya fue inicializado (begin) en main.cpp,
    // pero por si acaso podríamos hacer _spi->begin() si quisiéramos.
    // Dejamos que main.cpp haga SPI_ENC.begin().

    // Leer una vez para verificar que no devuelve 0x0000 ni 0xFFFF
    uint16_t raw = _readRaw();
    _ok = (raw != 0xFFFF) && (raw != 0x0000);

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

uint16_t MT6701::getRawCounts() {
    uint16_t raw = _readRaw();
    if (!_ok)
        return 0xFFFF;
    return (raw >> 2) & 0x3FFF;
}

// ── Privado ──────────────────────────────────────────────────────────────────

uint16_t MT6701::_readRaw() {
    _spi->beginTransaction(_spiSettings);
    digitalWrite(PIN_ENC_CS, LOW);

    // TODO (FASE 1.1): verificar el frame SPI exacto contra el datasheet MT6701
    // El MT6701 en modo SPI devuelve 16 bits: [15:2]=angle, [1:0]=status
    uint16_t raw = _spi->transfer16(0x0000);

    digitalWrite(PIN_ENC_CS, HIGH);
    _spi->endTransaction();

    // Validación básica: si todos los bits son iguales, probablemente hay error de bus
    _ok = (raw != 0x0000) && (raw != 0xFFFF);

    return raw;
}
