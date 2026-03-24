/**
 * SCLF Gripper v1.0 — Phase 1.4: RS485 Ping-Pong Test
 * Board:     STM32G474CEU6
 * Framework: Arduino (STM32duino) via PlatformIO
 *
 * Objetivo:  Implementar un mecanismo de respuesta (Ping-Pong) por RS485.
 *            Si recibe "1:PING:0\n", responde "1:PONG:0\n".
 */

#include "comms/RS485.h"
#include "config/pins.h"
#include <Arduino.h>

// --- Instances ---
RS485 comms;

void setup() {
    // 1. Debug via USB-C (Virtual COM Port)
    Serial.begin(115200);
    
    // Wait for Serial to be ready (timeout 2s)
    uint32_t t = millis();
    while (!Serial && (millis() - t) < 2000) {}

    // 2. Security: Disable motor outputs
    pinMode(PIN_AH, OUTPUT); digitalWrite(PIN_AH, LOW);
    pinMode(PIN_BH, OUTPUT); digitalWrite(PIN_BH, LOW);
    pinMode(PIN_CH, OUTPUT); digitalWrite(PIN_CH, LOW);
    pinMode(PIN_AL, OUTPUT); digitalWrite(PIN_AL, LOW);
    pinMode(PIN_BL, OUTPUT); digitalWrite(PIN_BL, LOW);
    pinMode(PIN_CL, OUTPUT); digitalWrite(PIN_CL, LOW);

    // Disable SPI Chip Selects for safety
    pinMode(PIN_DRV_CS, OUTPUT); digitalWrite(PIN_DRV_CS, HIGH);
    pinMode(PIN_ENC_CS, OUTPUT); digitalWrite(PIN_ENC_CS, HIGH);

    // LED
    pinMode(PIN_LED, OUTPUT);
    digitalWrite(PIN_LED, LOW);

    // 3. Initialize RS485 (ID 1, 115200 baud)
    Serial.println("\n\n========================================");
    Serial.println("  SCLF Gripper v1.0 — Phase 1.4         ");
    Serial.println("  RS485 Ping-Pong Test                  ");
    Serial.println("========================================");
    
    comms.begin(1, 115200);
    Serial.println("[RS485] Listening on ID 1...");
    Serial.println("[DEBUG] Logic test via USB enabled (Type 'PING').");
}

void loop() {
    RS485Frame frame;
    bool frameReceived = false;

    // A. Process incoming RS485 frames (Physical Bus)
    if (comms.update(frame)) {
        frameReceived = true;
    }

    // B. Process incoming Serial (VCP) characters (Simulation/Test)
    while (Serial.available()) {
        char c = Serial.read();
        char buf[2] = {c, '\0'};
        if (comms.simulateRx(buf, frame)) {
            frameReceived = true;
            break;
        }
    }

    // Handle frame if received via Bus or VCP
    if (frameReceived && frame.valid && comms.isForMe(frame)) {
        Serial.print("[RS485 Rx] CMD: ");
        Serial.println(frame.cmd);

        // Ping-Pong Logic
        if (strcmp(frame.cmd, "PING") == 0) {
            Serial.println("  -> Received PING! Sending PONG...");
            comms.send("1:PONG:0\n");
            // Also notify VCP for easier testing
            Serial.println("[VCP] Reply sent to RS485 bus: 1:PONG:0");
        } else {
            // Echo other commands for debug
            char response[64];
            snprintf(response, sizeof(response), "1:ACK_%s:%s\n", frame.cmd, frame.value);
            comms.send(response);
        }
    }

    // Heartbeat LED (1Hz)
    static uint32_t last_led = 0;
    if (millis() - last_led >= 500) {
        digitalWrite(PIN_LED, !digitalRead(PIN_LED));
        last_led = millis();
    }
}
