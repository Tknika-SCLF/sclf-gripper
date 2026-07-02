# 3.3 Fasea - UR Robot / URCap RS-485 Integrazio Probak

Karpeta honetan 3.3 Fasearen (`phase3_3_ur_rs485`) inplementazioa eta dokumentazioa dago. Fase honen helburua gripperra UR (Universal Robots) robot baten kontrolagailura konektatzea eta RS-485 bus bidezko komunikazioa eta kontrola balioztatzea da.

## Kableatu eta Konexio Diagrama

Konektatu robotaren RS-485 interfazea gripperraren **J3** konektorera honela:

| Gripper J3 Pin | Seinalea | UR Robot / RS-485 Egokitzailea |
|:---:|:---:|:---:|
| **Pin 7** | RS485+ (A) | RXD/TXD+ (A) |
| **Pin 8** | RS485- (B) | RXD/TXD- (B) |
| **Pin 1 & 2** | GND | GND |

> [!IMPORTANT]
> Seinaleak zuzen doazela ziurtatu eta kable biek lur berbera (GND) erabili behar dute zarata diferentziala saihesteko.

## Komandoen Zerrenda eta Protokoloa

Komunikazioak **115200 baudios**-eko abiadura lehenesten du, UR robotetako eta URCap komunikazioetan estandarra baita. formatua: `<device_id>:<cmd>:<value>\n` da (adibidez gailu ID = `1`).

| Komandoa | Deskribapena | Ekintza | Adibidea | Erantzuna |
|:---:|:---:|:---:|:---:|:---:|
| `PING` | Bus detekzioa | Komunikazioa probatu | `1:PING:0\n` | `1:PONG:0.0000\n` |
| `EN` | Habilitar | Gripperra gaitu | `1:EN:0\n` | `1:ACK_EN:1.0000\n` |
| `DIS` | Disable | Gripperra desgaitu | `1:DIS:0\n` | `1:ACK_DIS:0.0000\n` |
| `P` | Position (0-85) | Posizioa ezarri (mm-tan) | `1:P:42.5\n` | `1:ACK_P:42.5000\n` |
| `V` | Speed (0-100) | Abiadura ehunekoa ezarri | `1:V:80\n` | `1:ACK_V:80.0000\n` |
| `T` | Force (0-100) | Indar ehunekoa ezarri | `1:T:50\n` | `1:ACK_T:50.0000\n` |
| `?A` | Get Angle | Uneko posizioa lortu (mm) | `1:?A:0\n` | `1:A:<posizioa>\n` |
| `?S` | Get Status | Egoera bitak lortu (0x01: Galduta/Aktibatuta, 0x02: Mugimenduan, 0x04: Akatsa) | `1:?S:0\n` | `1:S:<egoera_bita>\n` |

## Nola Probatu

1. Ziurtatu `platformio.ini`-n `test_ur_rs485` ingurunea aukeratuta dagoela.
2. Konpilatu eta igo firmwarea gripper-era:
   ```bash
   pio run -e test_ur_rs485 --target upload
   ```
3. Zabaldu serieko terminal bat zure PCan (USB-RS485 egokitzaile batera konektatuta) **115200 baudios**-era.
4. Bidali komandoak eta ikusi erantzunak. Gripperrak mugimendua eta indarra simulatu egingo ditu modu ez-blokeatzailean.
