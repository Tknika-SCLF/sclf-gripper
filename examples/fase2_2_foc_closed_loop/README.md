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

4. **Comandos de Prueba**:
   - `M5`: Gira a 5 rad/s en lazo cerrado.
   - `M10`: Gira a 10 rad/s.
   - `M0`: Detiene el motor.
   - `ME0`: Deshabilita (el motor se queda suelto).
   - `ME1`: Habilita (el motor mantiene la posición/velocidad).

## Verificación
- El motor debe mantener la velocidad de forma mucho más precisa que en lazo abierto.
- Intenta frenar ligeramente el eje con la mano (¡con cuidado!); deberías sentir cómo el motor aumenta el torque para compensar y mantener la velocidad.

## Diagnóstico
- **Fallo en alineación**: Si el monitor dice que la alineación falló, revisa las conexiones del encoder (PA4, PA5, PA6).
- **Vibraciones**: Si el motor vibra violentamente al activar el lazo cerrado, puede que la dirección del sensor detectada sea incorrecta o el `zero_electric_angle` no sea estable. Intenta reiniciar.
