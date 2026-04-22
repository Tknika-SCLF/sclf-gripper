#include <Arduino.h>
#include <SimpleFOC.h>
#include "motor/MotorController.h"
#include "comms/VCP.h"
#include "faults/FaultManager.h"
#include "config/FlashConfig.h"

/**
 * PHASE 6: Persistence & Configuration
 * ------------------------------------
 * Este ejemplo valida el almacenamiento de parámetros en Flash.
 */

MotorController mc;
VCPManager vcp;
FaultManager faults(mc);
FlashConfig flash;

void setup() {
    Serial.begin(115200);
    while (!Serial && millis() < 3000);

    Serial.println("\n\n========================================");
    Serial.println("  SCLF Gripper v2.0 — PHASE 6           ");
    Serial.println("  Persistence & Flash Config Test       ");
    Serial.println("========================================");

    // 1. Inicializar Hardware (necesario para tener objetos creados)
    if (mc.begin()) {
        Serial.println("[MAIN] Motor Controller: OK");
    }

    // 2. Cargar Configuración desde Flash
    if (flash.load()) {
        Serial.println("[FLASH] Config loaded successfully.");
        flash.applyTo(mc);
    } else {
        Serial.println("[FLASH] No valid config found. Initializing with defaults.");
        flash.resetToDefaults();
        flash.applyTo(mc);
    }

    // 3. Inicializar Comunicaciones y Seguridad
    vcp.begin();
    vcp.registerMotor(mc);
    
    // 4. Añadir comando 'S' para guardar en Flash
    vcp.addCommand('S', [](char* cmd) { 
        flash.updateFrom(mc);
        if (flash.save(flash.getConfig())) {
            Serial.println("[FLASH] Config SAVED to Flash successfully.");
        } else {
            Serial.println("[FLASH] CRITICAL ERROR saving to Flash.");
        }
    }, "Save current config to Flash");

    // 5. Añadir comando 'R' para resetear a fábrica
    vcp.addCommand('R', [](char* cmd) {
        flash.resetToDefaults();
        flash.save(flash.getConfig());
        Serial.println("[FLASH] Config RESET to defaults. Restart board.");
    }, "Reset to factory defaults");

    Serial.println("[MAIN] System Ready.");
    Serial.println("Cambie algún parámetro (ej. 'MV P0.5') y use 'S' para guardar.");
    Serial.println("Luego reinicie la placa para verificar que el cambio persiste.");
}

void loop() {
    mc.update();
    faults.update();
    vcp.update();

    static uint32_t last_led = 0;
    if (millis() - last_led > 500) {
        digitalWrite(PIN_LED, !digitalRead(PIN_LED));
        last_led = millis();
    }
}
