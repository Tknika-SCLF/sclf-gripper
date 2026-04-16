/**
 * DRV8316.cpp — Implementación del driver DRV8316CRRGFR
 *
 * ⚠️  FASE 1.2 — Pendiente verificar con hardware real
 *
 * Frame SPI del DRV8316 (datasheet 8.5.1):
 *   TX (16 bits): [15]=R/nW, [14:11]=ADDR, [10:0]=DATA
 *   RX (16 bits): [15]=FAULT, [14:11]=ADDR_ECHO, [10:0]=DATA
 *   Modo: SPI_MODE1 (CPOL=0, CPHA=1)
 *   Velocidad máxima: 10 MHz
 */

#include "motor/DRV8316.h"

DRV8316::DRV8316()
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
    // Usar el nuevo método robusto (doble trama)
    uint16_t s1 = readRegister(DRV8316_REG::REG_STATUS1);
    _ok = (s1 != 0x7FF);

    if (_ok) {
        // MUY IMPORTANTE: Desbloquear los registros (por defecto DRV8316 arranca bloqueado)
        uint16_t ctrl2 = readRegister(DRV8316_REG::REG_CONTROL2);
        ctrl2 &= ~DRV8316_BIT::REG_LOCK_MASK;
        ctrl2 |= DRV8316_BIT::UNLOCK_ALL;
        writeRegister(DRV8316_REG::REG_CONTROL2, ctrl2);

        clearFaults();
    }

    return _ok;
}

void DRV8316::setCSAGain(uint16_t gain_mask) {
    uint16_t val = readRegister(DRV8316_REG::REG_CONTROL5);
    val &= ~DRV8316_BIT::CSA_GAIN_MASK;
    val |= (gain_mask & DRV8316_BIT::CSA_GAIN_MASK);
    writeRegister(DRV8316_REG::REG_CONTROL5, val);
}

float DRV8316::getCSAGainV_A() {
    uint16_t val = readRegister(DRV8316_REG::REG_CONTROL5);
    uint16_t gain_bits = (val & DRV8316_BIT::CSA_GAIN_MASK);
    
    switch (gain_bits) {
        case DRV8316_BIT::CSA_GAIN_0V15: return 0.15f;
        case DRV8316_BIT::CSA_GAIN_0V3:  return 0.30f;
        case DRV8316_BIT::CSA_GAIN_0V6:  return 0.60f;
        case DRV8316_BIT::CSA_GAIN_1V2:  return 1.20f;
        default: return 0.30f; // Default seguro
    }
}

uint16_t DRV8316::readRegister(uint8_t addr) {
    uint16_t txWord = READ_FLAG | (static_cast<uint16_t>(addr) << ADDR_SHIFT);

    _spi->beginTransaction(_spiSettings);
    
    // Trama 1: Solicitar lectura
    digitalWrite(PIN_DRV_CS, LOW);
    delayMicroseconds(1);
    _spi->transfer16(txWord);
    digitalWrite(PIN_DRV_CS, HIGH);

    delayMicroseconds(1); // Mínimo tReady (datasheet)

    // Trama 2: Recibir dato (enviando NOP)
    digitalWrite(PIN_DRV_CS, LOW);
    delayMicroseconds(1);
    uint16_t rxWord = _spi->transfer16(0x0000); // NOP
    digitalWrite(PIN_DRV_CS, HIGH);
    
    _spi->endTransaction();

    _lastRawRx = rxWord;

    if (rxWord == 0xFFFF || rxWord == 0x0000) {
        _lastFault = DRV8316Fault::SPI_FAULT;
    }

    return rxWord & DATA_MASK;
}

void DRV8316::writeRegister(uint8_t addr, uint16_t value) {
    uint16_t txWord =
        WRITE_FLAG | (static_cast<uint16_t>(addr) << ADDR_SHIFT) | (value & DATA_MASK);

    _spi->beginTransaction(_spiSettings);
    digitalWrite(PIN_DRV_CS, LOW);
    delayMicroseconds(1);
    _spi->transfer16(txWord);
    digitalWrite(PIN_DRV_CS, HIGH);
    _spi->endTransaction();
}

bool DRV8316::hasFault() {
    uint16_t s1 = readRegister(DRV8316_REG::REG_STATUS1);
    
    // Bit 0 de STATUS1 = FAULT (flag global)
    if (s1 & 0x01) {
        uint16_t s2 = readRegister(DRV8316_REG::REG_STATUS2);
        // Mapeo detallado
        if (s1 & (1 << 8)) _lastFault = DRV8316Fault::OTSD;
        else if (s1 & (1 << 9)) _lastFault = DRV8316Fault::OVP;
        else if (s2 & 0x3F) _lastFault = DRV8316Fault::OCP;
        else if (s1 & (1 << 7)) _lastFault = DRV8316Fault::OTP_ERR;
        else _lastFault = DRV8316Fault::FAULT;
        return true;
    }

    _lastFault = DRV8316Fault::NONE;
    return false;
}

void DRV8316::clearFaults() {
    uint16_t ctrl1 = readRegister(DRV8316_REG::REG_CONTROL1);
    writeRegister(DRV8316_REG::REG_CONTROL1, ctrl1 | 0x001);  // CLR_FLT = 1
    writeRegister(DRV8316_REG::REG_CONTROL1, ctrl1 & ~0x001); // CLR_FLT = 0
    _lastFault = DRV8316Fault::NONE;
}

uint16_t DRV8316::getStatus1Raw() { return readRegister(DRV8316_REG::REG_STATUS1); }
uint16_t DRV8316::getStatus2Raw() { return readRegister(DRV8316_REG::REG_STATUS2); }
