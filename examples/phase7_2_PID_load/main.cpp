/**
 * SCLF Gripper — Phase 7.2: PID Tuning with Load
 * Board:     STM32G474CEU6
 * Framework: Arduino (STM32duino) via PlatformIO
 */
#include <Arduino.h>
#include <SimpleFOC.h>
#include "motor/MotorController.h"
#include "comms/VCP.h"
#include "faults/FaultManager.h"
#include "config/FlashConfig.h"

MotorController mc;
VCPManager vcp;
FaultManager faults(mc);
FlashConfig flash;

void setup() {
    Serial.begin(115200);
    while (!Serial && millis() < 3000);

    Serial.println("\n\n========================================");
    Serial.println("  SCLF Gripper — PHASE 7.2              ");
    Serial.println("  PID Tuning with Load & Current FOC    ");
    Serial.println("========================================");

    // 1. Inicializar Hardware (Driver, Encoder, CurrentSense)
    if (mc.begin()) {
        Serial.println("[MAIN] Motor Controller Hardware: OK");
        
        // Imprimir offsets calibrados
        Serial.print("[DIAG] Current offsets: A=");
        Serial.print(mc.getCurrentSense().offset_ia);
        Serial.print(" B=");
        Serial.print(mc.getCurrentSense().offset_ib);
        Serial.print(" C=");
        Serial.println(mc.getCurrentSense().offset_ic);
    } else {
        Serial.println("[MAIN] CRITICAL: Motor Controller Hardware FAILED");
        while(1);
    }

    // Diagnóstico inmediato del DRV8316
    Serial.println("[DIAG] DRV8316 register dump at startup:");
    mc.dumpRegisters();

    // 2. Intentar cargar calibración previa de Flash
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

    // 3. Configurar controladores FOC de Corriente
    // Configuramos PI de corriente conservadores
    mc.getMotor().PID_current_q.P = 0.5f;
    mc.getMotor().PID_current_q.I = 10.0f;
    mc.getMotor().PID_current_d.P = 0.5f;
    mc.getMotor().PID_current_d.I = 10.0f;
    mc.getMotor().LPF_current_q.Tf = 0.01f;
    mc.getMotor().LPF_current_d.Tf = 0.01f;
    
    // Cambiar control de torque a corriente FOC (si la lectura de corriente falló en begin(), esto puede no funcionar bien)
    mc.getMotor().torque_controller = TorqueControlType::foc_current;

    // 4. Inicializar FOC
    if (mc.initFOC()) {
        Serial.println("[MAIN] FOC Initialized successfully.");
        Serial.print("[CAL] zero_electric_angle = ");
        Serial.println(mc.getMotor().zero_electric_angle, 4);
        Serial.print("[CAL] sensor_direction    = ");
        Serial.println((int)mc.getMotor().sensor_direction);
    } else {
        Serial.println("[MAIN] CRITICAL ERROR: FOC Initialization failed.");
        while(1);
    }

    // 5. Configurar comunicaciones
    vcp.begin();
    vcp.registerMotor(mc);

    // Comando para guardar la configuración
    vcp.addCommand('S', [](char* cmd) {
        mc.getMotor().disable();
        flash.updateFrom(mc);
        if (flash.save(flash.getConfig())) {
            Serial.println("[FLASH] Config saved! Press RESET to reload cleanly.");
        }
        mc.getMotor().enable();
        mc.getMotor().target = mc.getMotor().shaft_angle;
    }, "Save configuration");

    vcp.addCommand('D', [](char* cmd) { mc.dumpRegisters(); }, "Dump DRV registers");

    vcp.addCommand('F', [](char* cmd) {
        faults.clearFault();
        mc.getMotor().enable();
    }, "Clear faults and re-enable");

    // Comando X: pausar/reanudar detector de STALL
    vcp.addCommand('X', [](char* cmd) {
        bool newState = !faults.isStallEnabled();
        faults.setStallEnabled(newState);
        Serial.print("[DIAG] STALL detector: ");
        Serial.println(newState ? "ENABLED" : "DISABLED (tuning mode)");
    }, "Toggle STALL detector");

    // Inicializamos en modo Torque (Current FOC) con target=0
    mc.setMode(MotionControlType::torque);
    mc.getMotor().target = 0.0f;

    Serial.println("[MAIN] System Ready. Motor in TORQUE mode (Current FOC, target=0).");
    Serial.println("Commands: M=move, C=calibrate, F=clearfault, S=save, X=toggle-stall, T1=telemetry");
}

void loop() {
    // A. Bucle FOC
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
