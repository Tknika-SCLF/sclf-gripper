---
description: Fase berri bat hasteko Git prestaketa automatikoa (pull eta branch)
---
Fase berri batekin hasteko, ziurtatu behar da `main` adar nagusian gaudela, eguneratuta dagoela eta ondoren lanerako adar berri bat sortzen dela denok aldaketak zapaldu gabe lan egiteko.

1. Egiaztatu laneko zuhaitza garbi dagoela (`git status`). Egoera zikina bada, galdetu erabiltzaileari norabidea.
// turbo
2. Aldatu `main` adarrera: `git checkout main`
// turbo
3. Lortu GitHubeko azken aldaketak lankideenak jasotzeko: `git pull origin main`
4. Galdetu erabiltzaileari nola deitu nahi dion fase edo adar berriari (adibidez, `fase1_2_drv_spi`).
5. Sortu adar berri hori eta mugitu bertara: `git checkout -b <erabiltzaileak_emandako_izena>`
6. Adierazi erabiltzaileari ingurunea prest dagoela kodeketarekin hasteko.
