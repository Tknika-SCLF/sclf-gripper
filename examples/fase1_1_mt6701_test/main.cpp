/**
 * SCLF Gripper v1.0 — Firmware Hardware Initialization
 * Board:     STM32G474CEU6
 * Framework: Arduino (STM32duino) via PlatformIO
 *
 * ─── FASE 1.1 — MT6701 Standalone Test ─────────────────────────────────────
 * Objetivo: Leer el encoder magnético MT6701 vía SPI.
 * NOTA: USB/Serial desactivados. Debug por LED:
 *   - 3 destellos rápidos al arrancar → setup OK
 *   - LED cambia velocidad al girar el motor → encoder funcionando ✅
 *   - LED fast (100ms) constante → encoder no responde
 * ───────────────────────────────────────────────────────────────────────────
 */

#include "config/pins.h"
#include "encoder/MT6701.h"

#include <Arduino.h>
#include <SPI.h>

// ─── Instancias de Hardware ──────────────────────────────────────────────────
MT6701 sensor;

// SPI1 para el encoder. PA7 = SPI1_MOSI (no conectado al encoder, pero
// necesario para que el periférico SPI se inicialice correctamente).
// El MT6701 solo usa MISO (SDO) y CLK, nunca necesita MOSI.
SPIClass SPI_ENC(PA7, PIN_ENC_SDO, PIN_ENC_CLK);

// ─── setup() ──────────────────────────────────────────────────────────────────
void setup() {
    // Pines de seguridad — motor desactivado
    pinMode(PIN_DRV_CS, OUTPUT);  digitalWrite(PIN_DRV_CS, HIGH);
    pinMode(PIN_AH, OUTPUT);      digitalWrite(PIN_AH, LOW);
    pinMode(PIN_BH, OUTPUT);      digitalWrite(PIN_BH, LOW);
    pinMode(PIN_CH, OUTPUT);      digitalWrite(PIN_CH, LOW);

    pinMode(PIN_LED, OUTPUT);
    digitalWrite(PIN_LED, LOW);

    // Encoder CS
    pinMode(PIN_ENC_CS, OUTPUT);
    digitalWrite(PIN_ENC_CS, HIGH);

    // Inicializar puerto serie
    Serial.begin(115200);

    // Esperar a que el puerto serie se conecte (opcional, pero útil para no perder los primeros msjs)
    // while (!Serial) delay(10);
    
    Serial.println("=============================================");
    Serial.println("SCLF Gripper v1.0 - MT6701 Test (con Serial)");
    Serial.println("=============================================");

    // Inicializar SPI del encoder
    SPI_ENC.begin();

    // Inicializar MT6701
    sensor.begin(&SPI_ENC);

    // Blink 3 veces rápido = "setup OK"
    for (int i = 0; i < 3; i++) {
        digitalWrite(PIN_LED, HIGH); delay(80);
        digitalWrite(PIN_LED, LOW);  delay(80);
    }
}

// ─── loop() ───────────────────────────────────────────────────────────────────
void loop() {
    uint16_t raw = sensor.getRawCounts();

    // Test visual claro:
    //   0° - 180° (raw 0–8191)    → LED LENTO   (700ms ON / 700ms OFF)
    //   180° - 360° (raw 8192–16383) → LED RÁPIDO  (80ms ON  / 80ms  OFF)
    //
    // Si el encoder funciona: girar media vuelta cambia el patrón radicalmente.
    // Si el encoder NO funciona: raw=0 siempre → siempre LENTO.
    uint32_t interval;
    if (raw > 8191) {
        interval = 80;   // segunda mitad → LED rápido
    } else {
        interval = 700;  // primera mitad → LED lento
    }

    Serial.print("MT6701 Raw (con o sin 24V): ");
    Serial.println(raw);

    digitalWrite(PIN_LED, HIGH); delay(interval);

    digitalWrite(PIN_LED, HIGH); delay(interval);
    digitalWrite(PIN_LED, LOW);  delay(interval);
}

// ─── Configuración de Reloj Personalizada (Override) ─────────────────────────
// La variante nucleo_g474re original usa HSE, pero necesitamos asegurar que el
// oscilador HSI48 esté encendido para el USB CDC. Si no, Serial.begin() cuelga.
#ifdef __cplusplus
extern "C" {
#endif
void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {};
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {};

    // Configurar voltaje principal para 170MHz
    HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

    // Inicializar osciladores: HSE (16MHz externo), HSI (interno), HSI48 (USB)
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_HSI48 | RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;  // <-- CRÍTICO PARA USB
    
    // Configurar PLL: (16MHz / 4) * 85 / 2 = 170 MHz
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV4;
    RCC_OscInitStruct.PLL.PLLN = 85;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
    RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    // Inicializar buses
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK) {
        Error_Handler();
    }

    // Inicializar relojes de periféricos (CRÍTICO PARA USB)
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
    PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
        Error_Handler();
    }
}
#ifdef __cplusplus
}
#endif
