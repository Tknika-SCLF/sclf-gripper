# 4. Fasea: USB VCP eta Commander

Adibide honek **USB VCP (Virtual COM Port)** interfazearen eta **SimpleFOC (Commander)** komando-sistema interaktiboaren inplementazioa balioztatzen du.

## Helburuak
- USB CDC ataka abiadura handiko serie-komunikaziorako konfiguratzea.
- `VCPManager` osagaia inplementatzea komandoak eta telemetria kudeatzeko.
- PIDen eta motorraren parametroen doikuntza denbora errealean baimentzea, berriro konpilatu gabe.
- Telemetriak FOC begiztaren (>= 10kHz) errendimenduan interferentziarik ez duela ziurtatzea.

## Konexioak
- **USB Type-C**: PCra zuzenean konektatu.
- **Elikadura**: 24V behar ditu motorrarentzat, nahiz eta logika USB bidez elikatu daitekeen hasierako konfiguraziorako.

## Komando Erabilgarriak
Serie-monitorea irekitzean (115200 bps), komando hauek bidali daitezke:
- `M?`: Motorraren laguntza.
- `M1.5`: Helburua 1.5-ean ezarri (rad/s edo rad moduaren arabera).
- `ME0`: Motorra desgaitu.
- `ME1`: Motorra gaitu.
- `MC`: Kontrol modua aldatu.
- `MP`: Posizio PIDa doitu.
- `MV`: Abiadura PIDa doitu.
- `T1`: Telemetria jarraitua aktibatu.
- `T0`: Telemetria desgaitu.

## Erabilera

1. **Ingurunea Hautatu**: PlatformIO-n, hautatu `test_usb_vcp` ingurunea.
2. **Kargatu**: Konpilatu eta kargatu firmwarea ingurune espezifikoa erabiliz:
   ```bash
   pio run -e test_usb_vcp --target upload
   ```
3. **Serie-Monitorea**:
   - Ireki serie-monitorea (`pio device monitor`).
   - Ziurtatu ataka USB CDC-ari dagokiona dela.
   - Konfiguratu monitorea `LF` (Line Feed) bidaltzeko lerro amaiera gisa.
4. **Iradokitako probak**:
   - Bidali `?` sistemak erantzuten duela ikusteko.
   - Bidali `T1` telemetria aktibatzeko eta mugimendua denbora errealean ikusteko.
   - Bidali `M1.5` motorra 1.5 rad/s-ra biratzen has dadin.
   - Bidali `MV P0.2` abiaduraren P irabazia "beroan" doitzeko.

## Arazoen Konponbidea
- **Ez dago erantzunik**: Ziurtatu USB kablea plakako USB-C atakara konektatuta dagoela, ez soilik ST-Link-era.
- **Warn: \r detected**: Normala da zure terminalak `CR+LF` bidaltzen badu. Sistemak komandoa ondo prozesatuko du oharra agertu arren.

---

# Fase 4: USB VCP y Commander [ES]

Este ejemplo valida la implementación del interfaz **USB VCP (Virtual COM Port)** y el sistema de comandos interactivo de **SimpleFOC (Commander)**.

## Objetivos
- Configurar el puerto USB CDC para comunicación serie a alta velocidad.
- Implementar el componente `VCPManager` para gestionar comandos y telemetría.
- Permitir el ajuste en tiempo real de los PIDs y parámetros del motor sin recompilar.
- Validar que la telemetría no interfiera con el rendimiento del bucle FOC (>= 10kHz).

## Conexiones
- **USB Type-C**: Conectar directamente al PC.
- **Alimentación**: Requiere 24V para el motor, aunque la lógica puede alimentarse vía USB para configuración inicial.

## Comandos Disponibles
Al abrir el monitor serie (115200 bps), se pueden enviar los siguientes comandos:
- `M?`: Ayuda del motor.
- `M1.5`: Establecer target a 1.5 (rad/s o rad según modo).
- `ME0`: Desactivar motor.
- `ME1`: Activar motor.
- `MC`: Cambiar modo de control.
- `MP`: Ajustar PID de posición.
- `MV`: Ajustar PID de velocidad.
- `T1`: Activar telemetría continua.
- `T0`: Desactivar telemetría.

## Uso

1. **Seleccionar Entorno**: En PlatformIO, selecciona el entorno `test_usb_vcp`.
2. **Cargar**: Compila y carga el firmware usando el entorno específico:
   ```bash
   pio run -e test_usb_vcp --target upload
   ```
3. **Monitor Serie**: 
   - Abre el monitor serie (`pio device monitor`).
   - Asegúrate de que el puerto sea el que corresponda a tu USB CDC.
   - Configura el monitor para enviar `LF` (Line Feed) como fin de línea.
4. **Pruebas sugeridas**:
   - Envía `?` para ver que el sistema responde.
   - Envía `T1` para activar la telemetría y ver el movimiento en tiempo real.
   - Envía `M1.5` para que el motor empiece a girar a 1.5 rad/s.
   - Envía `MV P0.2` para ajustar la ganancia P de velocidad en caliente.

## Solución de Problemas
- **No hay respuesta**: Asegúrate de que el cable USB está conectado al puerto USB-C de la placa, no solo al ST-Link.
- **Warn: \r detected**: Es normal si tu terminal envía `CR+LF`. El sistema procesará el comando correctamente a pesar del aviso.

---

# Phase 4: USB VCP and Commander [EN]

This example validates the implementation of the **USB VCP (Virtual COM Port)** interface and the **SimpleFOC (Commander)** interactive command system.

## Objectives
- Configure the USB CDC port for high-speed serial communication.
- Implement the `VCPManager` component to manage commands and telemetry.
- Allow real-time adjustment of PIDs and motor parameters without recompiling.
- Validate that telemetry does not interfere with FOC loop performance (>= 10kHz).

## Connections
- **USB Type-C**: Connect directly to the PC.
- **Power**: Requires 24V for the motor, although the logic can be powered via USB for initial configuration.

## Available Commands
When opening the serial monitor (115200 bps), the following commands can be sent:
- `M?`: Motor help.
- `M1.5`: Set target to 1.5 (rad/s or rad depending on mode).
- `ME0`: Disable motor.
- `ME1`: Enable motor.
- `MC`: Change control mode.
- `MP`: Adjust position PID.
- `MV`: Adjust velocity PID.
- `T1`: Enable continuous telemetry.
- `T0`: Disable telemetry.

## Usage

1. **Select Environment**: In PlatformIO, select the `test_usb_vcp` environment.
2. **Upload**: Compile and upload the firmware using the specific environment:
   ```bash
   pio run -e test_usb_vcp --target upload
   ```
3. **Serial Monitor**:
   - Open the serial monitor (`pio device monitor`).
   - Ensure the port corresponds to your USB CDC.
   - Configure the monitor to send `LF` (Line Feed) as the end of line.
4. **Suggested Tests**:
   - Send `?` to see that the system responds.
   - Send `T1` to enable telemetry and see movement in real-time.
   - Send `M1.5` to start the motor rotating at 1.5 rad/s.
   - Send `MV P0.2` to adjust the velocity P gain on the fly.

## Troubleshooting
- **No response**: Ensure the USB cable is connected to the board's USB-C port, not just the ST-Link.
- **Warn: \r detected**: This is normal if your terminal sends `CR+LF`. The system will process the command correctly despite the warning.
