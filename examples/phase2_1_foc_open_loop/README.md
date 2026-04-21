# Phase 2.1 — FOC Open-Loop Test

Este test valida la rotación del motor en lazo abierto (Open-Loop) utilizando la librería **SimpleFOC** y la clase wrapper `MotorController`.

## Objetivo
- Verificar la rotación suave del motor sin necesidad de alineación del encoder.
- Validar que la inversión de fases (C-B-A) en el hardware coincide con la configuración del software.
- Comprobar que el driver DRV8316 y la etapa de potencia funcionan bajo carga (8V límite).

## Requisitos de Hardware
- Fuente de alimentación a **24V**.
- Conexión ST-Link para el flasheo.
- Cable USB-C conectado al puerto del gripper para telemetría.

## Cómo realizar el test

1. **Configuración de PlatformIO**:
   - Selecciona el entorno `test_foc_open_loop`.
   - Compila y sube el firmware.

2. **Monitor Serie**:
   - Abre el monitor serie a **115200 baudios**.
   - Deberías ver el mensaje: `[MC] System Initialized OK`.
   - El sistema arranca por defecto en `MotionControlType::velocity_openloop`.

3. **Comandos de Prueba**:
   - `M10`: Hace girar el motor a 10 rad/s en sentido horario.
   - `M-10`: Hace girar el motor a -10 rad/s (sentido antihorario).
   - `M0`: Detiene el motor.
   - `ME0`: Deshabilita el motor (torque cero).
   - `ME1`: Habilita el motor.

4. **Verificación**:
   - Observa si el motor gira de forma suave y silenciosa.
   - Verifica los valores de `Ang` y `Vel` en el reporte periódico. Aunque el control sea en lazo abierto, el encoder bit-bang MT6701 debería estar reportando el ángulo real de giro.

## Diagnóstico
- Si el motor hace ruido o vibra pero no gira: Verifica que la fuente de alimentación tenga corriente suficiente (mínimo 1.5A) y que los pines `PIN_AH/AL...` no tengan cortocircuitos.
- Si el monitor no muestra datos: Revisa la configuración del reloj `HSI48` en `main.cpp` (vía `MotorController`) para el USB CDC.
