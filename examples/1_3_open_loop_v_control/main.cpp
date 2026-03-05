/**
 * SCLF Gripper v1.0 — Firmware
 * Board:     STM32G474CEU6
 * Framework: Arduino (STM32duino) via PlatformIO
 *
 * ─── FASE 1.3 — Open-Loop Velocity Control ──────────────────────────────────
 * Objetivo: Motorra begizta irekian biraraztea, 6-PWM seinaleak probatzeko.
 * Motorra: iPower GM3506 (7 pole pairs). Tentsio muga: 2.0V
 * ───────────────────────────────────────────────────────────────────────────
 */

#include "config/pins.h"
#include "motor/DRV8316.h"

#include <Arduino.h>
#include <SPI.h>

#include <SimpleFOC.h>

// ─── Instancias de Hardware ──────────────────────────────────────────────────
// Motor de 7 pares de polos (14 polos en total)
BLDCMotor motor = BLDCMotor(7);

// Driver en modo 6-PWM
BLDCDriver6PWM driverFOC = BLDCDriver6PWM(PIN_AH, PIN_AL, PIN_BH, PIN_BL, PIN_CH, PIN_CL);

// Gestión de errores por SPI del DRV8316
DRV8316 drv8316;
SPIClass SPI_DRV(PIN_DRV_MOSI, PIN_DRV_MISO, PIN_DRV_CLK);

// Instancia de Commander para controlar por puerto serie
Commander command = Commander(Serial);
void doTarget(char* cmd) {
    command.motor(&motor, cmd);
}

// ─── setup() ──────────────────────────────────────────────────────────────────
void setup() {
    Serial.begin(115200);
    uint32_t t = millis();
    while (!Serial && (millis() - t) < 3000) {
    }

    Serial.println("========================================");
    Serial.println("  SCLF Gripper v1.0 — Fase 1.3          ");
    Serial.println("  Open-Loop Velocity Control            ");
    Serial.println("========================================");

    // 1. Configurar pines auxiliares
    pinMode(PIN_DRV_CS, OUTPUT);
    digitalWrite(PIN_DRV_CS, HIGH);

    pinMode(PIN_ENC_CS, OUTPUT);
    digitalWrite(PIN_ENC_CS, HIGH);  // Ignorar sensor

    pinMode(PIN_LED, OUTPUT);
    digitalWrite(PIN_LED, LOW);

    // 2. Inicializar driver SPI DRV8316 (para fallos/handshake)
    SPI_DRV.begin();
    delay(10);
    if (drv8316.begin(&SPI_DRV)) {
        Serial.println("  [DRV8316] Comunicacion SPI exitosa.");
    } else {
        Serial.println("  [DRV8316] ERROR de SPI. Revisa conexiones o voltaje.");
    }

    // 3. Configurar SimpleFOC Driver
    driverFOC.pwm_frequency = 20000;
    driverFOC.voltage_power_supply = 12.0;  // Voltaje de la fuente (11.1V - 12V)
    driverFOC.voltage_limit = 2.0;          // Límite extra de seguridad para el driver
    if (!driverFOC.init()) {
        Serial.println("  [FOC Driver] ERROR de inicializacion.");
        return;
    }
    motor.linkDriver(&driverFOC);

    // 4. Configurar SimpleFOC Motor
    motor.voltage_limit = 2.0;  // <-- CRÍTICO: Límite de hardware 2.0V
    motor.velocity_limit = 20;  // rad/s
    motor.controller = MotionControlType::velocity_openloop;

    // Inicializar motor
    motor.init();

    // Añadir comando a Commander
    command.add('T', doTarget, "Target velocity");

    Serial.println("========================================");
    Serial.println("  Motor listo en begizta irekia!");
    Serial.println("  Envía 'T3' para girar a 3 rad/s.");
    Serial.println("  Envía 'T0' para parar.");
    Serial.println("========================================");

    _delay(1000);  // Pequeña pausa
}

// ─── loop() ───────────────────────────────────────────────────────────────────
void loop() {
    // Bucle principal de SimpleFOC para open-loop
    // IMPORTANTE: driverFOC loop-ak behar ditu seinaleak jarraian deitzea open_loop-ean.
    motor.move();

    // Leer comandos del puerto serie
    command.run();

    // Check fallos del driver DRV8316 cada 1 segundo (sin bloquear)
    static uint32_t last_check = 0;
    if (millis() - last_check >= 1000) {
        if (drv8316.hasFault()) {
            Serial.println(">>> [DRV8316] FALLO ACTIVO DETECTADO <<<");
            drv8316.clearFaults();
        }
        last_check = millis();
    }

    // Toggle LED (indica que avanza el loop)
    static uint32_t last_led = 0;
    if (millis() - last_led >= 500) {
        digitalWrite(PIN_LED, !digitalRead(PIN_LED));
        last_led = millis();
    }
}
