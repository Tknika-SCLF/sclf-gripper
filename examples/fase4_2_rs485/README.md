# 4.2 Fasea - RS-485 Komunikazio Testa (MAX3485)

Karpeta honetan 4.2 Fasearen (`fase4_2_rs485`) kode egiaztatua dago. Fase honen helburua RS-485 half-duplex bus-a probatzea da, motorra aktibatu gabe.

### Nola funtzionatzen du?

Gripperrak MAX3485 transceptorea du RS-485 bus bitartez komunikatzeko. Protokoloa ASCII formatuan dago, `\n`-z amaitzen den mezu bakoitzak formatu hau du:

```
<id>:<komandoa>:<balioa>\n
```

Adibidez: `1:V:5.0\n` → 1 ID-ko gailuari, abiadura (V) komandoa, 5.0 balioarekin.

### Nola probatu

1. Konektatu USB-RS485 moldagailu bat PCra.
2. Konektatu moldagailuaren A/B kableak Gripper-aren RS-485 A/B pinetara.
3. Ordezkatu `src/main.cpp` karpeta honetako `main.cpp`-rekin.
4. Konpilatu eta igo:
   ```bash
   pio run --target upload
   ```
5. Ireki serie terminal bat PCan (Putty, Docklight, etab.) RS-485 serian, 115200 baud-era.
6. Bidali `1:V:5.0\n` eta egiaztatu Gripperrak `1:V_ACK:5.000\n` erantzuten duela.
7. Bidali `1:?S\n` eta ikusi `1:STATUS:IDLE\n` erantzuna.

### Pin mapa (MAX3485)

| Pina | Funtzioa | STM32 Pin |
|------|----------|-----------|
| TX   | UART TX  | PC10      |
| RX   | UART RX  | PC11      |
| DIR  | DE/RE    | PB9       |
