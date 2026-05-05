# Proiektuaren Arkitektura: Antigravity IDE + PlatformIO (SCLF Gripper)

Errepositorioa lau erantzukizun-eremu (domeinu) zurrunki isolatutan banatuta dago: Kognizioa (IA), Fisika (Hardwarea), Ingurunea (IDE/Konpiladorea) eta Exekuzioa (C++ Kodea).

```plaintext
/sclf-gripper/
├── .agent/              <- Kognizioa (IA, Arauak eta Skill-ak)
├── .vscode/             <- Editorearen konfigurazioa eta Debugger-a
├── doc/                 <- Dokumentazio teknikoa eta Arkitektura
├── examples/            <- Garapen faseak (Phase 0 - Phase 7)
├── hardware/            <- Diseinu elektronikoa (Eskematikoak, PCB, BOM)
├── images/              <- Dokumentaziorako irudiak
├── production/          <- Fabrikaziorako fitxategiak (Gerber, Pick&Place)
├── scripts/             <- Automatizazio script-ak (Python/Bash)
├── src/                 <- Iturburu Kode Modularra
│   ├── main.cpp
│   ├── motor/           <- SimpleFOC kontrola
│   ├── encoder/         <- MT6701 driver-a
│   ├── comms/           <- RS-485 eta VCP
│   ├── faults/          <- Segurtasun kudeatzailea
│   └── config/          <- Hardware konfigurazioa (pins.h)
├── .clang-format        <- Kode estiloaren arauak
├── AGENT.md             <- IArako instrukzio parametrikoak
├── MEMORY.md            <- Erabaki arkitektonikoen erregistroa
├── platformio.ini       <- Proiektuaren eraikuntza motorra (PlatformIO)
├── README.md            <- Proiektuaren sarrera orokorra
├── RULES.md             <- Garapen eta segurtasun arau kritikoak
├── SRS.md               <- Software Requirements Specification
└── TASKS.md             <- Garapen bide-orria eta egoera (Roadmap)
```

## 1. Kognizio Domeinua (IA Motorra)

Agente autonomoaren arrazoibide-mugak, segurtasun-protokoloak eta gaitasun prozeduralak definitzen ditu.

*   `.agent/rules/hardware_safety.md`: Proiektuaren lege hautsiezinak jasotzen ditu. Agenteak fitxategi hau etengabe ebaluatzen du (`always_on: true`). Kode sinkronoaren erabilera blokeatzen du (`delay`) eta hardwarearen balidazioa eskatzen du logika idatzi aurretik.
*   `.agent/skills/foc-hardware-setup/SKILL.md`: Funtzionamendu Estandarreko Prozedura (SOP) urratsez urrats definitzen du. IA-k KiCad eskematikoak analizatu behar ditu eta hardware-mapatzea modu enpirikoan atera kodea iradoki aurretik.
*   `AGENT.md`: IA-ren rol parametrikoa (FOC Firmware Ingeniari Seniorra) ezartzen duen dokumentua.
*   `MEMORY.md`: Erabaki arkitektonikoen erregistroa.
*   `RULES.md`: Garapen eta segurtasun arau kritikoak.
*   `SRS.md`: Softwarearen baldintza teknikoen zehaztapena.
*   `TASKS.md`: Proiektuaren egoera eta bide-orriaren (roadmap) jarraipena.

## 2. Domeinu Fisikoa (Hardwarea eta Erreferentzia)

Egia absolutuaren iturria. Agenteak sekzio honen gainean irakurketa analitikoko baimenak baino ez ditu.

*   `hardware/`: Diseinu elektronikoaren jatorrizko fitxategiak (KiCad eskematikoak eta PCB) eta materialen zerrenda (`bom.csv`).
*   `production/`: Fabrikaziorako fitxategiak (Gerber, Pick&Place).
*   `doc/`: Giza kontsumorako dokumentazio-biltegia. Eskuliburu teknikoak eta arkitektura gidak ditu.

## 3. Ingurunearen eta Konpilazioaren Domeinua

Kodea softwaretik mikrokontrolagailura mendekotasun-errorerik gabe igarotzeko behar den azpiegitura.

*   `.vscode/`: Editorearen interfazearen konfigurazio-fitxategiak. `launch.json` fitxategia bektore kritikoa da hardware-arazketa (debugger) bidez erregistroak denbora errealean ikuskatzea ahalbidetzeko.
*   `platformio.ini`: Proiektua eraikitzeko (build) motorra. Mikrokontrolagailuaren target zehatza, azpiko framework-a eta kanpoko mendekotasun zorrotzak definitzen ditu.

## 4. Exekuzio Domeinua (Iturburu Kodea)

Garapen-iterazioen ondoriozko azken produktua.

*   `src/`: Moduluetan antolatutako direktorioa. Erantzukizunak (motorra, enkoderrra, komunikazioak) banatzen ditu mantentze-lana eta eskalagarritasuna errazteko.
*   `examples/`: Garapen-mugarriak faseka banatuta ditu, osagai bakoitza modu isolatuan balioztatzea ahalbidetuz integrazio finalaren aurretik.
*   `lib/`: Maila baxuko C++ liburutegientzako edukiontzia.