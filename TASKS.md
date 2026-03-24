# TASKS.md — SCLF Gripper v1.0 Firmware
> Backlog completo del proyecto, organizado por fases de desarrollo.
> Estado: `[ ]` pendiente · `[~]` en progreso · `[x]` completado · `[!]` bloqueado

---

## FASE 0 — Entorno y Scaffolding
> Objetivo: proyecto compila y hace parpadear un LED. Nada más.

- [x] Crear `platformio.ini` con target `nucleo_g474re`, 170 MHz, ST-Link
- [x] Crear `src/config/pins.h` verificado contra el esquemático KiCad
- [x] Crear `SRS.md`, `AGENT.md`, `RULES.md`, `TASKS.md`, `MEMORY.md`
- [x] Instalar PlatformIO en Antigravity via `.vsix` (cpptools + platformio-ide)
- [x] Instalar extensión `cortex-debug` para debug con ST-Link
- [x] Verificar que `pio run` compila sin errores
- [x] Hacer `pio run --target upload` y confirmar que el ST-Link detecta la placa
- [x] Hacer parpadear `PIN_LED` (PC6/D4) — pre-test `fase0_led_heartbeat` ✅
- [x] Confirmar que el monitor serie USB VCP funciona (`Serial.begin()`) ✅

---

## FASE 1 — Drivers Básicos
> Objetivo: leer encoder, leer corriente, hablar por RS-485. Sin FOC todavía.

### 1.1 MT6701 Encoder Driver
- [x] Crear `src/encoder/MT6701.h` — API pública:
  - `bool begin()` — inicializa SPI (PA5=CLK, PA6=SDO, PA7=MOSI dummy)
  - `float getAngleRad()` — devuelve ángulo en radianes [0, 2π)
  - `uint16_t getRawCounts()` — devuelve valor crudo 14-bit [0–16383]
  - `bool isOk()` — comprueba que la lectura SPI es válida
- [x] Crear `src/encoder/MT6701.cpp` — implementación
- [x] Crear `examples/fase1_1_mt6701_test/main.cpp` — test manual
- [x] Test manual: imprimir ángulo/raw por VCP (Funciona con ST-Link 3.3V)
- [x] Verificar encoder: El LED cambia velocidad y USB muestra raw=0-16383
- [x] Verificar wrap-around correcto (16383 → 0 sin salto brusco en `getAngleRad`)
- [x] Implementar seguimiento de vueltas (ángulos acumulados > 360º)
- [x] Implementar inicialización de cero relativo al encendido

### 1.2 DRV8316 SPI Driver
- [x] Crear `src/motor/DRV8316.h` — API pública:
  - `bool begin()` — inicializa SPI (PC4/PB3/PB4/PB5) y configura registros
  - `uint16_t readRegister(uint8_t addr)`
  - `void writeRegister(uint8_t addr, uint16_t value)`
  - `bool hasFault()` — lee STATUS1/STATUS2 por SPI
  - `uint8_t getFaultCode()` — devuelve código de fallo
  - `void clearFaults()`
- [x] Crear `src/motor/DRV8316.cpp`
- [x] Leer y verificar registro DEVICE_ID del DRV8316 al arrancar
- [x] Imprimir estado de STATUS1 y STATUS2 por VCP
- [x] **IMPORTANTE:** No existe pin nFAULT en el STM32. Todos los fallos se detectan por SPI polling.

### 1.3 Current Sense
- [ ] Crear `src/motor/CurrentSense.h/.cpp` — wrapper para `InlineCurrentSense` de SimpleFOC
  - Shunts: PA0 (CURA), PA1 (CURB), PA2 (CURC)
  - Determinar ganancia del amplificador del DRV8316 (leer registro CSAGAIN por SPI)
- [ ] Calibrar offset de corriente en cero (motor parado, sin corriente)
- [ ] Imprimir corrientes de las 3 fases por VCP en reposo y con motor bloqueado

### 1.4 RS-485 Driver
- [x] Crear `src/comms/RS485.h` — API pública
- [x] Crear `src/comms/RS485.cpp`
  - [x] Control automático de PB9 (HIGH antes de TX, LOW después de TX)
- [x] Test de protocolo simplificado (sin dos puntos)
- [x] Test con dispositivo externo / VCP Simulation ✅

---

## FASE 2 — FOC Básico (Open-Loop)
> Objetivo: motor gira en open-loop con SimpleFOC. Validar que el hardware funciona.

- [ ] Añadir `askuric/Simple FOC @ ^2.3.3` a `lib_deps` en `platformio.ini`
- [ ] Crear `src/motor/MotorController.h/.cpp` — wrapper de SimpleFOC:
  - `BLDCMotor motor` con número de polos del motor objetivo
  - `BLDCDriver6PWM driver` con pines AH/BH/CH/AL/BL/CL
  - `void begin()` — inicializa driver, motor, encoder, current sense
  - `void update()` — llama `motor.loopFOC()` y `motor.move(target)` (llamar en `loop()`)
- [ ] Probar open-loop velocity: `motor.controller = MotionControlType::velocity_openloop`
- [ ] Verificar que el motor gira sin ruido excesivo (ajustar voltage limit)
- [ ] Probar open-loop position: `motor.controller = MotionControlType::angle_openloop`

---

## FASE 3 — FOC Closed-Loop
> Objetivo: FOC completo con encoder y current sense. El motor responde a comandos.

- [ ] Conectar encoder MT6701 a SimpleFOC: `MagneticSensorSPI encoder(PIN_ENC_CS, 14, 0x3FFF)`
- [ ] Ejecutar `motor.initFOC()` — alineación eléctrica automática
  - Capturar y guardar `motor.zero_electric_angle` y `motor.sensor_direction` en flash
- [ ] Probar torque mode: `motor.controller = MotionControlType::torque`
- [ ] Probar velocity closed-loop con encoder
- [ ] Probar position closed-loop con encoder
- [ ] Ajustar PID gains iniciales (KP, KI, KD) para cada loop
- [ ] Conectar `InlineCurrentSense` y activar FOC con corriente real
- [ ] Verificar que `motor.loopFOC()` corre a ≥ 10 kHz sin bloqueos

---

## FASE 4 — Protocolo RS-485
> Objetivo: el gripper obedece comandos del bus del robot.

- [ ] Diseñar e implementar parser de comandos en `src/comms/RS485.cpp`:
  ```
  <id>:<cmd>:<value>\n
  ```
- [ ] Implementar comandos:
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
- [ ] Implementar direccionamiento multi-dispositivo (ID configurable)
- [ ] Test de roundtrip: enviar comando desde PC, verificar respuesta correcta
- [ ] Test de timing: medir latencia TX→RX en bus RS-485

---

## FASE 5 — USB VCP y Commander
> Objetivo: tuning en vivo desde el PC por USB.

- [ ] Activar `SimpleFOCDebug::enable(&Serial)` — telemetría por USB VCP
- [ ] Crear `src/comms/VCP.cpp` — Commander de SimpleFOC:
  - Registrar motor, PID gains, modo de control
  - Stream de telemetría en tiempo real: ángulo, velocidad, corriente, target
- [ ] Verificar que VCP no bloquea el loop FOC (usar buffer circular o DMA)
- [ ] Documentar comandos Commander disponibles en `MEMORY.md`

---

## FASE 6 — Fault Manager
> Objetivo: el gripper nunca se destruye a sí mismo.

- [ ] Crear `src/faults/FaultManager.h/.cpp`:
  - `void update()` — llamar en cada loop, polling del DRV8316 STATUS por SPI
  - `bool hasFault()` — devuelve true si hay cualquier fallo activo
  - `FaultCode getActiveFault()` — enum con: OVERCURRENT, ENCODER_ERROR, STALL, DRV_FAULT
  - `void triggerSafeState()` — llama `motor.disable()` en < 1 ms
  - `void reportFault(FaultCode)` — envía código por RS-485 y VCP
- [ ] Detectar stall: velocidad < umbral durante > N ms con corriente > límite
- [ ] Detectar error de encoder: `MT6701::isOk()` devuelve false
- [ ] Test de fault: cortocircuitar brevemente una fase y verificar respuesta
- [ ] Verificar que `triggerSafeState()` ocurre en < 1 ms desde detección

---

## FASE 7 — Persistencia y Configuración
> Objetivo: los parámetros sobreviven a un reset.

- [ ] Definir estructura `Config` en flash (sector dedicado STM32G474):
  - `zero_electric_angle` (float)
  - `sensor_direction` (int)
  - `device_id` (uint8_t)
  - `pid_gains[3][3]` (float[loop][kp/ki/kd])
  - `control_mode` (uint8_t)
  - CRC32 para validación
- [ ] Implementar `src/config/FlashConfig.h/.cpp`:
  - `bool load()` — lee config de flash, valida CRC
  - `void save()` — escribe config en flash
  - `void resetDefaults()` — valores por defecto
- [ ] Al arrancar: si la config en flash es válida, saltar la alineación automática
- [ ] Comandos RS-485 para leer/escribir configuración y hacer reset de fábrica

---

## FASE 8 — Tests y Validación
> Objetivo: el firmware es fiable y reproducible.

- [ ] Crear tests en `test/` con PlatformIO Unity framework:
  - `test_pid.cpp` — verificar PID responde correctamente a setpoint conocido
  - `test_rs485_parser.cpp` — verificar parser con frames válidos e inválidos
  - `test_angle_wrap.cpp` — verificar que el wrap 16383→0 no genera salto
  - `test_fault_codes.cpp` — verificar que los fault codes se generan correctamente
- [ ] Ejecutar `pio test` y verificar 0 fallos
- [ ] Medir y documentar tiempos de loop (FOC, velocity, position) en `MEMORY.md`
- [ ] Medir consumo de flash y RAM, verificar < 80% de límite

---

## FASE 9 — Documentación Final
- [ ] Actualizar `MEMORY.md` con parámetros PID de partida validados
- [ ] Actualizar `MEMORY.md` con offset eléctrico y dirección del encoder
- [ ] Escribir `README.md` de usuario final (cómo conectar, cómo configurar ID, comandos RS-485)
- [ ] Crear diagrama de estados de la máquina de control (Mermaid)
- [ ] Anotar versión de SimpleFOC y STM32duino usadas y verificadas

---

## Backlog / Ideas Futuras
> Sin fecha. Evaluar cuando las fases principales estén completas.

- [ ] Soporte CAN bus (el STM32G474 tiene FDCAN — requiere hardware externo)
- [ ] Perfil de movimiento trapezoidal (aceleración/deceleración suave)
- [ ] Auto-tuning de PID por respuesta al escalón
- [ ] OTA firmware update por RS-485 (bootloader custom)
- [ ] Dashboard web de monitorización en tiempo real (PC side)

---

## Registro de Decisiones Técnicas

| Fecha | Decisión | Razón |
|---|---|---|
| 2026-02-25 | nFAULT del DRV8316 no conectado al STM32 | Confirmado en esquemático KiCad |
| 2026-02-25 | DRVOFF del DRV8316 a GND permanente | Driver siempre habilitado por hardware |
| 2026-02-25 | Fault detection por SPI polling, no por GPIO IRQ | Sin pin disponible para nFAULT |
| 2026-02-25 | SPI compartido entre DRV8316 (PC4 CS) y MT6701 (PA4 CS) | PB3/PB4/PB5 son el bus SPI1 |
| 2026-02-25 | RS-485 half-duplex con PB9 como DE/RE | Confirmado en esquemático |
| 2026-03-07 | Board cambiado de `genericSTM32G474CE` a `nucleo_g474re` | `generic` no soporta Arduino framework |
| 2026-03-07 | SPIClass MOSI: usar `PA7` en vez de `NC` | `NC` bloquea la inicialización del periférico SPI |
| 2026-03-07 | ST-Link 3.3V alimenta el MCU y el Encoder | **El MT6701 funciona sin la fuente de 24V**. Solo el driver lo necesita. |
| 2026-03-07 | **Solución USB CDC:** Añadido `SystemClock_Config` en `main.cpp` | STM32G4 requiere activar `HSI48` para el reloj USB |
| 2026-03-07 | Solución de testeo Half-Duplex RS-485 | MAX3485 bloquea RX durante TX. Creado `simulateRx()` para bypass y test. |
| 2026-03-14 | **HARDWARE BUG IDENTIFICADO en v1.0:** Pin 23 de U1 (DRV8316C) conectado a D5 | El nSLEEP estaba apagado a 0.25V. Retirar D5 y puntear a 3.3V para activar SPI. |
