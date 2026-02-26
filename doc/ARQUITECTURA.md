# Proiektuaren Arkitektura: Antigravity IDE + PlatformIO (SCLF Gripper)

Errepositorioa lau erantzukizun-eremu (domeinu) zurrunki isolatutan banatuta dago: Kognizioa (IA), Fisika (Hardwarea), Ingurunea (IDE/Konpiladorea) eta Exekuzioa (C++ Kodea).

```plaintext
/tu_proyecto_sclf/
├── .agent/
│   ├── rules/
│   │   └── hardware_safety.md
│   └── skills/
│       └── foc-hardware-setup/   <- (Gaurkotutako direktorioa)
│           └── SKILL.md
├── .vscode/
│   ├── extensions.json
│   ├── launch.json
│   └── settings.json
├── doc/
├── hardware/
│   ├── SCLF_Gripper_v1.0.kicad_pcb
│   ├── SCLF_Gripper_v1.0.kicad_sch
│   └── SCLF_Gripper_v1.0_bom.csv
├── lib/
├── src/
│   └── main.cpp
├── test/
├── .clang-format
├── .gitignore
├── AGENT.md
├── MEMORY.md
├── platformio.ini
└── README.md
```

## 1. Kognizio Domeinua (IA Motorra)

Agente autonomoaren arrazoibide-mugak, segurtasun-protokoloak eta gaitasun prozeduralak definitzen ditu.

*   `.agent/rules/hardware_safety.md`: Proiektuaren lege hautsiezinak jasotzen ditu. Agenteak fitxategi hau etengabe ebaluatzen du (`always_on: true`). Kode sinkronoaren erabilera blokeatzen du (`delay`) eta hardwarearen balidazioa eskatzen du logika idatzi aurretik.
*   `.agent/skills/foc-hardware-setup/SKILL.md`: Funtzionamendu Estandarreko Prozedura (SOP) urratsez urrats definitzen du. IA-k KiCad eskematikoak analizatu behar ditu eta hardware-mapatzea modu enpirikoan atera kodea iradoki aurretik.
*   `AGENT.md`: IA-ren rol parametrikoa (FOC Firmware Ingeniari Seniorra) ezartzen duen erroko dokumentua.
*   `MEMORY.md`: Dagoeneko hartutako erabaki arkitektonikoen erregistro aldaezina. IA-k begizta logikoetan sartzea edo erabiltzaileak balioztatutako konfigurazioak leheneratzea saihesten du.

## 2. Domeinu Fisikoa (Hardwarea eta Erreferentzia)

Egia absolutuaren iturria. Agenteak sekzio honen gainean irakurketa analitikoko baimenak baino ez ditu.

*   `hardware/`: Diseinu elektronikoaren jatorrizko fitxategiak (nodo logikoen sintaxia duten eskematikoak) eta materialen zerrenda (`bom.csv`) ditu. Plaka fisikoaren irudikapen matematikoa da.
*   `doc/`: Giza kontsumorako bakarrik den berrogeialdi-biltegia. Eskuliburu tekniko estatikoak ditu.

## 3. Ingurunearen eta Konpilazioaren Domeinua

Kodea softwaretik mikrokontrolagailura mendekotasun-errorerik gabe igarotzeko behar den azpiegitura.

*   `.vscode/`: Editorearen interfazearen konfigurazio-fitxategiak. `launch.json` fitxategia bektore kritikoa da hardware-arazketa (debugger) bidez erregistroak denbora errealean ikuskatzea ahalbidetzeko.
*   `platformio.ini`: Proiektua eraikitzeko (build) motorra. Mikrokontrolagailuaren target zehatza, azpiko framework-a eta kanpoko mendekotasun zorrotzak definitzen ditu.

## 4. Exekuzio Domeinua (Iturburu Kodea)

Garapen-iterazioen ondoriozko azken produktua.

*   `src/`: Balioztatutako exekutagarrientzat eta C++ fitxategientzat soilik den direktorioa.
*   `lib/`: Maila baxuko C++ liburutegientzako edukiontzia.
*   `test/`: Test unitarioen script-etarako edo kalibrazio errutinetarako ingurune isolatua.