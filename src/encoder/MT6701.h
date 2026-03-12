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

#include "config/pins.h"

#include <Arduino.h>
#include <SPI.h>

class MT6701 {
   public:
    // Constructor — usa los pines definidos en pins.h
    MT6701();

    /**
     * @brief Inicializa el bus SPI y prepara el sensor
     * @param spi_ptr Puntero al objeto SPIClass (ej. &SPI_ENC)
     * @return true si la comunicación inicial es válida
     */
    bool begin(SPIClass* spi_ptr = &SPI);

    /**
     * @brief Lee el ángulo actual en radianes
     * @return Ángulo [0, 2PI] o negativo en caso de error
     */
    float getAngleRad();

    /**
     * @brief Actualiza la cuenta de vueltas. Debe llamarse frecuentemente.
     */
    void update();

    /**
     * @brief Obtiene el ángulo acumulado en radianes (incluyendo vueltas)
     */
    float getCumulativeAngleRad();

    /**
     * @brief Obtiene el ángulo acumulado en grados (incluyendo vueltas)
     */
    float getCumulativeAngleDeg();

    /**
     * @brief Establece la posición actual como el cero (0 grados)
     */
    void initZero();

    /**
     * @brief Lee los conteos crudos del sensor (0 - 16383)
     */
    uint16_t getRawCounts();

    /**
     * Comprueba si la última lectura SPI fue válida.
     * @return true si OK, false si hubo error de comunicación.
     */
    bool isOk() const { return _ok; }

    // Constantes del sensor
    static constexpr uint16_t COUNTS_PER_REV = 16384;  // 2^14
    static constexpr float RAD_PER_COUNT = TWO_PI / COUNTS_PER_REV;

   private:
    SPISettings _spiSettings;
    SPIClass* _spi;
    bool _ok = false;

    // Seguimiento de vueltas
    int32_t _rotations = 0;
    float _lastAngleRad = 0.0f;
    float _offsetRad = 0.0f;

    uint16_t _readRaw();
};
