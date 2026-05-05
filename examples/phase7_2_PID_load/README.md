# 7.2 Fasea: PID Doikuntza Kargarekin

Adibide hau PIDs-en doikuntza finerako da, behin pintzak montatuta daudenean eta motorrak benetako karga mekanikoa duenean.

## Helburuak
- `KP`, `KI`, `KD` doitzea pintzen pisuarekin oszilazioak saihesteko.
- **Torque** kontrola balioztatzea `InlineCurrentSense` korronte sentsorearen bidez.
- Korronte muga errealistak definitzea objetuak hartzean ez kaltetzeko.

## Uneko egoera
- **Zai**: Lehenik 7.1 Fasea egin eta hardware mekanikoa montatu.

---

# Fase 7.2: PID Tuning con Carga [ES]

Este ejemplo está destinado al ajuste fino de los PIDs una vez las pinzas están montadas y el motor tiene carga mecánica real.

## Objetivos
- Ajustar `KP`, `KI`, `KD` para evitar oscilaciones con el peso de las pinzas.
- Validar el control de **Par (Torque)** mediante el sensor de corriente `InlineCurrentSense`.
- Definir límites de corriente realistas para evitar dañar objetos durante el agarre.

## Estado actual
- **En espera**: Realizar primero la Fase 7.1 y montar el hardware mecánico.

---

# Phase 7.2: PID Tuning with Load [EN]

This example is intended for fine-tuning the PIDs once the grippers are mounted and the motor has real mechanical load.

## Objectives
- Adjust `KP`, `KI`, `KD` to avoid oscillations with the weight of the grippers.
- Validate **Torque** control using the `InlineCurrentSense` current sensor.
- Define realistic current limits to avoid damaging objects during gripping.

## Current status
- **On hold**: Perform Phase 7.1 first and assemble the mechanical hardware.
