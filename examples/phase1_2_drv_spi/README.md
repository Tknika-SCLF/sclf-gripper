# 1.2 Fasea — DRV8316 SPI Konfigurazio Testa

Proba-ingurune honek DRV8316 motor-gidariarekin SPI komunikazioa hasieratzen du eta bere egoera-erregistroen (STATUS1 eta STATUS2) aldizkako kontsulta (polling) egiten du.

## Beharrezko Hardwarea
- SCLF Gripper plaka konektatuta.
- **24V-eko elikadura konektatuta** (DRV8316-ak VM tentsio nagusia behar du barne-logika gaitzeko eta SPI bidez erantzuteko).
- USB konexioa VCP atakara serie-monitorearentzat.

## Erabiltzeko Argibideak

1. **Konpilatu eta kargatu:**
   Exekutatu komando hau proiektuaren erroan ingurune espezifiko hau igotzeko:
   ```bash
   pio run -e test_drv_spi --target upload
   ```

2. **Serie-Monitorea:**
   Ireki serie-monitorea 115200-ko baud rate-arekin:
   ```bash
   pio device monitor --baud 115200
   ```

3. **Egiaztapena:**
   - SPI komunikazioa arrakastatsua da monitoreak MISO flotatzaile erroreik ematen ez badu (`(STATUS=0x7FF)`).
   - Testak STATUS1 eta STATUS2-ren balio gordinak erakutsiko ditu etengabe.
   - **Diseinu oharra:** Aldatu gabeko hardwarean (L1 konektatuta), normala da `STATUS2: 0x60` irakurtzea (Buck erreguladorearen UV/OCP akatsa), 3.3V-eko LDO-arekin dagoen gatazka elektrikoagatik. Irakurketa hau lortzeak SPI busa eta liburutegia %100ean funtzionatzen dutela baieztatzen du.

---

# FASE 1.2 — DRV8316 SPI Config Test [ES]

Este entorno de prueba (test) inicializa la comunicación SPI con el driver de motor DRV8316 y ejecuta un sondeo regular (polling) de sus registros de estado (STATUS1 y STATUS2).

## Hardware Requerido
- Placa SCLF Gripper conectada.
- **Alimentación de 24V conectada** (El DRV8316 necesita la tensión de bus principal VM para habilitar la lógica interna y responder por SPI).
- Conexión USB al puerto VCP para el monitor serie.

## Instrucciones de Uso

1. **Compilar y flashear:**
   Ejecuta el siguiente comando en la raíz del proyecto para subir este entorno específico:
   ```bash
   pio run -e test_drv_spi --target upload
   ```

2. **Monitor Serie:**
   Abre el monitor serie con un baud rate de 115200:
   ```bash
   pio device monitor --baud 115200
   ```

3. **Verificación:**
   - La comunicación SPI es exitosa si el monitor no reporta error de MISO flotante (`(STATUS=0x7FF)`).
   - El test mostrará continuamente el valor raw de STATUS1 y STATUS2.
   - **Nota de diseño:** En hardware sin modificar (con L1 conectada), es perfectamente normal leer `STATUS2: 0x60` (Fallo de UV/OCP en el regulador Buck) debido al conflicto eléctrico con el LDO de 3.3V. Obtener esta lectura confirma que el bus SPI y la librería están funcionando al 100%.

---

# Phase 1.2 — DRV8316 SPI Config Test [EN]

This test environment initializes SPI communication with the DRV8316 motor driver and performs regular polling of its status registers (STATUS1 and STATUS2).

## Required Hardware
- SCLF Gripper board connected.
- **24V power supply connected** (The DRV8316 needs the main bus voltage VM to enable internal logic and respond via SPI).
- USB connection to the VCP port for the serial monitor.

## Usage Instructions

1. **Compile and flash:**
   Run the following command in the project root to upload this specific environment:
   ```bash
   pio run -e test_drv_spi --target upload
   ```

2. **Serial Monitor:**
   Open the serial monitor with a baud rate of 115200:
   ```bash
   pio device monitor --baud 115200
   ```

3. **Verification:**
   - SPI communication is successful if the monitor does not report a floating MISO error (`(STATUS=0x7FF)`).
   - The test will continuously display the raw value of STATUS1 and STATUS2.
   - **Design note:** In unmodified hardware (with L1 connected), it is perfectly normal to read `STATUS2: 0x60` (Buck regulator UV/OCP fault) due to the electrical conflict with the 3.3V LDO. Obtaining this reading confirms that the SPI bus and the library are working 100%.
