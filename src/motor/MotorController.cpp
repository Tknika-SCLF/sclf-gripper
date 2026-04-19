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

    // 5. Alineación FOC (solo si no es open-loop, pero lo llamamos siempre para estar listos)
    // Nota: Si el modo inicial es open-loop, initFOC no hará mucho o fallará suavemente.
    _motor.initFOC();

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

void MotorController::_initHardware() {
    // LED de diagnóstico o pines extra si fuera necesario
    pinMode(PIN_LED, OUTPUT);
}

void MotorController::_configureMotor() {
    // Límites de seguridad ajustados a la fuente de alimentación (1.5A)
    _motor.voltage_limit = 8.0f;
    _driver.voltage_limit = 8.0f;
    _motor.velocity_limit = 20.0f;
    
    // Voltaje de alineación para superar fricción
    _motor.voltage_sensor_align = 5.0f;

    // Ganancias PID para velocidad (por defecto)
    _motor.PID_velocity.P = 0.5f;
    _motor.PID_velocity.I = 5.0f;
    _motor.PID_velocity.D = 0.0f;
    _motor.LPF_velocity.Tf = 0.01f;

    // Modo por defecto: Lazo Abierto (Velocidad)
    _motor.controller = MotionControlType::velocity_openloop;
}
