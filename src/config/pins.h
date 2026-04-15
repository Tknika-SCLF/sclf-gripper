#pragma once
// ============================================================
//  pins.h — SCLF Gripper Pin Definitions
//  VERIFIED directly from SCLF_Gripper.kicad_sch
//  Single source of truth. Never use magic numbers elsewhere.
//  Board: STM32G474CEU6 | Date: 2026-04-15
// ============================================================

// ─── SWD / Programming pads (managed by debugger) ───────────
// PA13 = SWDIO, PA14 = SWCLK — do NOT reconfigure these as GPIO
// PG10 = RST (also SWD reset)

// ─── Boot & Reset ────────────────────────────────────────────
#define PIN_BOOT        PB8     // BOOT0 button — hold during reset for DFU mode
#define PIN_RESET       PG10    // Hardware reset button

// ─── DRV8316C Gate Driver (SPI — shared bus with PB3/4/5) ───
// nFAULT is routed to PC13. DRVOFF is tied to GND (always enabled).
#define PIN_DRV_FAULT   PC13    // DRV8316 nFAULT — Hardware fault indicator (active low)
#define PIN_DRV_CS      PC4     // DRV8316 nSCS  — SPI Chip Select (active low)
#define PIN_DRV_CLK     PB3     // DRV8316 SCLK  — SPI Clock  (SPI1)
#define PIN_DRV_MISO    PB4     // DRV8316 SDO   — SPI MISO   (SPI1)
#define PIN_DRV_MOSI    PB5     // DRV8316 SDI   — SPI MOSI   (SPI1)

// ─── Motor PWM outputs (DRV8316 INH/INL ← HRTIM/TIM) ───────
// High-side (INHA=PA10, INHB=PA9, INHC=PA8 on DRV8316)
#define PIN_AH          PA10    // Phase A High
#define PIN_BH          PA9     // Phase B High
#define PIN_CH          PA8     // Phase C High
// Low-side (INLA=PB15, INLB=PB14, INLC=PB13 on DRV8316)
#define PIN_AL          PB15    // Phase A Low
#define PIN_BL          PB14    // Phase B Low
#define PIN_CL          PB13    // Phase C Low

// ─── Inline Current Sense ADC (DRV8316 SOA/SOB/SOC) ─────────
#define PIN_CURA        PA0     // Phase A current sense (ADC1_IN1)
#define PIN_CURB        PA1     // Phase B current sense (ADC1_IN2)
#define PIN_CURC        PA2     // Phase C current sense (ADC1_IN3) — also test pad

// ─── Voltage Sense ───────────────────────────────────────────
#define PIN_VSENS       PB11    // DC bus voltage sense (ADC)

// ─── MT6701 Magnetic Encoder (SPI — separate CS) ────────────
// Max SPI clock: 10 MHz. Use SPISettings(5000000, MSBFIRST, SPI_MODE0)
#define PIN_ENC_CS      PA4     // MT6701 ~CS  (active low)
#define PIN_ENC_CLK     PA5     // MT6701 CLK
#define PIN_ENC_SDO     PA6     // MT6701 SDO  (MISO — read only, no MOSI needed)

// ─── RS-485 (MAX3485 — UART3 / half-duplex) ─────────────────
#define PIN_RS485_TX    PC10    // UART TX → MAX3485 DI
#define PIN_RS485_RX    PC11    // UART RX ← MAX3485 RO
#define PIN_RS485_DIR   PB9     // DE/RE control: HIGH=TX, LOW=RX

// ─── USB Virtual COM Port ────────────────────────────────────
// PA11 = USB_DM (D-), PA12 = USB_DP (D+) — fixed hardware, no defines needed

// ─── LED ─────────────────────────────────────────────────────
#define PIN_LED         PC6     // Status LED

// ─── Crystal (16 MHz HSE) ────────────────────────────────────
// PF0 = OSC_IN, PF1 = OSC_OUT — configured as HSE, do NOT use as GPIO

// ─── Reserved (appear in schematic, function unknown) ────────
// PB6, PB7, PA15 — do not assign without schematic review
