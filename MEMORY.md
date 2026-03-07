# MEMORY.md — SCLF Gripper v1.0 Firmware
> Proiektuaren egoera iraunkorra garapen saioen artean.
> **Fitxategi hau eguneratu saio produktibo bakoitzaren amaieran.**
> IA agenteak fitxategi hau irakurri behar du saio bakoitzaren hasieran.

---

## Proiektuaren Uneko Egoera

| Eremua | Balioa |
|---|---|
| **Fase aktiboa** | FASE 1.2 / FASE 1.3 (BLOQUEADA) — Zain 24V iturria |
| **Azken eguneraketa** | 2026-03-07 |
| **Konpilazioa** | ✅ Egiaztatuta (Ondo konpilatzen du) |
| **ST-Link Flasheatzea** | ✅ Funtzionatzen du (upload OK) |
| **LED Heartbeat** | ✅ Egiaztatuta (PC6 / D4 keinuka) |
| **USB Serial CDC** | ✅ Funtzionatzen du (HSI48 clock fix) |
| **Encoder MT6701** | ✅ Egiaztatuta (3.3V-rekin ere funtzionatzen du!) |
| **Motorra probatuta** | ❌ Ez |

---

## Garapen Ingurunea

| Eremua | Balioa |
|---|---|
| IDE | Antigravity (Windsurf/VSCode fork-a) |
| PlatformIO | ✅ Instalatuta |
| cpptools | ✅ Instalatuta |
| cortex-debug | ✅ Instalatuta |
| Garatzailearen OS | Windows |
| ST-Link bertsioa | ST-Link V2 (klona) ✅ Funtzionatzen du |
| Board (platformio.ini) | `nucleo_g474re` |

### `.vsix` deskarga estekak
- cpptools: https://github.com/microsoft/vscode-cpptools/releases
- platformio-ide: https://github.com/platformio/platformio-vscode-ide/releases

---

## Egiaztatutako Hardwarea

### Eskematikoa
- **Fitxategia:** `SCLF_Gripper_v1_0.kicad_sch` ✅ Analizatuta
- **PCB:** `SCLF_Gripper_v1_0.kicad_pcb` ✅ Eskuragarri
- **BOM:** `SCLF_Gripper_v1_0_bom.csv` ✅ Analizatuta

### Eskematikoko aurkikuntza kritikoak
1. DRV8316-ren `nFAULT` **EZ DAGO KONEKTATUTA** STM32-ra — soilik tokiko pull-up
2. DRV8316-ren `DRVOFF` **GND**-ra konektatuta — driverra beti gaituta
3. MAX3485-en `DE/RE` = **PB9** (`RS485_DIR`)
4. SPI1 partekatua: DRV8316 (CS=PC4) + MT6701 (CS=PA4) bus berean PB3/PB4/PB5
5. PB6, PB7, PA15 izendatutako net gisa agertzen dira baina **funtziorik gabe** — erreserbatuta

---

## Baieztatutako Pin Map-a

| STM32 Pin | Net | Funtzioa |
|---|---|---|
| PA0 | CURA | Phase A current sense ADC |
| PA1 | CURB | Phase B current sense ADC |
| PA2 | CURC | Phase C current sense ADC (test pad) |
| PA4 | ENC_CS | MT6701 encoder SPI CS |
| PA5 | ENC_CLK | MT6701 encoder SPI CLK |
| PA6 | ENC_SDO | MT6701 encoder SPI MISO |
| PA8 | CH | Phase C High PWM |
| PA9 | BH | Phase B High PWM |
| PA10 | AH | Phase A High PWM |
| PA11 | D- | USB D- |
| PA12 | D+ | USB D+ |
| PA13 | SWDIO | SWD programming |
| PA14 | SWCLK | SWD programming |
| PB3 | DRV_CLK | DRV8316 SPI CLK (SPI1) |
| PB4 | DRV_MISO | DRV8316 SPI MISO (SPI1) |
| PB5 | DRV_MOSI | DRV8316 SPI MOSI (SPI1) |
| PB8 | BOOT0 | Boot button (DFU mode) |
| PB9 | RS485_DIR | RS-485 DE/RE direction |
| PB11 | VSENS | DC bus voltage sense |
| PB13 | AL | Phase A Low PWM |
| PB14 | BL | Phase B Low PWM |
| PB15 | CL | Phase C Low PWM |
| PC4 | DRV_CS | DRV8316 SPI CS |
| PC6 | LED_PC6 | Status LED |
| PC10 | RS485_TX | RS-485 UART TX |
| PC11 | RS485_RX | RS-485 UART RX |
| PF0 | XIN | Crystal 16 MHz OSC_IN |
| PF1 | XOUT | Crystal 16 MHz OSC_OUT |
| PG10 | RST | Reset button / SWD RST |

---

## Motorraren Parametroak

> ⚠️ Bete erabiliko den motor zehatza ezagutzen denean.

| Parametroa | Balioa | Egoera |
|---|---|---|
| Fabrikatzailea/Modeloa | ? | ⏳ Zain |
| Mota | BLDC / PMSM | — |
| Polo pare kopurua | ? | ⏳ Zain |
| Fase-erresistentzia (Ω) | ? | ⏳ Zain |
| Fase-induktantzia (mH) | ? | ⏳ Zain |
| Korronte izendatua (A) | ? | ⏳ Zain |
| Korronte-gailurra (A) | ? | ⏳ Zain |
| Abiadura maximoa (rpm) | ? | ⏳ Zain |

---

## Kalibratutako FOC Parametroak

> Bete `motor.initFOC()` lehen aldiz exekutatu ondoren.

| Parametroa | Balioa | Data |
|---|---|---|
| `zero_electric_angle` | ? | — |
| `sensor_direction` | ? | — |
| PWM frequency (kHz) | ? | — |
| FOC loop time (µs) | ? | — |

---

## PID Irabazi Aktiboak

> Balio kontserbadoreekin hasi. Hemen eguneratu aldatzen diren bakoitzean.

### Torque / Current loop
| Irabazia | Balioa | Oharrak |
|---|---|---|
| KP | 0.5 | Hasierakoa (kontserbadorea) |
| KI | 10.0 | Hasierakoa (kontserbadorea) |
| KD | 0.0 | — |

### Velocity loop
| Irabazia | Balioa | Oharrak |
|---|---|---|
| KP | 0.1 | Hasierakoa (kontserbadorea) |
| KI | 1.0 | Hasierakoa (kontserbadorea) |
| KD | 0.0 | — |

### Position loop
| Irabazia | Balioa | Oharrak |
|---|---|---|
| KP | 5.0 | Hasierakoa (kontserbadorea) |
| KI | 0.0 | — |
| KD | 0.0 | — |

---

## Segurtasun Muga Aktiboak

| Parametroa | Uneko balioa | Baimendutako maximoa |
|---|---|---|
| `motor.voltage_limit` | 2.0 V | — (motor errealarekin doitu) |
| `motor.current_limit` | 1.0 A | — (motor errealarekin doitu) |
| `motor.velocity_limit` | 10.0 rad/s | — (motor errealarekin doitu) |

---

## Egiaztatutako Liburutegi Bertsioak

| Liburutegia | Bertsioa | Egoera |
|---|---|---|
| SimpleFOC | ^2.3.3 | ✅ Probatuta (Konpilazioa OK) |
| STM32duino (platform) | latest | ✅ Probatuta (Konpilazioa OK) |
| cpptools | 1.23.6 | ✅ Instalatuta |
| platformio-ide | 3.3.4 | ✅ Instalatuta |

---

## Garapen Saioak

### 2026-02-25 — Hasierako saioa
- ✅ KiCad eskematiko osoa analizatuta
- ✅ STM32 pin guztiak egiaztatuta eskematikoarekin
- ✅ Sortuak: `SRS.md`, `AGENT.md`, `TASKS.md`, `RULES.md`, `MEMORY.md`, `pins.h`, `platformio.ini`
- ⏳ Zain: PlatformIO instalatzea Antigravityn eta lehen konpilazioa egiaztatzea

### 2026-02-26 — Euskera Garapena eta Konpilazioa Egiaztatzea
- ✅ Proiektuaren dokumentazioa Euskerara itzulia (garapena Euskeraz jarraitzeko).
- ✅ README-ak hiru hizkuntzatan egituratuta (EN, EU, ES).
- ✅ Firmwarea ondo konpilatzen dela egiaztatuta PlatformIO-ren bidez azken mendetasunekin.

### 2026-03-07 — STM32 Flasheatzea eta Debug Saioa
- ✅ ST-Link V2 klona erabili y upload OpenOCD funcionan OK
- ✅ LED Heartbeat testa y USB CDC Serial (HSI48 flag) OK
- ✅ MT6701 SPI kodea kargatua (`fase1_1_mt6701_test`). 💡 **Aurkikuntza:** 3.3V-tik dabil, ez du 24V behar!
- ✅ SPIClass MOSI fix-a: `NC` ordez `PA7`. Board aldaketa: `nucleo_g474re`.
- ✅ RS-485 Driver inplementazioa (`fase1_4_rs485_ping`). MAX3485 Half-Duplex funtzionamendua baliozkotua SIMULATZAILE bidez (RX injectatuz USB bidez).
- ⚠️ **BLOKEATUTA:** Fase 1.2 (SPI driver) eta Fase 1.3/2 (Motor testa) 24V elikadura eta *pogo pins* iritsi arte ezin dira probatu.

### YYYY-MM-DD — [bete beharrekoa]
- ...

---

## Oharrak eta Erabakiak (Zain)

- [ ] Baieztatu gripperrarekin erabiliko den BLDC motorraren eredu zehatza
- [ ] Baieztatu PB6/PB7/PA15 funtziorik daukaten azken diseinuan ala erreserbak diren
- [ ] Erabaki gailuaren RS-485 IDa hardware bidez (DIP switch?) ala software komando bidez konfiguratzen den
- [ ] Erabaki DRV8316 STATUS-aren polling maiztasuna (loop bakoitzeko? 10 ms-ro?)
