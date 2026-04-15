# TASKS.md — SCLF Gripper Firmware
> Complete project backlog, organized by development phases.
> Status: `[ ]` pending · `[~]` in progress · `[ ]` completed · `[!]` blocked

---
> 📢 **CURRENT STATUS & NEXT STEPS**
> **New prototype validation phase.**
> We have updated all pinouts (`hardware/DRV-uC_connections.md` and `src/config/pins.h`). The next step is to **re-run and validate Phases 1.1 (Encoder) and 1.2 (DRV SPI)** using the `platformio.ini` test environment to confirm the new connections work correctly.
---

---

## PHASE 0 — Environment and Scaffolding
> Goal: project compiles and blinks an LED. Nothing else.

- [x] Create `platformio.ini` with target `nucleo_g474re`, 170 MHz, ST-Link
- [x] Create `src/config/pins.h` verified against the KiCad schematic
- [x] Create `SRS.md`, `AGENT.md`, `RULES.md`, `TASKS.md`, `MEMORY.md`
- [x] Install PlatformIO in Antigravity via `.vsix` (cpptools + platformio-ide)
- [x] Install `cortex-debug` extension for ST-Link debugging
- [x] Verify `pio run` compiles without errors
- [x] Run `pio run --target upload` and confirm the ST-Link detects the board
- [x] Blink `PIN_LED` (PC6/D4) — pre-test `fase0_led_heartbeat` ✅
- [x] Confirm the USB VCP Serial Monitor works (`Serial.begin()`) ✅

---

## PHASE 1 — Basic Drivers
> Goal: read the encoder, read current, communicate via RS-485. No FOC yet.

### 1.1 MT6701 Encoder Driver [x]
- [x] Create `src/encoder/MT6701.h` — Public API:
  - `bool begin()` — initializes SPI (PA5=CLK, PA6=SDO, PA7=MOSI dummy)
  - `float getAngleRad()` — returns angle in radians [0, 2π)
  - `uint16_t getRawCounts()` — returns raw 14-bit value [0–16383]
  - `bool isOk()` — verifies SPI read is valid
- [x] Create `src/encoder/MT6701.cpp` — implementation
- [x] Create `examples/fase1_1_mt6701_test/main.cpp` — manual test
- [x] Manual test: print angle/raw via VCP (Works with ST-Link 3.3V)
- [x] Verify encoder: The LED changes speed and USB shows raw=0-16383
- [x] Verify correct wrap-around (16383 → 0 without abrupt jump in `getAngleRad`)
- [x] Implement multi-turn tracking (accumulated angles > 360º)
- [x] Implement relative zero initialization on startup

### 1.2 DRV8316 SPI Driver [x]
- [x] Create `src/motor/DRV8316.h` — Public API:
  - `bool begin()` — initializes SPI (PC4/PB3/PB4/PB5) and configures registers
  - `uint16_t readRegister(uint8_t addr)`
  - `void writeRegister(uint8_t addr, uint16_t value)`
  - `bool hasFault()` — reads STATUS1/STATUS2 via SPI
  - `uint8_t getFaultCode()` — returns error code
  - `void clearFaults()`
- [x] Create `src/motor/DRV8316.cpp`
- [x] Read and verify DRV8316 DEVICE_ID register on startup
- [x] Print STATUS1 and STATUS2 via VCP
- [x] **IMPORTANT:** There is no nFAULT pin on the STM32. All faults are detected via SPI polling.

### 1.3 Current Sense
- [ ] Create `src/motor/CurrentSense.h/.cpp` — wrapper for SimpleFOC's `InlineCurrentSense`
  - Shunts: PA0 (CURA), PA1 (CURB), PA2 (CURC)
  - Determine DRV8316 amplifier gain (read CSAGAIN register via SPI)
- [ ] Calibrate current offset at zero (motor stopped, no current)
- [ ] Print currents from the 3 phases via VCP with stopped/blocked motor

### 1.4 RS-485 Driver
- [ ] Create `src/comms/RS485.h` — Public API:
  - `void begin(uint32_t baudrate)` — UART3 (PC10/PC11) + direction PB9
  - `void send(uint8_t deviceId, const char* cmd, const char* value)`
  - `bool receive(char* buf, size_t maxLen)` — non-blocking, returns true if full frame
- [ ] Create `src/comms/RS485.cpp`
  - Automatic control of PB9 (HIGH before TX, LOW after TX)
  - Use `HAL_UART_Transmit` with short timeout, then lower PB9 immediately
- [ ] Loopback test: connect RS485+ physically to RS485- and verify echo
- [ ] Test with an external device (PC with USb-RS485 adapter)

---

## PHASE 2 — Basic FOC (Open-Loop)
> Goal: motor spins in open-loop using SimpleFOC. Validate hardware works.

- [ ] Add `askuric/Simple FOC @ ^2.3.3` to `lib_deps` in `platformio.ini`
- [ ] Create `src/motor/MotorController.h/.cpp` — SimpleFOC wrapper:
  - `BLDCMotor motor` with target motor pole count
  - `BLDCDriver6PWM driver` using AH/BH/CH/AL/BL/CL pins
  - `void begin()` — initializes driver, motor, encoder, and current sense
  - `void update()` — calls `motor.loopFOC()` and `motor.move(target)`
- [ ] Test open-loop velocity: `motor.controller = MotionControlType::velocity_openloop`
- [ ] Verify motor spins without excessive noise (tweak voltage limit)
- [ ] Test open-loop position: `motor.controller = MotionControlType::angle_openloop`

---

## PHASE 3 — Closed-Loop FOC
> Goal: Full FOC with encoder and current sense. Motor responds to targets.

- [ ] Connect MT6701 encoder to SimpleFOC: `MagneticSensorSPI encoder(PIN_ENC_CS, 14, 0x3FFF)`
- [ ] Run `motor.initFOC()` — automatic electrical alignment
  - Capture and save `motor.zero_electric_angle` and `motor.sensor_direction` to flash
- [ ] Test torque mode: `motor.controller = MotionControlType::torque`
- [ ] Test closed-loop velocity with encoder
- [ ] Test closed-loop position with encoder
- [ ] Tune initial PID gains (KP, KI, KD) for each loop
- [ ] Connect `InlineCurrentSense` and activate FOC with real currents
- [ ] Verify `motor.loopFOC()` runs at ≥ 10 kHz without blocking

---

## PHASE 4 — RS-485 Protocol
> Goal: the gripper obeys bus commands from the robot.

- [ ] Design and implement command parser in `src/comms/RS485.cpp`:
  ```
  <id>:<cmd>:<value>\n
  ```
- [ ] Implement commands:
  - `T` — set torque target (A)
  - `V` — set velocity target (rad/s)
  - `P` — set position target (rad)
  - `M` — set control mode (0=torque, 1=vel, 2=pos)
  - `EN` / `DIS` — enable/disable motor
  - `KP`, `KI`, `KD` — set PID gains
  - `?A` — get angle (rad)
  - `?V` — get velocity (rad/s)
  - `?I` — get current (A)
  - `?S` — get status / fault code
- [ ] Implement multi-device addressing (configurable ID)
- [ ] Roundtrip test: send command from PC, verify valid response
- [ ] Timing test: measure TX→RX latency on RS-485 bus

---

## PHASE 5 — USB VCP & Commander
> Goal: live tuning from PC over USB.

- [ ] Activate `SimpleFOCDebug::enable(&Serial)` — telemetry via USB VCP
- [ ] Create `src/comms/VCP.cpp` — SimpleFOC Commander:
  - Register motor, PID gains, control mode
  - Real-time telemetry streaming: angle, velocity, current, target
- [ ] Verify VCP does not block the FOC loop (use circular buffers or DMA)
- [ ] Document available Commander commands in `MEMORY.md`

---

## PHASE 6 — Fault Manager
> Goal: the gripper never self-destructs.

- [ ] Create `src/faults/FaultManager.h/.cpp`:
  - `void update()` — call every loop, poll DRV8316 STATUS via SPI
  - `bool hasFault()` — returns true if any active fault exists
  - `FaultCode getActiveFault()` — enum: OVERCURRENT, ENCODER_ERROR, STALL, DRV_FAULT
  - `void triggerSafeState()` — calls `motor.disable()` in < 1 ms
  - `void reportFault(FaultCode)` — reports code via RS-485 and VCP
- [ ] Detect stall: velocity < threshold for > N ms with current > limit
- [ ] Detect encoder error: `MT6701::isOk()` returns false
- [ ] Fault test: short-circuit a phase briefly and verify response
- [ ] Verify `triggerSafeState()` occurs in < 1 ms after detection

---

## PHASE 7 — Persistence & Configuration
> Goal: parameters survive power cycles and resets.

- [ ] Define `Config` struct in flash (dedicated STM32G474 sector):
  - `zero_electric_angle` (float)
  - `sensor_direction` (int)
  - `device_id` (uint8_t)
  - `pid_gains[3][3]` (float[loop][kp/ki/kd])
  - `control_mode` (uint8_t)
  - CRC32 validation
- [ ] Implement `src/config/FlashConfig.h/.cpp`:
  - `bool load()` — reads config from flash, validates CRC
  - `void save()` — writes config to flash
  - `void resetDefaults()` — applies default values
- [ ] On startup: if flash config is valid, skip automatic alignment
- [ ] RS-485 commands to read/write config and perform factory reset

---

## PHASE 8 — Tests & Validation
> Goal: firmware is reliable and stable.

- [ ] Create tests in `test/` using PlatformIO Unity framework:
  - `test_pid.cpp` — verify PID responds properly to setpoints
  - `test_rs485_parser.cpp` — verify parser with valid/invalid frames
  - `test_angle_wrap.cpp` — verify 16383→0 wrap generates no gaps
  - `test_fault_codes.cpp` — verify fault codes decode correctly
- [ ] Run `pio test` and check for 0 failures
- [ ] Measure and document loop times in `MEMORY.md`
- [ ] Measure flash/RAM consumption, ensure < 80% limit

---

## PHASE 9 — Final Documentation
- [ ] Update `MEMORY.md` with final starting PID parameters
- [ ] Update `MEMORY.md` with electric offset and encoder direction
- [ ] Write `README.md` for end users (connections, configuration, RS-485 commands)
- [ ] Create a State Machine diagram (Mermaid)
- [ ] Annotate verified versions of SimpleFOC and STM32duino

---

## Backlog / Future Ideas
> No date. Evaluate once main phases are complete.

- [ ] CAN bus support (STM32G474 has FDCAN — needs external hardware)
- [ ] Trapezoidal motion profile (smooth acceleration/deceleration)
- [ ] PID Auto-tuning via step response
- [ ] OTA firmware updates over RS-485 (custom bootloader)
- [ ] Live visual PC Dashboard

---

## Technical Decisions Log

| Date | Decision | Reason |
|---|---|---|
| 2026-02-25 | DRV8316 nFAULT not connected to STM32 | Verified in KiCad schematic |
| 2026-02-25 | DRV8316 DRVOFF to permanent GND | Driver always enabled by hardware |
| 2026-02-25 | Fault detection via SPI polling, not GPIO IRQ | No available pin for nFAULT |
| 2026-02-25 | SPI shared between DRV8316 (PC4 CS) & MT6701 (PA4 CS) | PB3/PB4/PB5 are the SPI1 bus |
| 2026-02-25 | RS-485 half-duplex with PB9 as DE/RE | Confirmed in schematic |
| 2026-03-07 | Board changed from `genericSTM32G474CE` to `nucleo_g474re` | `generic` does not support Arduino framework |
| 2026-03-07 | SPIClass MOSI: use `PA7` instead of `NC` | `NC` blocks SPI initialization |
| 2026-03-07 | ST-Link 3.3V powers MCU and Encoder | **MT6701 works without 24V supply**. Only the driver needs it. |
| 2026-03-07 | **USB CDC Fix:** Added `SystemClock_Config` in `main.cpp` | STM32G4 requires enabling HSI48 for the USB peripheral |
| 2026-03-07 | RS-485 Half-Duplex testing challenge | MAX3485 blocks RX during TX. Created `simulateRx()` test branch to bypass. |
| 2026-03-14 | **HARDWARE BUG v1: U1 Pin 23 to D5** | Driver nSLEEP was powered down. Will be fixed in v2. |
| 2026-03-24 | **WAITING FOR HARDWARE V2** | We have updated pins and are blocked waiting for PCB. |
