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
    // TODO (FASE 1.2): verificar SPI mode — algunos datasheets indican MODE1
    : _spiSettings(5'000'000, MSBFIRST, SPI_MODE1)
    , _lastFault(DRV8316Fault::NONE)
    , _ok(false)
{}

bool DRV8316::begin() {
    pinMode(PIN_DRV_CS, OUTPUT);
    digitalWrite(PIN_DRV_CS, HIGH);

    // SPI ya inicializado por MT6701 o lo iniciamos aquí
    SPI.begin();

    // Leer STATUS1 como handshake — si devuelve 0xFFFF hay error de bus
    uint16_t s1 = readRegister(DRV8316_REG::STATUS1);
    _ok = (s1 != 0xFFFF);

    if (_ok) {
        clearFaults();
    }

    return _ok;
}

uint16_t DRV8316::readRegister(uint8_t addr) {
    uint16_t txWord = READ_FLAG | (static_cast<uint16_t>(addr) << ADDR_SHIFT);

    SPI.beginTransaction(_spiSettings);
    digitalWrite(PIN_DRV_CS, LOW);
    uint16_t rxWord = SPI.transfer16(txWord);
    digitalWrite(PIN_DRV_CS, HIGH);
    SPI.endTransaction();

    if (rxWord == 0xFFFF) {
        _lastFault = DRV8316Fault::SPI_FAULT;
    }

    return rxWord & DATA_MASK;
}

void DRV8316::writeRegister(uint8_t addr, uint16_t value) {
    uint16_t txWord = WRITE_FLAG
                    | (static_cast<uint16_t>(addr) << ADDR_SHIFT)
                    | (value & DATA_MASK);

    SPI.beginTransaction(_spiSettings);
    digitalWrite(PIN_DRV_CS, LOW);
    SPI.transfer16(txWord);
    digitalWrite(PIN_DRV_CS, HIGH);
    SPI.endTransaction();
}

bool DRV8316::hasFault() {
    uint16_t s1 = readRegister(DRV8316_REG::STATUS1);
    uint16_t s2 = readRegister(DRV8316_REG::STATUS2);

    if (s1 == 0xFFFF || s2 == 0xFFFF) {
        _lastFault = DRV8316Fault::SPI_FAULT;
        return true;
    }

    // TODO (FASE 1.2): mapear bits de STATUS1/STATUS2 a DRV8316Fault
    // Bit 0 de STATUS1 = FAULT (flag global)
    if (s1 & 0x01) {
        // Determinar tipo según otros bits
        _lastFault = DRV8316Fault::FAULT;
        return true;
    }

    _lastFault = DRV8316Fault::NONE;
    return false;
}

void DRV8316::clearFaults() {
    // TODO (FASE 1.2): verificar el bit y registro exacto para CLR_FLT
    // En DRV8316 suele ser un bit en CONTROL1
    uint16_t ctrl1 = readRegister(DRV8316_REG::CONTROL1);
    writeRegister(DRV8316_REG::CONTROL1, ctrl1 | 0x001);  // bit CLR_FLT
    delayMicroseconds(10);
    writeRegister(DRV8316_REG::CONTROL1, ctrl1 & ~0x001); // limpiar bit
    _lastFault = DRV8316Fault::NONE;
}
