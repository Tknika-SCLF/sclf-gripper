#include <Arduino.h>
#include <SimpleFOC.h>
#include "motor/MotorController.h"
#include "comms/VCP.h"
#include "faults/FaultManager.h"

/**
 * PHASE 5: Fault Manager
 * ----------------------
 * Este ejemplo valida la detección de errores y la transición a estado seguro.
 * El FaultManager monitoriza el driver, el encoder y detecta bloqueos.
 */

MotorController mc;
VCPManager vcp;
FaultManager faults(mc);

void setup() {
    Serial.begin(115200);
    while (!Serial && millis() < 3000);

    Serial.println("\n\n========================================");
    Serial.println("  SCLF Gripper v2.0 — PHASE 5           ");
    Serial.println("  Fault Manager & Safety Test           ");
    Serial.println("========================================");

    // 1. Inicializar Hardware
    if (mc.begin()) {
        Serial.println("[MAIN] Motor Controller: OK");
    }

    // 2. Inicializar Comunicaciones
    vcp.begin();
    vcp.registerMotor(mc);
    
    // 3. Iniciar en modo velocidad para pruebas
    mc.setMode(MotionControlType::velocity);
    mc.getMotor().target = 0;

    // 4. Registrar comando para limpiar fallos
    vcp.addCommand('F', [](char* cmd) { faults.clearFault(); }, "Clear faults");

    Serial.println("[MAIN] System Ready. Monitoring faults...");
    Serial.println("Pruebe a bloquear el motor o desconectar el encoder para ver la reacción.");
    Serial.println("Si el motor entra en SAFE STATE, envíe 'F' para rearmar, y luego 'ME1' para encender.");
}

void loop() {
    // A. Bucle FOC (Crítico)
    mc.update();

    // B. Monitorización de Seguridad
    faults.update();

    // C. Gestión de Comandos y Telemetría
    vcp.update();

    // D. Feedback Visual (Parpadeo rápido en caso de error)
    static uint32_t last_led = 0;
    uint32_t led_period = faults.hasFault() ? 50 : 500;
    
    if (millis() - last_led > led_period) {
        digitalWrite(PIN_LED, !digitalRead(PIN_LED));
        last_led = millis();
    }
}
