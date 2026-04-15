# RULES.md — SCLF Gripper Firmware
> Reglas del proyecto. Se aplican siempre: al escribir código, al revisar código, y al pedir ayuda a un agente de IA.

---

## R01 · La seguridad es prioritaria sobre la funcionalidad

El motor maneja 24V y corrientes de varios amperios. Un bug puede destruir el hardware o causar daño físico.

- **Nunca** deshabilites las protecciones de corriente del DRV8316 para "hacer pruebas".
- **Siempre** verifica que `motor.disable()` funciona correctamente antes de probar velocidades altas.
- **Ante cualquier duda**, añade un `motor.disable()` primero y comprueba después.
- Los límites de tensión y corriente en SimpleFOC deben ser conservadores al principio: empieza con `motor.voltage_limit = 2.0` y sube gradualmente.

---

## R02 · El loop FOC nunca se bloquea

La función `loop()` contiene el corazón del sistema. Un `delay()` o una operación lenta aquí destruye el control.

```cpp
// ✅ CORRECTO
void loop() {
    motor.loopFOC();   // siempre, sin condiciones
    motor.move(target);
    faultManager.update();
    comms.update();    // non-blocking, buffered
}

// ❌ INCORRECTO
void loop() {
    delay(10);           // PROHIBIDO
    Serial.println(x);  // PROHIBIDO en el loop crítico si bloquea
    while (!rs485.ready()) {}  // PROHIBIDO
    motor.loopFOC();
}
```

**Regla:** Si una operación puede tardar más de 50 µs, no va en `loop()`. Va en una tarea de baja prioridad o en un buffer.

---

## R03 · Un único fichero para los pines

Todos los números de pin viven en `src/config/pins.h`. En ningún otro lugar.

```cpp
// ✅ CORRECTO
digitalWrite(PIN_RS485_DIR, HIGH);

// ❌ INCORRECTO  
digitalWrite(PB9, HIGH);   // magic number
digitalWrite(9, HIGH);     // peor aún
```

Si necesitas un pin nuevo, primero lo añades a `pins.h` con nombre descriptivo, luego lo usas. **No se aceptan números de pin literales fuera de `pins.h`.**

---

## R04 · Los fallos del DRV8316 se detectan por SPI, no por GPIO

El pin `nFAULT` del DRV8316 **no está conectado** al STM32 (confirmado en esquemático). La única forma de detectar un fallo del driver es leer los registros STATUS1 y STATUS2 por SPI.

```cpp
// ✅ CORRECTO — polling en cada ciclo o cada N ciclos
if (drv.hasFault()) {
    faultManager.triggerSafeState(FaultCode::DRV_FAULT);
}

// ❌ INCORRECTO — no existe este pin en el STM32
attachInterrupt(PIN_DRV_NFAULT, onFault, FALLING);
```

El `FaultManager` debe llamar a `drv.hasFault()` en cada iteración del loop principal.

---

## R05 · El RS-485 es half-duplex — el pin DE/RE es responsabilidad del código

El MAX3485 tiene DE y RE conectados juntos al pin `PIN_RS485_DIR` (PB9). El cambio de dirección **no es automático** — el firmware debe controlarlo.

```cpp
// ✅ CORRECTO — secuencia obligatoria para transmitir
void RS485::send(const char* frame) {
    digitalWrite(PIN_RS485_DIR, HIGH);  // 1. activar TX
    delayMicroseconds(2);               // 2. esperar propagación
    HAL_UART_Transmit(&huart3, ...);    // 3. transmitir
    // esperar a que el último bit salga físicamente
    while (__HAL_UART_GET_FLAG(&huart3, UART_FLAG_TC) == RESET) {}
    digitalWrite(PIN_RS485_DIR, LOW);   // 4. volver a RX
}
```

No usar `Serial.print()` para RS-485. Usar UART3 directamente con control manual de DE/RE.

---

## R06 · Las unidades físicas son siempre SI

| Magnitud | Unidad | Variable en código |
|---|---|---|
| Ángulo | Radianes | `float angle_rad` |
| Velocidad | rad/s | `float velocity_rads` |
| Corriente | Amperios | `float current_a` |
| Tensión | Voltios | `float voltage_v` |
| Tiempo | Segundos (o µs en `uint32_t`) | `float dt_s` / `uint32_t t_us` |

**Nunca** mezclar grados y radianes. **Nunca** almacenar corriente en mA sin indicarlo explícitamente en el nombre (`current_ma`).

---

## R07 · Estructura modular — cada módulo posee su periférico

| Módulo | Periférico que posee | No puede tocar |
|---|---|---|
| `MT6701` | SPI (PA4/PA5/PA6 + CS=PA4) | — |
| `DRV8316` | SPI (PB3/PB4/PB5 + CS=PC4) | — |
| `RS485` | UART3 (PC10/PC11 + DE=PB9) | VCP |
| `VCP` | USB Serial (`Serial`) | UART3 |
| `FaultManager` | — (consume otros módulos) | Periféricos directamente |
| `MotorController` | HRTIM/TIM, ADC | SPI, UART |

> ⚠️ SPI1 es compartido entre DRV8316 y MT6701. El bus es el mismo (PB3/PB4/PB5), solo el CS cambia (PC4 para DRV, PA4 para encoder). Usar `SPIClass` con CS manual y asegurar que nunca hay dos CS activos a la vez.

---

## R08 · Ningún `TODO` sin issue registrado

Si dejas un `TODO` en el código, **debes** añadir la tarea correspondiente en `TASKS.md` el mismo día.

```cpp
// ✅ CORRECTO
// TODO: calibrar ganancia del CSA del DRV8316 (ver TASKS.md Fase 1.3)
float gain = 0.15f;

// ❌ INCORRECTO  
// TODO: arreglar esto
float gain = 0.15f;
```

---

## R09 · Git — commits atómicos y mensajes descriptivos

Formato de commit: `<tipo>(<módulo>): <descripción corta>`

```
feat(encoder): implementar MT6701 SPI driver con wrap-around
fix(rs485): corregir timing de DE/RE en transmisión
refactor(motor): extraer PID gains a FlashConfig
test(encoder): añadir test de wrap 16383→0
docs(tasks): marcar Fase 1.1 como completada
```

Tipos: `feat` · `fix` · `refactor` · `test` · `docs` · `chore`

**Una tarea de `TASKS.md` = un commit** (como mínimo).

---

## R10 · Antes de hacer push — checklist mínimo

```
[ ] pio run          → compila sin warnings nuevos
[ ] pio test         → todos los tests pasan
[ ] El LED parpadea  → la placa arranca correctamente
[ ] VCP responde     → Serial.println("SCLF OK") aparece en monitor
[ ] TASKS.md actualizado con el estado real de las tareas
[ ] MEMORY.md actualizado si cambió algún parámetro crítico
```

---

## R11 · Reglas para el agente de IA

Cuando uses un agente de IA (Cursor, Copilot, Windsurf, etc.) para generar código:

1. **Siempre incluye `AGENT.md` como contexto** antes de pedir código.
2. **Nunca aceptes código que tenga números de pin literales** — pide que use `pins.h`.
3. **Nunca aceptes `delay()` dentro de `loop()`** — pide alternativa non-blocking.
4. **Siempre pide que el agente justifique** qué periférico STM32 usa (TIM1? SPI1? UART3?) antes de escribir el código.
5. Si el agente sugiere conectar una interrupción a `nFAULT`, **es incorrecto** — recuérdalo con R04.
6. **Actualiza `MEMORY.md`** después de cada sesión productiva con el agente.

---

## R12 · Parámetros de seguridad por defecto (no modificar sin justificación)

```cpp
motor.voltage_limit    = 2.0f;   // V — aumentar solo tras verificar térmica
motor.current_limit    = 1.0f;   // A — aumentar con medición real
motor.velocity_limit   = 10.0f;  // rad/s — aumentar con pruebas
```

Estos valores son conservadores a propósito. Documentar en `MEMORY.md` cada vez que se cambien y por qué.
