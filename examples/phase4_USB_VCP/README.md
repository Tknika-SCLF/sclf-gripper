# Fase 4: USB VCP y Commander

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
2. **Cargar**: Haz clic en "Upload" (flecha derecha) para flashear la placa.
3. **Monitor Serie**: 
   - Abre el monitor serie (`pio device monitor`).
   - Asegúrate de que el puerto sea `COM6` (o el que corresponda a tu USB CDC).
   - Configura el monitor para enviar `LF` (Line Feed) como fin de línea.
4. **Pruebas sugeridas**:
   - Envía `?` para ver que el sistema responde.
   - Envía `T1` para activar la telemetría y ver el movimiento en tiempo real.
   - Envía `M1.5` para que el motor empiece a girar a 1.5 rad/s.
   - Envía `MV P0.2` para ajustar la ganancia P de velocidad en caliente.

## Solución de Problemas
- **No hay respuesta**: Asegúrate de que el cable USB está conectado al puerto USB-C de la placa, no solo al ST-Link.
- **Warn: \r detected**: Es normal si tu terminal envía `CR+LF`. El sistema procesará el comando correctamente a pesar del aviso.
