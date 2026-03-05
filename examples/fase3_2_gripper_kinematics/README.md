# 3.2 Fasea - Pintzaren Zinematika (Gripper Kinematics)

Karpeta honetan 3.2 Fasearen (`fase3_2_gripper_kinematics`) kode egiaztatua dago. Fase honen helburua motorraren angelua (radianetan) pintzaren hatzen irekidurarekin (milimetrotan) lotzea da.

### Zer egiten du?

`GripperKinematics` klase parametrizatu bat sortu da, non lead-screw mekanismoaren zinematika bihurketak egiten diren:
- **Angelu → mm**: Motorraren angelua hatzen irekidurara bihurtu
- **mm → Angelu**: Nahi den irekidura motor angelura bihurtu
- **Portzentaje (%) → Angelu**: 0% = irekita, 100% = itxita
- **Clamping**: Muga fisikoetatik kanpoko balioak automatikoki mugatzen dira

### Konfigurazio Aurredefinituak

| Preseta | Stroke | Indar Max. |
|---------|--------|------------|
| `ROBOTIQ_2F85` | 85 mm | 235 N |
| `ROBOTIQ_2F140` | 140 mm | 249 N |

### Nola probatu

1. Ordezkatu `src/main.cpp` karpeta honetako `main.cpp` fitxategiarekin.
2. Konpilatu eta igo: `pio run --target upload`
3. Ireki Serie Monitorea (115200 baud).
4. Hasieran zinematikako proba automatiko bat exekutatzen da.
5. Serial-etik mm balio bat bidali eta bihurketak ikusiko dituzu.

### Nola egokitu zure pintzara

Editatu `GripperKinematics.h` fitxategian `GripperPresets` namespace-a, edo sortu zure `GripperConfig`:
```cpp
GripperConfig nireConfig = {
    .stroke_mm      = 60.0f,    // Zure irekidura (mm)
    .gear_ratio     = 3.5f,     // Zure engranaje erlazioa
    .lead_mm        = 2.0f,     // Zure usilloaren urratsa
    .min_angle_rad  = 0.0f,
    .max_angle_rad  = 0.0f,     // Auto-kalkulatua
    .max_force_N    = 100.0f,
};
gripper.begin(nireConfig);
```
