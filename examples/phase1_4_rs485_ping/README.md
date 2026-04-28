# 1.4 Fasea: RS485 Ping-Pong Testa

Adibide honek MAX3485 transzeptorea erabiliz oinarrizko RS485 komunikazioa erakusten du. Ping-Pong logika sinple bat inplementatzen du, non gailuak "PING" komandoari "PONG" mezuarekin erantzuten dion.

## Ezaugarri Nagusiak
- **Protokolo Sinplifikatua**: Hitz bakarreko komandoak onartzen ditu (bi punturik gabe). Bidali `PING` mezua `1:PING:0` bidali ordez.
- **VCP Simulazioa**: Logika zuzenean USB Virtual COM Port (Serie Monitorea) bidez probatzeko aukera ematen du, RS485-USB moldagailurik behar izan gabe.
- **Ikusizko Feedback-a**: LEDa (PC6) 1Hz-ra dir-dir egiten du martxan dagoen bitartean.

## Nola Probatu

### A Aukera: USB soilik erabiliz (Virtual COM Port)
1. Konpilatu eta kargatu firmwarea ingurune espezifikoa erabiliz:
   ```bash
   pio run -e test_rs485_ping --target upload
   ```
2. Ireki serie-monitorea 115200 baudean.
3. Idatzi `PING` eta sakatu Enter.
4. Hau ikusi beharko zenuke:
   ```text
   [RS485 Rx] CMD: PING
     -> Received PING! Sending PONG...
   [VCP] Reply sent to RS485 bus: 1:PONG:0
   ```

### B Aukera: RS485-USB moldagailua erabiliz (Bus fisikoa)
1. Konektatu moldagailuaren A/B lineak Gripper-era.
2. Bidali `1:PING:0\n` edo `PING\n` katea PCtik RS485 moldagailuaren bidez.
3. Gripper-ak `1:PONG:0\n` erantzun beharko luke.

---

# Fase 1.4: Test de Ping-Pong RS485 [ES]

Este ejemplo muestra la comunicación básica RS485 utilizando el transceptor MAX3485. Implementa una lógica simple de Ping-Pong donde el dispositivo responde a un comando "PING" con "PONG".

## Características Principales
- **Protocolo Simplificado**: Soporta comandos de una sola palabra (sin necesidad de dos puntos). Envía `PING` en lugar de `1:PING:0`.
- **Simulación VCP**: Permite probar la lógica directamente a través del puerto USB Virtual COM (Monitor Serie) sin necesidad de un adaptador RS485-USB.
- **Feedback Visual**: El LED (PC6) parpadea a 1Hz durante el funcionamiento.

## Cómo Probarlo

### Opción A: Usando solo USB (Virtual COM Port)
1. Compila y carga el firmware usando el entorno específico:
   ```bash
   pio run -e test_rs485_ping --target upload
   ```
2. Abre el monitor serie a 115200 baudios.
3. Escribe `PING` y pulsa Enter.
4. Deberías ver:
   ```text
   [RS485 Rx] CMD: PING
     -> Received PING! Sending PONG...
   [VCP] Reply sent to RS485 bus: 1:PONG:0
   ```

### Opción B: Usando adaptador RS485-USB (Bus físico)
1. Conecta las líneas A/B del adaptador al Gripper.
2. Envía la cadena `1:PING:0\n` o simplemente `PING\n` desde el PC a través del adaptador RS485.
3. El Gripper debería responder con `1:PONG:0\n`.

---

# Phase 1.4: RS485 Ping-Pong Test [EN]

This example demonstrates basic RS485 communication using the MAX3485 transceiver. It implements a simple Ping-Pong logic where the device responds to a "PING" command with "PONG".

## Key Features
- **Simplified Protocol**: Supports single-word commands (no colons required). Send `PING` instead of `1:PING:0`.
- **VCP Simulation**: Allows testing the logic directly via the USB Virtual COM Port (Serial Monitor) without needing an RS485-USB adapter.
- **Visual Feedback**: LED (PC6) heartbeats at 1Hz during operation.

## How to Test

### Option A: Using only USB (Virtual COM Port)
1. Compile and upload the firmware using the specific environment:
   ```bash
   pio run -e test_rs485_ping --target upload
   ```
2. Open the serial monitor at 115200 baud.
3. Type `PING` and press Enter.
4. You should see:
   ```text
   [RS485 Rx] CMD: PING
     -> Received PING! Sending PONG...
   [VCP] Reply sent to RS485 bus: 1:PONG:0
   ```

### Option B: Using RS485-USB adapter (Physical Bus)
1. Connect the A/B lines of the adapter to the Gripper.
2. Send the string `1:PING:0\n` or just `PING\n` from the PC through the RS485 adapter.
3. The Gripper should respond with `1:PONG:0\n`.
