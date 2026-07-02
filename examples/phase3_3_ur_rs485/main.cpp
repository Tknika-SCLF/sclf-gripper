/**
 * SCLF Gripper — Phase 3.3: UR Robot / URCap RS485 Communication Test
 * Board:     STM32G474CEU6
 *
 * Objetivo:  Validar la comunicación half-duplex RS485 con un robot UR (Universal Robots)
 *            o software de control URCap a través del conector J3.
 * 
 * Conexión física J3:
 *   - Pin 7: RS485+ (A)
 *   - Pin 8: RS485- (B)
 *   - Pines 1 & 2: GND
 * 
 * Configuración de pines interna (pins.h):
 *   - TX: PC10 (UART3 TX)
 *   - RX: PC11 (UART3 RX)
 *   - DIR: PB9 (DE/RE, HIGH = TX, LOW = RX)
 */

#include "comms/RS485.h"
#include "config/pins.h"
#include <Arduino.h>

// --- Configuración de Reloj (HSI 16MHz -> 170MHz) ---
extern "C" void SystemClock_Config(void) {
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSI48;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV4;
  RCC_OscInitStruct.PLL.PLLN = 85;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) { /* Error */ }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK) { /* Error */ }
}

// --- Estado Simulado para el Test del Gripper ---
struct GripperState {
    float target_position = 0.0f; // mm (0: totalmente abierto, 85: cerrado)
    float current_position = 0.0f;
    float target_force = 100.0f;  // % (0-100)
    float current_force = 0.0f;
    float velocity = 50.0f;       // % (0-100)
    bool enabled = false;
    bool moving = false;
    uint8_t fault_code = 0;       // 0 = OK
} state;

RS485 comms;

// Helper para convertir CommandType a String para depuración USB VCP
const char* getCmdName(CommandType type) {
    switch (type) {
        case CommandType::PING: return "PING";
        case CommandType::SET_TORQUE: return "SET_FORCE (T)";
        case CommandType::SET_VELOCITY: return "SET_VELOCITY (V)";
        case CommandType::SET_POSITION: return "SET_POSITION (P)";
        case CommandType::SET_MODE: return "SET_MODE (M)";
        case CommandType::ENABLE: return "ENABLE (EN)";
        case CommandType::DISABLE: return "DISABLE (DIS)";
        case CommandType::GET_ANGLE: return "GET_POSITION (?A)";
        case CommandType::GET_VELOCITY: return "GET_VELOCITY (?V)";
        case CommandType::GET_CURRENT: return "GET_FORCE (?I)";
        case CommandType::GET_STATUS: return "GET_STATUS (?S)";
        default: return "UNKNOWN";
    }
}

void processFrame(const RS485Frame& frame) {
    if (!frame.valid || !comms.isForMe(frame)) return;

    // Log de recepción en Serial (USB VCP)
    Serial.print("[RS485 Rx] ID: "); Serial.print(frame.deviceId);
    Serial.print(" | CMD: "); Serial.print(getCmdName(frame.type));
    Serial.print(" | Val: "); Serial.println(frame.value);

    const char* replyCmd = "";
    float replyVal = 0.0f;

    switch (frame.type) {
        case CommandType::PING:
            replyCmd = "PONG";
            replyVal = 0.0f;
            break;

        case CommandType::SET_POSITION:
            // Posición objetivo en mm (ej. de 0.0 a 85.0)
            state.target_position = constrain(frame.value, 0.0f, 85.0f);
            state.moving = (abs(state.target_position - state.current_position) > 0.1f);
            replyCmd = "ACK_P";
            replyVal = state.target_position;
            break;

        case CommandType::SET_VELOCITY:
            // Velocidad objetivo en % (0 a 100)
            state.velocity = constrain(frame.value, 0.0f, 100.0f);
            replyCmd = "ACK_V";
            replyVal = state.velocity;
            break;

        case CommandType::SET_TORQUE:
            // Fuerza/Torque objetivo en % (0 a 100)
            state.target_force = constrain(frame.value, 0.0f, 100.0f);
            replyCmd = "ACK_T";
            replyVal = state.target_force;
            break;

        case CommandType::ENABLE:
            state.enabled = true;
            state.fault_code = 0;
            replyCmd = "ACK_EN";
            replyVal = 1.0f;
            break;

        case CommandType::DISABLE:
            state.enabled = false;
            state.moving = false;
            replyCmd = "ACK_DIS";
            replyVal = 0.0f;
            break;

        case CommandType::GET_ANGLE:
            // Retorna la posición actual (mm)
            replyCmd = "A";
            replyVal = state.current_position;
            break;

        case CommandType::GET_VELOCITY:
            // Retorna la velocidad actual (mm/s o %)
            replyCmd = "V";
            replyVal = state.moving ? state.velocity : 0.0f;
            break;

        case CommandType::GET_CURRENT:
            // Retorna la corriente/fuerza actual
            replyCmd = "I";
            replyVal = state.current_force;
            break;

        case CommandType::GET_STATUS:
            // Retorna un byte de estado codificado:
            // Bit 0: Enabled, Bit 1: Moving, Bit 2: Fault
            {
                uint8_t statusByte = 0;
                if (state.enabled) statusByte |= 0x01;
                if (state.moving) statusByte |= 0x02;
                if (state.fault_code > 0) statusByte |= 0x04;
                replyCmd = "S";
                replyVal = (float)statusByte;
            }
            break;

        default:
            replyCmd = "ERR";
            replyVal = -1.0f;
            break;
    }

    // Enviar respuesta por el bus RS-485
    comms.reply(replyCmd, replyVal);
    
    // Espejo de la respuesta por USB para monitoreo
    Serial.print("[VCP Tx] Reply: "); Serial.print(frame.deviceId);
    Serial.print(":"); Serial.print(replyCmd); 
    Serial.print(":"); Serial.println(replyVal, 4);
}

void setup() {
    Serial.begin(115200); // USB VCP para diagnóstico local
    
    // Configurar pines de potencia e inhibirlos por seguridad
    pinMode(PIN_AH, OUTPUT); digitalWrite(PIN_AH, LOW);
    pinMode(PIN_BH, OUTPUT); digitalWrite(PIN_BH, LOW);
    pinMode(PIN_CH, OUTPUT); digitalWrite(PIN_CH, LOW);
    
    pinMode(PIN_AL, OUTPUT); digitalWrite(PIN_AL, LOW);
    pinMode(PIN_BL, OUTPUT); digitalWrite(PIN_BL, LOW);
    pinMode(PIN_CL, OUTPUT); digitalWrite(PIN_CL, LOW);

    // Inhabilitar CS de periféricos SPI
    pinMode(PIN_DRV_CS, OUTPUT); digitalWrite(PIN_DRV_CS, HIGH);
    pinMode(PIN_ENC_CS, OUTPUT); digitalWrite(PIN_ENC_CS, HIGH);

    pinMode(PIN_LED, OUTPUT);
    digitalWrite(PIN_LED, HIGH);

    // Inicializar el bus RS-485. Por defecto a 115200 baudios,
    // que es la velocidad estándar para comunicación con robots industriales (UR).
    comms.begin(1, 115200);

    Serial.println("\n\n================================================");
    Serial.println("  SCLF Gripper — Phase 3.3                      ");
    Serial.println("  UR Robot / URCap RS485 Integration Test        ");
    Serial.println("  Speed: 115200 Baud | Device ID: 1             ");
    Serial.println("================================================");
    Serial.println("[RS485] Listening on J3 Connector (Pins 7, 8)...");
}

void loop() {
    RS485Frame frame;
    
    // Leer y parsear tramas RS485 físicas
    if (comms.update(frame)) {
        processFrame(frame);
    }
    
    // Simular el movimiento del gripper hacia la posición objetivo de forma no bloqueante
    if (state.enabled && state.moving) {
        static uint32_t last_move_time = 0;
        if (millis() - last_move_time > 10) {
            float step = (state.velocity / 100.0f) * 0.5f; // velocidad simulada
            if (state.current_position < state.target_position) {
                state.current_position = min(state.current_position + step, state.target_position);
            } else if (state.current_position > state.target_position) {
                state.current_position = max(state.current_position - step, state.target_position);
            }
            
            // Simular fuerza de contacto al cerrar
            if (state.current_position >= 80.0f && state.target_position > 80.0f) {
                state.current_force = state.target_force;
            } else {
                state.current_force = 2.0f; // corriente/fuerza mínima en movimiento
            }

            if (state.current_position == state.target_position) {
                state.moving = false;
                if (state.current_position < 80.0f) {
                    state.current_force = 0.0f; // sin fuerza al detenerse sin contacto
                }
            }
            last_move_time = millis();
        }
    }

    // Permitir pruebas locales a través del puerto serie USB (VCP)
    while (Serial.available()) {
        char c = Serial.read();
        char buf[2] = {c, '\0'};
        if (comms.simulateRx(buf, frame)) {
            processFrame(frame);
        }
    }

    // Blink de estado del LED
    static uint32_t last_led = 0;
    uint32_t interval = state.enabled ? (state.moving ? 100 : 500) : 1000;
    if (millis() - last_led > interval) {
        digitalWrite(PIN_LED, !digitalRead(PIN_LED));
        last_led = millis();
    }
}
