#include <Arduino.h>
#include <SimpleFOC.h>
#include "motor/MotorController.h"

// Instancia global del controlador
MotorController mc;

// Comandante serie
Commander commander = Commander(Serial);
void onMotor(char* cmd){ commander.motor(&mc.getMotor(), cmd); }

// Comando personalizado para diagnóstico
void onDiag(char* cmd) { mc.dumpRegisters(); }

// Comando para recalibrar
void onRecalib(char* cmd) { mc.forceCalibration(); }

void setup() {
    // 1. Comunicación Serie (USB-C)
    Serial.begin(115200);
    while(!Serial && millis() < 3000); 
    SimpleFOCDebug::enable(&Serial); 

    Serial.println("\n\n========================================");
    Serial.println("  SCLF Gripper v1.0 — PHASE 2.2         ");
    Serial.println("  FOC Closed-Loop Velocity Test        ");
    Serial.println("========================================");

    // 2. Inicialización del MotorController
    if (mc.begin()) {
        Serial.println("[MC] System Initialized OK.");
    } else {
        Serial.println("[MC] CRITICAL: Initialization FAILED.");
    }

    // 3. Configurar Modo de Control (Lazo Cerrado por defecto en este test)
    mc.setMode(MotionControlType::velocity);
    Serial.println("[MC] Starting in CLOSED-LOOP VELOCITY mode.");

    // 4. Configurar Comandos
    commander.add('M', onMotor, "motor");
    commander.add('D', onDiag, "dump registers");
    commander.add('R', onRecalib, "force recalibration");
    
    Serial.println("System Ready.");
    Serial.println("Commands:");
    Serial.println("  M<val>  : Set velocity target (rad/s)");
    Serial.println("  ME<0/1> : Disable/Enable motor");
    Serial.println("  D       : Dump DRV registers");
    Serial.println("  R       : Recalibrate FOC");
}

void loop() {
    // Bucle de control FOC (Core)
    mc.update();

    // Comandos serie
    commander.run();

    // Telemetría de diagnóstico (cada 500ms)
    static uint32_t last_telemetry = 0;
    if (millis() - last_telemetry > 500) {
        Serial.print("Ang:");
        Serial.print(mc.getEncoder().getAngleRad(), 3);
        Serial.print(" rad | Vel:");
        Serial.print(mc.getMotor().shaft_velocity, 2);
        Serial.print(" rad/s | Target:");
        Serial.println(mc.getMotor().target, 2);
        last_telemetry = millis();
    }

    // Heartbeat LED
    static uint32_t last_led = 0;
    if (millis() - last_led > 200) {
        digitalWrite(PIN_LED, !digitalRead(PIN_LED));
        last_led = millis();
    }
}
