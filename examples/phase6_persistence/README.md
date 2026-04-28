# 6. Fasea: Iraunkortasuna eta Konfigurazioa (Persistence)

Adibide honek STM32G474-ko Flash memorian konfigurazio parametroak gordetzeko eta berreskuratzeko gaitasuna balioztatzen du.

## Helburuak
- `FlashConfig` osagaia inplementatzea Flash-ean irakurketa/idazketa kudeatzeko.
- Parametro kritikoak gordetzea:
    - **Gailuaren IDa**: RS-485 buserako identifikatzailea.
    - **Kontrol PIDak**: Denbora errealean doitutako irabaziak.
    - **Segurtasun mugak**: Tentsioa, korrontea eta abiadura maximoa.
    - **FOC kalibrazioa**: `zero_electric_angle` eta `sensor_direction`.
- Datuen osotasuna balioztatzea checksum (CRC32) bidez.
- Plaka berrabiaraztean parametroak ondo kargatzen direla egiaztatzea.

## Erabilera
1. Hautatu `test_persistence` ingurunea PlatformIO-n.
2. Konpilatu eta kargatu firmwarea:
   ```bash
   pio run -e test_persistence --target upload
   ```
3. Behatu serie-monitorea:
   - Abiaraztean `[FLASH] Config loaded successfully.` erakutsiko du datuak gordeta bazituen.
4. **Konfigurazioa gorde**:
   - Doitu PIDak edo mugak Commander bidez (adib. `MV P0.8`).
   - Bidali `S` komandoa aldaketak Flash-ean iraunarazteko.
5. **Fabrikako berrezarpena**:
   - Bidali `R` komandoa doikuntzak ezabatzeko eta lehenetsitako balioetara itzultzeko.

## Hausnarketa Teknikoak
- Gordetzea **1. Bankuko azken orrian (0x0803F800)** egiten da, STM32G474-ko Single eta Dual Bank konfigurazioekin bateragarritasuna bermatzeko.
- Balio magiko bat eta checksum bat (CRC32) erabiltzen dira datuen osotasuna balioztatzeko.

---

# Fase 6: Persistencia y Configuración [ES]

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
2. Compilar y cargar el firmware:
   ```bash
   pio run -e test_persistence --target upload
   ```
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

---

# Phase 6: Persistence and Configuration [EN]

This example validates the ability to save and retrieve configuration parameters in the STM32G474's Flash memory.

## Objectives
- Implement the `FlashConfig` component to manage Flash read/write.
- Save critical parameters:
    - **Device ID**: Identifier for the RS-485 bus.
    - **Control PIDs**: Gains adjusted in real-time.
    - **Safety limits**: Voltage, current, and maximum speed.
    - **FOC calibration**: `zero_electric_angle` and `sensor_direction`.
- Validate data integrity using a checksum (CRC32).
- Check that parameters are correctly loaded upon board restart.

## Usage
1. Select the `test_persistence` environment in PlatformIO.
2. Compile and upload the firmware:
   ```bash
   pio run -e test_persistence --target upload
   ```
3. Observe the serial monitor:
   - Upon startup, it will show `[FLASH] Config loaded successfully.` if data was already saved.
4. **Save configuration**:
   - Adjust PIDs or limits using the Commander (e.g., `MV P0.8`).
   - Send the `S` command to persist changes in Flash.
5. **Factory reset**:
   - Send the `R` command to clear settings and return to default values.

## Technical Considerations
- Storage is performed on the **last page of Bank 1 (0x0803F800)** to ensure compatibility with Single and Dual Bank configurations of the STM32G474.
- A magic value and a checksum (CRC32) are used to validate data integrity.
