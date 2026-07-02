#include <Arduino.h>
#include "motor/MotorController.h"

MotorController::MotorController()
    : _encoder(),
      _motor(11), // GM3506 (11 pole pairs)
      _driver(PIN_CH, PIN_CL, PIN_BH, PIN_BL, PIN_AH, PIN_AL),
      // SPI dedicado en PB5(MOSI), PB4(MISO), PB3(SCK) — nunca toca PA5/PA6
      _spi_drv(PIN_DRV_MOSI, PIN_DRV_MISO, PIN_DRV_CLK),
      _current_sense(_drv) {
}

bool MotorController::begin() {
    _initHardware();
    _configureMotor();

    // 1. Inicializar el driver del chip DRV8316
    // Usar instancia SPI dedicada (PB3/PB4/PB5) para no interferir con
    // el bit-banging del encoder MT6701 en PA5/PA6.
    _spi_drv.begin();

    if (!_drv.begin(&_spi_drv)) {
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

    // 3.5 Inicializar sensor de corriente y vincularlo
    _current_sense.getSimpleFOC().linkDriver(&_driver);
    if (!_current_sense.begin()) {
        SimpleFOCDebug::println("MC: CurrentSense Init FAILED");
    } else {
        _motor.linkCurrentSense(&_current_sense.getSimpleFOC());
    }

    // ⚠️ CRÍTICO: Re-forzar GPIO en los pines del encoder DESPUÉS de driver.init().
    // En STM32G474, PA6 = TIM1_BKIN (AF). Al inicializar TIM1 para 6-PWM,
    // STM32duino puede reconfigurarlo como función alternativa, rompiendo
    // el bit-banging del MT6701. pinMode() fuerza el modo GPIO de vuelta.
    pinMode(PIN_ENC_CS,  OUTPUT); digitalWrite(PIN_ENC_CS, HIGH);
    pinMode(PIN_ENC_CLK, OUTPUT); digitalWrite(PIN_ENC_CLK, LOW);
    pinMode(PIN_ENC_SDO, INPUT);

    // 4. Inicializar Motor SimpleFOC
    _motor.linkSensor(&_encoder);
    _motor.init();

    return true;
}

bool MotorController::initFOC() {
    SimpleFOCDebug::println("MC: Initializing FOC...");
    
    _motor.initFOC();
    _motor.enable();

    if (_motor.motor_status == FOCMotorStatus::motor_ready) {
        SimpleFOCDebug::println("MC: FOC Ready!");
        return true;
    } else {
        SimpleFOCDebug::println("MC: FOC Initialization FAILED");
        return false;
    }
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
    
    // Voltaje de alineación — suficiente para superar fricción y detent torque
    _motor.voltage_sensor_align = 10.0f;

    // ── PIDs — Valores conservadores para primer tuning ─────────────────────
    // GM3506: motor de gimbal, alta reluctancia, sensible a ganancias altas.
    // I=0 hasta encontrar P estable — el windup del integral bloquea el motor.
    _motor.PID_velocity.P  = 0.15f;
    _motor.PID_velocity.I  = 0.0f;   // ← CRÍTICO: I=0 hasta tuning estable
    _motor.PID_velocity.D  = 0.0f;
    _motor.LPF_velocity.Tf = 0.05f;  // Filtro más suave — reduce ruido del encoder

    // Lazo de posición — ajustar solo tras tener velocidad estable
    _motor.P_angle.P       = 5.0f;

    // Modo por defecto: velocidad cerrada con target=0 (no mueve al inicio)
    _motor.controller = MotionControlType::velocity;

}
