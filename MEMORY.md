# MEMORY.md — SCLF Gripper v1.0 Firmware
> Estado persistente del proyecto entre sesiones de desarrollo.
> **Actualizar este fichero al final de cada sesión productiva.**
> El agente de IA debe leer este fichero al inicio de cada sesión.

---

## Estado Actual del Proyecto

| Campo | Valor |
|---|---|
| **Fase activa** | FASE 1 — Inicialización FOC y Hardware |
| **Última actualización** | 2026-02-25 |
| **Compilación** | ✅ Exitosa. Solucionados bugs de PlatformIO (MAX_PATH y Unicode Linker). |
| **Hardware conectado** | ⏳ Pendiente probar subida (Upload) con ST-Link |
| **Motor probado** | ❌ No |

---

## Entorno de Desarrollo

| Campo | Valor |
|---|---|
| IDE | Antigravity / Solucionado bug Unicode en PlatformIO |
| PlatformIO Core | Configurado en `C:/Users/Public/.pio_core` para evitar MAX_PATH limits |

---

(Esquemático, Pin Map y Parámetros del Motor sin cambios)

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
| SimpleFOC | `v2.3.3` (Git fetch) | ✅ Compila / Ruta forzada por bugs LDF Windows |
| STM32duino (platform) | latest | ✅ Compila |

---

## Sesiones de Desarrollo

### 2026-02-25 — Sesión inicial y Hardware Init
- ✅ Analizado esquemático KiCad completo y pines Mapeados.
- ✅ Escrito el código inicial en `src/main.cpp` (DRV8316 + MT6701 SPI).
- ✅ Solucionados bugs críticos de PlatformIO en Windows (Unicode linker crash y MAX_PATH limit) redirigiendo el `core_dir` a `C:/Users/Public`.
- ✅ Compilación de firmware exitosa (`firmware.elf` y `.bin`).
- ⏳ Pendiente probar comando de subida (pio run -t upload) con placa conectada.

---

## Notas y Decisiones Pendientes

- [ ] Confirmar el modelo exacto de motor BLDC que se usará con el gripper
- [ ] Confirmar si PB6/PB7/PA15 tienen función en el diseño final o son reservas
- [ ] Confirmar OS del desarrollador para descargar la versión correcta de `.vsix`
- [ ] Decidir si el ID RS-485 del dispositivo se configura por hardware (DIP switch?) o por comando software
- [ ] Decidir frecuencia de polling del DRV8316 STATUS (cada loop? cada 10 ms?)
