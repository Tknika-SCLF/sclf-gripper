/**
 * SCLF Gripper v1.0 — Phase 3.1: Full RS485 Protocol Test (STABLE)
 * Board:     STM32G474CEU6
 *
 * Objetivo:  Validar comunicación RS485 usando HSE (16MHz Crystal) para 
 *            máxima precisión en el baudrate y bajando a 9600 para test.
 */

#include "comms/RS485.h"
#include "config/pins.h"
#include <Arduino.h>

// --- Erlojuaren Konfigurazioa (16MHz-eko kristala erabiltzeko) ---
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

// --- Estatua eta Instantziak ---
struct GripperState {
    float angle = 1.23f;
    bool enabled = false;
} state;

RS485 comms;

void setup() {
    Serial.begin(9600); // Sincronizado con RS-485 para evitar confusiones
    uint32_t t = millis();
    while (!Serial && (millis() - t) < 2000) {}

    // Segurtasuna: Irteerak itxi
    pinMode(PIN_AH, OUTPUT); digitalWrite(PIN_AH, LOW);
    pinMode(PIN_BH, OUTPUT); digitalWrite(PIN_BH, LOW);
    pinMode(PIN_CH, OUTPUT); digitalWrite(PIN_CH, LOW);
    pinMode(PIN_DRV_CS, OUTPUT); digitalWrite(PIN_DRV_CS, HIGH);
    pinMode(PIN_ENC_CS, OUTPUT); digitalWrite(PIN_ENC_CS, HIGH);
    pinMode(PIN_LED, OUTPUT);

    // RS485 Hasieratu: 9600 BAUD (Egonkortasunagatik)
    comms.begin(1, 9600);
    
    Serial.println("\n\n========================================");
    Serial.println("  SCLF Gripper v1.0 — Phase 4.2 (STABLE)");
    Serial.println("  RS485 Bus Speed: 9600 Baud");
    Serial.println("========================================");
}

void processFrame(const RS485Frame& frame) {
    if (!frame.valid || !comms.isForMe(frame)) return;

    Serial.print("[RS485 Rx] ID:");
    Serial.print(frame.deviceId);
    Serial.print(" CMD:");
    Serial.print(frame.cmd);
    Serial.print(" VAL:");
    Serial.println(frame.value);

    if (strcmp(frame.cmd, "?A") == 0) {
        comms.reply("A", state.angle);
        Serial.print("[VCP Tx] Reply: 1:A:"); Serial.println(state.angle);
    } else if (strcmp(frame.cmd, "EN") == 0) {
        state.enabled = true;
        comms.send("1:EN:1\n");
        Serial.println("[VCP Tx] Reply: 1:EN:1");
    } else if (strcmp(frame.cmd, "PING") == 0) {
        comms.send("1:PONG:0\n");
        Serial.println("[VCP Tx] Reply: 1:PONG:0");
    } else {
        char buf[64];
        snprintf(buf, sizeof(buf), "1:ACK_%s:%s\n", frame.cmd, frame.value);
        comms.send(buf);
        Serial.print("[VCP Tx] Reply: "); Serial.print(buf);
    }
}

void loop() {
    RS485Frame frame;
    bool frameReceived = false;

    if (comms.update(frame)) frameReceived = true;
    
    while (Serial.available()) {
        char c = Serial.read();
        char buf[2] = {c, '\0'};
        if (comms.simulateRx(buf, frame)) {
            frameReceived = true;
            break;
        }
    }

    if (frameReceived) {
        processFrame(frame);
    }

    static uint32_t last_led = 0;
    if (millis() - last_led >= 500) {
        digitalWrite(PIN_LED, !digitalRead(PIN_LED));
        last_led = millis();
    }
}
