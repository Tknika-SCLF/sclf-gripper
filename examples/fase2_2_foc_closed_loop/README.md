# Phase 2.2 — FOC Closed-Loop Test

Este test valida la rotación del motor en lazo cerrado (Closed-Loop Velocity) utilizando el encoder **MT6701** (vía bit-banging) y el driver **DRV8316**.

## Objetivo
- Realizar la alineación automática (`initFOC`) del encoder con las fases del motor.
- Validar el control de velocidad en lazo cerrado.
- Verificar la estabilidad de las ganancias PID iniciales (`P=0.5`, `I=5.0`).

## Cómo realizar el test

1. **Configuración de PlatformIO**:
   - Selecciona el entorno `test_foc_closed_loop`.
   - Compila y sube el firmware.

2. **Alineación (IMPORTANTE)**:
   - Al arrancar, el motor realizará un pequeño movimiento para alinear el encoder. **Asegúrate de que el motor pueda girar libremente** (sin carga bloqueante).
   - En el monitor serie verás los resultados de la alineación: `Zero electric angle` y `Sensor direction`.

3. **Monitor Serie**:
   - Deberías ver: `[MC] Starting in CLOSED-LOOP VELOCITY mode`.
   - El motor está ahora esperando un comando de velocidad.

4. **Comandos disponibles**:
   - `ME1`: Habilita el motor (mantiene la posición/velocidad).
   - `ME0`: Deshabilita (el motor queda suelto).
   - `M<val>`: Establece la velocidad objetivo en rad/s (ej. `M5` = 5 rad/s).
   - `D`: Vuelca los registros del DRV8316 y los parámetros FOC actuales.
   - `R`: Fuerza una recalibración del sensor.

---

### ⚠️ Cambios de velocidad: usar rampas progresivas

Al pasar de una velocidad alta a una baja de forma brusca (p.e. `M15` → `M0`), el motor vibra violentamente. Esto se debe a que la inercia del rotor supera la capacidad de frenado del PID con las ganancias actuales.

**Secuencia de prueba recomendada:**

```
ME1    → Habilitar
M2     → Velocidad baja de partida
M5     → Subir suavemente
M10    → Velocidad media
M15    → Velocidad alta (si se desea)
M10    → Bajar progresivamente
M5
M2
M0     → Parar
ME0    → Deshabilitar
```

> **Nota de futuro:** Para la integración en el sistema real habrá que implementar perfiles de velocidad trapezoidales (aceleración/deceleración controlada) que eviten este comportamiento. Ver Backlog en `TASKS.md`.

---

## Verificación

- El motor mantiene la velocidad de forma mucho más precisa que en lazo abierto.
- El motor se **calienta significativamente menos** que en lazo abierto gracias a la conmutación FOC.
- Intenta frenar ligeramente el eje con la mano (¡con cuidado!); deberías sentir cómo el motor aumenta el torque para compensar.

---

## Resultados del test (2026-04-21)

**Hardware:** SCLF Gripper v2.0 | Motor: GM3506 (11 pp) | Fuente: 24V

**Telemetría a 5 rad/s:**
```
Ang:0.086 rad | Vel:5.23 rad/s | Target:5.00
Ang:3.887 rad | Vel:4.43 rad/s | Target:5.00
Ang:1.387 rad | Vel:4.74 rad/s | Target:5.00
Ang:5.161 rad | Vel:5.07 rad/s | Target:5.00
Ang:2.650 rad | Vel:5.03 rad/s | Target:5.00
```
→ Error de velocidad ≈ ±0.5 rad/s (±10%) con ganancias PID por defecto. ✅

**Parámetros de calibración FOC obtenidos:**
```
Motor Status:      4  (motor_ready)
Sensor Direction: -1  (invertido — CW físico = CCW eléctrico)
Zero Elec Angle:   1.3426 rad
```

**Registros DRV8316:** Todos a `0x000` → sin fallos activos. ✅

---

## Diagnóstico

| Síntoma | Causa probable | Solución |
|---|---|---|
| Alineación fallida | Encoder desconectado o CS incorrecto | Revisar PA4/PA5/PA6 y el conector del encoder |
| Motor vibra al activar `ME1` | Dirección del sensor incorrecta o ángulo eléctrico inestable | Reiniciar o enviar `R` para recalibrar |
| Motor vibra al frenar | Cambio brusco de velocidad (ej. `M15`→`M0`) | Usar siempre rampas progresivas (ver sección anterior) |
| `Vel` siempre 0 en telemetría | `MT6701::update()` no llama a `Sensor::update()` | Bug resuelto el 2026-04-21; actualizar firmware |
