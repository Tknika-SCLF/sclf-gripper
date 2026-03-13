#pragma once
/**
 * DRV8316.h — Driver SPI para el gate driver DRV8316CRRGFR
 *
 * Pines (ver pins.h):
 *   CS   = PC4  (PIN_DRV_CS)
 *   CLK  = PB3  (PIN_DRV_CLK)
 *   MISO = PB4  (PIN_DRV_MISO)
 *   MOSI = PB5  (PIN_DRV_MOSI)
 *
 * ⚠️  CRÍTICO: nFAULT NO está conectado al STM32.
 *     Los fallos SOLO se detectan leyendo STATUS1/STATUS2 por SPI.
 *     Ver RULES.md R04.
 *
 * ⚠️  FASE 1.2 — Implementar en DRV8316.cpp
 */

#include "config/pins.h"

#include <Arduino.h>
#include <SPI.h>

// ── Mapa de registros DRV8316 (datasheet Table 7-x) ─────────────────────────
namespace DRV8316_REG {
constexpr uint8_t STATUS1 = 0x00;
constexpr uint8_t STATUS2 = 0x01;
constexpr uint8_t CONTROL1 = 0x02;
constexpr uint8_t CONTROL2 = 0x03;
constexpr uint8_t CONTROL3 = 0x04;
constexpr uint8_t CONTROL4 = 0x05;
constexpr uint8_t CONTROL5 = 0x06;
constexpr uint8_t CONTROL6 = 0x07;
constexpr uint8_t CONTROL7 = 0x08;
constexpr uint8_t CONTROL8 = 0x09;
constexpr uint8_t CONTROL9 = 0x0A;
constexpr uint8_t CONTROL10 = 0x0B;
}  // namespace DRV8316_REG

// ── Códigos de fallo ─────────────────────────────────────────────────────────
enum class DRV8316Fault : uint8_t {
    NONE = 0,
    FAULT = 1,      // Fallo genérico (leer STATUS1/2 para detalle)
    OCP = 2,        // Overcurrent protection
    OVP = 3,        // Overvoltage protection
    OTSD = 4,       // Overtemperature shutdown
    OTP_ERR = 5,    // OTP memory error
    SPI_FAULT = 6,  // Error de comunicación SPI
};

class DRV8316 {
   public:
    DRV8316();

    /**
     * Inicializa SPI y verifica comunicación con el DRV8316.
     * Lee el registro STATUS1 como handshake.
     * @param spi_ptr Puntero al objeto SPIClass (ej. &SPI_DRV)
     * @return true si la comunicación es correcta.
     */
    bool begin(SPIClass* spi_ptr);

    /**
     * Lee un registro de 11 bits del DRV8316.
     * @param addr  Dirección del registro (DRV8316_REG::*)
     * @return Valor del registro (11 bits). 0xFFFF si hay error SPI.
     */
    uint16_t readRegister(uint8_t addr);

    /**
     * Escribe un registro de 11 bits en el DRV8316.
     * @param addr  Dirección del registro
     * @param value Valor a escribir (11 bits)
     */
    void writeRegister(uint8_t addr, uint16_t value);

    /**
     * Comprueba si hay algún fallo activo leyendo STATUS1 y STATUS2 por SPI.
     * ⚠️  Es la ÚNICA forma de detectar fallos — nFAULT no está conectado al STM32.
     * @return true si hay fallo activo.
     */
    bool hasFault();

    /**
     * Devuelve el tipo de fallo más reciente detectado.
     */
    DRV8316Fault getFaultCode() const { return _lastFault; }

    /**
     * Limpia los fallos latched escribiendo en el registro de control.
     */
    void clearFaults();

    /**
     * Devuelve el valor crudo de STATUS1 (para diagnóstico/debug).
     */
    uint16_t getStatus1Raw() { return readRegister(DRV8316_REG::STATUS1); }

    /**
     * Devuelve el valor crudo de STATUS2 (para diagnóstico/debug).
     */
    uint16_t getStatus2Raw() { return readRegister(DRV8316_REG::STATUS2); }

    /**
     * Devuelve el último valor de 16-bits recibido por SPI sin enmascarar.
     * Útil para diagnosticar problemas de protocolo a bajo nivel.
     */
    uint16_t getLastRawRx() const { return _lastRawRx; }

   private:
    SPISettings _spiSettings;
    SPIClass* _spi;
    DRV8316Fault _lastFault = DRV8316Fault::NONE;
    bool _ok = false;
    uint16_t _lastRawRx = 0;

    // Frame SPI: 16 bits, [15]=R/W, [14:11]=addr, [10:0]=data
    static constexpr uint16_t READ_FLAG = 0x8000;
    static constexpr uint16_t WRITE_FLAG = 0x0000;
    static constexpr uint16_t DATA_MASK = 0x07FF;
    static constexpr uint16_t ADDR_SHIFT = 11;
};
