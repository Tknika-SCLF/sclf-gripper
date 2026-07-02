#pragma once

#include <Arduino.h>
#include <SPI.h>
#include <SimpleFOC.h>
#include "config/pins.h"
#include "motor/DRV8316.h"
#include "encoder/MT6701.h"
#include "motor/CurrentSense.h"

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
     * @brief Realiza la alineación FOC (eléctrica) entre motor y sensor.
     * @return true si la alineación fue exitosa.
     */
    bool initFOC();

    /**
     * @brief Establece el valor objetivo (depende del modo de control).
     */
    void setTarget(float target);

    /**
     * @brief Cambia el modo de control (ej. MotionControlType::velocity_openloop).
     */
    void setMode(MotionControlType mode);

    /**
     * @brief Vuelca el estado de todos los registros del DRV8316 por puerto serie.
     */
    void dumpRegisters();

    /**
     * @brief Fuerza la recalibración del sensor (alineación FOC).
     */
    void forceCalibration();

    /**
     * @brief Habilita o deshabilita el motor.
     */
    void enable(bool en);

    // Acceso a los objetos subyacentes (para Commander u otros diagnósticos)
    BLDCMotor& getMotor() { return _motor; }
    BLDCDriver6PWM& getDriver() { return _driver; }
    MT6701& getEncoder() { return _encoder; }
    DRV8316& getDrv() { return _drv; }
    SCLF_CurrentSense& getCurrentSense() { return _current_sense; }

private:
    // SPI dedicado para el DRV8316 (PB5=MOSI, PB4=MISO, PB3=SCK).
    // CRÍTICO: No usar el objeto SPI global para evitar conflicto con
    // el bit-banging del encoder MT6701 en PA5/PA6 (pines SPI por defecto).
    SPIClass _spi_drv;

    DRV8316 _drv;
    MT6701 _encoder;
    BLDCMotor _motor;
    BLDCDriver6PWM _driver;
    SCLF_CurrentSense _current_sense;

    void _initHardware();
    void _configureMotor();
};
