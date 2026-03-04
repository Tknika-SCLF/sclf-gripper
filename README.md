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

**2a.** Joan hona: https://github.com/microsoft/vscode-cpptools/releases/latest

Deskargatu zure sistema eragileari dagokion fitxategia:

| Sistema | Deskargatu beharreko fitxategia |
|---|---|
| Windows (64-bit) | `cpptools-windows-x64.vsix` |
| macOS (Apple Silicon) | `cpptools-macOS-arm64.vsix` |
| macOS (Intel) | `cpptools-macOS-x64.vsix` |
| Linux (64-bit) | `cpptools-linux-x64.vsix` |

**2b.** Antigravity-n: `Hedapenak / Extensions (Ctrl+Shift+X)` → `···` ikonoa (hiru puntu) → **Install from VSIX** → hautatu deskargatutako `.vsix` fitxategia.

---

### 3. Urratsa — PlatformIO Instalatu

**3a.** Joan hona: https://github.com/platformio/platformio-vscode-ide/releases/latest

Ireki azken bertsioaren **"Assets"** atala eta deskargatu fitxategia (adibidez: `platformio-ide-3.3.3.vsix`).

**3b.** Antigravity-n: `Hedapenak / Extensions (Ctrl+Shift+X)` → `···` → **Install from VSIX** → hautatu deskargatutako `.vsix` fitxategia.

**3c.** Berrabiarazi Antigravity. PlatformIOk bere mendekotasunak automatikoki instalatuko ditu lehenengo aldian (2-5 minutu har ditzake).

> ⚠️ **Ohar garrantzitsua:** PlatformIOren instalazioa Antigravity-n ez dago ofizialki onartuta. Konpilazio edo igoera (upload) arazoak badituzu, kontsultatu [Arazoen Konponbidea](#arazoen-konponbidea) atala dokumentu honen amaieran.

---

### 4. Urratsa — cortex-debug Instalatu (ST-Link-ekin debug egiteko)

> 💡 **Zer da Open VSX?** Kode irekiko luzapenen erregistro alternatiboa da (Eclipse Foundation-ek kudeatua). Antigravity-k eta VSCodium-ek ofizialki erabiltzen duten biltegia da, Microsoft-en murrizketak ekiditeko, eta segurua da.

**4a.** Joan hona: https://open-vsx.org/extension/marus25/cortex-debug

Eskuin aldean dagoen **"Version"** goitibeherako menuan, hautatu **"Pre-release"** jartzen EZ duen azken bertsio egonkorra (adibidez: `1.12.1`). Ondoren, sakatu azpian agertzen den **`DOWNLOAD`** botoi morean `.vsix` fitxategia eskuratzeko.

**4b.** Instalatu aurrekoak bezala (Install from VSIX).

**4c.** OpenOCD konfiguratzea:
> 💡 **Albiste ona:** Ez duzu ezer deskargatu edo instalatu behar! PlatformIO-k automatikoki deskargatzen du OpenOCD lehenengo aldiz proiektua konpilatzen edo plakan kargatzen duzunean. Bere kokapena hau izaten da Windows-en:
`C:\Users\ZURE_ERABILTZAILEA\.platformio\packages\tool-openocd\bin\openocd.exe`

*(Cortex-Debug-ek errorea ematen badu OpenOCD ez duela aurkitzen esanez, joan Antigravity-ren ezarpenetara eta ezarri ibilbide hori `cortex-debug.openocdPath` aukeran).*

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

### Arkitektura Modularraren Azalpena

Proiektu hau berezibila eta antolatua izateko garatu da:
* **`main.cpp` ("Zuzendaria"):** FOC kontrol-begizta eta hasierako konfigurazioa soilik ditu. Ez da kode nahaspilatuz betetzen.
* **`.cpp` espezifikoak ("Musikariak"):** Txip bakoitzak bere artxibo dedikatua dauka bere funtzio jakingarriagoetarako (adibidez `MT6701.cpp` posizioa zuzenean irakurtzeko erroreak kontrolatuz, edo `DRV8316.cpp` tenperatura edo zirkuitulabur kasuez informatzeko).
* **Modu honetan:** `main.cpp`-ak oinarrizko aginduak ematen ditu, eta modulu bakoitzak badaki bere tresna zehazki nola erabili, etorkizuneko hardware aldaketak izugarri erraztuz.

### Hardware Segurtasuna eta Garapena (⚠️ OSO GARRANTZITSUA)

Antigravity barruan proiektu hau garatzean, [Arau Zorrotzak (RULES.md)](RULES.md) ezarrita daude:
1. **Debekatuta dago Blokeatzea:** Inoiz ez erabili `delay()`, `HAL_Delay` edo `while()` begizta infiniturik FOC exekuzioan. SimpleFOC-ek mikroren %100 erabiltzea eskatzen du korrontea denbora errealean kalkulatzeko. Mikroa "lotaratzeak" motorra edo driverra segundo batean erretzea ekar dezake.
2. **`foc-hardware-setup` Tresna (Skill):** Kodea garatzen hasi aurretik, gure IA laguntzaileak badu tresna espezifiko bat KiCad-eko eskema elektrikoak zuzenean irakurtzeko (`.kicad_sch`) eta `pins.h` fitxategi ezin hobea sortzeko, inolako giza-akatsik gabe. Zure laguna da, erabili proiektuaren azken pinen zehaztapenak egiaztatzeko!
3. **Cortex-Debug Magia:** ST-Link v2 klonarekin edo jatorrizkoarekin, Plaka zuzenean kontrolatu eta pausatu daiteke. Programan aldagairen batek zergatik hartzen duen balio jakin bat deskubritu nahi bada, jarri Breakpoint (eten-puntu) bat Antigravity-n, eta kodea zuzen-zuzenean pausatuko da STM32-an!

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

> 💡 **Oharra ST-Link gabe DFU erabiltzearen inguruan (PA9 pina):**
> Ohiko STM32etan (F1/F4), USB kablea konektatuta dagoela nabaritzeko (VBUS sensing) `PA9` pina erabiltzen da. Gure diseinuan, ordea, `PA9` pina motorraren B fasera (DRV8316) konektatuta dago (`PIN_BH`). Baina ez larritu! **STM32G4 familiak VBUS sentsorea hardware bidez desgaituta dauka** bere Bootloader nagusian (AN2606). Hori dela eta, DFU moduak primeran funtzionatuko du soilik `PA11` eta `PA12` datu-pinak erabiliz, hardware aldaketen beharrik gabe.
> 
> **⚠️ KONTUZ Arazketa (Debugging) egiteko orduan:**
> Kontuan izan USB DFU metodoa "itsu-itsuan pentsatua" dela: kodearen irudia (`.bin`) mikroaren memorian kargatzeko **soilik** balio du. Garapen-fasean zaudenean eta SimpleFOC motorraren ezusteko portaera bat ikertu behar duzunean, **ezinbestekoa da ST-Link (edo SWD) bat erabiltzea**. ST-Link batek zuzenean ARM Cortex muinera konektatzen zaitu `PA13/PA14` bidez, eta aukera ematen dizu kodea pausatzeko (Breakpoints), aldagaien balioak denbora errealean irakurtzeko eta exekuzioa lerroz lerro aztertzeko VS Code-n. USB-ak ezin du horrelakorik egin.

---

## Arazoen Konponbidea

### PlatformIOk ez du konpilatzen / toolchain errorea
```bash
# STM32 plataforma berrinstalatu
pio platform install ststm32

# Garbitu eta berriro konpilatu
pio run --target clean && pio run
```

### Errorea: Unknown board ID `genericSTM32G474CE`
Errore hau konpilatzerakoan gertatzen da, PlatformIO-k ez duelako `genericSTM32G474CE` plaka ezagutzen Arduino framework-arekin konbinatuta.
Konpontzeko, `platformio.ini` fitxategia aldatu behar da:
1. Aldatu plaka hau jartzeko: `board = nucleo_g474re`. Nucleo plaka honek azpian mikrokontrolagailu bera du eta Arduino framework-a onartzen du.
2. Iruzkindu edo ezabatu `debug_server_arguments` lerroa, zaharkituta dagoelako eta abisuak botatzen dituelako.

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

### PlatformIOk "ms-vscode.cpptools" eskatzen du etengabe (Menpekotasunen Kirurgia)
PlatformIOk batzuetan Microsoft-en hedapena instalatzea exijitzen du, nahiz eta eskuz instalatuta izan. Arazoa konpontzeko:
1. Itxi Antigravity erabat.
2. Ireki fitxategi-esploratzailea eta joan hona: `%USERPROFILE%\.antigravity\extensions\` (Windows) edo `~/.antigravity/extensions/` (Linux/macOS).
3. Sartu `platformio.platformio-ide-...` izeneko karpetan.
4. Ireki `package.json` fitxategia testu-editore batekin.
5. Bilatu `"extensionDependencies": ["ms-vscode.cpptools"]` lerroa.
6. Utzi hutsik horrela: `"extensionDependencies": []`
7. Gorde fitxategia eta berrabiarazi Antigravity.

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
