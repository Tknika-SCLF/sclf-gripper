/**
 * SCLF Gripper v1.0 — Firmware Hardware Initialization
 * Board:     STM32G474CEU6
 * Framework: Arduino (STM32duino) via PlatformIO
 *
 * ─── FASE 1.2 — DRV8316 SPI Config Test ─────────────────────────────────────
 * Objetivo: Leer los registros STATUS1/STATUS2 del DRV8316 y limpiarlos.
 * NOTA: Los pines PWM están en LOW. No se aplica voltaje al motor.
 * ───────────────────────────────────────────────────────────────────────────
 */

#include "config/pins.h"
#include "motor/DRV8316.h"

#include <Arduino.h>
#include <SPI.h>

// ─── Instancias de Hardware ──────────────────────────────────────────────────
DRV8316 driver;

// Bus SPI dedicado para el Driver (PB3=CLK, PB4=MISO, PB5=MOSI)
SPIClass SPI_DRV(PIN_DRV_MOSI, PIN_DRV_MISO, PIN_DRV_CLK);

// ─── setup() ──────────────────────────────────────────────────────────────────
void setup() {
    Serial.begin(115200);
    uint32_t t = millis();
    while (!Serial && (millis() - t) < 3000) {
    }

    Serial.println("========================================");
    Serial.println("  SCLF Gripper v1.0 — Fase 1.2          ");
    Serial.println("  Iniciando Test DRV8316 (Driver SPI)   ");
    Serial.println("========================================");

    // 1. Configurar pines auxiliares
    pinMode(PIN_DRV_CS, OUTPUT);
    digitalWrite(PIN_DRV_CS, HIGH);

    // Configurar pines PWM del driver en LOW (0% Duty Cycle = Motor parado)
    pinMode(PIN_AH, OUTPUT);
    digitalWrite(PIN_AH, LOW);
    pinMode(PIN_BH, OUTPUT);
    digitalWrite(PIN_BH, LOW);
    pinMode(PIN_CH, OUTPUT);
    digitalWrite(PIN_CH, LOW);

    pinMode(PIN_LED, OUTPUT);
    digitalWrite(PIN_LED, LOW);

    // Ignorar sensor por ahora
    pinMode(PIN_ENC_CS, OUTPUT);
    digitalWrite(PIN_ENC_CS, HIGH);

    // 2. Inicializar bus SPI para el Driver
    SPI_DRV.begin();

    // 3. Inicializar Driver DRV8316
    Serial.print("  [DRV8316] Iniciando SPI... ");
    delay(10);  // Darle tiempo a despertar tras el SLEEP=HIGH

    if (driver.begin(&SPI_DRV)) {
        Serial.println("OK.");
        Serial.println("  -> Comunicacion SPI exitosa.");
    } else {
        Serial.println("ERROR. ¿DRV8316 alimentado (VM>8V)? ¿SPI conectado?");
    }

    Serial.println("========================================");
}

// ─── loop() ───────────────────────────────────────────────────────────────────
void loop() {
    // Leer estado del driver 1 vez por segundo
    static uint32_t last_print = 0;
    if (millis() - last_print >= 1000) {
        uint16_t stat1 = driver.getStatus1Raw();
        uint16_t stat2 = driver.getStatus2Raw();

        Serial.print("[DRV8316] STATUS1: 0x");
        Serial.print(stat1, HEX);
        Serial.print(" | STATUS2: 0x");
        Serial.print(stat2, HEX);

        if (driver.hasFault()) {
            Serial.print("  >>> FALLO ACTIVO DETECTADO <<<");
            // Limpiar el fallo para ver si es persistente
            driver.clearFaults();
        }

        Serial.println();

        last_print = millis();
    }

    // Toggle LED a 1Hz (latido)
    static uint32_t last_led = 0;
    if (millis() - last_led >= 500) {
        digitalWrite(PIN_LED, !digitalRead(PIN_LED));
        last_led = millis();
    }
}
