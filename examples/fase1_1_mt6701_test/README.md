# Fase 1.1: MT6701 Sentsorearen Testa (Standalone)

Adibide honen helburua MT6701 sentsore magnetikoa (encoder-a) SPI bidez irakurtzea da, angelu absolutua eta metatua (birak kontatuz) lortzeko. **Test honetan motorra eta driverra (DRV8316) erabat ezgaituta daude segurtasunagatik.**

## Prestaketa

1. Ziurtatu Gripper plaka elikatuta dagoela eta sentsorea konektatuta dagoela.
2. Kopiatu karpeta honetako `main.cpp` fitxategia proiektuaren `src/main.cpp` fitxategira.
3. Kargatu firmwarea mikrokontrolagailuan (`pio run --target upload`).

## Berrikuntzak (Orain)

* **Angelu Metatua (`Cumulative Angle`):** Programak orain birak zenbatzen ditu. Ardatzari buelta bat baino gehiago ematen badiozu, angelua 360º-tik gora igotzen joango da (edo negatiboa bihurtu kontrako noranzkoan).
* **Zero Posizioa:** Programa abiaraztean (edo reset sakatzean), dagoen posizioa "0" bezala hartzen da erreferentzia gisa.
* **Serie-Portua:** Datuak 500ms-ro inprimatzen dira `Serial` bidez.

## Nola Probatu

1. Ireki Serie-Monitorea (115200 baud).
2. Eskuz mugitu motorraren ardatza.
3. Pantailan hiru balore agertuko dira:
   - `>Angle_Abs_Deg`: Angelu absolutua (0-360º).
   - `>Angle_Cum_Deg`: Angelu metatua (birak kontuan hartuta).
   - `>Raw_Counts`: Sentsorearen balore gordina (0 - 16383).

---

# Fase 1.1: Test del Sensor MT6701 (Standalone) [ES]

El objetivo de este ejemplo es validar la lectura del encoder magnético MT6701 vía SPI, obteniendo tanto el ángulo absoluto como el acumulado (contando vueltas). **El motor y el driver (DRV8316) están desactivados por seguridad.**

## Cómo Funciona el Programa ahora

1.  **Inicialización a Cero**: Al arrancar el programa o presionar el botón de RESET, la posición actual del imán se define como el punto **0 grados**.
2.  **Seguimiento de Vueltas**: El programa llama a `sensor.update()` en cada ciclo para detectar cuándo el imán completa una vuelta. Esto permite calcular el **Angulo Acumulado**.
3.  **Salida de Datos**: Cada 500ms se envían por el puerto serie los siguientes datos para visualizar en el *Serial Plotter* o *Teleplot*:
    *   `Angle_Abs_Deg`: Ángulo actual dentro de una vuelta (0-360º).
    *   `Angle_Cum_Deg`: Ángulo total recorrido (ej: 720º si ha dado dos vueltas).
    *   `Raw_Counts`: Valor bruto del sensor (14 bits).

## Instrucciones de Uso

1.  Copia el `main.cpp` de esta carpeta a `src/main.cpp` del proyecto.
2.  Compila y carga el código (`pio run -t upload`).
3.  Abre el monitor serie a **115200 baudios**.
4.  Gira el eje manualmente y observa cómo `Angle_Cum_Deg` aumenta o disminuye continuamente sin saltos de 360 a 0.
