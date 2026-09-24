# 3.3 Fasea - UR Robot / URCap RS-485 Integrazio Probak [EU]

Karpeta honetan 3.3 Fasearen (`phase3_3_ur_rs485`) inplementazioa eta dokumentazioa dago. Fase honen helburua gripperra UR (Universal Robots) robot baten kontrolagailura konektatzea eta RS-485 bus bidezko komunikazioa eta kontrola balioztatzea da (URCap eta URSim bidez).

## Kableatu eta Konexio Diagrama

Konektatu robotaren RS-485 interfazea gripperraren **J3** konektorera honela:

| Gripper J3 Pin | Seinalea | UR Robot / RS-485 Egokitzailea |
|:---:|:---:|:---:|
| **Pin 7** | RS485+ (A) | RXD/TXD+ (A) |
| **Pin 8** | RS485- (B) | RXD/TXD- (B) |
| **Pin 1 & 2** | GND | GND |

> [!IMPORTANT]
> Seinaleak zuzen doazela ziurtatu eta kable biek lur berbera (GND) erabili behar dute zarata diferentziala saihesteko.

## Komandoen Zerrenda eta Protokoloa

Komunikazioak **115200 baudios**-eko abiadura lehenesten du, UR robotetako eta URCap komunikazioetan estandarra baita. Formatua: `<device_id>:<cmd>:<value>\n` da (adibidez gailu ID = `1`).

| Komandoa | Deskribapena | Ekintza | Adibidea | Erantzuna |
|:---:|:---:|:---:|:---:|:---:|
| `PING` | Bus detekzioa | Komunikazioa probatu | `1:PING:0\n` | `1:PONG:0.0000\n` |
| `EN` | Habilitar | Gripperra gaitu | `1:EN:0\n` | `1:ACK_EN:1.0000\n` |
| `DIS` | Disable | Gripperra desgaitu | `1:DIS:0\n` | `1:ACK_DIS:0.0000\n` |
| `P` | Position (0-85) | Posizioa ezarri (mm-tan) | `1:P:42.5\n` | `1:ACK_P:42.5000\n` |
| `V` | Speed (0-100) | Abiadura ehunekoa ezarri | `1:V:80\n` | `1:ACK_V:80.0000\n` |
| `T` | Force (0-100) | Indar ehunekoa ezarri | `1:T:50\n` | `1:ACK_T:50.0000\n` |
| `?A` | Get Angle | Uneko posizioa lortu (mm) | `1:?A:0\n` | `1:A:<posizioa>\n` |
| `?S` | Get Status | Egoera bitak lortu (0x01: Galduta/Aktibatuta, 0x02: Mugimenduan, 0x04: Akatsa) | `1:?S:0\n` | `1:S:<egoera_bita>\n` |

### LED eta Simulatutako Mugimendua
Fase honetan komunikazio protokoloa eta URCap-arekiko elkarreragina segurtasunez balioztatzen dira. Potentzia irteerak (PWM eta DRV CS) desgaituta daude hardware segurtasunagatik, eta mugimendua software bidez simulatzen da:
- **LED Keinua:** Desgaituta dagoenean 1000 ms, gaituta dagoenean 500 ms, eta mugimenduan dagoenean 100 ms (keinu azkarra).

## Nola Probatu

1. Ziurtatu `platformio.ini`-n `test_ur_rs485` ingurunea aukeratuta dagoela.
2. Konpilatu eta igo firmwarea gripper-era:
   ```bash
   pio run -e test_ur_rs485 --target upload
   ```
3. Exekutatu URSim eta URCap-a (edo RS-485 egokitzaile bat PC-an **115200 baudios**-era).
4. Bidali komandoak eta ikusi ACK erantzunak.

---

# Fase 3.3 - Pruebas de Integración RS-485 con Robot UR / URCap [ES]

En esta carpeta se encuentra la implementación y documentación de la Fase 3.3 (`phase3_3_ur_rs485`). El objetivo de esta fase es conectar el gripper al controlador de un robot UR (Universal Robots) y validar el bus RS-485 y el protocolo de comandos desde URCap y URSim.

## Diagrama de Cableado y Conexión

Conectar la interfaz RS-485 del robot al conector **J3** del gripper:

| Pin J3 Gripper | Señal | UR Robot / Adaptador RS-485 |
|:---:|:---:|:---:|
| **Pin 7** | RS485+ (A) | RXD/TXD+ (A) |
| **Pin 8** | RS485- (B) | RXD/TXD- (B) |
| **Pin 1 & 2** | GND | GND |

> [!IMPORTANT]
> Verificar la polaridad de las líneas A/B y asegurar la referencia común de masa (GND) para evitar ruido diferencial.

## Lista de Comandos y Protocolo

La comunicación opera a **115200 baudios**, que es el estándar en Universal Robots y daemons URCap. El formato es `<device_id>:<cmd>:<value>\n` (por defecto Device ID = `1`).

| Comando | Descripción | Acción | Ejemplo | Respuesta |
|:---:|:---:|:---:|:---:|:---:|
| `PING` | Detección de bus | Probar comunicación | `1:PING:0\n` | `1:PONG:0.0000\n` |
| `EN` | Habilitar | Habilitar gripper | `1:EN:0\n` | `1:ACK_EN:1.0000\n` |
| `DIS` | Deshabilitar | Deshabilitar gripper | `1:DIS:0\n` | `1:ACK_DIS:0.0000\n` |
| `P` | Posición (0-85) | Ajustar apertura (mm) | `1:P:42.5\n` | `1:ACK_P:42.5000\n` |
| `V` | Velocidad (0-100) | Ajustar % de velocidad | `1:V:80\n` | `1:ACK_V:80.0000\n` |
| `T` | Fuerza (0-100) | Ajustar % de par/fuerza | `1:T:50\n` | `1:ACK_T:50.0000\n` |
| `?A` | Consultar posición | Leer posición actual (mm) | `1:?A:0\n` | `1:A:<posicion>\n` |
| `?S` | Consultar estado | Leer byte de estado (0x01: Enabled, 0x02: Moving, 0x04: Fault) | `1:?S:0\n` | `1:S:<status_byte>\n` |

### Comportamiento del LED y Simulación
En esta fase se valida la robustez de las tramas de comunicación de forma aislada. Por seguridad de hardware, las salidas de los transistores PWM y el chip select del DRV8316 permanecen inhibidos. La cinemática se emula por software:
- **LED de estado:** 1000 ms cuando está deshabilitado (`DIS`), 500 ms cuando está habilitado (`EN`) y 100 ms (parpadeo rápido) mientras dura la traslación hacia la posición objetivo. Al llegar al objetivo, `state.moving` se apaga y el LED vuelve a 500 ms.

## Cómo Probarlo

1. Asegurarse de seleccionar el entorno `test_ur_rs485` en `platformio.ini`.
2. Compilar y cargar el firmware al gripper:
   ```bash
   pio run -e test_ur_rs485 --target upload
   ```
3. Ejecutar URSim con el URCap desplegado (o conectar mediante adaptador USB-RS485 a 115200 baudios).
4. Probar las funciones de ping, activación (`EN`), fijación de velocidad (`V`), fijación de posición (`P`) y consulta de estado (`?S`).

---

# Phase 3.3 - UR Robot / URCap RS-485 Integration Tests [EN]

This folder contains the implementation and documentation for Phase 3.3 (`phase3_3_ur_rs485`). The goal of this phase is to connect the gripper to a Universal Robots (UR) robot controller and validate RS-485 bus communications and command protocols using URCap and URSim.

## Wiring and Connection Diagram

Connect the robot's RS-485 interface to the gripper's **J3** connector:

| Gripper J3 Pin | Signal | UR Robot / RS-485 Adapter |
|:---:|:---:|:---:|
| **Pin 7** | RS485+ (A) | RXD/TXD+ (A) |
| **Pin 8** | RS485- (B) | RXD/TXD- (B) |
| **Pin 1 & 2** | GND | GND |

> [!IMPORTANT]
> Verify correct polarity on lines A and B and ensure a common ground reference (GND) to avoid differential noise.

## Command List and Protocol

Communication is set to **115200 baud**, the standard baud rate for Universal Robots tool communication and URCap daemons. Format: `<device_id>:<cmd>:<value>\n` (default Device ID = `1`).

| Command | Description | Action | Example | Response |
|:---:|:---:|:---:|:---:|:---:|
| `PING` | Bus scan | Verify communication | `1:PING:0\n` | `1:PONG:0.0000\n` |
| `EN` | Enable | Enable gripper | `1:EN:0\n` | `1:ACK_EN:1.0000\n` |
| `DIS` | Disable | Disable gripper | `1:DIS:0\n` | `1:ACK_DIS:0.0000\n` |
| `P` | Position (0-85) | Set position (mm) | `1:P:42.5\n` | `1:ACK_P:42.5000\n` |
| `V` | Speed (0-100) | Set speed percentage | `1:V:80\n` | `1:ACK_V:80.0000\n` |
| `T` | Force (0-100) | Set torque/force percentage | `1:T:50\n` | `1:ACK_T:50.0000\n` |
| `?A` | Get Angle | Read current position (mm) | `1:?A:0\n` | `1:A:<position>\n` |
| `?S` | Get Status | Read status byte (0x01: Enabled, 0x02: Moving, 0x04: Fault) | `1:?S:0\n` | `1:S:<status_byte>\n` |

### LED Behavior and Simulation
In this phase, communications and URCap protocol handling are validated in isolation. For hardware safety, the PWM outputs and DRV8316 chip select are held disabled. Motion is simulated via software:
- **Status LED:** 1000 ms blink when disabled (`DIS`), 500 ms when enabled and idle (`EN`), and 100 ms (fast blink) while actively moving towards the target position. Once the target position is reached, `state.moving` returns to false and the blink returns to 500 ms.

## How to Test

1. Ensure the `test_ur_rs485` environment is configured in `platformio.ini`.
2. Build and upload firmware to the gripper:
   ```bash
   pio run -e test_ur_rs485 --target upload
   ```
3. Run URSim with the URCap deployed (or use a USB-RS485 adapter at 115200 baud).
4. Send commands and verify immediate ACK replies and state transitions.
