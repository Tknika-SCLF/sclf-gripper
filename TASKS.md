# TASKS.md — SCLF Gripper Firmware
> Proiektuaren backlog osoa, garapen-faseka antolatuta.
> Egoera: `[ ]` egiteke · `[~]` martxan · `[ ]` amaituta · `[!]` blokeatuta

---
> 📢 **UNEKO EGOERA ETA HURRENGO URRATSAK**
> **v2.0 Prototipoa Balioztatuta (1.1, 1.2, 1.3, 1.4, 1.5, 2.1, 2.2).**
> **4. Fase osoa (RS-485 Protokoloa) AMAITUTA.** ✅
> Hurrengo urratsa **5. FASEA (Fault Manager)** [~] da.
---

---

## 0 FASEA — Ingurunea eta Scaffolding
> Helburua: proiektuak konpilatzea eta LED bat keinuka jartzea. Besterik ez.

- [x] `platformio.ini` sortu, `nucleo_g474re` target, 170 MHz, ST-Link
- [x] `src/config/pins.h` sortu, KiCad eskematikoarekin egiaztatuta
- [x] Sortu `SRS.md`, `AGENT.md`, `RULES.md`, `TASKS.md`, `MEMORY.md`
- [x] PlatformIO instalatu Antigravity-n `.vsix` bidez (cpptools + platformio-ide)
- [x] `cortex-debug` hedapena instalatu ST-Link bidez debug egiteko
- [x] `pio run` errore gabe konpilatzen duela egiaztatu
- [x] `pio run --target upload` exekutatu eta ST-Link plaka detektatzen duela baieztatu
- [x] `PIN_LED` (PC6/D4) keinuka jarri — pre-test `fase0_led_heartbeat` ✅
- [x] USB VCP serie-monitoreak funtzionatzen duela baieztatu (`Serial.begin()`) ✅

---

## 1 FASEA — Oinarrizko Gidariak (Drivers)
> Helburua: encoder-a irakurri, korrontea irakurri, RS-485 bidez hitz egin. FOC gabe oraindik.

### 1.1 MT6701 Encoder Gidaria [x]
- [x] `src/encoder/MT6701.h` sortu — API publikoa:
  - `bool begin()` — SPI hasieratzen du (PA5=CLK, PA6=SDO, PA7=MOSI dummy)
  - `float getAngleRad()` — angelua itzultzen du radianetan [0, 2π)
  - `uint16_t getRawCounts()` — balio gordina itzultzen du 14-bit [0–16383]
  - `bool isOk()` — SPI irakurketa baliozkoa dela egiaztatzen du
- [x] `src/encoder/MT6701.cpp` sortu — inplementazioa
- [x] `examples/fase1_1_mt6701_test/main.cpp` sortu — eskuzko proba
- [x] Eskuzko proba: inprimatu angelua/raw VCP bidez (ST-Link 3.3V bidez funtzionatzen du)
- [x] Egiaztatu encoder-a: LEDak abiadura aldatzen du eta USB monitoreak raw=0-16383 erakusten du
- [x] Wrap-around zuzena egiaztatu (16383 → 0 inolako jauzi bortitz gabe `getAngleRad`-en)
- [x] Bira kopuruaren neurketa inplementatu (angelu metatuak > 360º)
- [x] Abiaraztearekiko zero erlatiboaren hasieraketa inplementatu

### 1.2 DRV8316 SPI Gidaria [x]
- [x] `src/motor/DRV8316.h` sortu — API publikoa:
  - `bool begin()` — SPI (PC4/PB3/PB4/PB5) hasieratzen du eta erregistroak konfiguratzen ditu
  - `uint16_t readRegister(uint8_t addr)`
  - `void writeRegister(uint8_t addr, uint16_t value)`
  - `bool hasFault()` — STATUS1/STATUS2 irakurtzen du SPI bidez
  - `uint8_t getFaultCode()` — errore kodea itzultzen du
  - `void clearFaults()`
- [x] `src/motor/DRV8316.cpp` sortu
- [x] DRV8316-ren DEVICE_ID erregistroa irakurri eta egiaztatu abiaraztean
- [x] STATUS1 eta STATUS2 egoerak VCP bidez inprimatu
- [x] **GARRANTZITSUA:** Ez dago nFAULT pinik STM32-an. Akats guztiak SPI polling bidez detektatzen dira.

### 1.3 Lazo irekia / Open-Loop [x]
- [x] `examples/fase1_3_open_loop_v_control/main.cpp` sortu
- [x] 6-PWM konfiguratu (AH, AL, BH, BL, CH, CL)
- [x] Begizta irekian biratu (Open-Loop) — **v2.0 hardwarean egiaztatuta (24V)**

### 1.4 RS485 Ping-a / RS485 Ping [x]
- [x] `examples/fase1_4_rs485_ping/main.cpp` balioztatu
- [x] Ping-Pong proba (VCP eta Kanpoko bus-a)

### 1.5 Korronte Sentsorea (Current Sense) [x]
- [x] `src/motor/CurrentSense.h/.cpp` sortu — `InlineCurrentSense` SimpleFOC-entzat wrapper-a
  - Shunt-ak: PA0 (CURA), PA1 (CURB), PA2 (CURC)
  - DRV8316-ren anplifikadorearen irabazia zehaztu (CSAGAIN erregistroa irakurri SPI bidez)
- [x] Korrontearen offset-a zeroan kalibratu (motorra geldirik, korronterik gabe)
- [x] 3 faseetako korronteak inprimatu VCP bidez motorra geldirik eta blokeatuta dagoenean

---

## 2 FASEA — Oinarrizko FOC
> Helburua: motorrak open-loop eta gero closed-loop-ean biratzen du SimpleFOC erabiliz.

### 2.1 FOC Open-Loop [x]
- [x] Gehitu `askuric/Simple FOC @ ^2.3.3` `lib_deps`-en `platformio.ini` barruan
- [x] `src/motor/MotorController.h/.cpp` sortu — SimpleFOC-en wrapper-a
- [x] Biraketa balioztatu `examples/fase2_1_foc_open_loop/main.cpp`-n
- [x] Egiaztatu motorrak soinu handirik gabe biratzen duela (tentsio-muga doituz)

### 2.2 FOC Closed-Loop [x]
- [x] Lerrokatzea eta abiadura kontrola inplementatu `examples/fase2_2_foc_closed_loop/main.cpp`-n
- [x] Torke modua probatu: `motor.controller = MotionControlType::torque`
- [x] PID hasierako irabaziak doitu (KP, KI, KD) loop bakoitzerako

---

## 3 FASEA — RS-485 Protokoloa
> Helburua: gripper-ak robotaren bus-eko aginduak obeditzen ditu.

### 3.1 Protokolo Básico (Ping / ID / Info) [x]
- [x] Komando parser-a diseinatu eta inplementatu `src/comms/RS485.cpp`:
  ```
  <id>:<cmd>:<value>\n
  ```
- [x] Oinarrizko komandoak inplementatu:
  - [x] `PING` — Busaren konexioa egiaztatu (`PONG` erantzuna)
  - [x] `?A` — lortu angelua (rad) [Leku-markako balioa testean]
- [x] Gailu-anitzeko helbideratzea inplementatu (ID konfiguragarria)
- [x] Roundtrip proba: ordenagailutik komandoa bidali, erantzun zuzena egiaztatu ✅
- [x] `delayMicroseconds(200)` gehitu `RS485.cpp`-n "clipping" errorea saihesteko ✅

### 3.2 Kontrol Komandoak [x]
- [x] Komando aurreratuak inplementatu (T, V, P, M, EN, DIS, KP, KI, KD, ?V, ?I, ?S) ✅
- [x] Komando abreviatuak (Shorthands) gehitu (V5, P1, A, E, D) ✅
- [x] Roundtrip proba eta visual feedback hobetua (VCP Mirror) ✅
- [ ] Latentzia proba: neurtu latentzia TX→RX RS-485 bus-ean

---

## 4 FASEA — USB VCP eta Commander
> Helburua: zuzeneko doikuntza (tuning) PCtik USB bidez.

- [ ] Aktibatu `SimpleFOCDebug::enable(&Serial)` — telemetria USB VCP bidez
- [ ] Sortu `src/comms/VCP.cpp` — SimpleFOC-en Commander-a:
  - Erregistratu motorra, PID irabaziak, kontrol modua
  - Denbora errealeko telemetria isuria: angelua, abiadura, korrontea, jomuga
- [ ] Egiaztatu VCP-ak ez duela FOC loop-a blokeatzen (buffer zirkularra edo DMA erabili)
- [ ] Eskuragarri dauden Commander komandoak `MEMORY.md` barruan dokumentatu

---

## 5 FASEA — Akats Kudeatzailea (Fault Manager)
> Helburua: gripper-ak ez du inoiz bere burua suntsitzen.

- [ ] Sortu `src/faults/FaultManager.h/.cpp`:
  - `void update()` — loop bakoitzean deitu, DRV8316 STATUS-en polling SPI bidez
  - `bool hasFault()` — true itzultzen du edozein akats aktibo badago
  - `FaultCode getActiveFault()` — enum honekin: OVERCURRENT, ENCODER_ERROR, STALL, DRV_FAULT
  - `void triggerSafeState()` — `motor.disable()` deitzen du < 1 ms barruan
  - `void reportFault(FaultCode)` — kodea RS-485 eta VCP bidez bidaltzen du
- [ ] Blokeoa (stall) detektatu: abiadura < atalasea N ms baino gehiagoz korrontea > muga izanda
- [ ] Encoder erroreak detektatu: `MT6701::isOk()`-k false itzultzen duenean
- [ ] Akats proba: zirkuitulaburtu modu laburrean fase bat eta erantzuna egiaztatu
- [ ] Egiaztatu `triggerSafeState()` detekziotik < 1 ms gertatzen dela

---

## 6 FASEA — Iraunkortasuna eta Konfigurazioa (Persistence)
> Helburua: parametroek berrezarpen edo itzaltze bat irauten dute bizirik.

- [ ] Definatu `Config` egitura flash-ean (STM32G474-ko sektor espezifikoa):
  - `zero_electric_angle` (float)
  - `sensor_direction` (int)
  - `device_id` (uint8_t)
  - `pid_gains[3][3]` (float[loop][kp/ki/kd])
  - `control_mode` (uint8_t)
  - CRC32 balioztatzea
- [ ] Inplementatu `src/config/FlashConfig.h/.cpp`:
  - `bool load()` — konfigurazioa irakurri flash-etik, CRC balioztatu
  - `void save()` — konfigurazioa idatzi flash-ean
  - `void resetDefaults()` — balio lehenetsiak ezarri
- [ ] Abiaraztean: flash-eko informazioa zuzena bada, saihestu lerrokatze automatikoa
- [ ] Konfigurazioa irakurtzeko/idazteko eta fabrikako ezarpenetara itzultzeko RS-485 komandoak

---

## 7 FASEA — FOC Begi-itxia (Advanced Tuning)
> Helburua: FOC osoa encoder eta current sense erabiliz. Motorrak aginduei erantzuten die.
> **Oharra:** Fase hau pinzak muntatuta daudenean egitea gomendatzen da, torkea eta posizioa kargarekin doitzeko.

- [ ] Konektatu MT6701 encoderra SimpleFOC-era: `MagneticSensorSPI encoder(PIN_ENC_CS, 14, 0x3FFF)`
- [ ] `motor.initFOC()` exekutatu — lerrokatze elektriko automatikoa
  - `motor.zero_electric_angle` eta `motor.sensor_direction` flash-ean gorde
- [ ] Torke modua probatu: `motor.controller = MotionControlType::torque`
- [ ] Abiadura begi-itxia probatu encoderrarekin
- [ ] Posizio begi-itxia probatu encoderrarekin
- [ ] PID hasierako irabaziak doitu (KP, KI, KD) loop bakoitzerako
- [ ] Konektatu `InlineCurrentSense` eta aktibatu FOC korronte errealarekin
- [ ] Egiaztatu `motor.loopFOC()` ≥ 10 kHz ibiltzen dela trabarik gabe

---

## 8 FASEA — Testak eta Balioztatzea
> Helburua: firmwarea fidagarria eta egonkorra da.

- [ ] Proba automatizatuak (tests unitarioak) sortu `test/` karpetan PlatformIO Unity bitartez:
  - `test_pid.cpp` — PIDaren erantzun egokia setpoint baten aurrean
  - `test_rs485_parser.cpp` — parser-a balioztatu komando zuzen/okerrekin
  - `test_angle_wrap.cpp` — 16383→0 trantsizioak jauzirik sortzen ez duela egiaztatu
  - `test_fault_codes.cpp` — akats kodeak ondo kalkulatzen direla baieztatu
- [ ] Exekutatu `pio test` eta egiaztatu 0 akats daudela
- [ ] FOC denborak (loop times) neurtu eta dokumentatu `MEMORY.md`-n
- [ ] Flash eta RAM kontsumoak neurtu, mugaren %80a baino gutxiago direla baieztatu

---

## 9 FASEA — Azken Dokumentazioa
- [ ] Eguneratu `MEMORY.md` egiaztatutako azken PID parametroekin
- [ ] Eguneratu `MEMORY.md` offset elektriko eta encoder norabidearekin
- [ ] Amaierako erabiltzaileentzako `README.md` eguneratu (konexioak, konfigurazioa, komandoak)
- [ ] Kontrol-makinaren egoera diagrama marraztu Mermaid bidez
- [ ] Erabilitako SimpleFOC eta STM32duino bertsioak idatzi

---

## Backlog / Etorkizuneko Ideiak
> Datarik gabe. Ebaluatu aurreko faseak amaitzean.

- [ ] CAN bus euskarria (STM32G474-k FDCAN dauka — kanpoko hardwarea behar da)
- [ ] Mugimendu trapezoidal profila (azelerazio suabeko posizionamendua)
- [ ] PID auto-tuning
- [ ] OTA firmware eguneraketak RS-485 bitartez (custom bootloader)
- [ ] Zuzeneko web arbela PCan (dashboard)

---

## Erabaki Teknikoen Erregistroa

| Data | Erabakia | Arrazoia |
|---|---|---|
| 2026-02-25 | DRV8316-ren nFAULT STM32ra konektatu gabe | KiCad eskematikoan egiaztatuta |
| 2026-02-25 | DRV8316-ren DRVOFF beti GND-ra | Gidaria hardwarez beti gaituta |
| 2026-02-25 | Akats detekzioa SPI polling bidez ez GPIO IRQ | nFAULT pinerako ez dago erreserbarik |
| 2026-02-25 | SPI partekatua DRV8316 (PC4 CS) eta MT6701 (PA4 CS) artean | PB3/PB4/PB5 son el bus SPI1 |
| 2026-02-25 | RS-485 half-duplex PB9 erabiliz DE/RE gisa | Eskematikoan egiaztatuta |
| 2026-03-07 | Board `genericSTM32G474CE` -> `nucleo_g474re` aldaketa | `generic` ez du Arduino framework jasaten |
| 2026-03-07 | SPIClass MOSI: `PA7` erabili `NC` erabili ordez | `NC`-k blokeatzen du SPI periferikoaren abiaraztea |
| 2026-03-07 | ST-Link 3.3V-k MCUa eta Encoderra mantentzen ditu | **MT6701-a funtzionatzen du 24V gabe**. |
| 2026-03-07 | **USB CDC Konponbidea:** `SystemClock_Config` gehitu `main.cpp`-n | STM32G4 requiere activar `HSI48` para el reloj USB |
| 2026-03-07 | Solución de testeo Half-Duplex RS-485 | MAX3485 bloquea RX durante TX. Creado `simulateRx()` para bypass y test. |
| 2026-03-14 | **HARDWARE BUG IDENTIFICADO en v1.0:** Pin 23 de U1 (DRV8316C) conectado a D5 | El nSLEEP estaba apagado a 0.25V. Retirar D5 y puntear a 3.3V para activar SPI. |
| 2026-03-24 | **MIGRACIÓN A HARDWARE V2 EN ESPERA** | Pins y diagramas actualizados a V2 esperando la nueva PCB. |
| 2026-04-21 | **BIRANTOLAKETA: FOC Begi-itxia (Advanced) atzeratua** | Pinzak muntatuta egon arte itxoitea erabaki da. Comm eta safety faseak aurreratu dira. |
