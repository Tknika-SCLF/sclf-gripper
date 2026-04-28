#include <Arduino.h>
#include <SimpleFOC.h>
#include "motor/MotorController.h"
#include "comms/VCP.h"
#include "faults/FaultManager.h"
#include "config/FlashConfig.h"

/**
 * PHASE 7.1: Motor-Encoder Alignment (Closed Loop Bench Test)
 * ----------------------------------------------------------
 * Este código realiza la calibración automática del sensor y motor.
 */

MotorController mc;
VCPManager vcp;
FaultManager faults(mc);
FlashConfig flash;

void setup() {
    Serial.begin(115200);
    while (!Serial && millis() < 3000);

    Serial.println("\n\n========================================");
    Serial.println("  SCLF Gripper v2.0 — PHASE 7.1         ");
    Serial.println("  Motor-Encoder Alignment Test          ");
    Serial.println("========================================");

    // 1. Inicializar Hardware
    if (mc.begin()) {
        Serial.println("[MAIN] Motor Controller Hardware: OK");
    }

    // 2. Intentar cargar calibración previa de Flash
    if (flash.load()) {
        Serial.println("[FLASH] Calibration found. Applying...");
        flash.applyTo(mc);
    } else {
        Serial.println("[FLASH] No calibration found. Will perform alignment.");
    }

    // 3. Inicializar FOC (Alineación eléctrica)
    // Si ya cargamos de flash, initFOC() usará esos valores y no se moverá.
    // Si no, realizará la secuencia de calibración.
    if (mc.initFOC()) {
        Serial.println("[MAIN] FOC Initialized successfully.");
    } else {
        Serial.println("[MAIN] CRITICAL ERROR: FOC Initialization failed.");
        while(1);
    }

    // 4. Configurar comunicaciones
    vcp.begin();
    vcp.registerMotor(mc);
    
    // Comando para guardar la calibración encontrada
    vcp.addCommand('S', [](char* cmd) {
        flash.updateFrom(mc);
        if (flash.save(flash.getConfig())) {
            Serial.println("[FLASH] Calibration saved! Restart to verify.");
        }
    }, "Save calibration");

    // Comando para resetear fallos
    vcp.addCommand('F', [](char* cmd) { faults.clearFault(); }, "Clear faults");

    // Comando para forzar recalibración
    vcp.addCommand('C', [](char* cmd) { 
        Serial.println("[MAIN] Forcing FOC Calibration...");
        mc.getMotor().disable();
        mc.forceCalibration(); 
        mc.getMotor().enable();
        mc.getMotor().target = mc.getMotor().shaft_angle;
    }, "Force Calibration");

    // Comando para resetear a fábrica (limpiar Flash)
    vcp.addCommand('R', [](char* cmd) {
        flash.resetToDefaults();
        flash.save(flash.getConfig());
        Serial.println("[FLASH] Config RESET to defaults. Restart board.");
    }, "Reset to factory defaults");

    // Iniciar en modo posición por defecto para test
    mc.setMode(MotionControlType::angle);
    mc.getMotor().target = mc.getMotor().shaft_angle; // Mantener posición actual

    Serial.println("[MAIN] System Ready. Motor in ANGLE mode.");
    Serial.println("Commands: 'M [target]' to move, 'C' to calibrate, 'S' to save, 'R' to reset flash.");
}

void loop() {
    // A. Bucle FOC (Máxima frecuencia)
    mc.update();

    // B. Seguridad y Comunicaciones
    faults.update();
    vcp.update();

    static uint32_t last_led = 0;
    if (millis() - last_led > 200) {
        digitalWrite(PIN_LED, !digitalRead(PIN_LED));
        last_led = millis();
    }
}
