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

HardwareSerial RS485_SERIAL(PIN_RS485_RX, PIN_RS485_TX);

void RS485::begin(uint8_t deviceId, uint32_t baudrate) {
    _deviceId = deviceId;
    _rxIdx = 0;

    // Pin de dirección
    pinMode(PIN_RS485_DIR, OUTPUT);
    _setRxMode();  // por defecto: escuchar

    // UART
    RS485_SERIAL.begin(baudrate);
}

void RS485::send(const char* frame) {
    _setTxMode();

    RS485_SERIAL.print(frame);
    RS485_SERIAL.flush();  // espera a que el último byte salga del shift register

    // Pequeña espera para que el último bit llegue al bus antes de cambiar DIR
    delayMicroseconds(50);

    _setRxMode();
}

void RS485::reply(const char* cmd, float value) {
    char buf[RS485_MAX_FRAME];
    snprintf(buf, sizeof(buf), "%d:%s:%.4f\n", _deviceId, cmd, value);
    send(buf);
}

bool RS485::update(RS485Frame& out) {
    while (RS485_SERIAL.available()) {
        char c = RS485_SERIAL.read();

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

// ── Privado ──────────────────────────────────────────────────────────────────

bool RS485::_parseFrame(const char* raw, RS485Frame& out) {
    // Formato esperado: "<id>:<cmd>:<value>"
    out.valid = false;

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

    tok = strtok(nullptr, ":");
    if (tok) {
        strncpy(out.value, tok, sizeof(out.value) - 1);
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
