# 4.2 Fasea - RS-485 Kontrol Komandoak

Karpeta honetan 4.2 Fasearen (`phase4_2_rs485_control`) inplementazioa dago. Fase honen helburua oinarrizko PING/PONG-etik haratago joatea da, motorraren kontrolerako komando ASCII guztiak inplementatuz.

### Protokolo Hobetua
`RS485Frame` egitura refakturatu da `CommandType` enum-a erabiltzeko. Horrela, `main.cpp`-ko logikak ez ditu string-ak konparatu behar, integer-en bidezko `switch` bat baizik.

### Komando Zerrenda

| Komandoa | Deskribapena | Adibidea |
|:---:|:---:|:---:|
| `T` | Torque jomuga (A) | `1:T:0.5\n` |
| `V` | Abiadura jomuga (rad/s) | `1:V:10.0\n` |
| `P` | Posizio jomuga (rad) | `1:P:3.14\n` |
| `M` | Kontrol modua (0:T, 1:V, 2:P) | `1:M:1\n` |
| `EN` / `DIS` | Motorra gaitu / desgaitu | `1:EN:0\n` |
| `KP` / `KI` / `KD` | PID irabaziak | `1:KP:0.1\n` |
| `?A` | Angelua lortu | `1:?A:0\n` |
| `?V` | Abiadura lortu | `1:?V:0\n` |
| `?I` | Korrontea lortu | `1:?I:0\n` |
| `?S` | Egoera / Akatsak lortu | `1:?S:0\n` |

### Komando Abreviatuak (Monitor Serierako)
Testeo azkarragoa egiteko, gailu-id-a eta `:` delimitadoreak alde batera utzi daitezke (ID 1 hartuko da lehenetsitzat):

| Atajoa | Baliokidea | Ekintza |
|:---:|:---:|:---:|
| `V5` | `1:V:5` | Abiadura 5 rad/s-ra ipini |
| `P3.14` | `1:P:3.14` | Posizioa 3.14 rad-era ipini |
| `T0.2` | `1:T:0.2` | Torquea 0.2 A-ra ipini |
| `E` | `1:EN:0` | Motorra gaitu |
| `D` | `1:DIS:0` | Motorra desgaitu |
| `A` | `1:?A:0` | Angelua lortu |

### Nola probatu

1. Ziurtatu `platformio.ini`-n `test_rs485_control` ingurunea aukeratuta dagoela.
2. Konpilatu eta igo:
   ```bash
   pio run -e test_rs485_control --target upload
   ```
3. Ireki serie terminal bat COM6-n (RS-485 egokitzailea) 9600 baud-era.
4. Bidali komandoak eta egiaztatu `ACK` erantzuna edo datua.
