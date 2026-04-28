# 2.1 Fasea — FOC Open-Loop Testa

Test honek motorraren biraketa lazo irekian (Open-Loop) balioztatzen du, **SimpleFOC** liburutegia eta `MotorController` wrapper klasea erabiliz.

## Helburua
- Motorraren biraketa suabea egiaztatzea encoder-aren lerrokatzerik behar izan gabe.
- Hardwarean faseen alderantzikapena (C-B-A) softwarearen konfigurazioarekin bat datorrela balioztatzea.
- DRV8316 driver-a eta potentzia-etapa kargapean (8V-eko muga) funtzionatzen dutela egiaztatzea.

## Hardware Baldintzak
- **24V**-eko elikadura iturria.
- ST-Link konexioa firmwarea igotzeko.
- USB-C kablea gripper-aren atakara konektatuta telemetriarako.

## Nola egin testa

1. **Konpilatu eta kargatu**:
   Exekutatu komando hau proiektuaren erroan ingurune espezifiko hau igotzeko:
   ```bash
   pio run -e test_foc_open_loop --target upload
   ```

2. **Serie-Monitorea**:
   - Ireki serie-monitorea **115200 baudean**.
   - `[MC] System Initialized OK` mezua ikusi beharko zenuke.
   - Sistema lehenetsita `MotionControlType::velocity_openloop` moduan abiatzen da.

3. **Proba Komandoak**:
   - `M10`: Motorra 10 rad/s-ra birarazten du erlojuaren noranzkoan.
   - `M-10`: Motorra -10 rad/s-ra birarazten du (kontrako noranzkoan).
   - `M0`: Motorra gelditzen du.
   - `ME0`: Motorra ezgaitzen du (torque zero).
   - `ME1`: Motorra gaitzen du.

4. **Egiaztapena**:
   - Behatu motorra modu suabe eta isilean biratzen den.
   - Egiaztatu `Ang` eta `Vel` balioak aldizkako txostenean. Kontrola lazo irekian izan arren, MT6701 encoder-ak benetako biraketa angelua erakutsi beharko luke.

## Diagnostikoa
- Motorrak zarata edo bibrazioa egiten badu baina biratzen ez bada: Egiaztatu elikadura iturriak korronte nahikoa duela (gutxienez 1.5A) eta `PIN_AH/AL...` pinek zirkuitulaburrik ez dutela.
- Monitoreak daturik erakusten ez badu: Berrikusi `HSI48` erlojuaren konfigurazioa `main.cpp`-n (`MotorController` bidez) USB CDC-rako.

---

# Phase 2.1 — FOC Open-Loop Test [ES]

Este test valida la rotación del motor en lazo abierto (Open-Loop) utilizando la librería **SimpleFOC** y la clase wrapper `MotorController`.

## Objetivo
- Verificar la rotación suave del motor sin necesidad de alineación del encoder.
- Validar que la inversión de fases (C-B-A) en el hardware coincide con la configuración del software.
- Comprobar que el driver DRV8316 y la etapa de potencia funcionan bajo carga (8V límite).

## Requisitos de Hardware
- Fuente de alimentación a **24V**.
- Conexión ST-Link para el flasheo.
- Cable USB-C conectado al puerto del gripper para telemetría.

## Cómo realizar el test

1. **Compilar y flashear**:
   Ejecuta el siguiente comando en la raíz del proyecto para subir este entorno específico:
   ```bash
   pio run -e test_foc_open_loop --target upload
   ```

2. **Monitor Serie**:
   - Abre el monitor serie a **115200 baudios**.
   - Deberías ver el mensaje: `[MC] System Initialized OK`.
   - El sistema arranca por defecto en `MotionControlType::velocity_openloop`.

3. **Comandos de Prueba**:
   - `M10`: Hace girar el motor a 10 rad/s en sentido horario.
   - `M-10`: Hace girar el motor a -10 rad/s (sentido antihorario).
   - `M0`: Detiene el motor.
   - `ME0`: Deshabilita el motor (torque cero).
   - `ME1`: Habilita el motor.

4. **Verificación**:
   - Observa si el motor gira de forma suave y silenciosa.
   - Verifica los valores de `Ang` y `Vel` en el reporte periódico. Aunque el control sea en lazo abierto, el encoder bit-bang MT6701 debería estar reportando el ángulo real de giro.

## Diagnóstico
- Si el motor hace ruido o vibra pero no gira: Verifica que la fuente de alimentación tenga corriente suficiente (mínimo 1.5A) y que los pines `PIN_AH/AL...` no tengan cortocircuitos.
- Si el monitor no muestra datos: Revisa la configuración del reloj `HSI48` en `main.cpp` (vía `MotorController`) para el USB CDC.

---

# Phase 2.1 — FOC Open-Loop Test [EN]

This test validates the motor rotation in open-loop using the **SimpleFOC** library and the `MotorController` wrapper class.

## Objective
- Verify smooth motor rotation without the need for encoder alignment.
- Validate that the phase reversal (C-B-A) in hardware matches the software configuration.
- Check that the DRV8316 driver and power stage function under load (8V limit).

## Hardware Requirements
- **24V** power supply.
- ST-Link connection for flashing.
- USB-C cable connected to the gripper port for telemetry.

## How to Perform the Test

1. **Compile and upload**:
   Run the following command in the project root to upload this specific environment:
   ```bash
   pio run -e test_foc_open_loop --target upload
   ```

2. **Serial Monitor**:
   - Open the serial monitor at **115200 baud**.
   - You should see the message: `[MC] System Initialized OK`.
   - The system starts by default in `MotionControlType::velocity_openloop`.

3. **Test Commands**:
   - `M10`: Rotates the motor at 10 rad/s clockwise.
   - `M-10`: Rotates the motor at -10 rad/s (counter-clockwise).
   - `M0`: Stops the motor.
   - `ME0`: Disables the motor (zero torque).
   - `ME1`: Enables the motor.

4. **Verification**:
   - Observe if the motor rotates smoothly and quietly.
   - Verify the `Ang` and `Vel` values in the periodic report. Even though the control is in open-loop, the MT6701 bit-bang encoder should be reporting the actual rotation angle.

## Diagnosis
- If the motor makes noise or vibrates but does not rotate: Verify that the power supply has enough current (minimum 1.5A) and that the `PIN_AH/AL...` pins do not have short circuits.
- If the monitor does not show data: Review the `HSI48` clock configuration in `main.cpp` (via `MotorController`) for USB CDC.
