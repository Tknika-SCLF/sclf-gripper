#pragma once
/**
 * MT6701.h — Driver para el encoder magnético MT6701QT
 * Interfaz SPI, lectura de ángulo absoluto 14-bit.
 *
 * Pines (ver pins.h):
 *   CS  = PA4 (PIN_ENC_CS)
 *   CLK = PA5 (PIN_ENC_CLK)
 *   SDO = PA6 (PIN_ENC_SDO)
 *
 * ⚠️  FASE 1.1 — Implementar en MT6701.cpp
 *     Este fichero es el esqueleto de la API. No modificar la interfaz pública.
 */

#include <Arduino.h>
#include <SPI.h>
#include "config/pins.h"

class MT6701 {
public:
    // Constructor — usa los pines definidos en pins.h
    MT6701();

    /**
     * Inicializa el bus SPI y verifica comunicación con el sensor.
     * @return true si la comunicación SPI es correcta, false si falla.
     */
    bool begin();

    /**
     * Lee el ángulo actual.
     * @return Ángulo en radianes [0, 2π). Devuelve -1.0f si hay error de comunicación.
     */
    float getAngleRad();

    /**
     * Lee el valor crudo del sensor.
     * @return Valor 14-bit [0, 16383]. 0xFFFF si hay error.
     */
    uint16_t getRawCounts();

    /**
     * Comprueba si la última lectura SPI fue válida.
     * @return true si OK, false si hubo error de comunicación.
     */
    bool isOk() const { return _ok; }

    // Constantes del sensor
    static constexpr uint16_t COUNTS_PER_REV = 16384;   // 2^14
    static constexpr float    RAD_PER_COUNT  = TWO_PI / COUNTS_PER_REV;

private:
    SPISettings _spiSettings;
    bool        _ok = false;

    uint16_t _readRaw();
};
