#pragma once
// ============================================================
//  pins.h — SCLF Gripper v1.0 Pin Definitions
//  Single source of truth. Never use magic numbers elsewhere.
//  Board: STM32G474CEU6
// ============================================================

// --- SWD / Programming (hardware pads — not GPIO-controlled) ---
// PA13 = SWDIO, PA14 = SWCLK, PG10 = SWRST
// These are used by the debugger and should NOT be reconfigured.

// --- Boot & Reset buttons ---
#define PIN_BOOT        PB8   // BOOT0 button — hold during reset for DFU mode
#define PIN_RESET       PG10  // Hardware reset (also SWD RST pad)

// --- Test Points ---
#define PIN_CURR_TEST   PA2   // PA2 — current sense ADC test point
#define PIN_PWM_TEST    PA10  // PA10 — PWM test point

// --- DRV8316 Gate Driver ---
// TODO: Confirm exact pins from schematic after KiCad review
#define PIN_DRV_NFAULT  PA0   // nFAULT input (active low) — verify from schematic
#define PIN_DRV_ENABLE  PA1   // Driver enable — verify from schematic
// PWM phases are driven by HRTIM — configured in MotorController.cpp

// --- MT6701 Encoder (SPI) ---
// TODO: Confirm SPI bus and CS pin from schematic
#define PIN_ENC_CS      PB12  // Chip select — verify from schematic
#define PIN_ENC_SCK     PB13  // SPI clock — verify from schematic
#define PIN_ENC_MISO    PB14  // MISO — verify from schematic
// MOSI not needed for MT6701 read-only mode

// --- RS-485 (MAX3485) ---
// TODO: Confirm UART instance and DE/RE pin from schematic
#define PIN_RS485_TX    PC10  // UART TX — verify from schematic
#define PIN_RS485_RX    PC11  // UART RX — verify from schematic
#define PIN_RS485_DE    PC12  // Driver Enable / Receiver Enable (half-duplex control)

// --- USB (Virtual COM Port) ---
// PA11 = USB_DM, PA12 = USB_DP (fixed by STM32 USB FS peripheral)

// --- LEDs ---
// TODO: Confirm LED pins from schematic
#define PIN_LED_PWR1    PB0   // Power LED 1 — verify from schematic
#define PIN_LED_PWR2    PB1   // Power LED 2 — verify from schematic
