# FASE 1.2 — DRV8316 SPI Config Test

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
