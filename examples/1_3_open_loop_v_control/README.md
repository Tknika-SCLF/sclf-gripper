# 1.3 Fasea - Open Loop Velocity Control (GM3506)

Karpeta honetan 1.3 Fasearen (`1_3_open_loop_v_control`) kode egiaztatua dago. Fase honen helburua iPower GM3506 motorra begizta irekian (open-loop) biraraztea da, DRV8316 6-PWM bidez ondo kontrolatzen dela ziurtatzeko.

### Zergatik egin den?
Fase hau segurtasunagatik egiten dugu. Motorra birarazi aurretik sentsorea motorrarekin lerrokatzen saiatzen bagara (closed-loop), errore batek motorra edo driver-a erre dezake. Hori saihesteko, aurretik motorra modu seguruan biratzen ote den frogatzen dugu, **2.0V-eko muga zorrotza jarrita**.

### Nola probatu

1. Ziurtatu plakaren 24V elikadura **DESKONEKTATUTA** dagoela kableatzen ari zaren bitartean.
2. Motorraren 3 faseak ondo konektatu.
3. Elikatu plaka 24V iturriarekin.
4. Ordezkatu zure `src/main.cpp` fitxategia karpeta honetako `main.cpp` fitxategiarekin.
5. Konpilatu eta igo kodea:
   ```bash
   pio run --target upload
   ```
6. Ireki Serie Monitorea (USB kablea Gripper-era konektatuta):
   ```bash
   pio device monitor --baud 115200
   ```
7. Idatzi terminalean eta bidali `T1` agindua motorra 1 rad/s abiaduran biratzeko. `T2` jarri dezakezu azkarrago biratzeko.
8. Geratzeko, bidali `T0`.

### Motorraren ezaugarriak (iPower GM3506 130T)
- **Poloak (Poles):** 14
- **Polo-pareak (Pole pairs):** 7
- **Bira (Turns):** 130
- **Erresistentzia:** 12.5 Ω
- **Tentsio gomendatua:** 11.1V
