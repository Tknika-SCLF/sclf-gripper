# Fase 7.1: Alineación Motor-Encoder

Este ejemplo realiza la alineación eléctrica inicial entre el motor y el encoder magnético MT6701 utilizando SimpleFOC.

## Objetivos
- Ejecutar `motor.initFOC()` para encontrar el `zero_electric_angle` y el `sensor_direction`.
- Validar el control de **Posición** y **Velocidad** en lazo cerrado sin carga (Bench Test).
- Persistir los parámetros de calibración en la Flash para evitar recalibrar en cada inicio.

## Instrucciones
1. Seleccionar el entorno `test_foc_advanced` en PlatformIO.
2. Al arrancar, si hay datos en Flash, no se calibrará. Envíe el comando `C` para forzar la alineación. El motor realizará un pequeño movimiento de vaivén. **Asegúrese de que el motor pueda girar libremente.**
3. Una vez alineado, use el Commander para probar el movimiento:
   - `M 3.14`: Mover a 180 grados.
   - `M 0`: Mover a 0 grados.
   - `M C 1`: Cambiar a modo velocidad (Velocity Open/Closed).
   - `M 5`: Girar a 5 rad/s (si está en modo velocidad).
4. Si el movimiento es correcto, envíe `S` para guardar los parámetros de calibración en la Flash. Si algo va mal, `R` limpia la flash.

## Verificación
- El motor debe mantener su posición con firmeza al intentar moverlo con la mano (modo posición).
- No debe haber ruidos excesivos ni vibraciones en reposo.
