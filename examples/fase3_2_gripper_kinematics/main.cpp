/**
 * SCLF Gripper v1.0 — Firmware
 * Board:     STM32G474CEU6
 * Framework: Arduino (STM32duino) via PlatformIO
 *
 * ─── FASE 3.2 — Gripper Kinematics Test ────────────────────────────────────
 * Objetivo: Probar las conversiones cinemáticas (ángulo ↔ mm ↔ %)
 *           usando la configuración Robotiq 2F-85 como placeholder.
 *           No se activa el motor, solo Serial para verificar la matemática.
 * ───────────────────────────────────────────────────────────────────────────
 */

#include "config/pins.h"

#include <Arduino.h>

#include "kinematics/GripperKinematics.h"


// ─── Instancias ──────────────────────────────────────────────────────────────
GripperKinematics gripper;

// ─── setup() ──────────────────────────────────────────────────────────────────
void setup() {
    Serial.begin(115200);
    uint32_t t = millis();
    while (!Serial && (millis() - t) < 3000) {
    }

    Serial.println("========================================");
    Serial.println("  SCLF Gripper v1.0 — Fase 3.2          ");
    Serial.println("  Gripper Kinematics Test               ");
    Serial.println("========================================");

    // Apagar pines del driver y sensor por seguridad
    pinMode(PIN_DRV_CS, OUTPUT);
    digitalWrite(PIN_DRV_CS, HIGH);
    pinMode(PIN_AH, OUTPUT);
    digitalWrite(PIN_AH, LOW);
    pinMode(PIN_BH, OUTPUT);
    digitalWrite(PIN_BH, LOW);
    pinMode(PIN_CH, OUTPUT);
    digitalWrite(PIN_CH, LOW);
    pinMode(PIN_ENC_CS, OUTPUT);
    digitalWrite(PIN_ENC_CS, HIGH);
    pinMode(PIN_LED, OUTPUT);
    digitalWrite(PIN_LED, LOW);

    // Inicializar kinemática con Robotiq 2F-85 placeholder
    gripper.begin(GripperPresets::ROBOTIQ_2F85);
    gripper.printConfig();

    // ─── Test de conversiones ────────────────────────────────────────────
    Serial.println("\n─── Test: mm → angle → mm ───");
    float test_mm[] = {0.0f, 10.0f, 42.5f, 85.0f};
    for (float mm : test_mm) {
        float angle = gripper.mmToAngle(mm);
        float back = gripper.angleToMm(angle);
        Serial.print("  ");
        Serial.print(mm);
        Serial.print(" mm → ");
        Serial.print(angle, 4);
        Serial.print(" rad → ");
        Serial.print(back, 2);
        Serial.println(" mm");
    }

    Serial.println("\n─── Test: percent → angle → mm ───");
    float test_pct[] = {0.0f, 25.0f, 50.0f, 75.0f, 100.0f};
    for (float pct : test_pct) {
        float angle = gripper.percentToAngle(pct);
        float mm = gripper.angleToMm(angle);
        float back_pct = gripper.angleToPercent(angle);
        Serial.print("  ");
        Serial.print(pct, 0);
        Serial.print("% → ");
        Serial.print(angle, 4);
        Serial.print(" rad → ");
        Serial.print(mm, 2);
        Serial.print(" mm → ");
        Serial.print(back_pct, 1);
        Serial.println("%");
    }

    Serial.println("\n─── Test: clamping (out of range) ───");
    float clamped_angle = gripper.clampAngle(-5.0f);
    Serial.print("  clampAngle(-5.0) = ");
    Serial.println(clamped_angle, 4);

    clamped_angle = gripper.clampAngle(9999.0f);
    Serial.print("  clampAngle(9999) = ");
    Serial.println(clamped_angle, 4);

    float clamped_mm = gripper.clampMm(-10.0f);
    Serial.print("  clampMm(-10)     = ");
    Serial.println(clamped_mm, 2);

    clamped_mm = gripper.clampMm(200.0f);
    Serial.print("  clampMm(200)     = ");
    Serial.println(clamped_mm, 2);

    Serial.println("\n========================================");
    Serial.println("  Kinematics test amaituta!");
    Serial.println("========================================");
}

// ─── loop() ───────────────────────────────────────────────────────────────────
void loop() {
    // Bidali Serial-etik mm balio bat eta bihurtuko du
    if (Serial.available()) {
        float input_mm = Serial.parseFloat();
        if (input_mm >= 0) {
            float angle = gripper.mmToAngle(input_mm);
            float pct = gripper.angleToPercent(angle);
            Serial.print("[Input] ");
            Serial.print(input_mm, 2);
            Serial.print(" mm → ");
            Serial.print(angle, 4);
            Serial.print(" rad → ");
            Serial.print(pct, 1);
            Serial.println("%");
        }
    }

    // LED heartbeat
    static uint32_t last_led = 0;
    if (millis() - last_led >= 500) {
        digitalWrite(PIN_LED, !digitalRead(PIN_LED));
        last_led = millis();
    }
}
