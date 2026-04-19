#include <Arduino.h>
#include "motor/MotorController.h"

MotorController::MotorController()
    : _encoder(),
      _motor(11), // GM3506 (11 pole pairs)
      _driver(PIN_CH, PIN_CL, PIN_BH, PIN_BL, PIN_AH, PIN_AL) {
}

bool MotorController::begin() {
    _initHardware();
    _configureMotor();

    // 1. Inicializar el driver del chip DRV8316
    SPI.setSCLK(PIN_DRV_CLK);
    SPI.setMISO(PIN_DRV_MISO);
    SPI.setMOSI(PIN_DRV_MOSI);
    SPI.begin();

    if (!_drv.begin(&SPI)) {
        SimpleFOCDebug::println("MC: DRV8316 Init FAILED");
        return false;
    }
    _drv.setCSAGain(DRV8316_BIT::CSA_GAIN_0V3);
    
    // Asegurar modo 6-PWM y driver habilitado (Control 3: PWM_MODE=00, DRV_OFF=0)
    uint16_t ctrl3 = _drv.readRegister(DRV8316_REG::REG_CONTROL3);
    ctrl3 &= ~0x07; // Limpiar bits 2:0 (DRV_OFF y PWM_MODE)
    _drv.writeRegister(DRV8316_REG::REG_CONTROL3, ctrl3);
    
    _drv.clearFaults();

    // 2. Inicializar encoder (bit-banging)
    if (!_encoder.begin()) {
        SimpleFOCDebug::println("MC: MT6701 Init FAILED");
        // No salimos con error aquí necesariamente, el lazo abierto puede funcionar sin encoder
    }

    // 3. Inicializar Driver SimpleFOC
    _driver.voltage_power_supply = 24.0f;
    _driver.init();
    _motor.linkDriver(&_driver);

    // 4. Inicializar Motor SimpleFOC
    _motor.linkSensor(&_encoder);
    _motor.init();

    // 5. Alineación FOC (Forzamos detección de dirección)
    SimpleFOCDebug::println("MC: Force calibrating FOC...");
    _motor.sensor_direction = Direction::UNKNOWN;
    _motor.initFOC();

    if (_motor.motor_status == FOCMotorStatus::motor_ready) {
        SimpleFOCDebug::println("MC: FOC Ready!");
    } else {
        SimpleFOCDebug::println("MC: FOC Alignment FAILED");
    }

    return true;
}

void MotorController::update() {
    _motor.loopFOC();
    _motor.move();
}

void MotorController::setTarget(float target) {
    _motor.target = target;
}

void MotorController::setMode(MotionControlType mode) {
    _motor.controller = mode;
}

void MotorController::enable(bool en) {
    if (en) _motor.enable();
    else _motor.disable();
}

void MotorController::dumpRegisters() {
    SimpleFOCDebug::println("--- DRV8316 Register Dump ---");
    for (uint8_t i = 0; i <= 0x0B; i++) {
        uint16_t val = _drv.readRegister(i);
        Serial.print("Reg 0x");
        if (i < 0x10) Serial.print("0");
        Serial.print(i, HEX);
        Serial.print(": 0x");
        if (val < 0x100) Serial.print("0");
        if (val < 0x10) Serial.print("0");
        Serial.println(val, HEX);
    }
    
    Serial.print("Motor Status: "); Serial.println((int)_motor.motor_status);
    Serial.print("Sensor Direction: "); Serial.println((int)_motor.sensor_direction);
    Serial.print("Zero Elec Angle: "); Serial.println(_motor.zero_electric_angle, 4);
    SimpleFOCDebug::println("-----------------------------");
}

void MotorController::forceCalibration() {
    _motor.sensor_direction = Direction::UNKNOWN;
    _motor.initFOC();
}

void MotorController::_initHardware() {
    // LED de diagnóstico o pines extra si fuera necesario
    pinMode(PIN_LED, OUTPUT);
}

void MotorController::_configureMotor() {
    // Límites de seguridad ajustados a la fuente de alimentación (1.5A)
    _motor.voltage_limit = 8.0f;
    _driver.voltage_limit = 8.0f;
    _motor.velocity_limit = 20.0f;
    
    // Voltaje de alineación para superar fricción - Aumentado un poco para asegurar giro
    _motor.voltage_sensor_align = 6.0f;

    // Ganancias PID para velocidad (por defecto)
    _motor.PID_velocity.P = 0.5f;
    _motor.PID_velocity.I = 5.0f;
    _motor.PID_velocity.D = 0.0f;
    _motor.LPF_velocity.Tf = 0.01f;

    // Modo por defecto: Lazo Abierto (Velocidad)
    _motor.controller = MotionControlType::velocity_openloop;
}
