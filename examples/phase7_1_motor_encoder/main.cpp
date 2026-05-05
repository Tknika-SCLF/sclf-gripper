/**
 * SCLF Gripper — Phase 7.1: Motor-Encoder Alignment
 * Board:     STM32G474CEU6
 * Framework: Arduino (STM32duino) via PlatformIO
 */
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
    Serial.println("  SCLF Gripper — PHASE 7.1              ");
    Serial.println("  Motor-Encoder Alignment Test          ");
    Serial.println("========================================");

    // 1. Inicializar Hardware
    if (mc.begin()) {
        Serial.println("[MAIN] Motor Controller Hardware: OK");
    } else {
        Serial.println("[MAIN] CRITICAL: Motor Controller Hardware FAILED");
        while(1);
    }

    // Diagnóstico inmediato del DRV8316
    Serial.println("[DIAG] DRV8316 register dump at startup:");
    mc.dumpRegisters();

    // 2. Intentar cargar calibración previa de Flash
    // Solo es válida si sensor_direction != UNKNOWN (0)
    bool calibrationValid = false;
    if (flash.load()) {
        if (flash.getConfig().sensor_direction != 0) {
            Serial.println("[FLASH] Valid calibration found. Applying...");
            flash.applyTo(mc);
            calibrationValid = true;
        } else {
            Serial.println("[FLASH] Flash data has UNKNOWN direction — will calibrate.");
        }
    } else {
        Serial.println("[FLASH] No calibration found. Will perform alignment.");
    }

    // 3. Inicializar FOC (Alineación eléctrica)
    if (mc.initFOC()) {
        Serial.println("[MAIN] FOC Initialized successfully.");
        // Imprimir valores de calibración determinados por SimpleFOC
        Serial.print("[CAL] zero_electric_angle = ");
        Serial.println(mc.getMotor().zero_electric_angle, 4);
        Serial.print("[CAL] sensor_direction    = ");
        Serial.println((int)mc.getMotor().sensor_direction); // 1=CW, -1=CCW
        if (!calibrationValid) {
            Serial.println("[MAIN] Run 'S' to save this calibration to Flash.");
        }
    } else {
        Serial.println("[MAIN] CRITICAL ERROR: FOC Initialization failed.");
        Serial.println("[MAIN] Check motor connections and 24V supply.");
        mc.dumpRegisters();
        while(1);
    }

    // 4. Configurar comunicaciones
    vcp.begin();
    vcp.registerMotor(mc);

    // Comando para guardar la calibración encontrada
    vcp.addCommand('S', [](char* cmd) {
        // Deshabilitar motor durante la escritura en Flash para evitar desincronización FOC
        // (HAL_FLASH_Program bloquea el loop ~30ms)
        mc.getMotor().disable();
        flash.updateFrom(mc);
        if (flash.save(flash.getConfig())) {
            Serial.println("[FLASH] Calibration saved! Press RESET to reload cleanly.");
        }
        mc.getMotor().enable();
        mc.getMotor().target = mc.getMotor().shaft_angle;
    }, "Save calibration");

    // Comando para volcar registros del DRV8316 (diagnóstico)
    vcp.addCommand('D', [](char* cmd) { mc.dumpRegisters(); }, "Dump DRV registers");

    // Comando para resetear fallos
    vcp.addCommand('F', [](char* cmd) {
        faults.clearFault();
        mc.getMotor().enable();
    }, "Clear faults and re-enable");

    // Comando para forzar recalibración
    vcp.addCommand('C', [](char* cmd) {
        Serial.println("[MAIN] Forcing FOC Calibration...");
        mc.getMotor().disable();
        mc.forceCalibration();
        Serial.print("[CAL] zero_electric_angle = ");
        Serial.println(mc.getMotor().zero_electric_angle, 4);
        Serial.print("[CAL] sensor_direction    = ");
        Serial.println((int)mc.getMotor().sensor_direction);
        mc.getMotor().enable();
        mc.getMotor().target = mc.getMotor().shaft_angle;
    }, "Force Calibration");

    // Comando O: Open-Loop velocity (para verificar que el driver funciona sin FOC)
    vcp.addCommand('O', [](char* cmd) {
        float vel = atof(cmd);
        mc.setMode(MotionControlType::velocity_openloop);
        mc.setTarget(vel);
        Serial.print("[DIAG] Open-loop velocity: ");
        Serial.println(vel);
    }, "Open-loop velocity test (O3 = 3rad/s)");

    // Comando Z: Calcular zero_electric_angle correcto durante open-loop
    // Uso: 1) enviar O3 (motor girando), 2) enviar Z para calcular el offset correcto
    // En open-loop: el_angle_OL = pole_pairs * shaft_angle (sin offset)
    // En closed-loop: el_angle_CL = dir * pole_pairs * mech_angle + zero_electric_angle
    // Para que coincidan: zero_electric_angle = pole_pairs*(shaft_angle - dir*mech_angle)
    vcp.addCommand('Z', [](char* cmd) {
        float mech  = mc.getMotor().sensor->getMechanicalAngle();
        float shaft = mc.getMotor().shaft_angle;
        float dir   = (float)(int)mc.getMotor().sensor_direction;
        float pp    = (float)mc.getMotor().pole_pairs;

        // El ángulo eléctrico que usa open-loop
        float el_ol = fmod(pp * shaft, 2.0f * PI);
        if (el_ol < 0) el_ol += 2.0f * PI;

        // El ángulo eléctrico que calcula closed-loop con el ZEA actual
        float el_cl = mc.getMotor().electricalAngle();

        // El ZEA correcto que haría coincidir ambos ángulos
        float new_zea = mc.getMotor().zero_electric_angle + (el_ol - el_cl);
        while (new_zea > 2.0f * PI) new_zea -= 2.0f * PI;
        while (new_zea < 0)         new_zea += 2.0f * PI;

        Serial.print("[Z] mech_angle    = "); Serial.println(mech,  4);
        Serial.print("[Z] shaft_angle   = "); Serial.println(shaft, 4);
        Serial.print("[Z] el_openloop   = "); Serial.println(el_ol, 4);
        Serial.print("[Z] el_sensor     = "); Serial.println(el_cl, 4);
        Serial.print("[Z] current ZEA   = "); Serial.println(mc.getMotor().zero_electric_angle, 4);
        Serial.print("[Z] suggested ZEA = "); Serial.println(new_zea, 4);
        Serial.println("[Z] Send 'A<value>' to apply, e.g. A3.14");
    }, "Compute correct zero_electric_angle (run during O3)");

    // Comando A: Aplicar zero_electric_angle manualmente y probar torque
    vcp.addCommand('A', [](char* cmd) {
        float zea = atof(cmd);
        mc.getMotor().zero_electric_angle = zea;
        Serial.print("[A] zero_electric_angle applied: ");
        Serial.println(zea, 4);
        Serial.println("[A] Now test: M C 0 then M 2");
    }, "Apply zero_electric_angle (A3.14)");

    // Comando para resetear a fábrica (limpiar Flash)
    vcp.addCommand('R', [](char* cmd) {
        flash.resetToDefaults();
        flash.save(flash.getConfig());
        Serial.println("[FLASH] Config RESET to defaults. Restart board.");
    }, "Reset to factory defaults");

    // Arrancar en VELOCIDAD CERRADA con target=0 (motor mantiene posición sin que STALL dispare)
    // Protocolo de tuning:
    //   1. M 1      → 1 rad/s (lazo vel cerrado)
    //   2. M V P x  → ajustar P hasta rotación suave
    //   3. M C 0    → cambiar a modo posición
    //   4. M 1.57   → ir a 90°
    mc.setMode(MotionControlType::velocity);
    mc.getMotor().target = 0.0f;

    // Comando X: pausar/reanudar detector de STALL (solo para tuning en banco)
    vcp.addCommand('X', [](char* cmd) {
        bool newState = !faults.isStallEnabled();
        faults.setStallEnabled(newState);
        Serial.print("[DIAG] STALL detector: ");
        Serial.println(newState ? "ENABLED" : "DISABLED (tuning mode)");
    }, "Toggle STALL detector (for tuning)");

    Serial.println("[MAIN] System Ready. Motor in VELOCITY mode (target=0).");
    Serial.println("Commands: M=move, C=calibrate, O=openloop, F=clearfault, S=save, R=reset, D=dump, X=toggle-stall.");

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
