---
name: foc-hardware-setup
description: Analiza esquemáticos de KiCad para extraer el pinout de un microcontrolador STM32 y un driver de potencia para Simple FOC. Úsalo antes de escribir código de hardware.
---

# Configuración de Hardware FOC

Cuando se te pida configurar el hardware o mapear los pines, sigue estrictamente estos pasos:

## Paso 1: Lectura de Planos
- Lee el archivo `.kicad_sch` en la carpeta `/hardware/`.
- Rastrea las conexiones (nets) desde el MCU hasta el driver de potencia (ej. DRV8316) y el sensor de posición (ej. MT6701).

## Paso 2: Creación del Artefacto
- Genera una tabla en Markdown detallando: Pin del MCU, Función (PWM A/B/C, SPI, I2C, Current Sense) y Periférico/Timer interno del STM32.
- **Advertencia:** Asegúrate de asignar Timers avanzados a los pines PWM.

## Paso 3: Aprobación
- Detente y pide al usuario que revise y apruebe la tabla de pines generada.
- NO escribas código en `src/main.cpp` hasta recibir la confirmación explícita del usuario.