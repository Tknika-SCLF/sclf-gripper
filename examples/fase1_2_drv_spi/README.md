# Fase 1.2: DRV8316 SPI Testa (Standalone)

Adibide honen helburua DRV8316 motor driverraren kontrol-zirkuitueriarekin SPI bidez ondo komunikatzen garela ziurtatzea da (STATUS eta CONTROL errejistroak irakurriz). **Test honetan PWM seinaleak GND-ra (LOW) lotuta daude segurtasunagatik, beraz motorrak ez luke biratu behar.**

## Prestaketa

1. Ziurtatu Gripper plaka elikatuta dagoela. **GARRANTZITSUA:** DRV8316 txipak bere SPI logikarako barne-erreguladore bat dauka, eta erreguladore hori soilik pizten da plakari tentsio nagusia (VM > 8V, adib. 24V) sartzen zaionean. USB kable hutsekin konektatzean DRV ez da esnatuko eta SPI-ak huts egingo du (0xFFFF).
2. Kopiatu karpeta honetako `main.cpp` fitxategia proiektuaren `src/main.cpp` fitxategira.
3. Kargatu firmwarea mikrokontrolagailuan (`pio run --target upload` edo DFU erabiliz).

## Nola Probatu

1. Ireki PlatformIOren Serie-Monitorea (`pio device monitor`, 115200 baud).
2. Pantailak honako hau erakutsi beharko luke:
   ```text
   ========================================
     SCLF Gripper v1.0 — Fase 1.2
     Iniciando Test DRV8316 (Driver SPI)
   ========================================
     [DRV8316] Iniciando SPI... OK.
       -> Comunicacion SPI exitosa.
   ========================================
   ```
3. Begizta nagusian, segundoero DRV8316-aren STATUS1 eta STATUS2 errejistroen balore hexadezimala irakurriko duzu. Akatsik ez badago, `0x0` izan ohi dira.
4. Edozein kable kentzen baduzu edo elikadura mozten baduzu, "FALLO ACTIVO DETECTADO" mezua agertuko da, errejistroek `0xFFFF` emango dutelako.
