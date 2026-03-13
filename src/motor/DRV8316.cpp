/**
 * DRV8316.cpp — Implementación del driver DRV8316CRRGFR
 *
 * ⚠️  FASE 1.2 — Pendiente verificar con hardware real
 *
 * Frame SPI del DRV8316 (datasheet 8.5.1):
 *   TX (16 bits): [15]=R/nW, [14:11]=ADDR, [10:0]=DATA
 *   RX (16 bits): [15]=FAULT, [14:11]=ADDR_ECHO, [10:0]=DATA
 *   Modo: SPI_MODE1 (CPOL=0, CPHA=1) — confirmar con datasheet
 *   Velocidad máxima: 10 MHz
 */

#include "motor/DRV8316.h"

DRV8316::DRV8316()
    // DRV8316 datasheet §8.5.1: SPI_MODE1 (CPOL=0, CPHA=1), max 10 MHz.
    // Usar 1 MHz para mayor robustez en prototipo con cables largos.
    : _spiSettings(1'000'000, MSBFIRST, SPI_MODE1),
      _spi(&SPI),
      _lastFault(DRV8316Fault::NONE),
      _ok(false) {}


bool DRV8316::begin(SPIClass* spi_ptr) {
    if (spi_ptr != nullptr) {
        _spi = spi_ptr;
    }

    pinMode(PIN_DRV_CS, OUTPUT);
    digitalWrite(PIN_DRV_CS, HIGH);

    // Leer STATUS1 como handshake.
    // readRegister() devuelve rxWord & 0x07FF, por lo que 0x7FF = todos bits 1 = MISO flotante.
    // Causa habitual: DRV8316 sin alimentación VM (necesita >8V).
    uint16_t s1 = readRegister(DRV8316_REG::STATUS1);
    _ok = (s1 != 0x7FF);

    if (_ok) {
        clearFaults();
    }

    return _ok;
}

uint16_t DRV8316::readRegister(uint8_t addr) {
    uint16_t txWord = READ_FLAG | (static_cast<uint16_t>(addr) << ADDR_SHIFT);

    _spi->beginTransaction(_spiSettings);
    digitalWrite(PIN_DRV_CS, LOW);
    delayMicroseconds(2);  // CS setup time (DRV8316 tCSS ≥ 50 ns — marginal en prototype)
    uint16_t rxWord = _spi->transfer16(txWord);
    digitalWrite(PIN_DRV_CS, HIGH);
    _spi->endTransaction();

    _lastRawRx = rxWord;  // Guardar sin máscara para diagnóstico

    if (rxWord == 0xFFFF || rxWord == 0x0000) {
        _lastFault = DRV8316Fault::SPI_FAULT;  // MISO flotante alto o bajo
    }

    return rxWord & DATA_MASK;
}

void DRV8316::writeRegister(uint8_t addr, uint16_t value) {
    uint16_t txWord =
        WRITE_FLAG | (static_cast<uint16_t>(addr) << ADDR_SHIFT) | (value & DATA_MASK);

    _spi->beginTransaction(_spiSettings);
    digitalWrite(PIN_DRV_CS, LOW);
    _spi->transfer16(txWord);
    digitalWrite(PIN_DRV_CS, HIGH);
    _spi->endTransaction();
}

bool DRV8316::hasFault() {
    uint16_t s1 = readRegister(DRV8316_REG::STATUS1);
    uint16_t s2 = readRegister(DRV8316_REG::STATUS2);

    // 0x7FF = MISO flotante (readRegister ya aplica & 0x07FF — máximo posible es 0x7FF).
    if (s1 == 0x7FF && s2 == 0x7FF) {
        _lastFault = DRV8316Fault::SPI_FAULT;
        return true;
    }

    // Bit 0 de STATUS1 = FAULT (flag global)
    if (s1 & 0x01) {
        // Mapeo detallado de fallos (según Tabla 8-9 del datasheet)
        if (s1 & (1 << 8)) _lastFault = DRV8316Fault::OTSD;
        else if (s1 & (1 << 9)) _lastFault = DRV8316Fault::OVP;
        else if (s2 & 0x3F) _lastFault = DRV8316Fault::OCP; // Cualquiera de los bits de OCP en STATUS2
        else if (s1 & (1 << 7)) _lastFault = DRV8316Fault::OTP_ERR;
        else _lastFault = DRV8316Fault::FAULT;
        return true;
    }

    _lastFault = DRV8316Fault::NONE;
    return false;
}

void DRV8316::clearFaults() {
    // DRV8316 datasheet Table 8-9: CLR_FLT = CONTROL1 bit 0.
    // Pulsar a 1 y bajar tras >1 µs (no bloqueante — compatible con loopFOC).
    uint16_t ctrl1 = readRegister(DRV8316_REG::CONTROL1);
    writeRegister(DRV8316_REG::CONTROL1, ctrl1 | 0x001);  // CLR_FLT = 1
    // La transacción SPI previa ya satisface el tiempo mínimo (>1 µs).
    writeRegister(DRV8316_REG::CONTROL1, ctrl1 & ~0x001); // CLR_FLT = 0
    _lastFault = DRV8316Fault::NONE;
}
