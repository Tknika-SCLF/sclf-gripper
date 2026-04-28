# 0. Fasea: LED Heartbeat (Pre-testa)

Adibide honek STM32-a abiarazten dela eta kodea exekutatzen duela egiaztatzen du. Ez du 24V-eko elikadurarik edo kable gehigarririk behar.

## Beharrezkoak
- ST-Link SWD + Elikadura (ST-Link 3.3V edo USB-C).

## Diagnostikoa
- LEDak 5Hz-ra dir-dir egiten badu (200ms) → STM32 ondo funtzionatzen ari da ✅
- LEDak ez badu dir-dir egiten → Elikadura edo flash arazoa ❌

## Nola Probatu
1. Konpilatu eta kargatu firmwarea ingurune espezifikoa erabiliz:
   ```bash
   pio run -e test_led_heartbeat --target upload
   ```
2. Behatu plakako LEDa (PC6).
3. Serie-monitorean "STM32 Heartbeat OK" mezua ikusi beharko zenuke.

---

# Fase 0: LED Heartbeat (Pre-test) [ES]

Este ejemplo verifica que el STM32 arranca y ejecuta código correctamente. No requiere alimentación de 24V ni cables adicionales.

## Requisitos
- ST-Link SWD + Alimentación (ST-Link 3.3V o USB-C).

## Diagnóstico
- Si el LED parpadea a 5Hz (200ms) → El STM32 está funcionando correctamente ✅
- Si el LED no parpadea → Problema de alimentación o de flasheo ❌

## Cómo Probarlo
1. Compila y carga el firmware usando el entorno específico:
   ```bash
   pio run -e test_led_heartbeat --target upload
   ```
2. Observa el LED de la placa (PC6).
3. En el monitor serie deberías ver el mensaje "STM32 Heartbeat OK".

---

# Phase 0: LED Heartbeat (Pre-test) [EN]

This example verifies that the STM32 starts and executes code correctly. It does not require 24V power or additional cables.

## Requirements
- ST-Link SWD + Power (ST-Link 3.3V or USB-C).

## Diagnosis
- If the LED flashes at 5Hz (200ms) → STM32 is working correctly ✅
- If the LED does not flash → Power or flashing problem ❌

## How to Test
1. Compile and upload the firmware using the specific environment:
   ```bash
   pio run -e test_led_heartbeat --target upload
   ```
2. Observe the board's LED (PC6).
3. In the serial monitor, you should see the message "STM32 Heartbeat OK".
