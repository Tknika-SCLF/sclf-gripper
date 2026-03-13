# SCLF Gripper v1.0 — Firmware

[ Euskara ](README.md) | [ English ](README.en.md) | [ Español ](README.es.md)

> Motor controller firmware for Tknika's SCLF Gripper v1.0.
> STM32G474CEU6 · SimpleFOC · PlatformIO · Antigravity IDE
> License: CC BY-SA 4.0

---

## Quick Start

If you already have the environment configured:

```bash
# 1. Build
pio run

# 2. Flash (with ST-Link connected)
pio run --target upload

# 3. Serial Monitor (Gripper's USB-C to PC)
pio device monitor --baud 115200
```

---

## Environment Setup from Scratch

### Step 1 — Install Antigravity

Download and install Antigravity from its official website. It is a fork of VSCode/Windsurf and works identically.

---

### Step 2 — Install C/C++ Tools

> ⚠️ PlatformIO is **not available in Open VSX** (Antigravity's marketplace). Both extensions must be installed manually from `.vsix` files.

**2a.** Go to: https://github.com/microsoft/vscode-cpptools/releases/latest

Download the file for your operating system:

| System | File to download |
|---|---|
| Windows (64-bit) | `cpptools-windows-x64.vsix` |
| macOS (Apple Silicon) | `cpptools-macOS-arm64.vsix` |
| macOS (Intel) | `cpptools-macOS-x64.vsix` |
| Linux (64-bit) | `cpptools-linux-x64.vsix` |

**2b.** In Antigravity: `Extensions (Ctrl+Shift+X)` → `···` icon (three dots) → **Install from VSIX** → select the downloaded `.vsix` file.

---

### Step 3 — Install PlatformIO

**3a.** Go to: https://github.com/platformio/platformio-vscode-ide/releases/latest

Open the **"Assets"** section of the latest release and download the file (e.g., `platformio-ide-3.3.3.vsix`).

**3b.** In Antigravity: `Extensions (Ctrl+Shift+X)` → `···` → **Install from VSIX** → select the downloaded `.vsix` file.

**3c.** Restart Antigravity. PlatformIO will install its dependencies automatically the first time (may take 2-5 minutes).

> ⚠️ **Important Note:** PlatformIO inside Antigravity is not officially supported. If you have build or upload issues, check the [Troubleshooting](#troubleshooting) section at the end of this document.

---

### Step 4 — Install cortex-debug (for ST-Link debugging)

> 💡 **What is Open VSX?** It's an open-source extension registry managed by the Eclipse Foundation. It is the official and secure repository used by Antigravity and VSCodium to bypass Microsoft Marketplace restrictions.

**4a.** Go to: https://open-vsx.org/extension/marus25/cortex-debug

On the right side, in the **"Version"** dropdown menu, select the latest stable version that does NOT say **"Pre-release"** (e.g., `1.12.1`). Then, click the purple **`DOWNLOAD`** button below it to get the `.vsix` file.

**4b.** Install as above (Install from VSIX).

**4c.** Configure OpenOCD:
> 💡 **Good news:** You don't need to download or install it manually! PlatformIO automatically downloads OpenOCD the first time you build or upload the project. Its location on Windows is typically:
`C:\Users\YOUR_USERNAME\.platformio\packages\tool-openocd\bin\openocd.exe`

*(If Cortex-Debug throws an error saying it cannot find OpenOCD, go to Antigravity settings and set this exact path in the `cortex-debug.openocdPath` setting).*

---

### Step 5 — Install Git

To clone the project you need to have Git installed.

**5a.** Go to: https://git-scm.com/downloads

**5b.** Download and install the version for your operating system (you can keep the default options during installation).

---

### Step 6 — Open the Project

```bash
# Clone or copy the firmware folder
# Open Antigravity in the root folder (where platformio.ini is located)
```

In Antigravity: `File → Open Folder` → select the `SCLF_Gripper_v1_0_firmware/` folder.

PlatformIO will automatically detect `platformio.ini` and download the following:
- Platform `ststm32`
- Framework `framework-arduinoststm32`
- Toolchain `toolchain-gccarmnoneeabi`
- `SimpleFOC` library

---

### Step 7 — First Build

In Antigravity's bottom bar, click PlatformIO's ✓ (Build) icon, or from the terminal:

```bash
pio run
```

**Expected output:**
```
[SUCCESS] Took X.XX seconds
RAM:   [=         ]  X.X% (used XXXX bytes from 131072 bytes)
Flash: [=         ]  X.X% (used XXXXX bytes from 524288 bytes)
```

If it fails to build → see [Troubleshooting](#troubleshooting).

---

### Step 8 — Connect ST-Link and Flash

Connect the ST-Link to the SCLF Gripper's SWD port:

| ST-Link pin | Gripper pad | Signal |
|---|---|---|
| SWDIO | DIO | PA13 |
| SWCLK | CLK | PA14 |
| RST | RST | PG10 |
| GND | GND | — |
| 3.3V | 3V3 | (powers STM32 only, not encoder or driver) |

> ⚠️ **Power supply:**
> - The ST-Link's 3.3V pin **IS sufficient** to flash the STM32 and run basic code (LED blink).
> - **BUT** the MT6701 encoder and DRV8316 driver are powered from the DRV8316's internal step-down converter, which requires **external 24V**. Without 24V, the encoder always returns `0xFFFF` (floating MISO line).
> - For full tests (encoder + motor), always connect 24V.

Flash:
```bash
pio run --target upload
```

**Expected output:** `[SUCCESS]` and LED D4 (PC6) will start blinking.

#### Recommended pre-test: LED Heartbeat

Before attempting complex tests, verify the STM32 boots:

1. Change `build_src_filter` in `platformio.ini` to:
   ```ini
   build_src_filter = +<../examples/fase0_led_heartbeat>
   ```
2. Flash with `pio run --target upload`.
3. LED D4 should blink at 5Hz (200ms). If it doesn't blink → power or ST-Link wiring issue.
4. Restore the original `build_src_filter` when you want to go back to Phase 1.1.

---

### Step 9 — Verify Encoder (Phase 1.1)

> ⚠️ **Requirements:** 24V power supply connected (pogo pins or bench supply). Without 24V, the MT6701 encoder has no power and SPI always returns `0xFFFF`.

The current `fase1_1_mt6701_test` firmware uses LED D4 as visual diagnostics (no Serial):

1. Make sure `build_src_filter` in `platformio.ini` is:
   ```ini
   build_src_filter = +<../examples/fase1_1_mt6701_test> +<encoder/>
   ```
2. Flash: `pio run --target upload`
3. Observe LED D4:

| LED behavior | Meaning |
|---|---|
| 3 quick flashes on startup | `setup()` completed OK |
| LED slow (700ms, ~0.7Hz) | Motor in first half of rotation (0°-180°) |
| LED fast (80ms, ~6Hz) | Motor in second half of rotation (180°-360°) |
| **Changes when rotating motor** ✅ | **MT6701 encoder works!** |
| Always fast without changing | SPI returns 0xFFFF → encoder unpowered |

✅ **If the LED changes when you rotate the motor, Phase 1.1 is passed.**

### Step 9b — Verify USB Serial Monitor (VCP)

> ✅ **STATUS:** Works correctly. The firmware includes a `SystemClock_Config` override block in `main.cpp` that enables the internal HSI48 oscillator required for the STM32G4 USB peripheral.

1. Connect a USB-C cable between the Gripper and the PC.
2. Open PlatformIO's serial monitor (🔌 icon in the bottom bar or command palette) or run:
   ```bash
   pio device monitor --baud 115200
   ```
3. You should see the current test output (e.g. `MT6701 Raw (con o sin 24V): X` or a value changing as you rotate).

---

### Step 10 — Configure AI Agent

To work with Antigravity's built-in AI assistant:

1. Open the agent's chat panel (AI icon in the sidebar).
2. At the beginning of each session, paste:

```
Read these context files before helping:
- AGENT.md   (hardware, pins, architecture)
- MEMORY.md  (persistent state, calibrated parameters)
- TASKS.md   (the task I'm working on now)
- RULES.md   (rules generated code must respect)
```

3. Then, describe which task you want to tackle from `TASKS.md`.

---

## Project Structure

```
SCLF_Gripper_v1_0_firmware/
│
├── src/
│   ├── main.cpp                    ← Entry point. PHASE 0: smoke test
│   ├── config/
│   │   └── pins.h                  ← ALL Pins. Verified against schematic.
│   ├── encoder/
│   │   ├── MT6701.h                ← Encoder API (skeleton ready)
│   │   └── MT6701.cpp              ← Implement in PHASE 1.1
│   ├── motor/
│   │   ├── DRV8316.h               ← Gate driver API (skeleton ready)
│   │   └── DRV8316.cpp             ← Implement in PHASE 1.2
│   ├── comms/
│   │   ├── RS485.h                 ← RS-485 API (skeleton ready)
│   │   └── RS485.cpp               ← Implement in PHASE 1.4
│   └── faults/
│       ├── FaultManager.h          ← Fault manager API (skeleton ready)
│       └── FaultManager.cpp        ← Implement in PHASE 6
│
├── lib/                            ← Local libraries (currently empty)
├── test/                           ← PlatformIO Unity tests (PHASE 8)
│
├── .vscode/
│   ├── settings.json               ← IDE config (IntelliSense, formatting)
│   ├── extensions.json             ← Recommended extensions
│   └── launch.json                 ← Debug ST-Link with cortex-debug
│
├── platformio.ini                  ← Build and upload config
├── .clang-format                   ← Code style (applied on save)
├── .gitignore
│
├── AGENT.md    ← 📋 Context for the AI agent
├── MEMORY.md   ← 🧠 Persistent project state
├── TASKS.md    ← ✅ Full backlog by phases
├── RULES.md    ← 📏 Project rules
└── SRS.md      ← 📄 Software Requirements Specification
```

### Modular Architecture Explanation

This project is developed to be decoupled and organized:
* **`main.cpp` ("The Conductor"):** Contains only the FOC control loop and initial setup. It is not cluttered with spaghetti code.
* **Specific `.cpp` files ("The Musicians"):** Each chip has its own dedicated file for its more complex functions (e.g., `MT6701.cpp` to read position directly handling errors, or `DRV8316.cpp` to report temperature or short-circuit faults).
* **This way:** `main.cpp` gives basic commands, and each module knows exactly how to play its instrument, making future hardware changes incredibly easy.

### Hardware Safety & Development (⚠️ VERY IMPORTANT)

While developing this project in Antigravity, [Strict Rules (RULES.md)](RULES.md) are established:
1. **Blocking is Prohibited:** Never use `delay()`, `HAL_Delay`, or infinite `while()` loops within the FOC execution. SimpleFOC requires 100% of the MCU's attention to calculate real-time current. Putting the MCU to "sleep" can result in burning the motor or driver in a second.
2. **`foc-hardware-setup` Skill:** Before writing code, our AI assistant has a specific tool to read KiCad electrical schematics directly (`.kicad_sch`) and generate the perfect `pins.h` file with zero human error. It is your friend; use it to verify the final pin assignments of the project!
3. **Cortex-Debug Magic:** With an ST-Link v2 clone or original, the board can be live-controlled and paused. If you want to discover why a variable takes a certain value in the program, set a Breakpoint in Antigravity, and the code will pause directly on the STM32!

---

## Most Used PlatformIO Commands

```bash
pio run                          # Build
pio run --target upload          # Build and flash (ST-Link)
pio run --target upload_dfu      # Flash via USB DFU (BOOT + RST)
pio device monitor               # Serial monitor (USB VCP, 115200 baud)
pio test                         # Run unit tests
pio run --target clean           # Clean build artifacts
pio lib install "Simple FOC"     # Install library manually
pio pkg update                   # Update platform and libraries
```

---

## DFU Bootloader (Flash without ST-Link)

If you don't have an ST-Link available, you can flash via USB DFU:

1. Press and hold the **BOOT** (PB8) button.
2. Press and release the **RESET** (PG10) button.
3. Release **BOOT**.
4. The Gripper will appear as a DFU device on the PC.
5. Change to `upload_protocol = dfu` in `platformio.ini`.
6. Run `pio run --target upload`.

> 💡 **Note on using DFU without ST-Link (PA9 pin):**
> On older STM32s (like F1/F4), the `PA9` pin is required for USB VBUS sensing to detect if the cable is plugged in. In our design, `PA9` is routed to the motor driver's Phase B (`PIN_BH`). Do not worry! The **STM32G4 family has VBUS sensing disabled** in its internal ROM Bootloader (AN2606). Therefore, DFU flashing will work perfectly using only the `PA11` and `PA12` data pins, with no hardware modifications needed.
> 
> **⚠️ CAUTION Regarding Debugging:**
> Be aware that the USB DFU method is a "blind upload": it serves **only** to copy the code image (`.bin`) into the MCU's memory. When you are in the development phase and need to investigate unexpected behavior from the SimpleFOC motor, **using an ST-Link (or SWD probe) is essential**. An ST-Link connects directly to the ARM Cortex core via `PA13/PA14`, allowing you to pause execution (Breakpoints), read real-time variable values, and step through the code line by line in VS Code. USB DFU cannot do this.

---

## Troubleshooting

### PlatformIO fails to build / toolchain error
```bash
# Reinstall STM32 platform
pio platform install ststm32

# Clean and rebuild
pio run --target clean && pio run
```

### ST-Link does not detect the board
- Verify ST-Link GND is connected to Gripper GND.
- Verify board is powered (PWR LED is on).
- Try `openocd -f interface/stlink.cfg -f target/stm32g4x.cfg` in the terminal.
- If OpenOCD fails: update ST-Link firmware via STM32CubeProgrammer.

### USB VCP does not appear on PC
- Install STM32 VCP driver: https://www.st.com/en/development-tools/stsw-stm32102.html (Windows only)
- No driver needed on Linux/macOS. Check with `ls /dev/ttyACM*`.

### LED does not blink after flashing
- **Make sure you have the clock override:** The STM32duino framework with `-DUSBCON` flags (enabled by default in this project) tries to initialize USB CDC **before** `setup()`. If the `SystemClock_Config(void)` function enabling HSI48 is missing at the end of your `main.cpp` file, the firmware hangs silently.
- Try `examples/fase0_led_heartbeat` (LED only, no dependencies) making sure it builds and uploads fine. If that doesn't blink either → power or wiring issue.

### Serial Monitor shows no data (blank screen)
- Make sure you're connecting to the correct COM port. The ST-Link generates a separate COM port: that one is always silent. Look for "STM32 Virtual COM Port" in Device Manager.
- If you just flashed the board, Windows might take a second to recognize the new USB COM. If PlatformIO opened the monitor on the old port, it will hang. Cancel in the terminal with `Ctrl+C` and run `pio device monitor` again.
- The `monitor_port` in `platformio.ini` must NOT have a hardcoded Linux path (`/dev/ttyACM0`). On Windows, leave it empty for auto-detection.

### IntelliSense cannot find `Arduino.h` or STM32 headers
- Wait for PlatformIO to finish its first build (generates includes index).
- If issue persists: `Ctrl+Shift+P` → "C/C++: Reset IntelliSense Database".

### Antigravity / PlatformIO fails to communicate with board
- This is a known issue documented by the community. PlatformIO in Antigravity
  is not officially supported. If building works but upload fails,
  try running `pio run --target upload` **from the internal terminal** instead
  of using the PlatformIO UI button.

### PlatformIO keeps asking for "ms-vscode.cpptools" (Dependency Surgery)
PlatformIO sometimes forces the installation of the Microsoft extension even if it's already installed manually. To fix this:
1. Close Antigravity completely.
2. Open your file explorer and go to: `%USERPROFILE%\.antigravity\extensions\` (Windows) or `~/.antigravity/extensions/` (Linux/macOS).
3. Open the folder starting with `platformio.platformio-ide-...`.
4. Open the `package.json` file with a text editor.
5. Search for `"extensionDependencies": ["ms-vscode.cpptools"]`.
6. Leave it empty like this: `"extensionDependencies": []`
7. Save the file and restart Antigravity.

---

## Reference Hardware

| Component | Part | Datasheet |
|---|---|---|
| MCU | STM32G474CEU6 | [ST.com](https://www.st.com/resource/en/reference_manual/rm0440-stm32g4-series-advanced-armbased-32bit-mcus-stmicroelectronics.pdf) |
| Gate Driver | DRV8316CRRGFR | [TI.com](https://www.ti.com/lit/ds/symlink/drv8316.pdf) |
| Encoder | MT6701QT | [Magntek](https://www.magntek.com.cn/upload/MT6701_Rev.1.8.pdf) |
| RS-485 | MAX3485 | [Analog](https://www.analog.com/media/en/technical-documentation/data-sheets/MAX3483-MAX3491.pdf) |
| SimpleFOC | v2.x | [docs.simplefoc.com](https://docs.simplefoc.com) |

---

*Designed by Tknika — Licensed under CC BY-SA 4.0*
