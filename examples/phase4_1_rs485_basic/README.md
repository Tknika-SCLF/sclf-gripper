# 4.2 Fasea - RS-485 Komunikazio Testa (MAX3485)

Karpeta honetan 4.2 Fasearen (`fase4_2_rs485`) kode egiaztatua dago. Fase honen helburua RS-485 half-duplex bus-a probatzea da, motorra aktibatu gabe.

### Nola funtzionatzen du?

Gripperrak MAX3485 transceptorea du RS-485 bus bitartez komunikatzeko. Protokoloa ASCII formatuan dago, `\n`-z amaitzen den mezu bakoitzak formatu hau du:

```
<id>:<komandoa>:<balioa>\n
```

Adibidez: `1:V:5.0\n` → 1 ID-ko gailuari, abiadura (V) komandoa, 5.0 balioarekin.

### Nola probatu

1. **Konfigurazioa**: Ziurtatu `platformio.ini`-n `-D HSE_VALUE=16000000U` eta `-Wl,-u,_printf_float` flag-ak aktibatuta daudela (16MHz-eko kristala eta float-ak ondo erabiltzeko).
2. Konektatu USB-RS485 moldagailu bat PCra.
3. Konektatu moldagailuaren A/B kableak Gripper-aren RS-485 A/B pinetara.
4. Ordezkatu `src/main.cpp` karpeta honetako `main.cpp`-rekin.
5. Konpilatu eta igo:
   ```bash
   pio run -e test_rs485_protocol --target upload
   ```
6. Ireki serie terminal bat PCan (Putty, Docklight, etab.) RS-485 serian, 9600 baud-era.
7. Bidali `1:PING:0\n` eta egiaztatu Gripperrak `1:PONG:0\n` erantzuten duela.
8. Bidali `1:?A:0\n` eta ikusi `1:A:1.2300\n` erantzuna (float verified).

### USB-RS485 moldagailuaren konexioa

| Kablearen Kolorea | Seinalea | Gripper Pina |
|-------------------|----------|--------------|
| Laranja (D+)      | RS485 A+ | A            |
| Horia (D-)        | RS485 B- | B            |
| Beltza (GND)      | GND      | GND          |

### Pin mapa (MAX3485)

| Pina | Funtzioa | STM32 Pin |
|------|----------|-----------|
| TX   | UART TX  | PC10      |
| RX   | UART RX  | PC11      |
| DIR  | DE/RE    | PB9       |
