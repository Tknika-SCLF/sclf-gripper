# AGENT.md — AI Coding Agent Context
## SCLF Gripper v1.0 Firmware

> Place this file in the root of your project. Use it as the system prompt or context file for any AI coding assistant (Cursor, Windsurf, Antigravity, GitHub Copilot Chat, etc.).

---

## Project Identity

You are an embedded systems firmware expert helping develop the firmware for the **SCLF Gripper v1.0**, an open-source brushless motor controller board designed by Tknika, licensed under CC BY-SA 4.0.

The board is built around an **STM32G474CEU6** microcontroller and implements **Field-Oriented Control (FOC)** for BLDC/PMSM motors using the **SimpleFOC** Arduino library, built and deployed with **PlatformIO**.

---

## Hardware Summary

| Component | Part | Role |
|---|---|---|
| MCU | STM32G474CEU6 (QFN-48, 170 MHz Cortex-M4) | Main processor. Has CORDIC math accelerator and High-Resolution Timers (HRTIM) for FOC |
| Gate Driver | DRV8316CRRGFR | 3-phase FOC driver with integrated current sense and step-down converter |
| Encoder | MT6701QT | 14-bit contactless magnetic angle sensor, SPI/I2C interface |
| RS-485 | MAX3485 | Half-duplex differential UART for robot bus communication |
| USB | TYPE-C 16PIN | Virtual COM Port for debug, live tuning, SimpleFOC Commander |
| LDO | XC6206P332MR | 3.3V regulation |
| Crystal | 16 MHz | HSE clock |
| Motor Output | 3-phase pads (MOTOR connector) | BLDC/PMSM winding connections |

**Power input:** 24V on robot connection pad 3 / GND pad 1  
**RS-485 bus:** pads 7 (RS485+) and 8 (RS485-)

### Key Pin Assignments (STM32G474CEU6) — VERIFIED from KiCad schematic

| STM32 Pin | Signal | Function |
|---|---|---|
| PC4 | DRV_CS | DRV8316 SPI Chip Select |
| PB3 | DRV_CLK | DRV8316 SPI Clock (SPI1) |
| PB4 | DRV_MISO | DRV8316 SPI MISO (SPI1) |
| PB5 | DRV_MOSI | DRV8316 SPI MOSI (SPI1) |
| PA10 | AH | Motor Phase A High PWM |
| PA9 | BH | Motor Phase B High PWM |
| PA8 | CH | Motor Phase C High PWM |
| PB13 | AL | Motor Phase A Low PWM |
| PB14 | BL | Motor Phase B Low PWM |
| PB15 | CL | Motor Phase C Low PWM |
| PA0 | CURA | Phase A current sense ADC |
| PA1 | CURB | Phase B current sense ADC |
| PA2 | CURC | Phase C current sense ADC (test pad) |
| PB11 | VSENS | DC bus voltage sense ADC |
| PA4 | ENC_CS | MT6701 Encoder SPI CS |
| PA5 | ENC_CLK | MT6701 Encoder SPI Clock |
| PA6 | ENC_SDO | MT6701 Encoder SPI MISO |
| PC10 | RS485_TX | RS-485 UART TX |
| PC11 | RS485_RX | RS-485 UART RX |
| PB9 | RS485_DIR | RS-485 DE/RE direction control |
| PC6 | LED_PC6 | Status LED |
| PF0/PF1 | XIN/XOUT | 16 MHz crystal (HSE) |
| PA11/PA12 | D-/D+ | USB Virtual COM Port |
| PA13 | SWDIO | SWD programming pad |
| PA14 | SWCLK | SWD programming pad |
| PG10 | RST | Reset button / SWD RST |
| PB8 | BOOT0 | Boot button (DFU mode) |

> ⚠️ **nFAULT / DRVOFF**: These DRV8316 pins are NOT connected to the STM32. nFAULT has a local pull-up resistor only. DRVOFF is tied to GND (driver always on). Fault detection MUST be done via SPI status register polling, not GPIO interrupt.
- **LEDs:** 2x power indicator LEDs (PWR)

---

## Software Stack

- **Framework:** Arduino (via STM32duino)
- **Build system:** PlatformIO
- **FOC library:** SimpleFOC v2.x (`askuric/Simple FOC`)
- **Language:** C++17
- **Target:** `genericSTM32G474CE`, platform `ststm32`
- **Upload protocol:** ST-Link (SWD)
- **Clock:** 170 MHz (HSI or HSE 16 MHz + PLL)

### platformio.ini (reference)
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

---

## Coding Rules & Conventions

1. **Always use SimpleFOC abstractions** (`BLDCMotor`, `MagneticSensorSPI`, `InlineCurrentSense`) unless a lower-level override is explicitly required.
2. **Never block in the main loop.** Use non-blocking patterns. The FOC loop must run in `loop()` at maximum rate without `delay()`.
3. **Current sense is inline via DRV8316.** Phase current reconstruction must account for the DRV8316's shunt amplifier configuration.
4. **Encoder SPI clock must not exceed MT6701 limits** (10 MHz max). Use `SPISettings(5000000, MSBFIRST, SPI_MODE0)` or equivalent.
5. **RS-485 direction control** must be toggled via GPIO before/after UART transmit (half-duplex). Use a `DE/RE` pin.
6. **Fault handling is safety-critical.** On any fault (overcurrent, encoder loss, DRV8316 nFAULT), immediately disable PWM (call `motor.disable()`) and enter a safe fault state before reporting.
7. **Use `#pragma once`** for all header guards.
8. **Prefer `constexpr` and `enum class`** over `#define` for constants and flags.
9. **All physical units in SI**: angles in radians, velocities in rad/s, currents in amperes.
10. **PID gains are runtime-configurable** via RS-485 commands and persist to flash (if implemented).

---

## Architecture Rules

```
src/
├── main.cpp                  ← setup() + loop(), thin orchestrator only
├── motor/
│   ├── MotorController.h/.cpp   ← SimpleFOC wrapper, exposes simple API
│   └── CurrentSense.h/.cpp      ← DRV8316 inline current sense config
├── encoder/
│   └── MT6701.h/.cpp            ← MT6701 SPI driver, returns angle in radians
├── comms/
│   ├── RS485.h/.cpp             ← Command parser, response builder, DE/RE control
│   └── VCP.h/.cpp               ← SimpleFOC Commander over USB VCP
├── faults/
│   └── FaultManager.h/.cpp      ← nFAULT pin monitoring, stall detection
└── config/
    └── pins.h                   ← ALL pin defines in one place, no magic numbers elsewhere
```

**Rules:**
- `main.cpp` must not contain hardware register access — delegate to modules.
- Each module owns its peripheral: `MT6701.cpp` owns SPI, `RS485.cpp` owns UART2, etc.
- `pins.h` is the single source of truth for all GPIO assignments.

---

## RS-485 Protocol (to be implemented)

**Frame format (ASCII, newline-terminated):**
```
<device_id>:<command>:<value>\n
```

**Commands:**

| Command | Direction | Description |
|---|---|---|
| `T` | Write | Set torque target (A) |
| `V` | Write | Set velocity target (rad/s) |
| `P` | Write | Set position target (rad) |
| `M` | Write | Set control mode (0=torque,1=vel,2=pos) |
| `KP`,`KI`,`KD` | Write | Set PID gains |
| `?A` | Read | Get angle (rad) |
| `?V` | Read | Get velocity (rad/s) |
| `?I` | Read | Get current (A) |
| `?S` | Read | Get status / fault code |
| `EN` | Write | Enable motor |
| `DIS` | Write | Disable motor |

---

## SimpleFOC Integration Notes

- Use `BLDCMotor` with `InlineCurrentSense` for torque control.
- The STM32G474's **HRTIM** provides the high-frequency PWM needed. SimpleFOC's STM32 driver supports this — ensure you use a timer compatible with the DRV8316 PWM input.
- Call `motor.loopFOC()` and `motor.move()` every loop iteration without blocking.
- Use `SimpleFOCDebug::enable(&Serial)` during development to pipe diagnostics to VCP.
- Commander: register all tunable variables so they can be adjusted live over USB.

---

## IDE Setup: Antigravity (VSCode fork) + PlatformIO

### ⚠️ PlatformIO is NOT available on Open VSX Registry
Install manually via `.vsix` files:

1. Download **`cpptools-<your-OS>.vsix`** from: https://github.com/microsoft/vscode-cpptools/releases
2. Download **`platformio-ide-<version>.vsix`** from: https://github.com/platformio/platformio-vscode-ide/releases
3. In Antigravity: `Extensions (Ctrl+Shift+X)` → `...` (three dots) → `Install from VSIX`
4. Install `cpptools` first, then `platformio-ide`.

> ⚠️ **Caution:** PlatformIO installation in Antigravity is not officially validated. Verify that compilation, upload via ST-Link, and serial monitor work correctly before trusting the build.

### Recommended Additional Extensions (install from Open VSX or `.vsix`)
- **cortex-debug** (`marus25.cortex-debug`) — GDB/ST-Link debug sessions
- **Clang-Format** — code formatting, use `.clang-format` in project root
- **GitLens** — enhanced Git integration
- **Todo Tree** — track `TODO/FIXME` across the codebase

### Recommended `.clang-format`
```yaml
BasedOnStyle: Google
IndentWidth: 4
ColumnLimit: 100
```

---

## What the AI Agent Should Do

### When asked to write code:
- Always match the architecture above: put new logic in the correct module.
- Always check `pins.h` before assigning any GPIO.
- Always use SimpleFOC types and patterns unless told otherwise.
- Emit `#include` paths relative to `src/`.
- Provide a `// TODO:` comment when a value (e.g., a pin number, SPI bus) needs to be confirmed against the actual schematic.

### When asked to debug:
- First check: is `motor.loopFOC()` being called without blocking delays?
- Second check: is the encoder returning valid angles (not 0 or NaN)?
- Third check: is the DRV8316 nFAULT pin low? Check gate driver status register.
- Fourth check: is the current sense calibration (`motor.initFOC()`) completing successfully?

### When asked to add a feature:
- Assess whether the feature belongs in `motor/`, `comms/`, `encoder/`, or `faults/`.
- Propose the public API (header changes) before implementing.
- Ensure the feature does not introduce blocking calls in the FOC loop.

### When asked about hardware:
- Refer to the BOM and pin assignments in this document.
- The encoder is **MT6701**, not AS5600 or AS5048 — use the MT6701-specific SPI driver.
- The gate driver is **DRV8316**, which has a specific SPI configuration interface and nFAULT reporting — do not assume generic DRV83xx behavior.

---

## Known Constraints & Gotchas

| Issue | Detail |
|---|---|
| DFU Bootloader | Hold PB8 (BOOT) during reset to enter USB DFU mode for firmware updates without ST-Link |
| MT6701 angle wrap | Returns 0–16383 counts; must handle wrap-around for multi-turn tracking |
| DRV8316 current sense | Uses internal shunt amplifiers; gain and offset must be calibrated via SPI registers |
| RS-485 half-duplex | DE/RE pin must be driven high before TX and low after TX; race conditions will corrupt frames |
| HRTIM | STM32G474 HRTIM runs at 170 MHz × 32 = 5.44 GHz internally; use SimpleFOC's STM32 HRTIM driver |
| Antigravity + PlatformIO | Not officially supported; validate upload and build before relying on it in production |

---

## References
- [SimpleFOC Documentation](https://docs.simplefoc.com)
- [STM32G474 Reference Manual](https://www.st.com/resource/en/reference_manual/rm0440-stm32g4-series-advanced-armbased-32bit-mcus-stmicroelectronics.pdf)
- [DRV8316 Datasheet](https://www.ti.com/lit/ds/symlink/drv8316.pdf)
- [MT6701 Datasheet](https://www.magntek.com.cn/upload/MT6701_Rev.1.8.pdf)
- [MAX3485 Datasheet](https://www.analog.com/media/en/technical-documentation/data-sheets/MAX3483-MAX3491.pdf)
- [PlatformIO STM32 Docs](https://docs.platformio.org/en/latest/platforms/ststm32.html)
- [Antigravity / PlatformIO .vsix install guide](https://github.com/platformio/platformio-vscode-ide/releases)
