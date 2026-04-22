# Fase 5: Fault Manager

Este ejemplo valida el sistema de gestión de seguridad (**Fault Manager**) del SCLF Gripper. El sistema monitoriza constantemente el hardware para evitar daños en caso de bloqueo mecánico o fallo de sensores.

## Objetivos
- **Protección Activa**: Desactivar el motor (`motor.disable()`) inmediatamente al detectar un fallo.
- **Filtro de Ruido**: Implementar un contador de errores para el encoder MT6701 para evitar falsos positivos por ruido SPI.
- **Detección de Stall**: Identificar si el motor está bloqueado mecánicamente durante más de 1 segundo.
- **Interfaz de Rearme**: Implementar un flujo seguro de recuperación tras un fallo.

## Instrucciones de Testeo Detalladas

1. **Seleccionar Entorno**: En PlatformIO, selecciona el entorno `test_fault_manager` y cárgalo en la placa.
2. **Monitor Serie**: Abre el terminal (115200 bps).
3. **Puesta en Marcha**:
   - Envía `M2` para hacer girar el motor a 2 rad/s.
4. **Prueba de Bloqueo (Stall)**:
   - Sujeta el eje del motor con la mano firmemente para impedir su rotación.
   - **Resultado esperado**: Tras **1 segundo** de bloqueo, el motor se detendrá por completo. Verás el mensaje `[SAFETY] CRITICAL FAULT DETECTED: STALL`.
   - El LED de la placa parpadeará muy rápido (50ms) indicando el estado de error.
5. **Prueba de Rearme Seguro**:
   - Intenta encender el motor enviando `ME1`.
   - **Resultado esperado**: El sistema rechazará la orden y enviará el mensaje `[SAFETY] Motor disabled. Clear fault first!`.
   - Para recuperar el control, envía el comando `F` (Clear Faults).
   - Ahora ya puedes enviar `ME1` para habilitar el motor y volver a operar.

## Fallos Detectados
- **DRV_FAULT**: Errores internos del driver DRV8316 (sobrecorriente, temperatura).
- **ENCODER_ERROR**: Fallo de comunicación SPI con el MT6701 (requiere 10 fallos consecutivos).
- **STALL**: Velocidad < 0.5 rad/s con objetivo activo durante > 1000ms.
