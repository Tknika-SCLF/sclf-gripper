/**
 * SCLF Gripper v1.0 — Firmware Hardware Initialization
 * Board:     STM32G474CEU6
 * Framework: Arduino (STM32duino) via PlatformIO
 *
 * ─── FASE 1.4 — RS-485 Driver Test (Ping-Pong) ─────────────────────────────
 * Objetivo: Validar la comunicación half-duplex del integrado RS-485 (MAX3485).
 * 
 * Hardware requerido:
 *   - ST-Link conectado (3.3V, GND, SWDIO, SWCLK, TX, RX).
 *   - NO SE NECESITAN 24V.
 * 
 * Uso:
 *   1. Abre el Serial Monitor (USB CDC o VCP del ST-Link) a 115200 baudios.
 *   2. Envía un comando en el formato `<id>:<cmd>:<value>\n`.
 *      Ejemplo: "1:P:ping\n"
 *   3. El STM32 lo recibirá vía UART3 (PC10/PC11), encenderá el LED, y 
 *      responderá de vuelta por el mismo bus RS-485.
 *   4. Como no tenemos un segundo dispositivo RS-485 conectado, este test
 *      se completará conectando un adaptador USB-RS485 al PC, o haciendo un
 *      loopback físico temporal si se desea.
 * 
 * ⚠️  NOTA IMPORTANTE: 
 *   Para poder leer cómodamente sin adaptador RS-485 en este primer test, 
 *   este programa hará un "eco" de los comandos RS-485 recibidos 
 *   hacia el puerto Serial (USB CDC) para depuración.
 * ───────────────────────────────────────────────────────────────────────────
 */

#include <Arduino.h>
#include "config/pins.h"
#include "comms/RS485.h"

// Instancia global del driver RS485
RS485 rs485;

// Mi ID de dispositivo en el bus RS-485
const uint8_t MY_DEVICE_ID = 1;

void setup() {
    // Configurar LED de estado
    pinMode(PIN_LED, OUTPUT);
    digitalWrite(PIN_LED, LOW);

    // Inicializar puerto serie USB (CDC) para debug
    Serial.begin(115200);
    
    // Inicializar el bus RS-485 a 115200 baudios
    rs485.begin(MY_DEVICE_ID, 115200);

    // Blink 3 veces para indicar arranque OK
    for (int i = 0; i < 3; i++) {
        digitalWrite(PIN_LED, HIGH); delay(100);
        digitalWrite(PIN_LED, LOW);  delay(100);
    }
    
    Serial.println("=================================================");
    Serial.println("SCLF Gripper v1.0 - FASE 1.4: RS-485 Ping Test");
    Serial.println("=================================================");
    Serial.println("Esperando tramas RS-485...");
    Serial.println("Formato: <id>:<cmd>:<value>");
    Serial.println("Ejemplo: 1:T:peticion_torque");
}

// Función para procesar frames que llegan (ya sea reales o simulados)
void handleFrame(RS485Frame& frame) {
    // Destello corto del LED al recibir un frame
    digitalWrite(PIN_LED, HIGH);

    // Echo debug al USB Serial Monitor
    Serial.print("\n[RS-485 RX] Target ID: ");
    Serial.print(frame.deviceId);
    Serial.print(" | CMD: ");
    Serial.print(frame.cmd);
    Serial.print(" | VAL: ");
    Serial.println(frame.value);

    // Si el frame es válido y es para nosotros (o broadcast ID=0)
    if (frame.valid && rs485.isForMe(frame)) {
        Serial.println("-> Frame ACEPTADO. Enviando respuesta (ACK) por RS-485...");
        
        // Responder por el bus RS-485
        char replyBuf[64];
        snprintf(replyBuf, sizeof(replyBuf), "%d:ACK:%s\n", MY_DEVICE_ID, frame.cmd);
        rs485.send(replyBuf);
        
        Serial.print("-> Enviado: ");
        Serial.print(replyBuf);
    } else {
        if (!frame.valid) {
             Serial.println("-> Frame IGNORADO (Invalido o corrupto).");
        } else {
             Serial.println("-> Frame IGNORADO (Dirigido a otro ID).");
        }
    }
    
    // Apagar LED
    delay(10);
    digitalWrite(PIN_LED, LOW);
}

void loop() {
    RS485Frame frame;

    // Verificar si ha llegado un frame completo por RS-485
    if (rs485.update(frame)) {
        handleFrame(frame);
    }

    // --- MODO "SIMULADOR" DESDE EL PC PARA DEPURACION ---
    // Si no tienes un adaptador USB-RS485, puedes escribir comandos en el 
    // Serial Monitor del PC (USB CDC), y el STM32 fingirá que entraron por el RS-485.
    if (Serial.available()) {
        String input = Serial.readStringUntil('\n');
        input.trim();
        
        if (input.length() > 0) {
            Serial.print("\n[USB TX Simulado] -> Inyectando a RS-485: ");
            Serial.println(input);
            
            input += "\n";
            RS485Frame simFrame;
            // simulateRx inyectará el string en el buffer y devolverá true si encuentra el '\n' y el frame es válido
            if (rs485.simulateRx(input.c_str(), simFrame)) {
                 handleFrame(simFrame);
            }
            
            // Pequeño parpadeo verde para indicar TX
            digitalWrite(PIN_LED, HIGH);
            delay(10);
            digitalWrite(PIN_LED, LOW);
        }
    }
}

// ─── Configuración Crítica de Reloj (Override) ───────────
// Necesario para que el USB CDC funcione (requiere HSI48 encendido).
#ifdef __cplusplus
extern "C" {
#endif
void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {};
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {};

    HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_HSI48 | RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;  // <-- CRÍTICO PARA USB
    
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV4;
    RCC_OscInitStruct.PLL.PLLN = 85;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
    RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) { Error_Handler(); }

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK) { Error_Handler(); }

    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
    PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) { Error_Handler(); }
}
#ifdef __cplusplus
}
#endif
