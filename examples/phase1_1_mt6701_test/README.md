# Fase 1.1: MT6701 Sentsorearen Testa (Standalone)

Adibide honen helburua MT6701 sentsore magnetikoa (encoder-a) SPI bidez irakurtzea da, angelu absolutua eta metatua (birak kontatuz) lortzeko. **Test honetan motorra eta driverra (DRV8316) erabat ezgaituta daude segurtasunagatik.**

## Prestaketa

1. Ziurtatu Gripper plaka elikatuta dagoela eta sentsorea konektatuta dagoela.
2. Konpilatu eta kargatu firmwarea ingurune espezifikoa erabiliz:
   ```bash
   pio run -e test_mt6701 --target upload
   ```

## Programaren funtzionamendua

* **Angelu Metatua (`Cumulative Angle`):** Programak birak zenbatzen ditu. Ardatzari buelta bat baino gehiago ematen badiozu, angelua 360º-tik gora igotzen joango da (edo negatiboa bihurtu kontrako noranzkoan).
* **Zero Posizioa:** Programa abiaraztean (edo reset sakatzean), dagoen posizioa "0" bezala hartzen da erreferentzia gisa.
* **Serie-Portua:** Datuak 500ms-ro inprimatzen dira `Serial` bidez.

## Nola Probatu

1. Ireki Serie-Monitorea (pio device monitor --baud 115200).
2. Eskuz mugitu motorraren ardatza.
3. Pantailan hiru balore agertuko dira:
   - `>Angle_Abs_Deg`: Angelu absolutua (0-360º).
   - `>Angle_Cum_Deg`: Angelu metatua (birak kontuan hartuta).
   - `>Raw_Counts`: Sentsorearen balore gordina (0 - 16383).

---

# Fase 1.1: Test del Sensor MT6701 (Standalone) [ES]

El objetivo de este ejemplo es validar la lectura del encoder magnético MT6701 vía SPI, obteniendo tanto el ángulo absoluto como el acumulado (contando vueltas). **El motor y el driver (DRV8316) están desactivados por seguridad.**

## Cómo Funciona el Programa

1.  **Inicialización a Cero**: Al arrancar el programa o presionar el botón de RESET, la posición actual del imán se define como el punto **0 grados**.
2.  **Seguimiento de Vueltas**: El programa llama a `sensor.update()` en cada ciclo para detectar cuándo el imán completa una vuelta. Esto permite calcular el **Angulo Acumulado**.
3.  **Salida de Datos**: Cada 500ms se envían por el puerto serie los siguientes datos para visualizar en el *Serial Plotter* o *Teleplot*:
    *   `Angle_Abs_Deg`: Ángulo actual dentro de una vuelta (0-360º).
    *   `Angle_Cum_Deg`: Ángulo total recorrido (ej: 720º si ha dado dos vueltas).
    *   `Raw_Counts`: Valor bruto del sensor (14 bits).

## Instrucciones de Uso

1.  Compila y carga el código usando el entorno específico:
    ```bash
    pio run -e test_mt6701 --target upload
    ```
2.  Abre el monitor serie (pio device monitor --baud 115200).
3.  Gira el eje manualmente y observa cómo `Angle_Cum_Deg` aumenta o disminuye continuamente sin saltos de 360 a 0.

---

# Phase 1.1: MT6701 Sensor Test (Standalone) [EN]

The goal of this example is to validate the reading of the MT6701 magnetic encoder via SPI, obtaining both the absolute and cumulative angle (counting turns). **The motor and driver (DRV8316) are completely disabled for safety.**

## How the Program Works

1.  **Zero Initialization**: When the program starts or the RESET button is pressed, the current magnet position is defined as the **0 degrees** point.
2.  **Turn Tracking**: The program calls `sensor.update()` in each cycle to detect when the magnet completes a turn. This allows calculating the **Cumulative Angle**.
3.  **Data Output**: Every 500ms, the following data is sent through the serial port for visualization in the *Serial Plotter* or *Teleplot*:
    *   `Angle_Abs_Deg`: Current angle within one turn (0-360º).
    *   `Angle_Cum_Deg`: Total angle traveled (e.g., 720º if it has turned twice).
    *   `Raw_Counts`: Raw sensor value (14 bits).

## Usage Instructions

1.  Compile and upload the code using the specific environment:
    ```bash
    pio run -e test_mt6701 --target upload
    ```
2.  Open the serial monitor (pio device monitor --baud 115200).
3.  Rotate the axis manually and observe how `Angle_Cum_Deg` increases or decreases continuously without 360 to 0 jumps.
