#include <Arduino.h>
#include <SimpleFOC.h>
#include "motor/MotorController.h"
#include "comms/VCP.h"

/**
 * PHASE 4: USB VCP & Commander
 * ----------------------------
 * Este ejemplo demuestra el uso de VCPManager para gestionar el interfaz
 * interactivo Commander de SimpleFOC y la telemetría vía USB.
 */

// Instancias globales
MotorController mc;
VCPManager vcp;

void setup() {
    // 1. Inicialización de comunicación Serie (USB CDC)
    Serial.begin(115200);
    while (!Serial && millis() < 3000); 

    Serial.println("\n\n========================================");
    Serial.println("  SCLF Gripper v2.0 — PHASE 4           ");
    Serial.println("  USB VCP & Commander Interface         ");
    Serial.println("========================================");

    // 2. Inicialización del hardware del motor
    if (mc.begin()) {
        Serial.println("[MAIN] Motor Controller: OK");
    } else {
        Serial.println("[MAIN] Motor Controller: ERROR");
    }

    // 3. Configuración del VCPManager
    vcp.begin();
    vcp.registerMotor(mc);
    
    // Configuración inicial del modo de control
    mc.setMode(MotionControlType::velocity);
    mc.getMotor().target = 0;

    Serial.println("[MAIN] System Ready.");
    Serial.println("Envíe 'M?' para ayuda del motor o 'T1' para telemetría.");
}

void loop() {
    // Bucle FOC de alta frecuencia
    mc.update();

    // Gestión de comandos y telemetría USB (baja frecuencia)
    vcp.update();

    // Heartbeat LED
    static uint32_t last_led = 0;
    if (millis() - last_led > 500) {
        digitalWrite(PIN_LED, !digitalRead(PIN_LED));
        last_led = millis();
    }
}
