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

### Step 5 — Open the Project

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

### Step 6 — First Build

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

### Step 7 — Connect ST-Link and Flash

Connect the ST-Link to the SCLF Gripper's SWD port:

| ST-Link pin | Gripper pad | Signal |
|---|---|---|
| SWDIO | DIO | PA13 |
| SWCLK | CLK | PA14 |
| RST | RST | PG10 |
| GND | GND | — |
| 3.3V | — | (power the board separately with 24V) |

> ⚠️ **Do not power the board from the ST-Link**. Use an external 24V power supply from the robot or a desktop PSU.

Flash:
```bash
pio run --target upload
```

**Expected output:** LED (PC6) will start blinking every 500ms.

---

### Step 8 — Verify USB VCP

1. Connect a USB-C cable between the Gripper and the PC.
2. Open PlatformIO's serial monitor (🔌 icon in the bottom bar) or:
   ```bash
   pio device monitor --baud 115200
   ```
3. You should see this:
   ```
   ========================================
     SCLF Gripper v1.0 — Firmware PHASE 0
   ========================================
     MCU: STM32G474CEU6 @ 170 MHz
     State: SMOKE TEST (No FOC, No motor)
   ========================================
     Pins initialized correctly.
   [0s] loops/s≈XXXXX  LED=ON
   [1s] loops/s≈XXXXX  LED=OFF
   ```

✅ **If you see this, the environment is 100% operational.** Update `MEMORY.md` and mark PHASE 0 tasks as completed in `TASKS.md`.

---

### Step 9 — Configure AI Agent

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
