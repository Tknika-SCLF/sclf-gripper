/**
 * RS485.cpp — Implementación del driver RS-485 (MAX3485)
 *
 * ⚠️  FASE 1.4 — Implementar y validar con hardware
 *
 * UART3 del STM32G474:
 *   TX = PC10 (AF7: USART3_TX)
 *   RX = PC11 (AF7: USART3_RX)
 */

#include "comms/RS485.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Instanciar específicamente el puerto Serial3 usando los pines configurados
HardwareSerial Serial3(PIN_RS485_RX, PIN_RS485_TX);

void RS485::begin(uint8_t deviceId, uint32_t baudrate) {
    _deviceId = deviceId;
    _rxIdx = 0;

    // Pin de dirección
    pinMode(PIN_RS485_DIR, OUTPUT);
    _setRxMode();  // por defecto: escuchar

    // Configurar pines explícitamente y luego inicializar
    Serial3.setRx(PIN_RS485_RX);
    Serial3.setTx(PIN_RS485_TX);
    Serial3.begin(baudrate, SERIAL_8N1);
}

void RS485::send(const char* frame) {
    _setTxMode();

    Serial3.print(frame);
    Serial3.flush();  // espera a que el último byte salgo del buffer TX

    // En STM32duino, flush() espera a que el registro de desplazamiento esté vacío,
    // pero un pequeño delay asegura que el stop bit ha salido completamente.
    delayMicroseconds(20); 

    _setRxMode();
}

void RS485::reply(const char* cmd, float value) {
    char buf[RS485_MAX_FRAME];
    snprintf(buf, sizeof(buf), "%d:%s:%.4f\n", _deviceId, cmd, value);
    send(buf);
}

bool RS485::update(RS485Frame& out) {
    while (Serial3.available()) {
        char c = Serial3.read();

        if (c == '\n' || c == '\r') {
            if (_rxIdx > 0) {
                _rxBuf[_rxIdx] = '\0';
                bool ok = _parseFrame(_rxBuf, out);
                _rxIdx = 0;
                if (ok)
                    return true;
            }
        } else if (_rxIdx < RS485_MAX_FRAME - 1) {
            _rxBuf[_rxIdx++] = c;
        } else {
            // Buffer overflow — descartar frame corrupto
            _rxIdx = 0;
        }
    }
    return false;
}

bool RS485::simulateRx(const char* str, RS485Frame& out) {
    bool gotFrame = false;
    while (*str) {
        char c = *str++;
        if (c == '\n' || c == '\r') {
            if (_rxIdx > 0) {
                _rxBuf[_rxIdx] = '\0';
                bool ok = _parseFrame(_rxBuf, out);
                _rxIdx = 0;
                if (ok) gotFrame = true;
            }
        } else if (_rxIdx < RS485_MAX_FRAME - 1) {
            _rxBuf[_rxIdx++] = c;
        } else {
            // Buffer overflow
            _rxIdx = 0;
        }
    }
    return gotFrame;
}

// ── Privado ──────────────────────────────────────────────────────────────────

bool RS485::_parseFrame(const char* raw, RS485Frame& out) {
    // Formato esperado: "<id>:<cmd>:<value>" o simplemente "<cmd>"
    out.valid = false;

    // Caso simplificado: No hay dos puntos (ej. "PING")
    if (strchr(raw, ':') == nullptr) {
        out.deviceId = 0; // Broadcast
        strncpy(out.cmd, raw, sizeof(out.cmd) - 1);
        out.cmd[sizeof(out.cmd) - 1] = '\0';
        out.value[0] = '\0';
        out.valid = true;
        return true;
    }

    char tmp[RS485_MAX_FRAME];
    strncpy(tmp, raw, sizeof(tmp) - 1);
    tmp[sizeof(tmp) - 1] = '\0';

    char* tok = strtok(tmp, ":");
    if (!tok)
        return false;
    out.deviceId = static_cast<uint8_t>(atoi(tok));

    tok = strtok(nullptr, ":");
    if (!tok)
        return false;
    strncpy(out.cmd, tok, sizeof(out.cmd) - 1);
    out.cmd[sizeof(out.cmd) - 1] = '\0';

    tok = strtok(nullptr, ":");
    if (tok) {
        strncpy(out.value, tok, sizeof(out.value) - 1);
        out.value[sizeof(out.value) - 1] = '\0';
    } else {
        out.value[0] = '\0';
    }

    out.valid = true;
    return true;
}

void RS485::_setTxMode() {
    digitalWrite(PIN_RS485_DIR, HIGH);
    delayMicroseconds(2);  // propagation delay del MAX3485
}

void RS485::_setRxMode() {
    digitalWrite(PIN_RS485_DIR, LOW);
}
