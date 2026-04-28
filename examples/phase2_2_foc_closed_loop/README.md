# 2.2 Fasea — FOC Closed-Loop Testa

Test honek motorraren biraketa lazo itxian (Closed-Loop Velocity) balioztatzen du, **MT6701** encoder-a (bit-banging bidez) eta **DRV8316** driver-a erabiliz.

## Helburua
- Encoder-aren eta motorraren faseen arteko lerrokatze automatikoa (`initFOC`) egitea.
- Abiadura kontrola lazo itxian balioztatzea.
- Hasierako PID irabazien egonkortasuna egiaztatzea (`P=0.5`, `I=5.0`).

## Nola egin testa

1. **Konpilatu eta kargatu**:
   Exekutatu komando hau proiektuaren erroan ingurune espezifiko hau igotzeko:
   ```bash
   pio run -e test_foc_closed_loop --target upload
   ```

2. **Lerrokatzea (GARRANTZITSUA)**:
   - Abiaraztean, motorrak mugimendu txiki bat egingo du encoder-a lerrokatzeko. **Ziurtatu motorra libreki biratu daitekeela** (karga blokeatzailerik gabe).
   - Serie-monitorean lerrokatzearen emaitzak ikusiko dituzu: `Zero electric angle` eta `Sensor direction`.

3. **Serie-Monitorea**:
   - `[MC] Starting in CLOSED-LOOP VELOCITY mode` ikusi beharko zenuke.
   - Motorra orain abiadura komando baten zain dago.

4. **Komando erabilgarriak**:
   - `ME1`: Motorra gaitzen du (posizioa/abiadura mantentzen du).
   - `ME0`: Ezgaitzen du (motorra libre geratzen da).
   - `M<balioa>`: Helburu-abiadura ezartzen du rad/s-tan (adibidez, `M5` = 5 rad/s).
   - `D`: DRV8316-ren erregistroak eta uneko FOC parametroak erakusten ditu.
   - `R`: Sentsorearen birkalibrazioa behartzen du.

---

### ⚠️ Abiadura aldaketak: erabili arrapala progresiboak

Abiadura altu batetik baxu batera bat-batean pasatzean (adibidez, `M15` → `M0`), motorrak dardara bortitza egiten du. Hau gertatzen da errotorearen inertziak uneko irabaziekin PIDak duen baliratzeko gaitasuna gainditzen duelako.

**Gomendatutako proba sekuentzia:**

```
ME1    → Gaitzea
M2     → Irteerako abiadura baxua
M5     → Suabe igo
M10    → Abiadura ertaina
M15    → Abiadura altua (nahi izanez gero)
M10    → Progresiboki jaitsi
M5
M2
M0     → Gelditu
ME0    → Ezgaitu
```

> **Etorkizuneko oharra:** Benetako sisteman integratzeko, abiadura-profil trapezoidalak inplementatu beharko dira (azelerazio/dezelerazio kontrolatua), portaera hau saihesteko. Ikusi Backlog-a `TASKS.md` fitxategian.

---

## Egiaztapena

- Motorrak abiadura lazo irekian baino askoz modu zehatzagoan mantentzen du.
- Motorra **nabarmen gutxiago berotzen da** lazo irekian baino, FOC kommutazioari esker.
- Saiatu ardatza eskuz pixka bat frenatzen (kontu handiz!); motorrak torquea nola handitzen duen sentitu beharko zenuke konpentsatzeko.

---

## Testaren emaitzak (2026-04-21)

**Hardwarea:** SCLF Gripper v2.0 | Motorra: GM3506 (11 pp) | Iturria: 24V

**Telemetria 5 rad/s-ra:**
```
Ang:0.086 rad | Vel:5.23 rad/s | Target:5.00
Ang:3.887 rad | Vel:4.43 rad/s | Target:5.00
Ang:1.387 rad | Vel:4.74 rad/s | Target:5.00
Ang:5.161 rad | Vel:5.07 rad/s | Target:5.00
Ang:2.650 rad | Vel:5.03 rad/s | Target:5.00
```
→ Abiadura errorea ≈ ±0.5 rad/s (±10%) lehenetsitako PID irabaziekin. ✅

**Lortutako FOC kalibrazio parametroak:**
```
Motor Status:      4  (motor_ready)
Sensor Direction: -1  (alderantzikatua — CW fisikoa = CCW elektrikoa)
Zero Elec Angle:   1.3426 rad
```

**DRV8316 erregistroak:** Denak `0x000` → ez dago akats aktiborik. ✅

---

## Diagnostikoa

| Sintoma | Arrazoi posiblea | Konponbidea |
|---|---|---|
| Lerrokatzeak huts egiten du | Encoderra deskonektatuta edo CS okerra | Berrikusi PA4/PA5/PA6 eta encoderraren konektorea |
| Motorrak bibratzen du `ME1` aktibatzean | Sentsorearen norabidea okerra edo angelu elektriko ezegonkorra | Berrabiarazi edo bidali `R` birkalibratzeko |
| Motorrak bibratzen du frenatzean | Abiadura aldaketa zakarra (adibidez, `M15`→`M0`) | Erabili beti arrapala progresiboak (ikus aurreko atala) |

---

# Phase 2.2 — FOC Closed-Loop Test [ES]

Este test valida la rotación del motor en lazo cerrado (Closed-Loop Velocity) utilizando el encoder **MT6701** (vía bit-banging) y el driver **DRV8316**.

## Objetivo
- Realizar la alineación automática (`initFOC`) del encoder con las fases del motor.
- Validar el control de velocidad en lazo cerrado.
- Verificar la estabilidad de las ganancias PID iniciales (`P=0.5`, `I=5.0`).

## Cómo realizar el test

1. **Compilar y flashear**:
   Ejecuta el siguiente comando en la raíz del proyecto para subir este entorno específico:
   ```bash
   pio run -e test_foc_closed_loop --target upload
   ```

2. **Alineación (IMPORTANTE)**:
   - Al arrancar, el motor realizará un pequeño movimiento para alinear el encoder. **Asegúrate de que el motor pueda girar libremente** (sin carga bloqueante).
   - En el monitor serie verás los resultados de la alineación: `Zero electric angle` y `Sensor direction`.

3. **Monitor Serie**:
   - Deberías ver: `[MC] Starting in CLOSED-LOOP VELOCITY mode`.
   - El motor está ahora esperando un comando de velocidad.

4. **Comandos disponibles**:
   - `ME1`: Habilita el motor (mantiene la posición/velocidad).
   - `ME0`: Deshabilita (el motor queda suelto).
   - `M<val>`: Establece la velocidad objetivo en rad/s (ej. `M5` = 5 rad/s).
   - `D`: Vuelca los registros del DRV8316 y los parámetros FOC actuales.
   - `R`: Fuerza una recalibración del sensor.

---

### ⚠️ Cambios de velocidad: usar rampas progresivas

Al pasar de una velocidad alta a una baja de forma brusca (p.e. `M15` → `M0`), el motor vibra violentamente. Esto se debe a que la inercia del rotor supera la capacidad de frenado del PID con las ganancias actuales.

**Secuencia de prueba recomendada:**

```
ME1    → Habilitar
M2     → Velocidad baja de partida
M5     → Subir suavemente
M10    → Velocidad media
M15    → Velocidad alta (si se desea)
M10    → Bajar progresivamente
M5
M2
M0     → Parar
ME0    → Deshabilitar
```

> **Nota de futuro:** Para la integración en el sistema real habrá que implementar perfiles de velocidad trapezoidales (aceleración/deceleración controlada) que eviten este comportamiento. Ver Backlog en `TASKS.md`.

---

## Verificación

- El motor mantiene la velocidad de forma mucho más precisa que en lazo abierto.
- El motor se **calienta significativamente menos** que en lazo abierto gracias a la conmutación FOC.
- Intenta frenar ligeramente el eje con la mano (¡con cuidado!); deberías sentir cómo el motor aumenta el torque para compensar.

---

## Resultados del test (2026-04-21)

**Hardware:** SCLF Gripper v2.0 | Motor: GM3506 (11 pp) | Fuente: 24V

**Telemetría a 5 rad/s:**
```
Ang:0.086 rad | Vel:5.23 rad/s | Target:5.00
Ang:3.887 rad | Vel:4.43 rad/s | Target:5.00
Ang:1.387 rad | Vel:4.74 rad/s | Target:5.00
Ang:5.161 rad | Vel:5.07 rad/s | Target:5.00
Ang:2.650 rad | Vel:5.03 rad/s | Target:5.00
```
→ Error de velocidad ≈ ±0.5 rad/s (±10%) con ganancias PID por defecto. ✅

**Parámetros de calibración FOC obtenidos:**
```
Motor Status:      4  (motor_ready)
Sensor Direction: -1  (invertido — CW físico = CCW eléctrico)
Zero Elec Angle:   1.3426 rad
```

**Registros DRV8316:** Todos a `0x000` → sin fallos activos. ✅

---

## Diagnóstico

| Síntoma | Causa probable | Solución |
|---|---|---|
| Alineación fallida | Encoder desconectado o CS incorrecto | Revisar PA4/PA5/PA6 y el conector del encoder |
| Motor vibra al activar `ME1` | Dirección del sensor incorrecta o ángulo eléctrico inestable | Reiniciar o enviar `R` para recalibrar |
| Motor vibra al frenar | Cambio brusco de velocidad (ej. `M15`→`M0`) | Usar siempre rampas progresivas (ver sección anterior) |

---

# Phase 2.2 — FOC Closed-Loop Test [EN]

This test validates the motor rotation in closed-loop (Closed-Loop Velocity) using the **MT6701** encoder (via bit-banging) and the **DRV8316** driver.

## Objective
- Perform automatic alignment (`initFOC`) of the encoder with the motor phases.
- Validate closed-loop velocity control.
- Verify the stability of the initial PID gains (`P=0.5`, `I=5.0`).

## How to Perform the Test

1. **Compile and upload**:
   Run the following command in the project root to upload this specific environment:
   ```bash
   pio run -e test_foc_closed_loop --target upload
   ```

2. **Alignment (IMPORTANT)**:
   - Upon startup, the motor will perform a small movement to align the encoder. **Ensure the motor can rotate freely** (without blocking load).
   - In the serial monitor, you will see the alignment results: `Zero electric angle` and `Sensor direction`.

3. **Serial Monitor**:
   - You should see: `[MC] Starting in CLOSED-LOOP VELOCITY mode`.
   - The motor is now waiting for a velocity command.

4. **Available Commands**:
   - `ME1`: Enables the motor (maintains position/velocity).
   - `ME0`: Disables the motor (the motor becomes loose).
   - `M<val>`: Sets the target velocity in rad/s (e.g., `M5` = 5 rad/s).
   - `D`: Dumps DRV8316 registers and current FOC parameters.
   - `R`: Forces a sensor recalibration.

---

### ⚠️ Velocity changes: use progressive ramps

When switching from high speed to low speed abruptly (e.g., `M15` → `M0`), the motor vibrates violently. This is because the rotor's inertia exceeds the PID's braking capacity with the current gains.

**Recommended test sequence:**

```
ME1    → Enable
M2     → Low starting speed
M5     → Increase smoothly
M10    → Medium speed
M15    → High speed (if desired)
M10    → Decrease progressively
M5
M2
M0     → Stop
ME0    → Disable
```

> **Future Note:** For integration into the real system, trapezoidal velocity profiles (controlled acceleration/deceleration) must be implemented to avoid this behavior. See Backlog in `TASKS.md`.

---

## Verification

- The motor maintains speed much more precisely than in open-loop.
- The motor **heats up significantly less** than in open-loop thanks to FOC switching.
- Try to lightly brake the shaft with your hand (carefully!); you should feel the motor increase torque to compensate.

---

## Test Results (2026-04-21)

**Hardware:** SCLF Gripper v2.0 | Motor: GM3506 (11 pp) | Power: 24V

**Telemetry at 5 rad/s:**
```
Ang:0.086 rad | Vel:5.23 rad/s | Target:5.00
Ang:3.887 rad | Vel:4.43 rad/s | Target:5.00
Ang:1.387 rad | Vel:4.74 rad/s | Target:5.00
Ang:5.161 rad | Vel:5.07 rad/s | Target:5.00
Ang:2.650 rad | Vel:5.03 rad/s | Target:5.00
```
→ Velocity error ≈ ±0.5 rad/s (±10%) with default PID gains. ✅

**FOC calibration parameters obtained:**
```
Motor Status:      4  (motor_ready)
Sensor Direction: -1  (inverted — physical CW = electrical CCW)
Zero Elec Angle:   1.3426 rad
```

**DRV8316 Registers:** All at `0x000` → no active faults. ✅

---

## Diagnosis

| Symptom | Probable cause | Solution |
|---|---|---|
| Alignment failed | Encoder disconnected or incorrect CS | Check PA4/PA5/PA6 and the encoder connector |
| Motor vibrates when activating `ME1` | Incorrect sensor direction or unstable electrical angle | Restart or send `R` to recalibrate |
| Motor vibrates when braking | Abrupt speed change (e.g., `M15`→`M0`) | Always use progressive ramps (see previous section) |
