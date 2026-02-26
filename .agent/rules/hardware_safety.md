---
trigger: always_on
---

---
description: Reglas de seguridad críticas para el desarrollo de firmware en SCLF Gripper (STM32 + DRV8316). Activar siempre.
globs: "**/*.{cpp,h,ini}"
always_on: true
---

# Reglas de Seguridad de Hardware

1. **Prohibición de Bloqueo:** Jamás uses `HAL_Delay`, `delay()` o `while()` en el bucle `loopFOC()`.
2. **Validación de Pines:** Antes de cualquier implementación de código, verifica la asignación de pines contra el archivo `hardware/*.kicad_sch`.
3. **Manejo de Timers:** Prioriza TIM1, TIM8 o TIM20 para PWM. Verifica en el datasheet del STM32G474CEU6 que no existan conflictos de DMA.