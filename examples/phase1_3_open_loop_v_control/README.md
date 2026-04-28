# 1.3 Fasea - Open Loop Velocity Control (GM3506)

Karpeta honetan 1.3 Fasearen (`1_3_open_loop_v_control`) kode egiaztatua dago. Fase honen helburua iPower GM3506 motorra begizta irekian (open-loop) biraraztea da, DRV8316 6-PWM bidez ondo kontrolatzen dela ziurtatzeko.

### Zergatik egin den?
Fase hau segurtasunagatik egiten dugu. Motorra birarazi aurretik sentsorea motorrarekin lerrokatzen saiatzen bagara (closed-loop), errore batek motorra edo driver-a erre dezake. Hori saihesteko, aurretik motorra modu seguruan biratzen ote den frogatzen dugu, **2.0V-eko muga zorrotza jarrita**.

### Nola probatu

1. Ziurtatu plakaren 24V elikadura **DESKONEKTATUTA** dagoela kableatzen ari zaren bitartean.
2. Motorraren 3 faseak ondo konektatu.
3. Elikatu plaka 24V iturriarekin.
4. Konpilatu eta kargatu firmwarea ingurune espezifikoa erabiliz:
   ```bash
   pio run -e test_open_loop --target upload
   ```
5. Ireki Serie Monitorea (USB kablea Gripper-era konektatuta):
   ```bash
   pio device monitor --baud 115200
   ```
6. Idatzi terminalean eta bidali `T1` agindua motorra 1 rad/s abiaduran biratzeko. `T2` jarri dezakezu azkarrago biratzeko.
7. Geratzeko, bidali `T0`.

### Motorraren ezaugarriak (iPower GM3506 130T)
- **Poloak (Poles):** 14
- **Polo-pareak (Pole pairs):** 7
- **Bira (Turns):** 130
- **Erresistentzia:** 12.5 Ω
- **Tentsio gomendatua:** 11.1V

---

# Fase 1.3 - Control de Velocidad en Lazo Abierto (GM3506) [ES]

En esta carpeta se encuentra el código verificado de la Fase 1.3 (`1_3_open_loop_v_control`). El objetivo de esta fase es hacer girar el motor iPower GM3506 en lazo abierto (open-loop) para asegurar que el DRV8316 lo controla correctamente mediante 6-PWM.

### ¿Por qué se hace?
Esta fase se realiza por seguridad. Si intentamos alinear el sensor con el motor (lazo cerrado) antes de comprobar que el motor gira, un error podría quemar el motor o el driver. Para evitarlo, probamos primero si el motor gira de forma segura, estableciendo un **límite estricto de 2.0V**.

### Cómo Probarlo

1. Asegúrate de que la alimentación de 24V de la placa esté **DESCONECTADA** mientras realizas el cableado.
2. Conecta correctamente las 3 fases del motor.
3. Alimenta la placa con una fuente de 24V.
4. Compila y carga el código usando el entorno específico:
   ```bash
   pio run -e test_open_loop --target upload
   ```
5. Abre el Monitor Serie (con el cable USB conectado al Gripper):
   ```bash
   pio device monitor --baud 115200
   ```
6. Escribe en la terminal y envía el comando `T1` para que el motor gire a 1 rad/s. Puedes poner `T2` para que gire más rápido.
7. Para parar, envía `T0`.

### Características del Motor (iPower GM3506 130T)
- **Polos:** 14
- **Pares de polos:** 7
- **Vueltas (Turns):** 130
- **Resistencia:** 12.5 Ω
- **Tensión recomendada:** 11.1V

---

# Phase 1.3 - Open Loop Velocity Control (GM3506) [EN]

This folder contains the verified code for Phase 1.3 (`1_3_open_loop_v_control`). The goal of this phase is to rotate the iPower GM3506 motor in open-loop to ensure it is correctly controlled by the DRV8316 via 6-PWM.

### Why is it done?
This phase is performed for safety. If we attempt to align the sensor with the motor (closed-loop) before verifying that the motor rotates, an error could burn the motor or the driver. To prevent this, we first test if the motor rotates safely, setting a **strict 2.0V limit**.

### How to Test

1. Ensure the board's 24V power supply is **DISCONNECTED** while wiring.
2. Correctly connect the 3 motor phases.
3. Power the board with a 24V source.
4. Compile and upload the code using the specific environment:
   ```bash
   pio run -e test_open_loop --target upload
   ```
5. Open the Serial Monitor (with the USB cable connected to the Gripper):
   ```bash
   pio device monitor --baud 115200
   ```
6. Type in the terminal and send the `T1` command to make the motor rotate at 1 rad/s. You can use `T2` to make it rotate faster.
7. To stop, send `T0`.

### Motor Characteristics (iPower GM3506 130T)
- **Poles:** 14
- **Pole pairs:** 7
- **Turns:** 130
- **Resistance:** 12.5 Ω
- **Recommended voltage:** 11.1V
