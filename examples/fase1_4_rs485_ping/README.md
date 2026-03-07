# Fase 1.4: RS-485 Ping Test (Half-Duplex Simulado)

Este ejemplo prueba la capa de comunicaciones RS-485 del Gripper usando el chip MAX3485 (half-duplex) conectado al UART3 (PC10/PC11) y el pin de dirección `DIR` (PB9).

## Hardware Requerido
- **Solo ST-Link**: 3.3V, GND, SWDIO, SWCLK, TX, RX.
- **NO SE NECESITAN 24V**. El chip RS-485 y el microcontrolador operan completamente con la línea lógica de 3.3V.

## Uso y Testeo (Loopback Simulado)

Debido a que el chip MAX3485 es **half-duplex** (sus pines RE y DE están unidos al mismo pin de control `DIR` del STM32), el chip se queda físicamente "sordo" en su pin RX mientras está transmitiendo por su pin TX. 

Por tanto, no podemos hacer un test de "eco" escuchando lo que nosotros mismos enviamos al bus físico sin tener un segundo dispositivo RS-485 conectado.

Para probar la lógica del protocolo sin hardware adicional, este código incluye un modo **simulador RX**:
1. Conecta la placa y abre el Serial Monitor por USB (`pio device monitor --baud 115200`).
2. Escribe una trama RS-485 válida para el `ID 1`. Por ejemplo:
   ```text
   1:T:hola
   ```
3. El programa inyectará esa cadena directamente a la memoria del procesador (`simulateRx()`) simulando que entró por el bus RS-485.
4. El parser detectará la trama válida, parpadeará el LED de la placa, y **responderá** físicamente a través de la red RS-485 (`rs485.send()`) con un mensaje como `1:ACK:T`. También verás la salida de depuración en tu consola USB.

Este test confirma que los tiempos de cambio del pin `DIR`, el vaciado del búfer UART (`flush()`) y el parser de comandos de texto de la librería `RS485.cpp` operan correctamente.
