# Fase 6: Persistencia y Configuración

Este ejemplo valida la capacidad de guardar y recuperar parámetros de configuración en la memoria Flash del STM32G474.

## Objetivos
- Implementar el componente `FlashConfig` para gestionar la lectura/escritura en Flash.
- Guardar parámetros críticos:
    - **ID del dispositivo**: Identificador para el bus RS-485.
    - **PIDs de control**: Ganancias ajustadas en tiempo real.
    - **Límites de seguridad**: Voltaje, corriente y velocidad máxima.
    - **Calibración FOC**: `zero_electric_angle` y `sensor_direction`.
- Validar la integridad de los datos mediante un checksum (CRC32).
- Comprobar que los parámetros se cargan correctamente al reiniciar la placa.

## Uso
1. Seleccionar el entorno `test_persistence` en PlatformIO.
2. Compilar y cargar.
3. Observar el monitor serie:
   - Al arrancar mostrará `[FLASH] Config loaded successfully.` si ya había datos guardados.
4. **Guardar configuración**:
   - Ajusta los PIDs o límites mediante el Commander (ej. `MV P0.8`).
   - Envía el comando `S` para persistir los cambios en la Flash.
5. **Resetear a fábrica**:
   - Envía el comando `R` para borrar los ajustes y volver a los valores por defecto.

## Consideraciones Técnicas
- El almacenamiento se realiza en la **última página del Banco 1 (0x0803F800)** para garantizar compatibilidad con configuraciones de Single y Dual Bank del STM32G474.
- Se utiliza un valor mágico y un checksum (CRC32) para validar la integridad de los datos.
