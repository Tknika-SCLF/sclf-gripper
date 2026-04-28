# 5. Fasea: Akats Kudeatzailea (Fault Manager)

Adibide honek SCLF Gripper-aren segurtasun kudeaketa sistema (**Fault Manager**) balioztatzen du. Sistemak hardwarea etengabe monitorizatzen du kalteak saihesteko blokeo mekaniko edo sentsore akatsen kasuan.

## Helburuak
- **Babes Aktiboa**: Motorra berehala desgaitu (`motor.disable()`) akats bat detektatzean.
- **Zarata Iragazkia**: MT6701 encoder-arentzat akats kontagailu bat inplementatu, SPI zarata dela eta faltsu positiboak saihesteko.
- **Blokeo Detekzioa (Stall)**: Motorra mekanikoki blokeatuta dagoen identifikatu segundo bat baino gehiagoz.
- **Berrezartze Interfazea**: Akats baten ondoren berreskuratze fluxu seguru bat inplementatu.

## Testeo Argibide Xehatuak

1. **Ingurunea Hautatu**: PlatformIO-n, hautatu `test_fault_manager` ingurunea.
2. **Kargatu**: Konpilatu eta kargatu firmwarea ingurune espezifikoa erabiliz:
   ```bash
   pio run -e test_fault_manager --target upload
   ```
3. **Serie-Monitorea**: Ireki terminalea (115200 bps).
4. **Abiaraztea**:
   - Bidali `M2` motorra 2 rad/s-ra birarazteko.
5. **Blokeo Proba (Stall)**:
   - Heldu motorraren ardatzari eskuz gogor, biraketa galarazteko.
   - **Aurreikusitako emaitza**: **Segundo 1** blokeatuta egon ondoren, motorra erabat geldituko da. `[SAFETY] CRITICAL FAULT DETECTED: STALL` mezua ikusiko duzu.
   - Plakako LEDak oso azkar egingo du dir-dir (50ms), errore egoera adieraziz.
6. **Berrezartze Seguruaren Proba**:
   - Saiatu motorra pizten `ME1` bidaliz.
   - **Aurreikusitako emaitza**: Sistemak agindua ukatuko du eta `[SAFETY] Motor disabled. Clear fault first!` mezua bidaliko du.
   - Kontrola berreskuratzeko, bidali `F` komandoa (Clear Faults).
   - Orain bidali dezakezu `ME1` motorra gaitzeko eta berriro lan egiteko.

## Detektatutako Akatsak
- **DRV_FAULT**: DRV8316 driver-aren barne akatsak (gainkorrontea, tenperatura).
- **ENCODER_ERROR**: MT6701-arekiko SPI komunikazio akatsa (10 akats jarraian behar ditu).
- **STALL**: Abiadura < 0.5 rad/s helburu aktiboarekin > 1000ms-z.

---

# Fase 5: Fault Manager [ES]

Este ejemplo valida el sistema de gestión de seguridad (**Fault Manager**) del SCLF Gripper. El sistema monitoriza constantemente el hardware para evitar daños en caso de bloqueo mecánico o fallo de sensores.

## Objetivos
- **Protección Activa**: Desactivar el motor (`motor.disable()`) inmediatamente al detectar un fallo.
- **Filtro de Ruido**: Implementar un contador de errores para el encoder MT6701 para evitar falsos positivos por ruido SPI.
- **Detección de Stall**: Identificar si el motor está bloqueado mecánicamente durante más de 1 segundo.
- **Interfaz de Rearme**: Implementar un flujo seguro de recuperación tras un fallo.

## Instrucciones de Testeo Detalladas

1. **Seleccionar Entorno**: En PlatformIO, selecciona el entorno `test_fault_manager`.
2. **Cargar**: Compila y carga el firmware usando el entorno específico:
   ```bash
   pio run -e test_fault_manager --target upload
   ```
3. **Monitor Serie**: Abre el terminal (115200 bps).
4. **Puesta en Marcha**:
   - Envía `M2` para hacer girar el motor a 2 rad/s.
5. **Prueba de Bloqueo (Stall)**:
   - Sujeta el eje del motor con la mano firmemente para impedir su rotación.
   - **Resultado esperado**: Tras **1 segundo** de bloqueo, el motor se detendrá por completo. Verás el mensaje `[SAFETY] CRITICAL FAULT DETECTED: STALL`.
   - El LED de la placa parpadeará muy rápido (50ms) indicando el estado de error.
6. **Prueba de Rearme Seguro**:
   - Intenta encender el motor enviando `ME1`.
   - **Resultado esperado**: El sistema rechazará la orden y enviará el mensaje `[SAFETY] Motor disabled. Clear fault first!`.
   - Para recuperar el control, envía el comando `F` (Clear Faults).
   - Ahora ya puedes enviar `ME1` para habilitar el motor y volver a operar.

## Fallos Detectados
- **DRV_FAULT**: Errores internos del driver DRV8316 (sobrecorriente, temperatura).
- **ENCODER_ERROR**: Fallo de comunicación SPI con el MT6701 (requiere 10 fallos consecutivos).
- **STALL**: Velocidad < 0.5 rad/s con objetivo activo durante > 1000ms.

---

# Phase 5: Fault Manager [EN]

This example validates the safety management system (**Fault Manager**) of the SCLF Gripper. The system constantly monitors the hardware to prevent damage in case of mechanical blockage or sensor failure.

## Objectives
- **Active Protection**: Immediately disable the motor (`motor.disable()`) upon detecting a fault.
- **Noise Filter**: Implement an error counter for the MT6701 encoder to avoid false positives due to SPI noise.
- **Stall Detection**: Identify if the motor is mechanically blocked for more than 1 second.
- **Reset Interface**: Implement a safe recovery flow after a fault.

## Detailed Testing Instructions

1. **Select Environment**: In PlatformIO, select the `test_fault_manager` environment.
2. **Upload**: Compile and upload the firmware using the specific environment:
   ```bash
   pio run -e test_fault_manager --target upload
   ```
3. **Serial Monitor**: Open the terminal (115200 bps).
4. **Startup**:
   - Send `M2` to rotate the motor at 2 rad/s.
5. **Stall Test**:
   - Firmly hold the motor shaft by hand to prevent its rotation.
   - **Expected Result**: After **1 second** of blockage, the motor will stop completely. You will see the message `[SAFETY] CRITICAL FAULT DETECTED: STALL`.
   - The board's LED will flash very fast (50ms) indicating the error state.
6. **Safe Reset Test**:
   - Try to turn on the motor by sending `ME1`.
   - **Expected Result**: The system will reject the command and send the message `[SAFETY] Motor disabled. Clear fault first!`.
   - To regain control, send the `F` command (Clear Faults).
   - Now you can send `ME1` to enable the motor and resume operation.

## Detected Faults
- **DRV_FAULT**: Internal DRV8316 driver errors (overcurrent, temperature).
- **ENCODER_ERROR**: SPI communication failure with the MT6701 (requires 10 consecutive failures).
- **STALL**: Velocity < 0.5 rad/s with active target for > 1000ms.
