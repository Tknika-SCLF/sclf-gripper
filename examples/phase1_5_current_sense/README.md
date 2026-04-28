# 1.5 Fasea: Korronte-Sentsorearen Balioztatzea

Adibide honek motorraren faseetako korronteak nola irakurtzen diren erakusten du, DRV8316 driver-ak dituen korronte-anplifikadore integratuak erabiliz.

## Ezaugarri Nagusiak
- **CSA Integratua**: DRV8316-ren barneko korronte-anplifikadoreak (Current Sense Amplifiers) erabiltzen ditu.
- **SimpleFOC Integrazioa**: `InlineCurrentSense` erabiltzen du ADC irakurketak Amperetara bihurtzeko.
- **Offset Kalibrazioa**: Abiaraztean automatikoki kalibratzen du zero-korrontearen offset-a.
- **Serie-Monitorea**: CURA, CURB eta CURC korronteak inprimatzen ditu (Amperetan) arazketarako.

## Hardware Konfigurazioa
- **MCU Pinak**: PA0 (CURA), PA1 (CURB), PA2 (CURC).
- **Gidaria (Driver)**: DRV8316 elikatuta egon behar da (VM > 8V) anplifikadoreek funtziona dezaten.
- **Irabazia (Gain)**: SPI bidez konfiguratua (0.3 V/A-ko balio lehenetsia test honetan).

## Nola Probatu
1. Konpilatu eta kargatu firmwarea ingurune espezifikoa erabiliz:
   ```bash
   pio run -e test_current_sense --target upload
   ```
2. Ireki serie-monitorea 115200 baudean.
3. Plakak lehenik offset-ak kalibratuko ditu (ziurtatu motorra ez dela mugitzen eta korronterik ez dabilela).
4. Behatu korronte irakurketak. Motorra kargarik gabe dagoenean, 0.00A-tik hurbil egon beharko lukete.
5. Motorra eskuz biratzen baduzu edo lazo irekian karga aplikatzen baduzu, balioak aldatzen direla ikusiko duzu.

---

# Fase 1.5: Validación del Sensor de Corriente [ES]

Este ejemplo muestra cómo leer las corrientes de fase del motor utilizando los amplificadores de detección de corriente integrados del DRV8316.

## Características Principales
- **CSA Integrado**: Utiliza los amplificadores de detección de corriente (CSA) internos del DRV8316.
- **Integración con SimpleFOC**: Utiliza `InlineCurrentSense` para mapear las lecturas del ADC a Amperios.
- **Calibración de Offset**: Calibra automáticamente el offset de corriente cero al arrancar.
- **Salida por Monitor Serie**: Imprime CURA, CURB y CURC en Amperios para depuración.

## Configuración de Hardware
- **Pines MCU**: PA0 (CURA), PA1 (CURB), PA2 (CURC).
- **Driver**: El DRV8316 debe estar alimentado (VM > 8V) para que los amplificadores funcionen.
- **Ganancia (Gain)**: Configurada vía SPI (por defecto 0.3 V/A en este test).

## Cómo Probarlo
1. Compila y carga el código usando el entorno específico:
   ```bash
   pio run -e test_current_sense --target upload
   ```
2. Abre el monitor serie a 115200 baudios.
3. La placa primero calibrará los offsets (asegúrate de que el motor no se mueva y no fluya corriente).
4. Observa las lecturas de corriente. Sin carga en el motor, deberían estar cerca de 0.00A.
5. Si giras el motor manualmente o aplicas una carga en lazo abierto, verás que los valores cambian.

---

# Phase 1.5: Current Sense Validation [EN]

This example demonstrates how to read motor phase currents using the integrated current-sense amplifiers of the DRV8316.

## Key Features
- **Integrated CSA**: Uses the DRV8316's internal current sense amplifiers.
- **SimpleFOC Integration**: Uses `InlineCurrentSense` to map ADC readings to Amperes.
- **Offset Calibration**: Automatically calibrates the zero-current offset at startup.
- **Serial Monitor Output**: Prints CURA, CURB, and CURC in Amps for debugging.

## Hardware Setup
- **MCU Pins**: PA0 (CURA), PA1 (CURB), PA2 (CURC).
- **Driver**: DRV8316 must be powered (VM > 8V) for the amplifiers to work.
- **Gain**: Configured via SPI (defaulting to 0.3 V/A in this test).

## How to Test
1. Compile and upload the firmware using the specific environment:
   ```bash
   pio run -e test_current_sense --target upload
   ```
2. Open the serial monitor at 115200 baud.
3. The board will first calibrate offsets (ensure motor is not moving and no current is flowing).
4. Observe the current readings. Without motor load, they should be close to 0.00A.
5. If you rotate the motor manually or apply a load in open loop, you should see the values change.
