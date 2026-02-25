# MEMORY.md — SCLF Gripper v1.0 Firmware
> Estado persistente del proyecto entre sesiones de desarrollo.
> **Actualizar este fichero al final de cada sesión productiva.**
> El agente de IA debe leer este fichero al inicio de cada sesión.

---

## Estado Actual del Proyecto

| Campo | Valor |
|---|---|
| **Fase activa** | FASE 0 — Entorno y Scaffolding |
| **Última actualización** | 2026-02-25 |
| **Compilación** | ⏳ No verificada aún (PlatformIO no instalado todavía) |
| **Hardware conectado** | ⏳ Pendiente de primera conexión con ST-Link |
| **Motor probado** | ❌ No |

---

## Entorno de Desarrollo

| Campo | Valor |
|---|---|
| IDE | Antigravity (fork de Windsurf/VSCode) |
| PlatformIO | ⏳ Pendiente instalar via `.vsix` |
| cpptools | ⏳ Pendiente instalar via `.vsix` |
| cortex-debug | ⏳ Pendiente instalar |
| OS del desarrollador | Desconocido — confirmar para descargar `.vsix` correcto |
| ST-Link versión | Desconocida — confirmar |

### Links de descarga `.vsix`
- cpptools: https://github.com/microsoft/vscode-cpptools/releases
- platformio-ide: https://github.com/platformio/platformio-vscode-ide/releases

---

## Hardware Verificado

### Esquemático
- **Archivo:** `SCLF_Gripper_v1_0.kicad_sch` ✅ Analizado
- **PCB:** `SCLF_Gripper_v1_0.kicad_pcb` ✅ Disponible
- **BOM:** `SCLF_Gripper_v1_0_bom.csv` ✅ Analizado

### Hallazgos críticos del esquemático
1. `nFAULT` del DRV8316 **NO conectado** al STM32 — solo pull-up local
2. `DRVOFF` del DRV8316 conectado a **GND** — driver siempre habilitado
3. `DE/RE` del MAX3485 = **PB9** (`RS485_DIR`)
4. SPI1 compartido: DRV8316 (CS=PC4) + MT6701 (CS=PA4) en mismo bus PB3/PB4/PB5
5. PB6, PB7, PA15 aparecen como nets nombrados pero **sin función asignada** — reservados

---

## Pin Map Confirmado

| STM32 Pin | Net | Función |
|---|---|---|
| PA0 | CURA | Phase A current sense ADC |
| PA1 | CURB | Phase B current sense ADC |
| PA2 | CURC | Phase C current sense ADC (test pad) |
| PA4 | ENC_CS | MT6701 encoder SPI CS |
| PA5 | ENC_CLK | MT6701 encoder SPI CLK |
| PA6 | ENC_SDO | MT6701 encoder SPI MISO |
| PA8 | CH | Phase C High PWM |
| PA9 | BH | Phase B High PWM |
| PA10 | AH | Phase A High PWM |
| PA11 | D- | USB D- |
| PA12 | D+ | USB D+ |
| PA13 | SWDIO | SWD programming |
| PA14 | SWCLK | SWD programming |
| PB3 | DRV_CLK | DRV8316 SPI CLK (SPI1) |
| PB4 | DRV_MISO | DRV8316 SPI MISO (SPI1) |
| PB5 | DRV_MOSI | DRV8316 SPI MOSI (SPI1) |
| PB8 | BOOT0 | Boot button (DFU mode) |
| PB9 | RS485_DIR | RS-485 DE/RE direction |
| PB11 | VSENS | DC bus voltage sense |
| PB13 | AL | Phase A Low PWM |
| PB14 | BL | Phase B Low PWM |
| PB15 | CL | Phase C Low PWM |
| PC4 | DRV_CS | DRV8316 SPI CS |
| PC6 | LED_PC6 | Status LED |
| PC10 | RS485_TX | RS-485 UART TX |
| PC11 | RS485_RX | RS-485 UART RX |
| PF0 | XIN | Crystal 16 MHz OSC_IN |
| PF1 | XOUT | Crystal 16 MHz OSC_OUT |
| PG10 | RST | Reset button / SWD RST |

---

## Parámetros del Motor

> ⚠️ Rellenar cuando se conozca el motor concreto a usar.

| Parámetro | Valor | Estado |
|---|---|---|
| Fabricante/Modelo | ? | ⏳ Pendiente |
| Tipo | BLDC / PMSM | — |
| Número de pares de polos | ? | ⏳ Pendiente |
| Resistencia de fase (Ω) | ? | ⏳ Pendiente |
| Inductancia de fase (mH) | ? | ⏳ Pendiente |
| Corriente nominal (A) | ? | ⏳ Pendiente |
| Corriente pico (A) | ? | ⏳ Pendiente |
| Velocidad máxima (rpm) | ? | ⏳ Pendiente |

---

## Parámetros FOC Calibrados

> Rellenar tras ejecutar `motor.initFOC()` por primera vez.

| Parámetro | Valor | Fecha |
|---|---|---|
| `zero_electric_angle` | ? | — |
| `sensor_direction` | ? | — |
| PWM frequency (kHz) | ? | — |
| FOC loop time (µs) | ? | — |

---

## PID Gains Activos

> Empezar con valores conservadores. Actualizar aquí cada vez que se cambien.

### Torque / Current loop
| Gain | Valor | Notas |
|---|---|---|
| KP | 0.5 | Inicial conservador |
| KI | 10.0 | Inicial conservador |
| KD | 0.0 | — |

### Velocity loop
| Gain | Valor | Notas |
|---|---|---|
| KP | 0.1 | Inicial conservador |
| KI | 1.0 | Inicial conservador |
| KD | 0.0 | — |

### Position loop
| Gain | Valor | Notas |
|---|---|---|
| KP | 5.0 | Inicial conservador |
| KI | 0.0 | — |
| KD | 0.0 | — |

---

## Límites de Seguridad Activos

| Parámetro | Valor actual | Máximo permitido |
|---|---|---|
| `motor.voltage_limit` | 2.0 V | — (ajustar con motor real) |
| `motor.current_limit` | 1.0 A | — (ajustar con motor real) |
| `motor.velocity_limit` | 10.0 rad/s | — (ajustar con motor real) |

---

## Versiones de Librerías Verificadas

| Librería | Versión | Estado |
|---|---|---|
| SimpleFOC | ^2.3.3 | ⏳ No probada aún |
| STM32duino (platform) | latest | ⏳ No probada aún |
| cpptools | ? | ⏳ Pendiente instalar |
| platformio-ide | ? | ⏳ Pendiente instalar |

---

## Sesiones de Desarrollo

### 2026-02-25 — Sesión inicial
- ✅ Analizado esquemático KiCad completo
- ✅ Verificados todos los pines STM32 contra el esquemático
- ✅ Creados: `SRS.md`, `AGENT.md`, `TASKS.md`, `RULES.md`, `MEMORY.md`, `pins.h`, `platformio.ini`
- ⏳ Pendiente: instalar PlatformIO en Antigravity y verificar primera compilación

### YYYY-MM-DD — [rellenar]
- ...

---

## Notas y Decisiones Pendientes

- [ ] Confirmar el modelo exacto de motor BLDC que se usará con el gripper
- [ ] Confirmar si PB6/PB7/PA15 tienen función en el diseño final o son reservas
- [ ] Confirmar OS del desarrollador para descargar la versión correcta de `.vsix`
- [ ] Decidir si el ID RS-485 del dispositivo se configura por hardware (DIP switch?) o por comando software
- [ ] Decidir frecuencia de polling del DRV8316 STATUS (cada loop? cada 10 ms?)
