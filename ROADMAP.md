# SCLF Gripper - Firmware Garapen Plana (Roadmap)

Proiektua une honetan hardwarea (STM32G4, DRV8316, MT6701) ondo kableatuta eta PlatformIO garapen ingurunea prestatuta daukan fasean dago. Hemendik aurrera firmwarea garatzeko pausoak ematen hasiko gara.

## Proposatutako Estrategia

Garapen prozesu seguru eta sistematiko bat ziurtatzeko, firmwarearen garapena fase inkrementaletan banatu behar dugu. Horrek piezak (motor driverra, etab.) erretzea edo zati mekanikoak apurtzea ekidingo du, PI kontrolagailu txar baten edo sentsorearen lerrokatze oker baten ondorioz.

### 1 Fasea: Hardwarearen Egiaztapena (Hardware Bring-up)
*   **(EGINA) 1.1 MT6701 Sentsorearen Testa:** Sentsore magnetikoak SPI bidez angelua ondo irakurtzen duela ziurtatu (motorra piztu gabe).
*   **(EGINA) 1.2 DRV8316 SPI Konfigurazioa:** DRV8316 driverrarekin SPI bidez komunikatu 3-PWM modua konfiguratzeko eta akatsak (faults) garbitzeko.
*   **(EGINA) 1.3 Begizta Irekiko (Open-Loop) Tentsio Kontrola:** Motorra sentsorerik gabe birarazi (`velocity_openloop` edo `angle_openloop`). Horrek 3-PWM seinaleak eta driverra ondo dabiltzala frogatuko du. *Kritikoa: Oso tentsio muga baxuarekin hasi (adib. 1-2V).*

### 2 Fasea: Begizta Itxiko Kalibrazioa (Closed-Loop SimpleFOC)
*   **2.1 Sentsore-Motor Lerrokatzea:** Sentsorea motorrari lotu SimpleFOC-en eta `initFOC()` errutina exekutatu. Motorrak "dantza" txiki bat egingo du angelu elektrikoa zeroan jartzeko eta polo pareen norabidea aurkitzeko.
*   **2.2 Begizta Itxiko Indar Kontrola (Torque):** Motorra `torque` (indar) moduan jarri (tentsio bidezkoa). Tentsio positibo eta negatibo txikiak aplikatu eta motorrak logikaz eta seguru erantzuten duela egiaztatu.

### 3 Fasea: Zinematika eta Kontrolaren Doikuntza
*   **3.1 Abiadura eta Angeluaren Doikuntza (Tuning):** PI kontrolagailuak abiadurarako eta P kontrolagailua angelurako doitu (tuneatu). Motorra oszilaziorik gabe angelu zehatzetara joateko gai dela ziurtatu.
*   **(EGINA) 3.2 Pintzaren Zinematika (Gripper Kinematics):** Motorraren angelua (radianak) pintzaren hatzen distantziarekin (mm) lotu. Diseinu mekanikoaren arabera, muga fisikoak zehaztu (`min_angle`, `max_angle`).

### 4 Fasea: Goi-Mailako Logika eta Komunikazioa
*   **4.1 Indar Sentsatzea (Sentsorerik gabe):** Pintzak objektu bati heltzean (istripua / stall) harrapatu duela detektatzeko modu bat inplementatu. Hori motorraren `motor.current_sp` (edo tentsio/korronte aginduak) irakurriz eta pikoak detektatuz egin daiteke.
*   **(EGINA) 4.2 RS485 / Aginduak:** `RS485.cpp` modulua gaitu edo Serial Commander erabili robot kontrolagailu nagusitik aginduak jasotzeko, esaterako `GRIP(force)`, `RELEASE()`, edo `MOVE(position)`.
