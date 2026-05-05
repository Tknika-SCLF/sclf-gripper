# 7.1 Fasea: Motor-Encoder Lerrokatzea

Adibide honek motorraren (GM3506, 11 pare polo) eta MT6701 encoder magnetikoaren arteko lerrokatze elektrikoa egiten du SimpleFOC liburutegia erabiliz, eta lazo itxiko kontrola balioztatzen du (Torque, Abiadura eta Posizio moduak).

## Helburuak
- `motor.initFOC()` exekutatzea `zero_electric_angle` eta `sensor_direction` fisikoki determinatzeko.
- **Torque**, **Abiadura** eta **Posizio** kontrola lazo itxian balioztatzea.
- Kalibrazio parametroak Flash-ean iraunaraztea.

## Hardware Ezaugarriak
- **Motorra**: GM3506 gimbal motorra (11 pare polo, 24N22P)
- **Enkoderra**: MT6701 (bit-bang SPI, PA5/PA6/PA4 pinen bidez)
- **Dribera**: DRV8316 (6-PWM, SPI dedicatua PB3/PB4/PB5)
- **MCU**: STM32G474CEU6

## Argibideak: Lehen Erabileran

### 1. Kargatu eta Abiatu
```bash
pio run -e test_foc_advanced --target upload
```

### 2. STALL desaktibatu tuning-erako (lehenengo aldiz)
```
X
```
Irteera: `[DIAG] STALL detector: DISABLED (tuning mode)`

### 3. Kalibratu fisikoki (Flash hutsik badago)
Arrazoiarazten bada `[FLASH] Flash data has UNKNOWN direction`, motorrak bere kabuz lerrokatuko da (`initFOC()`).
Irteerak honelako itxura izango du:
```
[MAIN] FOC Initialized successfully.
[CAL] zero_electric_angle = 1.1098
[CAL] sensor_direction    = -1
```
> ⚠️ El motor se bloqueará brevemente durante la alineación. **Asegúrese de que puede girar libremente.**

### 4. Torque modua probatu (lazo itxiko egiaztapena)
```
M C 0      ← Torque modua
M 2        ← 2V zuzenean q-ardatzean
T1         ← Telemetria aktibatu
```
Emaitza zuzena: `V: ~35 rad/s` eta `A:` hazten (motorra biratzen).

### 5. Abiadura modua probatu
```
M C 1      ← Abiadura modua
M V P 2    ← P handiagoa (P=0.3 ez da nahikoa hasierako frikaziorako)
M V I 0    ← I=0 hasierako tuning-erako (windupa saihesteko)
M 1        ← 1 rad/s helburua
T1
```
Emaitza zuzena: `V: ~1.00` egonkortu.

### 6. Posizio modua probatu
```
M C 2           ← Posizio modua (angle mode)
M 800           ← Uneko shaft_angle-tik gertu dagoen balioa
M 820           ← Beste posizio bat
```
> **Oharra**: `shaft_angle` metatzen da rotazio guztietan. `M 3.14` erabili beharrean, uneko `A:` baliora gertu dauden helburuak erabili.

### 7. Kalibrazioa gorde
```
S
```
`[FLASH] Calibration saved! Press RESET to reload cleanly.` mezua ikusi ondoren, **RESET fisikoa sakatu**.

### 8. Flash-etik kargatu egiaztatu
Berrabiaraztean, ez da kalibrazio fisikorik egingo:
```
[FLASH] Valid calibration found. Applying...
[MAIN] FOC Initialized successfully.
[CAL] zero_electric_angle = 1.1098
[CAL] sensor_direction    = -1
```

## Komandoak

| Komandoa | Deskribapena |
|---|---|
| `M C 0` | Torque modua |
| `M C 1` | Abiadura lazo itxia |
| `M C 2` | Posizio modua |
| `M <balio>` | Helburua ezarri (modua arabera: torque V, abiadura rad/s, posizio rad) |
| `M V P <balio>` | PID abiadura P ezarri |
| `M V I <balio>` | PID abiadura I ezarri |
| `O <balio>` | Lazo irekiko abiadura (testetzeko) |
| `T1` / `T0` | Telemetria piztu/itzali |
| `X` | STALL detektagailua aktibatu/desaktibatu |
| `S` | Kalibrazioa Flash-ean gorde |
| `R` | Flash berrezarri (hurrengo abiaraztean kalibratu) |
| `C` | Kalibrazio fisikoa behartu |
| `D` | DRV8316 erregistroen diagnostikoa |
| `F` | STALL faila garbitu eta motorra berraktibatu |
| `A <balio>` | `zero_electric_angle` eskuz ezarri |
| `Z` | ZEA diagnostikoa (lazo irekian) |

## Egiaztapena
- ✅ `initFOC()` arrakastaz exekutatu (`zero_electric_angle` eta `sensor_direction` lortuz)
- ✅ Torque moduan `V: ~35 rad/s` lortu `Uq=2V`-rekin
- ✅ Abiadura moduan `V: ~1.00 rad/s` egonkortu `P=2, I=0`-rekin
- ✅ Posizio moduan helburura mugitu eta mantendu
- ✅ Flash-etik kalibrazioa kargatu eta kalibrazio fisikorik gabe funtzionatu

## Arazoen Konponbidea (Troubleshooting)
- **Motorrak dar-dar egiten du baina ez du biratzen Torque/Abiadura moduan**:
  - *Zergatia*: `zero_electric_angle` okerra, kalibrazioan errotorea aske ez zegoelako.
  - *Konponbidea*: `R` komandoa bidali Flash-a garbitzeko, RESET sakatu eta utzi motorrari oztoporik gabe kalibratzen.
- **Motorrak txistu egiten du eta ez da mugitzen Abiadura moduan**:
  - *Zergatia*: PID-aren `P` irabazia baxuegia da marruskadura gainditzeko.
  - *Konponbidea*: Igo `P` pixkanaka (`M V P 2`, `M V P 3`) mugimendu leuna lortu arte.
- **Bibrazio bortitza Abiadura moduan PID-a doitzean**:
  - *Zergatia*: Termino integrala (I) errorea metatzen ari da (windup).
  - *Konponbidea*: Ezarri integrala zerora (`M V I 0`) P egonkorra aurkitu arte.
- **Posizio modura aldatzean motorrak bira zoroak egiten ditu**:
  - *Zergatia*: `shaft_angle`-k aurreko probetako (adibidez, torque modua) bira guztiak metatzen ditu.
  - *Konponbidea*: Irakurri uneko angelua telemetriarekin (`T1`) eta bidali hortik gertu dagoen helburu bat, edo berrabiarazi plaka.
- **STALL errorea agertzen da eta motorra gelditzen da**:
  - *Zergatia*: STALL detektagailuak motorra babesten du blokeatuta badago eta tentsio altua jasotzen badu. Tuning garaian, irabazi baxuekin gerta daiteke.
  - *Konponbidea*: Desaktibatu STALL behin-behinean `X` komandoa erabiliz.

## Konpondu diren Bug Kritikoak

### 1. TIM1_BKIN Gateazka (PA6)
`driver.init()` exekutatzean, TIM1 periferikoak PA6 pina `BKIN` funtzio alternatiboetara konfiguratzen zuen, MT6701 enkoderra hautsiz.
**Konponketa**: `driver.init()` ostean `pinMode()` erabiliz PA6 GPIO modura berriz ezarri.

### 2. SPI Bektorioa Gatazka
`SPI` objektu globalak PA5/PA6 erabil ditzake, enkoderre-ak erabiltzen dituen pinekin talka eginez.
**Konponketa**: DRV8316-rako `SPIClass` instantzia dedikatua (`_spi_drv`) sortu PB3/PB4/PB5 pinekin.

### 3. Flash Kalibrazio Balioztatzea
`resetToDefaults()` funtzioak `sensor_direction=CW` ezartzen zuen, kalibrazio faltsu bat sortuz.
**Konponketa**: Default-ak `sensor_direction=UNKNOWN (0)` ezartzeko aldatu, berriro kalibratzera behartuz.

### 4. Flash Idazketa Blokeoa
`HAL_FLASH_Program()` ~30ms blokeatzen du, FOC begizta desinkronizatuz.
**Konponketa**: `S` komandoa exekutatu aurretik motorra desaktibatu eta ostean berriro aktibatu.

### 5. Zero Elekrikoa Okerra (zero_electric_angle)
Kalibrazio saio batean `ZEA=4.9195, dir=1` lortu zen, baina closed-loop torquea ez zen sortzen. Arrazoiak: motor ez-askea lerrokatzean, edo alineazio tentsio baxuegia (`voltage_sensor_align`).
**Konponketa**: Flash reseteatu eta fisiko kalibrazio berria egin (`voltage_sensor_align = 10V`). Emaitza zuzena: `ZEA=1.1098, dir=-1`.

### 6. STALL Detekzio Gatazka
`FaultManager` STALL moduan modu-aldaketan berrehuntzen zen (target balioa modu berrian oker interpretatuz).
**Konponketa**: Modu-aldaketan `_stallSince` berrezarri eta `highEffort` baldintza gehitu (STALL soilik potentzia handian aktibatzeko).

---

# Fase 7.1: Alineación Motor-Encoder [ES]

Este ejemplo realiza la alineación eléctrica inicial entre el motor GM3506 y el encoder magnético MT6701 usando SimpleFOC, y valida el control en lazo cerrado en tres modos: Torque, Velocidad y Posición.

## Objetivos
- Ejecutar `motor.initFOC()` para determinar físicamente `zero_electric_angle` y `sensor_direction`.
- Validar el control de **Torque**, **Velocidad** y **Posición** en lazo cerrado sin carga.
- Persistir la calibración en Flash para no recalibrar en cada inicio.

## Características Hardware
- **Motor**: GM3506 gimbal (11 pares de polo, 24N22P)
- **Encoder**: MT6701 (bit-bang SPI, pines PA5/PA6/PA4)
- **Driver**: DRV8316 (6-PWM, SPI dedicado PB3/PB4/PB5)
- **MCU**: STM32G474CEU6

## Instrucciones: Primera Ejecución

### 1. Compilar y cargar
```bash
pio run -e test_foc_advanced --target upload
```

### 2. Desactivar STALL para tuning (solo primera vez)
```
X
```
Salida: `[DIAG] STALL detector: DISABLED (tuning mode)`

### 3. Calibración física (si Flash está vacío o reseteado)
Si aparece `[FLASH] Flash data has UNKNOWN direction`, el motor se alineará automáticamente. La salida correcta es:
```
[MAIN] FOC Initialized successfully.
[CAL] zero_electric_angle = 1.1098
[CAL] sensor_direction    = -1
```
> ⚠️ El motor se bloqueará brevemente durante la alineación. **Asegúrese de que puede girar libremente.**

### 4. Probar modo Torque (verificación de FOC)
```
M C 0      ← Modo torque (Uq directo)
M 2        ← 2V en eje Q
T1         ← Telemetría
```
**Resultado correcto**: `V: ~35 rad/s` y `A:` creciendo continuamente (motor girando).

### 5. Probar modo Velocidad
```
M C 1      ← Modo velocidad cerrada
M V P 2    ← P=2 (P=0.3 insuficiente para superar fricción estática)
M V I 0    ← I=0 para tuning estable sin windup
M 1        ← 1 rad/s objetivo
T1
```
**Resultado correcto**: `V: ~1.00` estable.

### 6. Probar modo Posición
```
M C 2            ← Modo posición (angle mode)
M 800            ← Ir a un ángulo cercano al shaft_angle actual
M 820
```
> **Nota**: `shaft_angle` es acumulativo (multi-vuelta). Usar objetivos cercanos al valor actual de `A:`.

### 7. Guardar calibración
```
S
```
Tras `[FLASH] Calibration saved!`, pulsar **RESET físico** para recargar limpiamente.

### 8. Verificar carga desde Flash
En el siguiente arranque, sin recalibrar:
```
[FLASH] Valid calibration found. Applying...
[CAL] zero_electric_angle = 1.1098
[CAL] sensor_direction    = -1
```

## Referencia de Comandos

| Comando | Descripción |
|---|---|
| `M C 0` | Modo Torque |
| `M C 1` | Modo Velocidad lazo cerrado |
| `M C 2` | Modo Posición |
| `M <valor>` | Fijar objetivo (torque: V, velocidad: rad/s, posición: rad) |
| `M V P <valor>` | Ajustar P del PID de velocidad |
| `M V I <valor>` | Ajustar I del PID de velocidad |
| `O <valor>` | Velocidad en lazo abierto (diagnóstico) |
| `T1` / `T0` | Telemetría ON/OFF |
| `X` | Activar/desactivar detector STALL |
| `S` | Guardar calibración en Flash |
| `R` | Resetear Flash (recalibrará al reiniciar) |
| `C` | Forzar calibración física |
| `D` | Volcar registros DRV8316 |
| `F` | Limpiar fallo STALL y rehabilitar motor |
| `A <valor>` | Aplicar `zero_electric_angle` manualmente |
| `Z` | Diagnóstico de ZEA (en lazo abierto) |

## Resultados de Validación

| Modo | Resultado | Parámetros |
|---|---|---|
| Torque directo | ✅ `V: ~35 rad/s` con `Uq=2V` | `M C 0`, `M 2` |
| Velocidad cerrada | ✅ `V: ~1.00 rad/s` estable | `P=2, I=0` |
| Posición cerrada | ✅ Motor converge al target | `P_angle=5` |
| Carga desde Flash | ✅ Sin recalibración física | `ZEA=1.1098, dir=-1` |

## Troubleshooting / Solución de Problemas
- **El motor vibra pero no gira en modo Torque/Velocidad**: 
  - *Causa*: Posible `zero_electric_angle` incorrecto debido a una calibración donde el rotor no estaba libre.
  - *Solución*: Envíe el comando `R` para resetear la Flash y pulse RESET físico para forzar una nueva calibración libre de fricción.
- **El motor emite un pitido agudo y no se mueve en modo Velocidad**:
  - *Causa*: Ganancia P demasiado baja para superar la fricción estática del motor.
  - *Solución*: Aumente `P` gradualmente (`M V P 2`, `M V P 3`) hasta que logre moverse suavemente.
- **Vibración violenta al usar PID de velocidad**:
  - *Causa*: El término Integral (I) está acumulando error (windup).
  - *Solución*: Ponga la integral a cero (`M V I 0`) hasta encontrar un P estable.
- **El motor salta a un ángulo enorme al activar el modo Posición**:
  - *Causa*: El `shaft_angle` acumula las vueltas de pruebas anteriores (ej. modo torque o abierto).
  - *Solución*: Lea el ángulo actual con telemetría (`T1`) y envíe un objetivo cercano a ese valor, o reinicie la placa.
- **Aparece el error STALL y se deshabilita el motor inmediatamente**:
  - *Causa*: El detector STALL actúa si el motor no se mueve y además está comandando un esfuerzo alto. Durante las pruebas de tuning con ganancias bajas puede suceder.
  - *Solución*: Desactive el STALL temporalmente enviando la letra `X`.

## Bugs Críticos Resueltos

### 1. Conflicto TIM1_BKIN (PA6)
Al inicializar `driver.init()`, el periférico TIM1 reconfiguraba PA6 como función alternativa `BKIN`, rompiendo el encoder MT6701.
**Fix**: Re-afirmar `pinMode()` en PA6 como GPIO tras `driver.init()`.

### 2. Conflicto SPI Global
El objeto `SPI` global podía usar PA5/PA6, colisionando con el bit-banging del encoder.
**Fix**: Instancia `SPIClass` dedicada para el DRV8316 en PB3/PB4/PB5.

### 3. Validación de Calibración en Flash
`resetToDefaults()` establecía `sensor_direction=CW`, generando una calibración falsa.
**Fix**: Default cambiado a `sensor_direction=UNKNOWN (0)`, forzando recalibración real.

### 4. Bloqueo por Escritura en Flash
`HAL_FLASH_Program()` bloquea el loop ~30ms, desincronizando el FOC.
**Fix**: Deshabilitar el motor antes de la escritura y rehabilitarlo tras ella.

### 5. Zero Eléctrico Incorrecto
En una sesión de calibración se obtuvo `ZEA=4.9195, dir=1`, pero no producía torque en lazo cerrado. El motor se bloqueaba sin girar (Uq iba al eje D, no al Q).
**Fix**: Reset de Flash y nueva calibración física. Resultado correcto: `ZEA=1.1098, dir=-1`.

### 6. STALL Falso en Cambios de Modo
El `FaultManager` disparaba STALL al cambiar de modo (el `target` del modo anterior se interpretaba incorrectamente en el nuevo modo).
**Fix**: Resetear `_stallSince` al detectar cambio de modo. Añadir condición `highEffort` (STALL solo si se aplica >50% del voltaje límite).

---

# Phase 7.1: Motor-Encoder Alignment [EN]

This example performs the electrical alignment between the GM3506 motor and the MT6701 magnetic encoder using SimpleFOC, and validates closed-loop control in Torque, Velocity, and Position modes.

## Objectives
- Run `motor.initFOC()` to physically determine `zero_electric_angle` and `sensor_direction`.
- Validate **Torque**, **Velocity**, and **Position** closed-loop control under no load.
- Persist calibration to Flash to avoid re-calibrating on each startup.

## Hardware
- **Motor**: GM3506 gimbal (11 pole pairs, 24N22P)
- **Encoder**: MT6701 (bit-bang SPI, pins PA5/PA6/PA4)
- **Driver**: DRV8316 (6-PWM, dedicated SPI PB3/PB4/PB5)
- **MCU**: STM32G474CEU6

## Instructions: First Use

### 1. Build and Upload
```bash
pio run -e test_foc_advanced --target upload
```

### 2. Disable STALL for tuning
```
X
```
Output: `[DIAG] STALL detector: DISABLED (tuning mode)`

### 3. Physical Calibration (empty or reset Flash)
If `[FLASH] Flash data has UNKNOWN direction` appears, the motor will self-align. Expected output:
```
[MAIN] FOC Initialized successfully.
[CAL] zero_electric_angle = 1.1098
[CAL] sensor_direction    = -1
```
> ⚠️ The motor will briefly lock during alignment. **Ensure it can rotate freely.**

### 4. Validate Torque Mode (FOC verification)
```
M C 0      ← Torque mode (direct Uq)
M 2        ← 2V on Q-axis
T1         ← Enable telemetry
```
**Expected**: `V: ~35 rad/s`, `A:` incrementing continuously.

### 5. Validate Velocity Mode
```
M C 1      ← Closed-loop velocity
M V P 2    ← P=2 (P=0.3 insufficient to overcome static friction)
M V I 0    ← I=0 for stable initial tuning
M 1        ← Target: 1 rad/s
T1
```
**Expected**: `V: ~1.00` stable.

### 6. Validate Position Mode
```
M C 2            ← Position mode
M 800            ← Target near current shaft_angle
M 820
```
> **Note**: `shaft_angle` is cumulative. Use targets near the current `A:` value shown in telemetry.

### 7. Save Calibration
```
S
```
After `[FLASH] Calibration saved!`, press the **physical RESET button** for a clean reload.

### 8. Verify Flash Load
On the next startup, no physical alignment occurs:
```
[FLASH] Valid calibration found. Applying...
[CAL] zero_electric_angle = 1.1098
[CAL] sensor_direction    = -1
```

## Command Reference

| Command | Description |
|---|---|
| `M C 0` | Torque mode |
| `M C 1` | Closed-loop velocity mode |
| `M C 2` | Position mode |
| `M <value>` | Set target (torque: V, velocity: rad/s, position: rad) |
| `M V P <value>` | Set velocity PID P gain |
| `M V I <value>` | Set velocity PID I gain |
| `O <value>` | Open-loop velocity (diagnostic) |
| `T1` / `T0` | Telemetry ON/OFF |
| `X` | Toggle STALL detector |
| `S` | Save calibration to Flash |
| `R` | Reset Flash (will recalibrate on next boot) |
| `C` | Force physical calibration |
| `D` | Dump DRV8316 registers |
| `F` | Clear STALL fault and re-enable motor |
| `A <value>` | Manually set `zero_electric_angle` |
| `Z` | Diagnose ZEA offset (run during open-loop) |

## Validation Results

| Mode | Result | Parameters Used |
|---|---|---|
| Direct torque | ✅ `V: ~35 rad/s` with `Uq=2V` | `M C 0`, `M 2` |
| Closed-loop velocity | ✅ `V: ~1.00 rad/s` stable | `P=2, I=0` |
| Closed-loop position | ✅ Motor converges to target | `P_angle=5` |
| Flash reload | ✅ No physical alignment needed | `ZEA=1.1098, dir=-1` |

## Troubleshooting
- **The motor vibrates but does not spin in Torque/Velocity mode**: 
  - *Cause*: Incorrect `zero_electric_angle`, likely because the rotor was not free during calibration.
  - *Fix*: Send the `R` command to reset the Flash, press the physical RESET button, and allow the motor to self-align without obstruction.
- **The motor emits a high-pitched beep and does not move in Velocity mode**:
  - *Cause*: PID `P` gain is too low to overcome the motor's static friction.
  - *Fix*: Gradually increase `P` (`M V P 2`, `M V P 3`) until it moves smoothly.
- **Violent vibration while tuning Velocity PID**:
  - *Cause*: The Integral (I) term is experiencing windup.
  - *Fix*: Set the integral to zero (`M V I 0`) until a stable P gain is found.
- **The motor spins rapidly to a huge angle when switching to Position mode**:
  - *Cause*: The `shaft_angle` is cumulative and retains all rotations from previous tests (like torque mode).
  - *Fix*: Read the current angle using telemetry (`T1`) and send a target close to that value, or restart the board.
- **STALL fault occurs and motor disables immediately**:
  - *Cause*: The STALL detector triggers if the motor doesn't move despite high effort. During tuning, low gains can trigger this.
  - *Fix*: Temporarily disable the STALL detector by sending the `X` command.

## Critical Bugs Fixed During Development

| Bug | Root Cause | Fix |
|---|---|---|
| Encoder always reads 0 | `driver.init()` reconfigured PA6 as TIM1_BKIN | Re-assert GPIO after `driver.init()` |
| SPI interference | Global `SPI` object touched PA5/PA6 | Dedicated `SPIClass _spi_drv` on PB3/PB4/PB5 |
| Flash loads stale calibration | Default `sensor_direction=CW` bypassed alignment | Default changed to `UNKNOWN (0)` |
| Motor desyncs after Flash save | `HAL_FLASH_Program()` blocks loop ~30ms | Disable motor before save, enable after |
| Wrong `zero_electric_angle` | Motor partially blocked during alignment | `voltage_sensor_align=10V`, fresh calibration |
| False STALL on mode change | Stale `target` misinterpreted in new mode | Reset `_stallSince` on mode change; add `highEffort` condition |
