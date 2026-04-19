#pragma once

#include <SimpleFOC.h>
#include "config/pins.h"
#include "motor/DRV8316.h"
#include "encoder/MT6701.h"

/**
 * @brief MotorController — Wrapper para SimpleFOC adaptado al SCLF Gripper.
 * 
 * Centraliza la gestión del motor BLDC, el driver y el encoder.
 */
class MotorController {
public:
    MotorController();

    /**
     * @brief Inicializa el driver, el motor y el sensor.
     * @return true si la inicialización básica fue correcta.
     */
    bool begin();

    /**
     * @brief Ejecuta el bucle de control FOC. Debe llamarse en el loop() principal.
     */
    void update();

    /**
     * @brief Establece el valor objetivo (depende del modo de control).
     */
    void setTarget(float target);

    /**
     * @brief Cambia el modo de control (ej. MotionControlType::velocity_openloop).
     */
    void setMode(MotionControlType mode);

    /**
     * @brief Habilita o deshabilita el motor.
     */
    void enable(bool en);

    // Acceso a los objetos subyacentes (para Commander u otros diagnósticos)
    BLDCMotor& getMotor() { return _motor; }
    BLDCDriver6PWM& getDriver() { return _driver; }
    MT6701& getEncoder() { return _encoder; }
    DRV8316& getDrv() { return _drv; }

private:
    DRV8316 _drv;
    MT6701 _encoder;
    BLDCMotor _motor;
    BLDCDriver6PWM _driver;

    void _initHardware();
    void _configureMotor();
};
