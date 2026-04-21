# Phase 1.4: RS485 Ping-Pong Test

This example demonstrates basic RS485 communication using the MAX3485 transceiver. It implements a simple Ping-Pong logic where the device responds to a "PING" command with "PONG".

## Key Features
- **Simplified Protocol**: Supports single-word commands (no colons required). Send `PING` instead of `1:PING:0`.
- **VCP Simulation**: Allows testing the logic directly via the USB Virtual COM Port (Serial Monitor) without needing an RS485-USB adapter.
- **Visual Feedback**: LED (PC6) heartbeats at 1Hz during operation.

## How to Test

### 16. Option A: Using only USB (Virtual COM Port)
1. Flash the firmware using the `test_rs485_ping` environment.
2. Open the serial monitor at 115200 baud.
3. Type `PING` and press Enter.
4. You should see:
   ```text
   [RS485 Rx] CMD: PING
     -> Received PING! Sending PONG...
   [VCP] Reply sent to RS485 bus: 1:PONG:0
   ```

### 17. Option B: Using RS485-USB adapter (Physical Bus)
1. Connect the A/B lines of the adapter to the Gripper.
2. Send the string `1:PING:0\n` or just `PING\n` from the PC through the RS485 adapter.
3. The Gripper should respond with `1:PONG:0\n`.
