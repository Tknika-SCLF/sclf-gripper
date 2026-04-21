/**
 * SCLF Gripper v1.0 — Phase 4.2: Full RS485 Control Commands
 * Board:     STM32G474CEU6
 *
 * Objetivo:  Implementar el parser completo de comandos (T, V, P, M, EN, ?A, etc.)
 *            usando la nueva estructura RS485Frame con enums.
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

// --- Estado Simulado para el Test ---
struct GripperState {
    float angle = 1.23f;
    float velocity = 0.0f;
    float current = 0.1f;
    int mode = 0; // 0:T, 1:V, 2:P
    bool enabled = false;
    float kp = 0.5f, ki = 0.1f, kd = 0.01f;
} state;

RS485 comms;

const char* getCmdName(CommandType type) {
    switch (type) {
        case CommandType::PING: return "PING";
        case CommandType::SET_TORQUE: return "SET_TORQUE";
        case CommandType::SET_VELOCITY: return "SET_VELOCITY";
        case CommandType::SET_POSITION: return "SET_POSITION";
        case CommandType::SET_MODE: return "SET_MODE";
        case CommandType::ENABLE: return "ENABLE";
        case CommandType::DISABLE: return "DISABLE";
        case CommandType::GET_ANGLE: return "GET_ANGLE";
        case CommandType::GET_VELOCITY: return "GET_VELOCITY";
        case CommandType::GET_CURRENT: return "GET_CURRENT";
        case CommandType::GET_STATUS: return "GET_STATUS";
        case CommandType::SET_KP: return "SET_KP";
        case CommandType::SET_KI: return "SET_KI";
        case CommandType::SET_KD: return "SET_KD";
        default: return "UNKNOWN";
    }
}

void processFrame(const RS485Frame& frame) {
    if (!frame.valid || !comms.isForMe(frame)) return;

    // Log de recepción en Serial (USB VCP)
    Serial.print("[RS485 Rx] CMD: "); Serial.print(getCmdName(frame.type));
    Serial.print(" | Val: "); Serial.println(frame.value);

    // Variable para capturar la respuesta y mostrarla por USB también
    const char* replyCmd = "";
    float replyVal = 0.0f;

    switch (frame.type) {
        case CommandType::PING:         replyCmd = "PONG"; replyVal = 0; break;
        case CommandType::SET_TORQUE:   state.mode = 0; state.current = frame.value; replyCmd = "ACK_T"; replyVal = state.current; break;
        case CommandType::SET_VELOCITY: state.mode = 1; state.velocity = frame.value; replyCmd = "ACK_V"; replyVal = state.velocity; break;
        case CommandType::SET_POSITION: state.mode = 2; state.angle = frame.value; replyCmd = "ACK_P"; replyVal = state.angle; break;
        case CommandType::SET_MODE:     state.mode = (int)frame.value; replyCmd = "ACK_M"; replyVal = (float)state.mode; break;
        case CommandType::ENABLE:       state.enabled = true; replyCmd = "ACK_EN"; replyVal = 1.0f; break;
        case CommandType::DISABLE:      state.enabled = false; replyCmd = "ACK_DIS"; replyVal = 0.0f; break;
        case CommandType::SET_KP:       state.kp = frame.value; replyCmd = "ACK_KP"; replyVal = state.kp; break;
        case CommandType::SET_KI:       state.ki = frame.value; replyCmd = "ACK_KI"; replyVal = state.ki; break;
        case CommandType::SET_KD:       state.kd = frame.value; replyCmd = "ACK_KD"; replyVal = state.kd; break;
        case CommandType::GET_ANGLE:    replyCmd = "A"; replyVal = state.angle; break;
        case CommandType::GET_VELOCITY: replyCmd = "V"; replyVal = state.velocity; break;
        case CommandType::GET_CURRENT:  replyCmd = "I"; replyVal = state.current; break;
        case CommandType::GET_STATUS:   replyCmd = "S"; replyVal = (float)state.enabled; break;
        default:                        replyCmd = "ERR"; replyVal = -1.0f; break;
    }

    // Enviar respuesta por RS-485
    comms.reply(replyCmd, replyVal);
    
    // Espejo de la respuesta por USB para verla en el monitor serie
    Serial.print("[VCP Tx] Reply: 1:"); Serial.print(replyCmd); 
    Serial.print(":"); Serial.println(replyVal, 4);
}

void setup() {
    Serial.begin(9600); // Para loggear localmente
    
    // Inicializar RS485
    comms.begin(1, 9600);

    // Salidas de seguridad
    pinMode(PIN_LED, OUTPUT);
    digitalWrite(PIN_LED, HIGH);

    Serial.println("SCLF Gripper Phase 4.2 — Control Command Test Ready");
}

void loop() {
    RS485Frame frame;
    
    // Leer puerto RS485 físico
    if (comms.update(frame)) {
        processFrame(frame);
    }
    
    // Permitir testeo desde terminal USB Serial (sin adaptador RS-485 externo si se desea)
    while (Serial.available()) {
        char c = Serial.read();
        char buf[2] = {c, '\0'};
        if (comms.simulateRx(buf, frame)) {
            processFrame(frame);
        }
    }

    // Heartbeat LED
    static uint32_t last_led = 0;
    if (millis() - last_led > 500) {
        digitalWrite(PIN_LED, !digitalRead(PIN_LED));
        last_led = millis();
    }
}
