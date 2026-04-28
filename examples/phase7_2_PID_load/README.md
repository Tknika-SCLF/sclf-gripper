# Fase 7.2: PID Tuning con Carga

Este ejemplo está destinado al ajuste fino de los PIDs una vez las pinzas están montadas y el motor tiene carga mecánica real.

## Objetivos
- Ajustar `KP`, `KI`, `KD` para evitar oscilaciones con el peso de las pinzas.
- Validar el control de **Par (Torque)** mediante el sensor de corriente `InlineCurrentSense`.
- Definir límites de corriente realistas para evitar dañar objetos durante el agarre.

## Estado actual
- **En espera**: Realizar primero la Fase 7.1 y montar el hardware mecánico.
