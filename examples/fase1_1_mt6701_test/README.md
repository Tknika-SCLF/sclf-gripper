# Fase 1.1: MT6701 Sentsorearen Testa (Standalone)

Adibide honen helburua MT6701 sentsore magnetikoa (encoder-a) ondo kableatuta dagola eta SPI bidez angelua irakurtzen ari dela egiaztatzea da. **Test honetan motorra eta driverra (DRV8316) erabat ezgaituta daude segurtasunagatik.**

## Prestaketa

1. Ziurtatu Gripper plaka elikatuta dagoela eta sentsorea konektatuta dagoela.
2. Motorraren kableak konektatuta egon daitezke, baina ez zaie tentsiorik sartuko.
3. Kopiatu karpeta honetako `main.cpp` fitxategia proiektuaren `src/main.cpp` fitxategira.
4. Kargatu firmwarea mikrokontrolagailuan (`pio run --target upload` edo DFU erabiliz).

## Nola Probatu

1. Ireki PlatformIOren Serie-Monitorea (`pio device monitor`) edo, are hobeto, **Serial Plotter** bat (115200 baud).
2. Eskuz mugitu (biratu) Gripperraren motorra/ardatza.
3. Pantailan bi balore inprimatuko dira etengabe (50ms-ro):
   - `>Angle_Rad`: Motorraren angelua radianetan.
   - `>Raw_Counts`: MT6701 sentsorearen balore gordina (0 - 16383).

**Emaitza Egokia:** Ardatza biratzen duzun heinean, zenbakiek gora eta behera egin behar dute modu leunean, jauzi arrarorik (glitches) edo zero konstante bat eman gabe.

## Arazo Bateratuen Konponbidea (Troubleshooting)

* **`ERROR. ¿Magneto alineado? ¿SPI conectado?` mezua agertzen bada:** SPI komunikazioak huts egin du. Berrikusi hardwareko SPI pinak (PB13, PB14) edo imana sentsoretik gertuegi/urrutiegi dagoen.
* **Balorea jartzen da baina ez da mugitzen:** Baliteke sentsoreak imana ez detektatzea. Mugitu imana zentrurantz.
