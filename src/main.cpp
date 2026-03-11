/**
 * SCLF Gripper v1.0 — Firmware Hardware Initialization
 * Board:     STM32G474CEU6
 * Framework: Arduino (STM32duino) via PlatformIO
 *
 * ─── FASE 1.1 — MT6701 Standalone Test ─────────────────────────────────────
 * Objetivo: Leer correctamente el encoder magnético MT6701 vía SPI (o SSI).
 * NOTA: Motor y Driver desactivados por seguridad en esta fase.
 * ───────────────────────────────────────────────────────────────────────────
 */

#include "config/pins.h"
#include "encoder/MT6701.h"

#include <Arduino.h>
#include <SPI.h>

// ─── Instancias de Hardware ──────────────────────────────────────────────────
MT6701 sensor;

// Bus SPI dedicado para el sensor
SPIClass SPI_ENC(NC, PIN_ENC_SDO, PIN_ENC_CLK);

// ─── setup() ──────────────────────────────────────────────────────────────────
void setup() {
    Serial.begin(115200);
    uint32_t t = millis();
    while (!Serial && (millis() - t) < 3000) {
    }

    Serial.println("========================================");
    Serial.println("  SCLF Gripper v1.0 — Fase 1.1          ");
    Serial.println("  Iniciando Test MT6701 (Sensor)        ");
    Serial.println("========================================");

    // 1. Configurar pines auxiliares (desactivar motor)
    pinMode(PIN_DRV_CS, OUTPUT);
    digitalWrite(PIN_DRV_CS, HIGH);  // Driver deshabilitado

    // Configurar pines PWM del driver en LOW por seguridad
    pinMode(PIN_AH, OUTPUT);
    digitalWrite(PIN_AH, LOW);
    pinMode(PIN_BH, OUTPUT);
    digitalWrite(PIN_BH, LOW);
    pinMode(PIN_CH, OUTPUT);
    digitalWrite(PIN_CH, LOW);

    pinMode(PIN_LED, OUTPUT);
    digitalWrite(PIN_LED, LOW);

    pinMode(PIN_ENC_CS, OUTPUT);
    digitalWrite(PIN_ENC_CS, HIGH);

    // 2. Inicializar bus SPI para el encoder (Pines PB14=MISO, PB13=CLK)
    SPI_ENC.begin();

    // 3. Inicializar Sensor MT6701
    Serial.print("  [MT6701] Iniciando... ");
    // MT6701.cpp ahora acepta la instancia SPI_ENC correctamente
    if (sensor.begin(&SPI_ENC)) {
        Serial.println("OK.");
    } else {
        Serial.println("ERROR. ¿Magneto alineado? ¿SPI conectado?");
    }

    Serial.println("========================================");
}

// ─── loop() ───────────────────────────────────────────────────────────────────
void loop() {
    // Leer el sensor
    float current_angle = sensor.getAngleRad();
    uint16_t raw_counts = sensor.getRawCounts();

    // Imprimir por Serial Plotter cada 50ms para ver la curva suave
    static uint32_t last_print = 0;
    if (millis() - last_print >= 50) {
        // Enviar formato para Teleplot/Serial Plotter
        Serial.print(">Angle_Rad:");
        Serial.println(current_angle, 4);
        Serial.print(">Raw_Counts:");
        Serial.println(raw_counts);

        last_print = millis();
    }

    // Toggle LED a 1Hz
    static uint32_t last_led = 0;
    if (millis() - last_led >= 500) {
        digitalWrite(PIN_LED, !digitalRead(PIN_LED));
        last_led = millis();
    }
}
