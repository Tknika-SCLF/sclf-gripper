---
description: Fase bat amaitzean, adibidea examples/ karpetan gorde, dokumentatu eta main adarrera igo
---
Fase berri bat behar bezala testatu denean, kode horren adibidea etorkizunerako gorde, agiriak eguneratu, eta `main` adarrera fusionatzen (merge) da automatikoki.

1. Galdetu erabiltzaileari zein izan den azken fase honen izena (adib: `fase1_2_drv_spi`) eta zein den funtzionamenduaren/deskribapenaren laburpena.
// turbo
2. Sortu direktorio berria: `mkdir examples/<fasearen_izena>`
// turbo
3. Kopiatu oraingo probako kodea: `cp src/main.cpp examples/<fasearen_izena>/main.cpp`
4. Erabili `write_to_file` tresna `README.md` bat sortzeko `examples/<fasearen_izena>/README.md` helbidean. Idatzi bertan nola probatu kodea eta azalpide garrantzitsuak.
5. Erabili `multi_replace_file_content` tresna proiektuaren erroko `README.md`-n "Faseen Probak eta Adibideak" atalean lotura (link) edo aipamen labur bat gehitzeko aurrera eramandako testari buruz. Zihurtatu ere `ROADMAP.md` fitxategian fasea `(EGINA)` bezela markatzen duzula.
// turbo
6. Gorde aldaketa guztiak Git-en: `git add . && git commit -m "docs: <fasearen_izena> amaituta eta examples karpetan eskuragarri"`
// turbo
7. Bidali zure adarra GitHubera: `git push origin <uneko_adarra>`
// turbo
8. Aldatu `main` adarrera fusionatzeko: `git checkout main`
// turbo
9. Deskargatu azken bertsioa inork aldaketak egin baditu gatazkak ekiditeko: `git pull origin main`
// turbo
10. Batu fase berria `main` adarrean: `git merge <uneko_adarra>`
// turbo
11. Igo `main` eguneratua: `git push origin main`
12. Jakinarazi erabiltzaileari prozesua ehuneko ehunean osatu dela eta prest gaudela hurrengo faserako.
