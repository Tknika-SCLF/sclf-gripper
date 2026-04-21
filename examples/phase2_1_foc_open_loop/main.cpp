#include <Arduino.h>
#include <SimpleFOC.h>
#include "motor/MotorController.h"

// Instancia global del controlador
MotorController mc;

// Comandante serie
Commander commander = Commander(Serial);
void onMotor(char* cmd){ commander.motor(&mc.getMotor(), cmd); }

// Comando personalizado para cambiar el modo de control: MC <0:torque, 1:vel, 2:pos, 3:vel_open, 4:angle_open>
void onControlMode(char* cmd) {
    int mode = atoi(cmd);
    switch(mode) {
        case 0: mc.setMode(MotionControlType::torque); break;
        case 1: mc.setMode(MotionControlType::velocity); break;
        case 2: mc.setMode(MotionControlType::angle); break;
        case 3: mc.setMode(MotionControlType::velocity_openloop); break;
        case 4: mc.setMode(MotionControlType::angle_openloop); break;
        default: Serial.println("Modo no valido (0-4)"); return;
    }
    Serial.print("Control Mode: "); Serial.println(mode);
}

void setup() {
    // 1. Comunicación Serie (USB-C)
    Serial.begin(115200);
    while(!Serial && millis() < 3000); 
    SimpleFOCDebug::enable(&Serial); 

    Serial.println("\n\n========================================");
    Serial.println("  SCLF Gripper v1.0 — PHASE 2.1         ");
    Serial.println("  FOC Test (Open-Loop Priority)        ");
    Serial.println("========================================");

    // 2. Inicialización del MotorController
    if (mc.begin()) {
        Serial.println("[MC] System Initialized OK.");
    } else {
        Serial.println("[MC] CRITICAL: Initialization FAILED.");
    }

    // 3. Configurar Comandos
    commander.add('M', onMotor, "motor");
    commander.add('C', onControlMode, "control mode (0-4)");
    
    Serial.println("System Ready.");
    Serial.println("Commands:");
    Serial.println("  M<val>  : Set velocity/target");
    Serial.println("  ME<0/1> : Disable/Enable motor");
    Serial.println("  MC<0-4> : Mode (3: Vel Open-Loop, 1: Vel Closed-Loop)");
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
