/**
 * SCLF Gripper v1.0 — Phase 1.5: Current Sense Test
 * Board:     STM32G474CEU6
 * Framework: Arduino (STM32duino) via PlatformIO
 *
 * Objetivo:  Validar la lectura de corriente de las 3 fases (CURA, CURB, CURC).
 *            Configura el DRV8316 para una ganancia de 0.3 V/A y muestra
 *            los valores en Amperios por el puerto serie.
 */

#include <Arduino.h>
#include "config/pins.h"
#include "motor/DRV8316.h"
#include "motor/CurrentSense.h"

// --- Instances ---
SPIClass SPI_DRV(PIN_DRV_MOSI, PIN_DRV_MISO, PIN_DRV_CLK);
DRV8316 drv;
SCLF_CurrentSense cs(drv); 

void setup() {
    // 0. Vital signs
    pinMode(PIN_LED, OUTPUT);
    digitalWrite(PIN_LED, HIGH); delay(200);
    digitalWrite(PIN_LED, LOW); delay(200);
    digitalWrite(PIN_LED, HIGH); delay(200);
    digitalWrite(PIN_LED, LOW);

    // 1. Debug via USB-C (Virtual COM Port)
    Serial.begin(115200);
    // Mas tiempo de espera para el CDC en Windows
    for(int i=0; i<50 && !Serial; i++) { delay(100); }

    Serial.println("\n\n========================================");
    Serial.println("  SCLF Gripper v1.0 — Phase 1.5         ");
    Serial.println("  Current Sense Validation              ");
    Serial.println("========================================");

    // 1.5 Disable Encoder CS to prevent SPI collisions
    pinMode(PIN_ENC_CS, OUTPUT);
    digitalWrite(PIN_ENC_CS, HIGH);
    
    // Explicitly initialize SPI pins for the driver
    SPI_DRV.begin();

    // 2. Initialize Driver (SPI)
    if (drv.begin(&SPI_DRV)) {
        Serial.println("[DRV8316] SPI Initialized OK.");
        Serial.print("[DRV8316] RAW S1: 0x"); Serial.print(drv.getStatus1Raw(), HEX);
        Serial.print(" | RAW S2: 0x"); Serial.println(drv.getStatus2Raw(), HEX);
        
        // Configurar ganancia a 1.2 V/A para mayor resolución en pruebas manuales
        drv.setCSAGain(DRV8316_BIT::CSA_GAIN_1V2); 
        Serial.print("[DRV8316] Sensitivity set to: ");
        Serial.print(drv.getCSAGainV_A(), 2);
        Serial.println(" V/A");
    } else {
        Serial.println("[DRV8316] SPI Initialization FAILED!");
        // We continue anyway since we can fallback to default 0.15 V/A gain
        // The hardware SPI bus might be broken or disconnected
    }

    // --- MUST CLOSE THE CIRCUIT TO MEASURE CURRENT ---
    // Si los MOSFETs están apagados (estado Hi-Z), girar el motor no generará corriente 
    // porque el circuito está abierto. Para medir corriente manual, debemos cortocircuitar
    // las fases a GND (Frenado Activo) encendiendo los MOSFETs Low-Side.
    // Como la SPI puede estar rota, el driver arranca por defecto en modo 6-PWM.
    pinMode(PIN_AH, OUTPUT); digitalWrite(PIN_AH, LOW);
    pinMode(PIN_BH, OUTPUT); digitalWrite(PIN_BH, LOW);
    pinMode(PIN_CH, OUTPUT); digitalWrite(PIN_CH, LOW);

    pinMode(PIN_AL, OUTPUT); digitalWrite(PIN_AL, HIGH);
    pinMode(PIN_BL, OUTPUT); digitalWrite(PIN_BL, HIGH);
    pinMode(PIN_CL, OUTPUT); digitalWrite(PIN_CL, HIGH);

    // 3. Initialize Current Sense
    Serial.println("[CS] Calibrating offsets... (Keep motor still)");
    if (cs.begin()) {
        Serial.println("[CS] Calibration DONE.");
    } else {
        Serial.println("[CS] Initialization FAILED!");
    }

    // LED
    pinMode(PIN_LED, OUTPUT);
}

void loop() {
    // 1. Update current readings
    cs.update();

    // 2. Print values every 200ms
    static uint32_t last_print = 0;
    if (millis() - last_print >= 200) {
        float ia = cs.getAmpsA();
        float ib = cs.getAmpsB();
        float ic = cs.getAmpsC();

        Serial.print("CUR [A] -> IA: ");
        Serial.print(ia, 3);
        Serial.print(" | IB: ");
        Serial.print(ib, 3);
        Serial.print(" | IC: ");
        Serial.println(ic, 3);

        last_print = millis();
    }

    // Heartbeat LED (2Hz during testing)
    static uint32_t last_led = 0;
    if (millis() - last_led >= 250) {
        digitalWrite(PIN_LED, !digitalRead(PIN_LED));
        last_led = millis();
    }
}
