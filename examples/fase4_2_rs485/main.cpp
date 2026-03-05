/**
 * SCLF Gripper v1.0 — Firmware Hardware Initialization
 * Board:     STM32G474CEU6
 * Framework: Arduino (STM32duino) via PlatformIO
 *
 * ─── FASE 4.2 — RS485 Communication Test ───────────────────────────────────
 * Objetivo:  Probar el transceptor MAX3485 (Half-Duplex) sin activar motores.
 * ───────────────────────────────────────────────────────────────────────────
 */

#include "comms/RS485.h"
#include "config/pins.h"

#include <Arduino.h>


// ─── Instancias de Hardware ──────────────────────────────────────────────────
RS485 comms;

// ─── setup() ──────────────────────────────────────────────────────────────────
void setup() {
    // 1. Configuración de UART interno para debug por USB-C (opcional)
    Serial.begin(115200);
    /* Descomentar si se quiere esperar al monitor serie (bloqueará el RS485 si no se conecta USB)
    uint32_t t = millis();
    while (!Serial && (millis() - t) < 3000) {}
    */

    Serial.println("========================================");
    Serial.println("  SCLF Gripper v1.0 — Fase 4.2          ");
    Serial.println("  Test de Comunicacion RS-485 aislado   ");
    Serial.println("========================================");

    // 2. Apagar pines del driver por seguridad
    pinMode(PIN_DRV_CS, OUTPUT);
    digitalWrite(PIN_DRV_CS, HIGH);  // CS Inactivo

    pinMode(PIN_AH, OUTPUT);
    digitalWrite(PIN_AH, LOW);
    pinMode(PIN_BH, OUTPUT);
    digitalWrite(PIN_BH, LOW);
    pinMode(PIN_CH, OUTPUT);
    digitalWrite(PIN_CH, LOW);

    // Apagar sensor
    pinMode(PIN_ENC_CS, OUTPUT);
    digitalWrite(PIN_ENC_CS, HIGH);

    pinMode(PIN_LED, OUTPUT);
    digitalWrite(PIN_LED, LOW);

    // 3. Inicializar RS485
    // Dispositivo ID = 1, Baudrate = 115200
    Serial.println("[RS485] Iniciando bus en ID 1...");
    comms.begin(1, 115200);

    Serial.println("[RS485] Bus listo. Esperando comandos...");

    // Enviar un mensaje inicial por el RS485 para que la consola PC lo vea
    comms.send("1:INFO:SCLF_Gripper_Ready\n");
}

// ─── loop() ───────────────────────────────────────────────────────────────────
void loop() {
    RS485Frame frame;

    // Procesar datos RS485 de forma asíncrona
    if (comms.update(frame)) {
        if (frame.valid) {
            // Mostrar en el VCP debug qué nos ha llegado
            Serial.print("[RS485 Rx] ID:");
            Serial.print(frame.deviceId);
            Serial.print(" CMD:");
            Serial.print(frame.cmd);
            Serial.print(" VAL:");
            Serial.println(frame.value);

            // Verificar si el comando es para nosotros o broadcast (0)
            if (comms.isForMe(frame)) {
                // Procesamiento simulado (ECHO)
                if (strcmp(frame.cmd, "V") == 0) {
                    float vel = atof(frame.value);
                    Serial.print("  -> Set Velocidad detectado: ");
                    Serial.println(vel);

                    // Responder por RS485 con el nuevo estado simulado
                    comms.reply("V_ACK", vel);

                } else if (strcmp(frame.cmd, "?S") == 0) {
                    Serial.println("  -> Status request");
                    comms.send("1:STATUS:IDLE\n");

                } else {
                    // Comando genérico: hacer echo del comando recibido para confirmar
                    char buf[64];
                    snprintf(buf, sizeof(buf), "1:ACK_%s:%s\n", frame.cmd, frame.value);
                    comms.send(buf);
                }
            }
        } else {
            Serial.println("[RS485] Error de parseo: trama invalida.");
        }
    }

    // Toggle LED (indica que avanza el loop, 1Hz)
    static uint32_t last_led = 0;
    if (millis() - last_led >= 500) {
        digitalWrite(PIN_LED, !digitalRead(PIN_LED));
        last_led = millis();
    }
}
