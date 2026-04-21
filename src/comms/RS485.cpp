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
    out.valid = false;
    out.type = CommandType::UNKNOWN;
    out.value = 0.0f;

    // Caso simplificado (Shorthand): Solo el comando o comando+valor sin ":"
    if (strchr(raw, ':') == nullptr) {
        out.deviceId = 1; // Por defecto ID 1 para atajos
        char cmd = raw[0];
        const char* valPart = raw + 1;

        if (cmd == 'V' || cmd == 'v') out.type = CommandType::SET_VELOCITY;
        else if (cmd == 'P' || cmd == 'p') out.type = CommandType::SET_POSITION;
        else if (cmd == 'T' || cmd == 't') out.type = CommandType::SET_TORQUE;
        else if (cmd == 'M' || cmd == 'm') out.type = CommandType::SET_MODE;
        else if (cmd == 'E' || cmd == 'e') out.type = CommandType::ENABLE;
        else if (cmd == 'D' || cmd == 'd') out.type = CommandType::DISABLE;
        else if (cmd == 'A' || cmd == 'a') { out.type = CommandType::GET_ANGLE; valPart = nullptr; }
        else if (strcasecmp(raw, "PING") == 0) out.type = CommandType::PING;
        
        if (out.type != CommandType::UNKNOWN) {
            if (valPart && *valPart != '\0') out.value = atof(valPart);
            out.valid = true;
            return true;
        }
        return false;
    }

    char tmp[RS485_MAX_FRAME];
    strncpy(tmp, raw, sizeof(tmp) - 1);
    tmp[sizeof(tmp) - 1] = '\0';

    char* tokId = strtok(tmp, ":");
    char* tokCmd = strtok(nullptr, ":");
    char* tokVal = strtok(nullptr, ":");

    if (!tokId || !tokCmd) return false;

    out.deviceId = static_cast<uint8_t>(atoi(tokId));
    
    // Mapeo de Comandos
    if (strcasecmp(tokCmd, "T") == 0) out.type = CommandType::SET_TORQUE;
    else if (strcasecmp(tokCmd, "V") == 0) out.type = CommandType::SET_VELOCITY;
    else if (strcasecmp(tokCmd, "P") == 0) out.type = CommandType::SET_POSITION;
    else if (strcasecmp(tokCmd, "M") == 0) out.type = CommandType::SET_MODE;
    else if (strcasecmp(tokCmd, "EN") == 0) out.type = CommandType::ENABLE;
    else if (strcasecmp(tokCmd, "DIS") == 0) out.type = CommandType::DISABLE;
    else if (strcasecmp(tokCmd, "KP") == 0) out.type = CommandType::SET_KP;
    else if (strcasecmp(tokCmd, "KI") == 0) out.type = CommandType::SET_KI;
    else if (strcasecmp(tokCmd, "KD") == 0) out.type = CommandType::SET_KD;
    else if (strcasecmp(tokCmd, "?A") == 0) out.type = CommandType::GET_ANGLE;
    else if (strcasecmp(tokCmd, "?V") == 0) out.type = CommandType::GET_VELOCITY;
    else if (strcasecmp(tokCmd, "?I") == 0) out.type = CommandType::GET_CURRENT;
    else if (strcasecmp(tokCmd, "?S") == 0) out.type = CommandType::GET_STATUS;
    else if (strcasecmp(tokCmd, "PING") == 0) out.type = CommandType::PING;
    else if (strcasecmp(tokCmd, "ACK") == 0) out.type = CommandType::ACK;

    if (tokVal) {
        out.value = atof(tokVal);
    }

    out.valid = (out.type != CommandType::UNKNOWN);
    return out.valid;
}

void RS485::_setTxMode() {
    digitalWrite(PIN_RS485_DIR, HIGH);
    // Aumentamos el delay a 200us para evitar "clipping" del primer byte.
    // A 9600 baudios, 1 bit dura ~104us. 200us asegura que el bus esté estable.
    delayMicroseconds(200); 
}

void RS485::_setRxMode() {
    digitalWrite(PIN_RS485_DIR, LOW);
}
