# SCLF Gripper v1.0 — Firmwarea

[ Euskara ](README.md) | [ English ](README.en.md) | [ Español ](README.es.md)

> Tknikaren SCLF Gripper v1.0-rako motor kontrolagailuaren firmwarea.
> STM32G474CEU6 · SimpleFOC · PlatformIO · Antigravity IDE
> Lizentzia: CC BY-SA 4.0

---

## Hasiera Azkarra

Ingurunea konfiguratuta baduzu dagoeneko:

```bash
# 1. Konpilatu
pio run

# 2. Flasheatu (ST-Link konektatuta)
pio run --target upload

# 3. Serie-monitorea (Gripper-aren USB-C PC-ra)
pio device monitor --baud 115200
```

---

## Ingurunearen Konfigurazioa Hutsetik

### 1. Urratsa — Antigravity Instalatu

Deskargatu eta instalatu Antigravity bere webgune ofizialetik. VSCode/Windsurf-en fork bat da eta modu berean funtzionatzen du.

---

### 2. Urratsa — C/C++ Tools Instalatu

> ⚠️ PlatformIO **ez dago Open VSX-en** (Antigravity-ren marketplace-a). Bi hedapenak eskuz instalatu behar dira `.vsix` fitxategietatik.

**2a.** Joan hona: https://github.com/microsoft/vscode-cpptools/releases

Deskargatu zure sistema eragileari dagokion fitxategia:

| Sistema | Deskargatu beharreko fitxategia |
|---|---|
| Windows (64-bit) | `cpptools-win32-x64.vsix` |
| macOS (Apple Silicon) | `cpptools-osx-arm64.vsix` |
| macOS (Intel) | `cpptools-osx-x64.vsix` |
| Linux (64-bit) | `cpptools-linux-x64.vsix` |

**2b.** Antigravity-n: `Hedapenak / Extensions (Ctrl+Shift+X)` → `···` ikonoa (hiru puntu) → **Install from VSIX** → hautatu deskargatutako `.vsix` fitxategia.

---

### 3. Urratsa — PlatformIO Instalatu

**3a.** Joan hona: https://github.com/platformio/platformio-vscode-ide/releases

Deskargatu fitxategi berriena: `platformio-ide-X.X.X.vsix`

**3b.** Antigravity-n: `Hedapenak / Extensions (Ctrl+Shift+X)` → `···` → **Install from VSIX** → hautatu `platformio-ide-X.X.X.vsix`.

**3c.** Berrabiarazi Antigravity. PlatformIOk bere mendekotasunak automatikoki instalatuko ditu lehenengo aldian (2-5 minutu har ditzake).

> ⚠️ **Ohar garrantzitsua:** PlatformIOren instalazioa Antigravity-n ez dago ofizialki onartuta. Konpilazio edo igoera (upload) arazoak badituzu, kontsultatu [Arazoen Konponbidea](#arazoen-konponbidea) atala dokumentu honen amaieran.

---

### 4. Urratsa — cortex-debug Instalatu (ST-Link-ekin debug egiteko)

**4a.** Joan hona: https://github.com/Marus/cortex-debug/releases

Deskargatu: `cortex-debug-X.X.X.vsix`

**4b.** Instalatu aurrekoak bezala (Install from VSIX).

**4c.** Instalatu OpenOCD ere, ez baduzu. PlatformIOk ekarri ohi du:
```
~/.platformio/packages/tool-openocd/
```

---

### 5. Urratsa — Proiektua Ireki

```bash
# Klonatu edo kopiatu firmwarearen karpeta
# Ireki Antigravity erro karpetan (platformio.ini dagoen lekuan)
```

Antigravity-n: `File → Open Folder` → hautatu `SCLF_Gripper_v1_0_firmware/` karpeta.

PlatformIOk automatikoki detektatuko du `platformio.ini` eta honako hauek deskargatuko ditu:
- Platform `ststm32`
- Framework `framework-arduinoststm32`
- Toolchain `toolchain-gccarmnoneeabi`
- `SimpleFOC` liburutegia

---

### 6. Urratsa — Lehenengo Konpilazioa

Antigravity-ren beheko barran, egin klik PlatformIOren ✓ (Build) ikonoan, edo terminaletik:

```bash
pio run
```

**Espero den emaitza:**
```
[SUCCESS] Took X.XX seconds
RAM:   [=         ]  X.X% (used XXXX bytes from 131072 bytes)
Flash: [=         ]  X.X% (used XXXXX bytes from 524288 bytes)
```

Konpilatzen ez badu → ikusi [Arazoen Konponbidea](#arazoen-konponbidea).

---

### 7. Urratsa — ST-Link Konektatu eta Flasheatu

Konektatu ST-Link SCLF Gripper-aren SWD atakara:

| ST-Link pin | Gripper pad | Seinalea |
|---|---|---|
| SWDIO | DIO | PA13 |
| SWCLK | CLK | PA14 |
| RST | RST | PG10 |
| GND | GND | — |
| 3.3V | — | (plaka bereizita elikatu 24V-rekin) |

> ⚠️ **Ez elikatu plaka ST-Link-etik**. Erabili 24V-ko kanpo elikadura robota edo elikadura iturritik.

Flasheatu:
```bash
pio run --target upload
```

**Espero den emaitza:** LEDa (PC6) 500ms-ro keinu egiten hasiko da.

---

### 8. Urratsa — USB VCP Egiaztatu

1. Konektatu USB-C kable bat Gripperraren eta PCaren artean.
2. Ireki PlatformIOren serie-monitorea (🔌 ikonoa beheko barran) edo:
   ```bash
   pio device monitor --baud 115200
   ```
3. Hau ikusi beharko zenuke:
   ```
   ========================================
     SCLF Gripper v1.0 — Firmwarea 0 FASEA
   ========================================
     MCU: STM32G474CEU6 @ 170 MHz
     Egoera: SMOKE TEST (FOC gabe, motor gabe)
   ========================================
     Pinak ondo hasieratu dira.
   [0s] loops/s≈XXXXX  LED=ON
   [1s] loops/s≈XXXXX  LED=OFF
   ```

✅ **Hau ikusten baduzu, ingurunea %100 operatibo dago.** Eguneratu `MEMORY.md` eta markatu 0 FASEKO zereginak beteta bezala `TASKS.md`-n.

---

### 9. Urratsa — IA Agentea Konfiguratu

Antigravity-n barneratuta dagoen IA laguntzailearekin lan egiteko:

1. Ireki agentearen txat-panela (IA ikonoa alboko barran).
2. Saio bakoitzaren hasieran, idatzi:

```
Irakurri testuinguru-fitxategi hauek lagundu aurretik:
- AGENT.md   (hardwarea, pinak, arkitektura)
- MEMORY.md  (uneko egoera, kalibratutako parametroak)
- TASKS.md   (orain lantzen ari naizen zeregina)
- RULES.md   (sortutako kodeak errespetatu behar dituen arauak)
```

3. Gero, deskribatu zein zeregin landu nahi duzun `TASKS.md`-tik.

---

## Proiektuaren Egitura

```
SCLF_Gripper_v1_0_firmware/
│
├── src/
│   ├── main.cpp                    ← Sarrera puntua. 0 FASEA: smoke test
│   ├── config/
│   │   └── pins.h                  ← Pin GUZTIAK. Eskematikoarekin egiaztatuta.
│   ├── encoder/
│   │   ├── MT6701.h                ← Encoder-aren APIa (eskeletoa prest)
│   │   └── MT6701.cpp              ← 1.1 FASEAN inplementatu
│   ├── motor/
│   │   ├── DRV8316.h               ← Gate driver-aren APIa (eskeletoa prest)
│   │   └── DRV8316.cpp             ← 1.2 FASEAN inplementatu
│   ├── comms/
│   │   ├── RS485.h                 ← RS-485 APIa (eskeletoa prest)
│   │   └── RS485.cpp               ← 1.4 FASEAN inplementatu
│   └── faults/
│       ├── FaultManager.h          ← Akats kudeatzailearen APIa (eskeletoa prest)
│       └── FaultManager.cpp        ← 6 FASEAN inplementatu
│
├── lib/                            ← Bertako liburutegiak (oraindik hutsik)
├── test/                           ← PlatformIO Unity testak (8 FASEA)
│
├── .vscode/
│   ├── settings.json               ← IDE konfigurazioa (IntelliSense, formatua)
│   ├── extensions.json             ← Gomendatutako hedapenak
│   └── launch.json                 ← Debug ST-Link cortex-debug-ekin
│
├── platformio.ini                  ← Build eta upload konfigurazioa
├── .clang-format                   ← Kode estiloa (gordetzean aplikatzen da)
├── .gitignore
│
├── AGENT.md    ← 📋 IA agentearentzako testuingurua
├── MEMORY.md   ← 🧠 Proiektuaren egoera iraunkorra
├── TASKS.md    ← ✅ Backlog osoa faseka
├── RULES.md    ← 📏 Proiektuaren arauak
└── SRS.md      ← 📄 Eskakizunen zehaztapena
```

---

## PlatformIO Komando Erabilienak

```bash
pio run                          # Konpilatu
pio run --target upload          # Konpilatu eta flasheatu (ST-Link)
pio run --target upload_dfu      # USB DFU bidez flasheatu (BOOT + RST)
pio device monitor               # Serie-monitorea (USB VCP, 115200 baud)
pio test                         # Test unitarioak exekutatu
pio run --target clean           # Konpilazio-artefaktuak garbitu
pio lib install "Simple FOC"     # Eskuz instalatu liburutegia
pio pkg update                   # Plataforma eta liburutegiak eguneratu
```

---

## DFU Bootloader (Flasheatu ST-Link gabe)

ST-Link erabilgarririk ez baduzu, USB DFU bidez flashea dezakezu:

1. Mantendu sakatuta **BOOT** (PB8) botoia.
2. Sakatu eta askatu **RESET** (PG10) botoia.
3. Askatu **BOOT**.
4. Gripperra DFU gailu gisa agertuko da PCan.
5. Aldatu `upload_protocol = dfu` fitxategian `platformio.ini`.
6. Exekutatu `pio run --target upload`.

---

## Arazoen Konponbidea

### PlatformIOk ez du konpilatzen / toolchain errorea
```bash
# STM32 plataforma berrinstalatu
pio platform install ststm32

# Garbitu eta berriro konpilatu
pio run --target clean && pio run
```

### ST-Link-ek ez du plaka detektatzen
- Egiaztatu ST-Link-eko GNDa gripperreko GND-ra konektatuta dagoela.
- Egiaztatu plakak elikadura duela (PWR LEDa piztuta).
- Probatu `openocd -f interface/stlink.cfg -f target/stm32g4x.cfg` terminalean.
- OpenOCD-k huts egiten badu: eguneratu ST-Link firmwarea STM32CubeProgrammer-etik.

### USB VCP ez da agertzen PCan
- Instalatu STM32 VCP kontrolatzailea (driverra): https://www.st.com/en/development-tools/stsw-stm32102.html (Windows soilik)
- Linux/macOS sistemetan ez dago kontrolatzailerik behar. Egiaztatu `ls /dev/ttyACM*` erabiliz.

### IntelliSense-k ez du `Arduino.h` edo STM32 header-ak aurkitzen
- Itxaron PlatformIOk lehenengo konpilazioa amaitu arte (includes indizea sortzen du).
- Arazoak jarraitzen badu: `Ctrl+Shift+P` → "C/C++: Reset IntelliSense Database".

### Antigravity / PlatformIO ez da plakarekin komunikatzen
- Hau komunitatean dokumentatuta dagoen arazo ezaguna da. PlatformIO Antigravity-n
  ez dago ofizialki onartuta. Konpilazioak funtzionatzen badu baina igotzeak (upload) huts egiten badu,
  saiatu `pio run --target upload` exekutatzen **barne terminaletik**, PlatformIOko
  UI botoiaren ordez.

---

## Erreferentziazko Hardwarea

| Osagaia | Zatia | Datasheet-a |
|---|---|---|
| MCU | STM32G474CEU6 | [ST.com](https://www.st.com/resource/en/reference_manual/rm0440-stm32g4-series-advanced-armbased-32bit-mcus-stmicroelectronics.pdf) |
| Gate Driver | DRV8316CRRGFR | [TI.com](https://www.ti.com/lit/ds/symlink/drv8316.pdf) |
| Encoder | MT6701QT | [Magntek](https://www.magntek.com.cn/upload/MT6701_Rev.1.8.pdf) |
| RS-485 | MAX3485 | [Analog](https://www.analog.com/media/en/technical-documentation/data-sheets/MAX3483-MAX3491.pdf) |
| SimpleFOC | v2.x | [docs.simplefoc.com](https://docs.simplefoc.com) |

---

*Tknikak diseinatua — Licensed under CC BY-SA 4.0*
