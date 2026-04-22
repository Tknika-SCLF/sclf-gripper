# Fase 5: Fault Manager

Este ejemplo valida el sistema de gestión de errores (**Fault Manager**) del SCLF Gripper. El objetivo es garantizar la integridad del hardware detectando condiciones anómalas y forzando un estado seguro.

## Objetivos
- Implementar el componente `FaultManager` para monitorizar el estado del driver DRV8316 y otros sensores.
- Detectar fallos críticos:
    - **DRV_FAULT**: Errores reportados por el driver (sobrecorriente, sobretemperatura, etc.).
    - **STALL**: Bloqueo del motor (corriente alta con velocidad nula).
    - **ENCODER_ERROR**: Pérdida de comunicación con el sensor MT6701.
- Accionar el **Safe State**: Desactivar el motor (`motor.disable()`) en menos de 1ms tras la detección.
- Reportar el error por USB VCP y RS-485.

## Conexiones
- Las mismas que en fases anteriores. El test de fallos puede requerir provocar situaciones de bloqueo (con cuidado) o desconectar cables del motor para verificar la detección.

## Uso
1. Seleccionar el entorno `test_fault_manager` en PlatformIO.
2. Compilar y cargar.
3. Observar el monitor serie.
4. **Rearme tras fallo**:
   - Si ocurre un fallo (ej. STALL), el motor se bloquea por seguridad.
   - Envía `F` para limpiar el estado de fallo en el gestor.
   - Envía `ME1` para volver a habilitar el motor.
