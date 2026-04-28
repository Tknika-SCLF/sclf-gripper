# 7. Fasea - Pintzaren Zinematika (Gripper Kinematics)

Karpeta honetan 7. Fasearen (`phase7_gripper_kinematics`) kode egiaztatua dago. Fase honen helburua motorraren angelua (radianetan) pintzaren hatzen irekidurarekin (milimetrotan) lotzea da.

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

1. Konpilatu eta kargatu firmwarea ingurune espezifikoa erabiliz:
   ```bash
   pio run -e test_gripper_kinematics --target upload
   ```
2. Ireki Serie Monitorea (115200 baud).
3. Hasieran zinematikako proba automatiko bat exekutatzen da.
4. Serial-etik mm balio bat bidali eta bihurketak ikusiko dituzu.

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

---

# Fase 7 - Cinemática de la Pinza (Gripper Kinematics) [ES]

En esta carpeta se encuentra el código verificado de la Fase 7 (`phase7_gripper_kinematics`). El objetivo de esta fase es relacionar el ángulo del motor (en radianes) con la apertura de los dedos de la pinza (en milímetros).

### ¿Qué hace?

Se ha creado una clase parametrizada `GripperKinematics` donde se realizan las conversiones cinemáticas del mecanismo lead-screw:
- **Ángulo → mm**: Convierte el ángulo del motor a la apertura de los dedos
- **mm → Ángulo**: Convierte la apertura deseada al ángulo del motor
- **Porcentaje (%) → Ángulo**: 0% = abierto, 100% = cerrado
- **Clamping**: Los valores fuera de los límites físicos se limitan automáticamente

### Configuraciones Predefinidas

| Preset | Stroke | Fuerza Máx. |
|---------|--------|------------|
| `ROBOTIQ_2F85` | 85 mm | 235 N |
| `ROBOTIQ_2F140` | 140 mm | 249 N |

### Cómo Probarlo

1. Compila y carga el código usando el entorno específico:
   ```bash
   pio run -e test_gripper_kinematics --target upload
   ```
2. Abre el Monitor Serie (115200 baudios).
3. Al inicio se ejecuta una prueba cinemática automática.
4. Envía un valor en mm por el puerto serie y verás las conversiones.

### Cómo adaptarlo a tu pinza

Edita el namespace `GripperPresets` en el archivo `GripperKinematics.h`, o crea tu propia `GripperConfig`:
```cpp
GripperConfig miConfig = {
    .stroke_mm      = 60.0f,    // Tu apertura (mm)
    .gear_ratio     = 3.5f,     // Tu relación de engranajes
    .lead_mm        = 2.0f,     // Tu paso de husillo
    .min_angle_rad  = 0.0f,
    .max_angle_rad  = 0.0f,     // Auto-calculado
    .max_force_N    = 100.0f,
};
gripper.begin(miConfig);
```

---

# Phase 7 - Gripper Kinematics [EN]

This folder contains the verified code for Phase 7 (`phase7_gripper_kinematics`). The goal of this phase is to link the motor angle (in radians) with the gripper finger opening (in millimeters).

### What does it do?

A parameterized class `GripperKinematics` has been created to perform the kinematic conversions of the lead-screw mechanism:
- **Angle → mm**: Convert motor angle to finger opening
- **mm → Angle**: Convert desired opening to motor angle
- **Percentage (%) → Angle**: 0% = open, 100% = closed
- **Clamping**: Values outside physical limits are automatically constrained

### Predefined Configurations

| Preset | Stroke | Max. Force |
|---------|--------|------------|
| `ROBOTIQ_2F85` | 85 mm | 235 N |
| `ROBOTIQ_2F140` | 140 mm | 249 N |

### How to Test

1. Compile and upload the firmware using the specific environment:
   ```bash
   pio run -e test_gripper_kinematics --target upload
   ```
2. Open the Serial Monitor (115200 baud).
3. An automatic kinematics test is executed at startup.
4. Send a mm value through the serial port and you will see the conversions.

### How to adapt it to your gripper

Edit the `GripperPresets` namespace in the `GripperKinematics.h` file, or create your own `GripperConfig`:
```cpp
GripperConfig myConfig = {
    .stroke_mm      = 60.0f,    // Your opening (mm)
    .gear_ratio     = 3.5f,     // Your gear ratio
    .lead_mm        = 2.0f,     // Your lead-screw pitch
    .min_angle_rad  = 0.0f,
    .max_angle_rad  = 0.0f,     // Auto-calculated
    .max_force_N    = 100.0f,
};
gripper.begin(myConfig);
```
