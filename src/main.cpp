/**
 * SCLF Gripper v1.0 — Firmware Hardware Initialization
 * Board:     STM32G474CEU6
 * Framework: Arduino (STM32duino) via PlatformIO
 * Library:   SimpleFOC
 *
 * ─── FASE 1 — Hardware Configuration & FOC Init ────────────────────────────
 * Objetivo: inicializar el sensor MT6701, configurar el DRV8316C en modo
 * 3-PWM a través de SPI e iniciar la librería SimpleFOC en bucle abierto o cerrado.
 * ───────────────────────────────────────────────────────────────────────────
 */

#include "config/pins.h"

#include <Arduino.h>
#include <SPI.h>

#include <SimpleFOC.h>

#include "communication/SimpleFOCDebug.h"

// ─── Instancias de Hardware ──────────────────────────────────────────────────
MagneticSensorSPI sensor = MagneticSensorSPI(PIN_ENC_CS, 14, 0x0000);
BLDCMotor motor = BLDCMotor(11);  // 11 pares de polos por defecto (ajustar según el motor real)
BLDCDriver3PWM driver = BLDCDriver3PWM(PIN_AH, PIN_BH, PIN_CH);

// Buses SPI separados para el sensor y el driver
SPIClass SPI_DRV(PIN_DRV_MOSI, PIN_DRV_MISO, PIN_DRV_CLK);
SPIClass SPI_ENC(NC, PIN_ENC_SDO, PIN_ENC_CLK);

// ─── Funciones Auxiliares ────────────────────────────────────────────────────
void configureDRV8316() {
    // Configura el DRV8316 en modo 3-PWM vía SPI
    // Trama SPI: 16 bits -> [15] W0/R1 | [14:9] ADDR | [8:0] DATA
    // Dirección 0x02 (PWM_CTRL_1), Datos: [1:0] = 01b (3-PWM mode)
    uint16_t cmd = 0x0000;
    cmd |= (0 << 15);    // Comando de escritura
    cmd |= (0x02 << 9);  // Registro 0x02 (PWM_CTRL_1)
    cmd |= (0x01);       // Dato: PWM_MODE = 01b (3-PWM)

    // El DRV8316 usa SPI_MODE1 (CPOL=0, CPHA=1) hasta 10 MHz
    SPI_DRV.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE1));
    digitalWrite(PIN_DRV_CS, LOW);
    SPI_DRV.transfer16(cmd);
    digitalWrite(PIN_DRV_CS, HIGH);
    SPI_DRV.endTransaction();

    Serial.println("  [DRV8316] Configurado en 3-PWM Mode.");
}

// ─── setup() ──────────────────────────────────────────────────────────────────
void setup() {
    Serial.begin(115200);
    uint32_t t = millis();
    while (!Serial && (millis() - t) < 3000) {
    }

    Serial.println("========================================");
    Serial.println("  SCLF Gripper v1.0 — Hardware Init     ");
    Serial.println("========================================");

    // 1. Configurar pines auxiliares
    pinMode(PIN_DRV_CS, OUTPUT);
    digitalWrite(PIN_DRV_CS, HIGH);

    pinMode(PIN_LED, OUTPUT);
    digitalWrite(PIN_LED, LOW);

    pinMode(PIN_RS485_DIR, OUTPUT);
    digitalWrite(PIN_RS485_DIR, LOW);  // Modo RX por defecto

    // 2. Inicializar buses SPI
    SPI_DRV.begin();
    SPI_ENC.begin();

    // 3. Configurar DRV8316 vía SPI a 3-PWM
    configureDRV8316();

    // 4. Inicializar Sensor (MT6701)
    sensor.init(&SPI_ENC);
    motor.linkSensor(&sensor);
    Serial.println("  [Sensor] MT6701 inicializado.");

    // 5. Inicializar Driver (SimpleFOC)
    driver.voltage_power_supply = 24.0;  // Voltaje del bus
    driver.init();
    motor.linkDriver(&driver);
    Serial.println("  [Driver] SimpleFOC 3-PWM inicializado.");

    // 6. Configurar FOC y Motor
    motor.voltage_sensor_align = 2.0;  // Voltaje para la rutina de alineación
    motor.velocity_limit = 10.0;       // Límite de velocidad
    motor.controller = MotionControlType::angle;

    // Iniciar FOC y alineación
    motor.init();
    motor.initFOC();

    Serial.println("  [SimpleFOC] Init completado.");
    Serial.println("========================================");
}

// ─── loop() ───────────────────────────────────────────────────────────────────
void loop() {
    // Mantener la ejecución del FOC en todo momento
    motor.loopFOC();
    motor.move(0);  // Objetivo 0 en ángulo (mantener posición)

    // Telemetría básica a 1 Hz
    static uint32_t last_print = 0;
    if (millis() - last_print > 1000) {
        Serial.print("Angulo Sensor: ");
        Serial.println(sensor.getAngle());

        // Toggle LED
        digitalWrite(PIN_LED, !digitalRead(PIN_LED));
        last_print = millis();
    }
}
