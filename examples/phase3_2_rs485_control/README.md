# 3.2 Fasea - RS-485 Kontrol Komandoak

Karpeta honetan 3.2 Fasearen (`phase3_2_rs485_control`) inplementazioa dago. Fase honen helburua oinarrizko PING/PONG-etik haratago joatea da, motorraren kontrolerako komando ASCII guztiak inplementatuz.

### Protokolo Hobetua
`RS485Frame` egitura refakturatu da `CommandType` enum-a erabiltzeko. Horrela, `main.cpp`-ko logikak ez ditu string-ak konparatu behar, integer-en bidezko `switch` bat baizik.

### Komando Zerrenda

| Komandoa | Deskribapena | Adibidea |
|:---:|:---:|:---:|
| `T` | Torque jomuga (A) | `1:T:0.5\n` |
| `V` | Abiadura jomuga (rad/s) | `1:V:10.0\n` |
| `P` | Posizio jomuga (rad) | `1:P:3.14\n` |
| `M` | Kontrol modua (0:T, 1:V, 2:P) | `1:M:1\n` |
| `EN` / `DIS` | Motorra gaitu / desgaitu | `1:EN:0\n` |
| `KP` / `KI` / `KD` | PID irabaziak | `1:KP:0.1\n` |
| `?A` | Angelua lortu | `1:?A:0\n` |
| `?V` | Abiadura lortu | `1:?V:0\n` |
| `?I` | Korrontea lortu | `1:?I:0\n` |
| `?S` | Egoera / Akatsak lortu | `1:?S:0\n` |

### Komando Abreviatuak (Monitor Serierako)
Testeo azkarragoa egiteko, gailu-id-a eta `:` delimitadoreak alde batera utzi daitezke (ID 1 hartuko da lehenetsitzat):

| Atajoa | Baliokidea | Ekintza |
|:---:|:---:|:---:|
| `V5` | `1:V:5` | Abiadura 5 rad/s-ra ipini |
| `P3.14` | `1:P:3.14` | Posizioa 3.14 rad-era ipini |
| `T0.2` | `1:T:0.2` | Torquea 0.2 A-ra ipini |
| `E` | `1:EN:0` | Motorra gaitu |
| `D` | `1:DIS:0` | Motorra desgaitu |
| `A` | `1:?A:0` | Angelua lortu |

### Nola probatu

1. Ziurtatu `platformio.ini`-n `test_rs485_control` ingurunea aukeratuta dagoela.
2. Konpilatu eta kargatu firmwarea ingurune espezifikoa erabiliz:
   ```bash
   pio run -e test_rs485_control --target upload
   ```
3. Ireki serie terminal bat PCan (RS-485 egokitzailea) 9600 baud-era.
4. Bidali komandoak eta egiaztatu `ACK` erantzuna edo datua.

---

# Fase 3.2 - Comandos de Control RS-485 [ES]

En esta carpeta se encuentra la implementación de la Fase 3.2 (`phase3_2_rs485_control`). El objetivo de esta fase es ir más allá del PING/PONG básico, implementando todos los comandos ASCII para el control del motor.

### Protocolo Mejorado
Se ha refactorizado la estructura `RS485Frame` para utilizar el enum `CommandType`. De esta forma, la lógica en `main.cpp` no necesita comparar strings, sino que utiliza un `switch` mediante números enteros.

### Lista de Comandos

| Comando | Descripción | Ejemplo |
|:---:|:---:|:---:|
| `T` | Objetivo de Torque (A) | `1:T:0.5\n` |
| `V` | Objetivo de Velocidad (rad/s) | `1:V:10.0\n` |
| `P` | Objetivo de Posición (rad) | `1:P:3.14\n` |
| `M` | Modo de control (0:T, 1:V, 2:P) | `1:M:1\n` |
| `EN` / `DIS` | Habilitar / Deshabilitar motor | `1:EN:0\n` |
| `KP` / `KI` / `KD` | Ganancias PID | `1:KP:0.1\n` |
| `?A` | Obtener ángulo | `1:?A:0\n` |
| `?V` | Obtener velocidad | `1:?V:0\n` |
| `?I` | Obtener corriente | `1:?I:0\n` |
| `?S` | Obtener estado / errores | `1:?S:0\n` |

### Comandos Abreviados (para Monitor Serie)
Para realizar pruebas rápidas, se pueden omitir el ID del dispositivo y los delimitadores `:` (se tomará el ID 1 por defecto):

| Atajo | Equivalente | Acción |
|:---:|:---:|:---:|
| `V5` | `1:V:5` | Poner velocidad a 5 rad/s |
| `P3.14` | `1:P:3.14` | Poner posición a 3.14 rad |
| `T0.2` | `1:T:0.2` | Poner torque a 0.2 A |
| `E` | `1:EN:0` | Habilitar motor |
| `D` | `1:DIS:0` | Deshabilitar motor |
| `A` | `1:?A:0` | Obtener ángulo |

### Cómo Probarlo

1. Asegúrate de que en `platformio.ini` esté seleccionado el entorno `test_rs485_control`.
2. Compila y carga el código usando el entorno específico:
   ```bash
   pio run -e test_rs485_control --target upload
   ```
3. Abre un terminal serie en el puerto correspondiente al adaptador RS-485 a 9600 baudios.
4. Envía los comandos y verifica la respuesta `ACK` o el dato solicitado.

---

# Phase 3.2 - RS-485 Control Commands [EN]

This folder contains the implementation for Phase 3.2 (`phase3_2_rs485_control`). The goal of this phase is to go beyond basic PING/PONG by implementing all ASCII commands for motor control.

### Improved Protocol
The `RS485Frame` structure has been refactored to use the `CommandType` enum. This way, the logic in `main.cpp` doesn't need to compare strings, but instead uses a `switch` with integers.

### Command List

| Command | Description | Example |
|:---:|:---:|:---:|
| `T` | Torque target (A) | `1:T:0.5\n` |
| `V` | Velocity target (rad/s) | `1:V:10.0\n` |
| `P` | Position target (rad) | `1:P:3.14\n` |
| `M` | Control mode (0:T, 1:V, 2:P) | `1:M:1\n` |
| `EN` / `DIS` | Enable / Disable motor | `1:EN:0\n` |
| `KP` / `KI` / `KD` | PID gains | `1:KP:0.1\n` |
| `?A` | Get angle | `1:?A:0\n` |
| `?V` | Get velocity | `1:?V:0\n` |
| `?I` | Get current | `1:?I:0\n` |
| `?S` | Get status / faults | `1:?S:0\n` |

### Short Commands (for Serial Monitor)
For faster testing, the device ID and `:` delimiters can be omitted (ID 1 will be used by default):

| Shortcut | Equivalent | Action |
|:---:|:---:|:---:|
| `V5` | `1:V:5` | Set velocity to 5 rad/s |
| `P3.14` | `1:P:3.14` | Set position to 3.14 rad |
| `T0.2` | `1:T:0.2` | Set torque to 0.2 A |
| `E` | `1:EN:0` | Enable motor |
| `D` | `1:DIS:0` | Disable motor |
| `A` | `1:?A:0` | Get angle |

### How to Test

1. Ensure that the `test_rs485_control` environment is selected in `platformio.ini`.
2. Compile and upload the firmware using the specific environment:
   ```bash
   pio run -e test_rs485_control --target upload
   ```
3. Open a serial terminal on the COM port corresponding to the RS-485 adapter at 9600 baud.
4. Send commands and verify the `ACK` response or the requested data.
