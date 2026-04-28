# 3.1 Fasea - RS485 Protokolo Básicoa (STABLE)

Karpeta honetan 3.1 Fasearen (`fase3_1_rs485`) kode egiaztatua dago. Fase honen helburua RS-485 half-duplex bus-a probatzea da, motorra aktibatu gabe.

### Nola funtzionatzen du?

Gripperrak MAX3485 transceptorea du RS-485 bus bitartez komunikatzeko. Protokoloa ASCII formatuan dago, `\n`-z amaitzen den mezu bakoitzak formatu hau du:

```
<id>:<komandoa>:<balioa>\n
```

Adibidez: `1:V:5.0\n` → 1 ID-ko gailuari, abiadura (V) komandoa, 5.0 balioarekin.

### Nola probatu

1. **Konfigurazioa**: Ziurtatu `platformio.ini`-n `-D HSE_VALUE=16000000U` eta `-Wl,-u,_printf_float` flag-ak aktibatuta daudela (16MHz-eko kristala eta float-ak ondo erabiltzeko).
2. Konektatu USB-RS485 moldagailu bat PCra.
3. Konektatu moldagailuaren A/B kableak Gripper-aren RS-485 A/B pinetara.
4. Konpilatu eta kargatu firmwarea ingurune espezifikoa erabiliz:
   ```bash
   pio run -e test_rs485_protocol --target upload
   ```
5. Ireki serie terminal bat PCan (Putty, Docklight, etab.) RS-485 serian, 9600 baud-era.
6. Bidali `1:PING:0\n` eta egiaztatu Gripperrak `1:PONG:0\n` erantzuten duela.
7. Bidali `1:?A:0\n` eta ikusi `1:A:1.2300\n` erantzuna (float verified).

### USB-RS485 moldagailuaren konexioa

| Kablearen Kolorea | Seinalea | Gripper Pina |
|-------------------|----------|--------------|
| Laranja (D+)      | RS485 A+ | A            |
| Horia (D-)        | RS485 B- | B            |
| Beltza (GND)      | GND      | GND          |

### Pin mapa (MAX3485)

| Pina | Funtzioa | STM32 Pin |
|------|----------|-----------|
| TX   | UART TX  | PC10      |
| RX   | UART RX  | PC11      |
| DIR  | DE/RE    | PB9       |

---

# Fase 3.1 - Protocolo Básico RS485 (ESTABLE) [ES]

En esta carpeta se encuentra el código verificado de la Fase 3.1 (`fase3_1_rs485`). El objetivo de esta fase es probar el bus RS-485 half-duplex sin activar el motor.

### ¿Cómo funciona?

El Gripper cuenta con un transceptor MAX3485 para comunicarse a través del bus RS-485. El protocolo está en formato ASCII, donde cada mensaje termina en `\n` y sigue este formato:

```
<id>:<comando>:<valor>\n
```

Ejemplo: `1:V:5.0\n` → Al dispositivo con ID 1, comando de velocidad (V), con valor 5.0.

### Cómo Probarlo

1. **Configuración**: Asegúrate de que en `platformio.ini` estén activados los flags `-D HSE_VALUE=16000000U` y `-Wl,-u,_printf_float` (para usar correctamente el cristal de 16MHz y los floats).
2. Conecta un adaptador USB-RS485 al PC.
3. Conecta los cables A/B del adaptador a los pines RS-485 A/B del Gripper.
4. Compila y carga el código usando el entorno específico:
   ```bash
   pio run -e test_rs485_protocol --target upload
   ```
5. Abre un terminal serie en el PC (Putty, Docklight, etc.) en el puerto RS-485 a 9600 baudios.
6. Envía `1:PING:0\n` y verifica que el Gripper responde `1:PONG:0\n`.
7. Envía `1:?A:0\n` y observa la respuesta `1:A:1.2300\n` (float verificado).

### Conexión del adaptador USB-RS485

| Color del Cable | Señal | Pin del Gripper |
|-------------------|----------|--------------|
| Naranja (D+)      | RS485 A+ | A            |
| Amarillo (D-)     | RS485 B- | B            |
| Negro (GND)       | GND      | GND          |

### Mapa de Pines (MAX3485)

| Pin | Función | Pin STM32 |
|------|----------|-----------|
| TX   | UART TX  | PC10      |
| RX   | UART RX  | PC11      |
| DIR  | DE/RE    | PB9       |

---

# Phase 3.1 - Basic RS485 Protocol (STABLE) [EN]

This folder contains the verified code for Phase 3.1 (`fase3_1_rs485`). The goal of this phase is to test the RS-485 half-duplex bus without activating the motor.

### How it works?

The Gripper features a MAX3485 transceiver to communicate via the RS-485 bus. The protocol is in ASCII format, where each message ends in `\n` and follows this format:

```
<id>:<command>:<value>\n
```

Example: `1:V:5.0\n` → To the device with ID 1, velocity (V) command, with value 5.0.

### How to Test

1. **Configuration**: Ensure that in `platformio.ini` the flags `-D HSE_VALUE=16000000U` and `-Wl,-u,_printf_float` are enabled (to correctly use the 16MHz crystal and floats).
2. Connect a USB-RS485 adapter to the PC.
3. Connect the A/B wires of the adapter to the Gripper's RS-485 A/B pins.
4. Compile and upload the firmware using the specific environment:
   ```bash
   pio run -e test_rs485_protocol --target upload
   ```
5. Open a serial terminal on the PC (Putty, Docklight, etc.) on the RS-485 port at 9600 baud.
6. Send `1:PING:0\n` and verify that the Gripper responds with `1:PONG:0\n`.
7. Send `1:?A:0\n` and observe the response `1:A:1.2300\n` (float verified).

### USB-RS485 Adapter Connection

| Wire Color | Signal | Gripper Pin |
|-------------------|----------|--------------|
| Orange (D+)       | RS485 A+ | A            |
| Yellow (D-)       | RS485 B- | B            |
| Black (GND)       | GND      | GND          |

### Pin Map (MAX3485)

| Pin | Function | STM32 Pin |
|------|----------|-----------|
| TX   | UART TX  | PC10      |
| RX   | UART RX  | PC11      |
| DIR  | DE/RE    | PB9       |
