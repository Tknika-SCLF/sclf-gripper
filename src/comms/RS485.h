#pragma once
/**
 * RS485.h — Driver de comunicación RS-485 half-duplex (MAX3485)
 *
 * Pines (ver pins.h):
 *   TX  = PC10 (PIN_RS485_TX)
 *   RX  = PC11 (PIN_RS485_RX)
 *   DIR = PB9  (PIN_RS485_DIR)  HIGH=TX, LOW=RX
 *
 * Protocolo (ASCII, newline-terminated):
 *   <device_id>:<cmd>:<value>\n
 *   Ejemplo: "1:V:10.5\n"  → dispositivo 1, set velocity, 10.5 rad/s
 *
 * ⚠️  REGLA: DIR siempre vuelve a LOW después de transmitir (R05 de RULES.md)
 * ⚠️  FASE 1.4 — Implementar en RS485.cpp
 */

#include <Arduino.h>
#include "config/pins.h"

// Tamaño máximo de un frame RS-485
static constexpr size_t RS485_MAX_FRAME = 64;

// Comandos soportados (Mapeo a enum CommandType para facilitar switch)
enum class CommandType {
    UNKNOWN,
    PING,
    SET_TORQUE,    // 'T'
    SET_VELOCITY,  // 'V'
    SET_POSITION,  // 'P'
    SET_MODE,      // 'M'
    ENABLE,        // 'EN'
    DISABLE,       // 'DIS'
    GET_ANGLE,     // '?A'
    GET_VELOCITY,  // '?v'
    GET_CURRENT,   // '?I'
    GET_STATUS,    // '?S'
    SET_KP,        // 'KP'
    SET_KI,        // 'KI'
    SET_KD,        // 'KD'
    ACK            // Respuesta genérica o confirmación
};

struct RS485Frame {
    uint8_t     deviceId;
    CommandType type;
    float       value;
    bool        valid;
};

class RS485 {
public:
    /**
     * Inicializa UART3 y el pin DIR.
     * @param deviceId  ID de este dispositivo en el bus (1-254)
     * @param baudrate  Velocidad del bus (default: 115200)
     */
    void begin(uint8_t deviceId, uint32_t baudrate = 115200);

    /**
     * Envía una respuesta al bus. Gestiona DIR automáticamente.
     * ⚠️  Bloquea hasta que el último bit sale físicamente.
     * @param frame  String ASCII terminado en '\n'
     */
    void send(const char* frame);

    /**
     * Formatea y envía una respuesta con valor float.
     * Ejemplo: reply("?A", 1.234f) → "1:?A:1.234\n"
     */
    void reply(const char* cmd, float value);

    /**
     * Inyecta bytes directamente al buffer RX local. Útil para testear
     * la lógica de parseo desde un puerto paralelo (ej. USB).
     * @param str  String a inyectar
     * @param out  Frame parseado si devuelve true (alcanza un '\n')
     */
    bool simulateRx(const char* str, RS485Frame& out);

    /**
     * Procesa bytes recibidos. Llamar en cada iteración del loop.
     * Non-blocking — devuelve true solo cuando hay un frame completo.
     * @param out  Frame parseado si devuelve true
     */
    bool update(RS485Frame& out);

    /**
     * Comprueba si este frame va dirigido a este dispositivo.
     */
    bool isForMe(const RS485Frame& frame) const {
        return frame.deviceId == _deviceId || frame.deviceId == 0; // 0=broadcast
    }

private:
    uint8_t _deviceId = 1;
    char    _rxBuf[RS485_MAX_FRAME];
    size_t  _rxIdx = 0;

    bool _parseFrame(const char* raw, RS485Frame& out);
    void _setTxMode();
    void _setRxMode();
};
