# Software Requirements Specification (SRS)
## SCLF Gripper v1.0 — Brushless Motor Controller Firmware
**Project:** SCLF_Gripper_v1_0  
**Author:** Tknika  
**Version:** 1.0  
**Date:** 2026-02-25  
**License:** CC BY-SA 4.0

---

## 1. Introduction

### 1.1 Purpose
This document specifies the software requirements for the firmware of the SCLF Gripper v1.0, an open-source brushless motor controller designed for robotic gripper applications. The firmware runs on an STM32G474CEU6 microcontroller and implements Field-Oriented Control (FOC) using the SimpleFOC library.

### 1.2 Project Scope
The firmware controls a three-phase BLDC/PMSM motor via the DRV8316C gate driver, reads position from a MT6701 hall-based magnetic encoder, and communicates with external systems over RS-485. Configuration and debugging are available via USB-C (virtual COM port) and SWD programming pads.

### 1.3 Hardware Overview

| Component | Part | Function |
|---|---|---|
| MCU | STM32G474CEU6 | 32-bit Cortex-M4, 170 MHz, CORDIC, Hi-Res Timers |
| Gate Driver | DRV8316CRRGFR | 3-phase FOC driver with current sense & step-down |
| Encoder | MT6701QT | Contactless magnetic angle sensor (SPI/I2C) |
| RS-485 | MAX3485 | Half-duplex serial communication |
| LDO | XC6206P332MR | 3.3V supply |
| Crystal | 16 MHz | HSE clock source |
| USB | TYPE-C 16PIN | Virtual COM Port (VCP) |

### 1.4 Definitions and Acronyms

| Term | Definition |
|---|---|
| FOC | Field-Oriented Control |
| BLDC | Brushless DC Motor |
| PMSM | Permanent Magnet Synchronous Motor |
| SWD | Serial Wire Debug |
| VCP | Virtual COM Port |
| RS-485 | Half-duplex differential serial bus |
| SimpleFOC | Arduino/STM32 open-source FOC library |
| PlatformIO | Embedded development platform and build system |

---

## 2. System Architecture

### 2.1 Software Stack

```
┌─────────────────────────────────────┐
│         Application Layer           │  Gripper control logic, state machine
├─────────────────────────────────────┤
│         SimpleFOC Layer             │  FOC algorithms, PID controllers
├─────────────────────────────────────┤
│         HAL / Driver Layer          │  STM32 HAL, DRV8316, MT6701 drivers
├─────────────────────────────────────┤
│         STM32G474 Hardware          │  HRTIM, ADC, SPI, UART, DMA
└─────────────────────────────────────┘
```

### 2.2 Communication Interfaces

| Interface | Peripheral | Usage |
|---|---|---|
| SWD | PA13 (DIO), PA14 (CLK), PG10 (RST) | Programming and debug |
| USB-C | USB FS | VCP: monitoring and configuration |
| RS-485 | UART + MAX3485 | Robot bus communication (pads 7, 8) |
| SPI/I2C | — | MT6701 encoder reading |

### 2.3 Power Supply
- Input: 24V (pad 3) with GND (pad 1)
- Internal 3.3V: XC6206P332MR LDO
- DRV8316C internal step-down for gate drive power

---

## 3. Functional Requirements

### 3.1 Motor Control (FOC)

| ID | Requirement |
|---|---|
| FR-MOT-01 | The firmware SHALL implement Field-Oriented Control (FOC) for three-phase BLDC/PMSM motors. |
| FR-MOT-02 | The firmware SHALL support torque control mode, velocity control mode, and position control mode. |
| FR-MOT-03 | The firmware SHALL use the DRV8316C to generate three-phase PWM signals via STM32 HRTIM. |
| FR-MOT-04 | The current control loop SHALL run at ≥ 10 kHz. |
| FR-MOT-05 | The velocity control loop SHALL run at ≥ 1 kHz. |
| FR-MOT-06 | The position control loop SHALL run at ≥ 200 Hz. |
| FR-MOT-07 | The firmware SHALL implement PID controllers for all control loops with configurable gains. |

### 3.2 Encoder (MT6701)

| ID | Requirement |
|---|---|
| FR-ENC-01 | The firmware SHALL read the MT6701 magnetic angle sensor via SPI or I2C at the current control loop rate. |
| FR-ENC-02 | The firmware SHALL provide a 14-bit absolute angle reading (0–16383 counts = 0–360°). |
| FR-ENC-03 | The firmware SHALL detect encoder communication errors and trigger a safe fault state. |
| FR-ENC-04 | The firmware SHALL support full multi-turn position tracking. |

### 3.3 RS-485 Communication

| ID | Requirement |
|---|---|
| FR-COM-01 | The firmware SHALL implement a command/response protocol over RS-485. |
| FR-COM-02 | The firmware SHALL support at minimum: set target, get position, get velocity, get current, set PID gains, get status. |
| FR-COM-03 | The protocol SHALL support multi-device addressing. |
| FR-COM-04 | RS-485 direction control SHALL be handled automatically via a GPIO. |

### 3.4 USB / VCP

| ID | Requirement |
|---|---|
| FR-USB-01 | The firmware SHALL expose a Virtual COM Port over USB-C for monitoring and live tuning. |
| FR-USB-02 | Real-time telemetry (angle, velocity, current, target) SHALL be streamable via VCP. |
| FR-USB-03 | SimpleFOC Commander interface SHALL be available over VCP. |

### 3.5 Fault Management

| ID | Requirement |
|---|---|
| FR-FLT-01 | The firmware SHALL detect and handle overcurrent events from the DRV8316C. |
| FR-FLT-02 | The firmware SHALL detect motor stall conditions. |
| FR-FLT-03 | On any fault, the firmware SHALL disable PWM outputs and set the motor to a safe state within 1 ms. |
| FR-FLT-04 | Fault codes SHALL be reported via RS-485 and VCP. |

### 3.6 Boot and Reset

| ID | Requirement |
|---|---|
| FR-BOOT-01 | The BOOT button (PB8) SHALL enter DFU bootloader mode when held during reset. |
| FR-BOOT-02 | The RESET button (PG10) SHALL perform a hardware reset. |
| FR-BOOT-03 | On power-up, the firmware SHALL perform motor auto-alignment to identify electrical angle offset. |

### 3.7 Indicators

| ID | Requirement |
|---|---|
| FR-LED-01 | PWR LED SHALL indicate 3.3V power presence. |
| FR-LED-02 | A status LED SHALL indicate firmware state (idle, running, fault). |

---

## 4. Non-Functional Requirements

| ID | Requirement |
|---|---|
| NFR-01 | The firmware SHALL be written in C++ compatible with the Arduino/SimpleFOC framework on PlatformIO. |
| NFR-02 | The build system SHALL use PlatformIO with the `STM32` platform and `genericSTM32G474CE` board target. |
| NFR-03 | All critical sections SHALL be protected against interrupt preemption. |
| NFR-04 | Flash usage SHALL not exceed 80% of the STM32G474 flash (512 KB). |
| NFR-05 | RAM usage SHALL not exceed 80% of the STM32G474 RAM (128 KB). |
| NFR-06 | The codebase SHALL follow a modular structure with clear separation between HAL, drivers, and application logic. |
| NFR-07 | The project SHALL include unit tests for PID controllers and protocol parsers. |

---

## 5. Development Environment

### 5.1 IDE: Antigravity (VSCode fork)
- Install PlatformIO via `.vsix` (see section 5.3)
- Install C/C++ tools via `cpptools` `.vsix`

### 5.2 Required PlatformIO Libraries
```ini
lib_deps =
    askuric/Simple FOC @ ^2.3.3
    stm32duino/STM32duino LowPower
```

### 5.3 platformio.ini Template
```ini
[env:SCLF_Gripper]
platform = ststm32
board = genericSTM32G474CE
framework = arduino
board_build.mcu = stm32g474ceux
board_build.f_cpu = 170000000L
upload_protocol = stlink
debug_tool = stlink
monitor_speed = 115200

lib_deps =
    askuric/Simple FOC @ ^2.3.3

build_flags =
    -DHAL_TIM_MODULE_ENABLED
    -DHAL_SPI_MODULE_ENABLED
    -DHAL_UART_MODULE_ENABLED
    -DHAL_ADC_MODULE_ENABLED
```

### 5.4 Pin Mapping (STM32G474CEU6)

| Signal | STM32 Pin | Notes |
|---|---|---|
| DIO (SWD) | PA13 | Programming pad |
| CLK (SWD) | PA14 | Programming pad |
| RST | PG10 | Reset / SWD reset |
| BOOT | PB8 | Boot0 button |
| CURR (sense) | PA2 | ADC current sense test point |
| PWM (test) | PA10 | PWM test point |
| Motor A/B/C | DRV8316 | Via HRTIM |

---

## 6. Folder Structure

```
SCLF_Gripper_v1_0_firmware/
├── src/
│   ├── main.cpp               # Entry point, setup/loop
│   ├── motor/
│   │   ├── MotorController.h/.cpp   # SimpleFOC wrapper
│   │   └── CurrentSense.h/.cpp      # DRV8316 inline current sense
│   ├── encoder/
│   │   └── MT6701.h/.cpp            # SPI/I2C encoder driver
│   ├── comms/
│   │   ├── RS485.h/.cpp             # RS-485 protocol handler
│   │   └── VCP.h/.cpp               # USB Virtual COM Port
│   ├── faults/
│   │   └── FaultManager.h/.cpp      # Fault detection and handling
│   └── config/
│       └── pins.h                   # Pin definitions
├── lib/                       # Local libraries
├── test/                      # Unit tests
├── platformio.ini
├── SRS.md
└── AGENT.md                   # AI agent context prompt
```

---

## 7. Revision History

| Version | Date | Author | Description |
|---|---|---|---|
| 1.0 | 2026-02-25 | — | Initial SRS based on SCLF Gripper v1.0 hardware |
