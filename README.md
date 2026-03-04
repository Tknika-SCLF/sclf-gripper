# SCLF Gripper v1.0 — Firmware

> Motor controller firmware para el SCLF Gripper v1.0 de Tknika.
> STM32G474CEU6 · SimpleFOC · PlatformIO · Antigravity IDE
> Licencia: CC BY-SA 4.0

---

## Inicio Rápido

Si ya tienes el entorno configurado:

```bash
# 1. Compilar
pio run

# 2. Flashear (ST-Link conectado)
pio run --target upload

# 3. Monitor serie (USB-C del gripper al PC)
pio device monitor --baud 115200
```

---

## Configuración del Entorno desde Cero

### Paso 1 — Instalar Antigravity

Descarga e instala Antigravity desde su web oficial. Es un fork de VSCode/Windsurf y funciona igual.

---

### Paso 2 — Instalar C/C++ Tools

> ⚠️ PlatformIO **no está en Open VSX** (el marketplace de Antigravity). Hay que instalar ambas extensiones manualmente desde archivos `.vsix`.

**2a.** Ve a: https://github.com/microsoft/vscode-cpptools/releases

Descarga el archivo correspondiente a tu sistema operativo:

| Sistema | Archivo a descargar |
|---|---|
| Windows (64-bit) | `cpptools-win32-x64.vsix` |
| macOS (Apple Silicon) | `cpptools-osx-arm64.vsix` |
| macOS (Intel) | `cpptools-osx-x64.vsix` |
| Linux (64-bit) | `cpptools-linux-x64.vsix` |

**2b.** En Antigravity: `Extensiones (Ctrl+Shift+X)` → icono `···` (tres puntos) → **Install from VSIX** → selecciona el archivo `.vsix` descargado.

---

### Paso 3 — Instalar PlatformIO

**3a.** Ve a: https://github.com/platformio/platformio-vscode-ide/releases

Descarga el archivo más reciente: `platformio-ide-X.X.X.vsix`

**3b.** En Antigravity: `Extensiones (Ctrl+Shift+X)` → `···` → **Install from VSIX** → selecciona `platformio-ide-X.X.X.vsix`.

**3c.** Reinicia Antigravity. PlatformIO instalará sus dependencias automáticamente la primera vez (puede tardar 2-5 minutos).

> ⚠️ **Aviso importante:** La instalación de PlatformIO en Antigravity no está oficialmente soportada. Si encuentras problemas de compilación o upload, consulta la sección [Resolución de Problemas](#resolución-de-problemas) al final de este documento.

---

### Paso 4 — Instalar cortex-debug (debug con ST-Link)

**4a.** Ve a: https://github.com/Marus/cortex-debug/releases

Descarga: `cortex-debug-X.X.X.vsix`

**4b.** Instala igual que las anteriores (Install from VSIX).

**4c.** Instala también OpenOCD si no lo tienes. PlatformIO suele incluirlo:
```
~/.platformio/packages/tool-openocd/
```

---

### Paso 5 — Abrir el proyecto

```bash
# Clona o copia la carpeta del firmware
# Abre Antigravity en la carpeta raíz (donde está platformio.ini)
```

En Antigravity: `File → Open Folder` → selecciona la carpeta `SCLF_Gripper_v1_0_firmware/`

PlatformIO detectará el `platformio.ini` automáticamente y descargará:
- Platform `ststm32`
- Framework `framework-arduinoststm32`
- Toolchain `toolchain-gccarmnoneeabi`
- Librería `SimpleFOC`

---

### Paso 6 — Primera compilación

En la barra inferior de Antigravity, haz clic en el icono ✓ (Build) de PlatformIO, o desde terminal:

```bash
pio run
```

**Resultado esperado:**
```
[SUCCESS] Took X.XX seconds
RAM:   [=         ]  X.X% (used XXXX bytes from 131072 bytes)
Flash: [=         ]  X.X% (used XXXXX bytes from 524288 bytes)
```

Si no compila → ver [Resolución de Problemas](#resolución-de-problemas).

---

### Paso 7 — Conectar el ST-Link y flashear

Conecta el ST-Link al puerto SWD del SCLF Gripper:

| ST-Link pin | Gripper pad | Señal |
|---|---|---|
| SWDIO | DIO | PA13 |
| SWCLK | CLK | PA14 |
| RST | RST | PG10 |
| GND | GND | — |
| 3.3V | — | (alimentar la placa por separado con 24V) |

> ⚠️ **No alimentar la placa desde el ST-Link**. Usar 24V externos desde el robot o fuente de alimentación.

Flashear:
```bash
pio run --target upload
```

**Resultado esperado:** El LED (PC6) empieza a parpadear cada 500ms.

---

### Paso 8 — Verificar USB VCP

1. Conecta un cable USB-C entre el Gripper y el PC.
2. Abre el monitor serie de PlatformIO (icono 🔌 en la barra inferior) o:
   ```bash
   pio device monitor --baud 115200
   ```
3. Deberías ver:
   ```
   ========================================
     SCLF Gripper v1.0 — Firmware FASE 0
   ========================================
     MCU: STM32G474CEU6 @ 170 MHz
     Estado: SMOKE TEST (sin FOC, sin motor)
   ========================================
     Pines inicializados correctamente.
   [0s] loops/s≈XXXXX  LED=ON
   [1s] loops/s≈XXXXX  LED=OFF
   ```

✅ **Si ves esto, el entorno está 100% operativo.** Actualiza `MEMORY.md` y marca las tareas de FASE 0 como completadas en `TASKS.md`.

---

### Paso 9 — Configurar el agente de IA

Para trabajar con el asistente de IA integrado en Antigravity:

1. Abre el panel de chat del agente (icono de IA en la barra lateral).
2. Al inicio de cada sesión, escribe:

```
Lee los siguientes ficheros de contexto antes de ayudarme:
- AGENT.md   (hardware, pines, arquitectura)
- MEMORY.md  (estado actual, parámetros calibrados)
- TASKS.md   (tarea en la que estoy trabajando ahora)
- RULES.md   (reglas que debe respetar el código generado)
```

3. Luego describe qué tarea de `TASKS.md` quieres trabajar.

---

## Estructura del Proyecto

```
SCLF_Gripper_v1_0_firmware/
│
├── src/
│   ├── main.cpp                    ← Punto de entrada. FASE 0: smoke test
│   ├── config/
│   │   └── pins.h                  ← TODOS los pines. Verificado vs esquemático.
│   ├── encoder/
│   │   ├── MT6701.h                ← API del encoder (esqueleto listo)
│   │   └── MT6701.cpp              ← Implementar en FASE 1.1
│   ├── motor/
│   │   ├── DRV8316.h               ← API del gate driver (esqueleto listo)
│   │   └── DRV8316.cpp             ← Implementar en FASE 1.2
│   ├── comms/
│   │   ├── RS485.h                 ← API RS-485 (esqueleto listo)
│   │   └── RS485.cpp               ← Implementar en FASE 1.4
│   └── faults/
│       ├── FaultManager.h          ← API gestor de fallos (esqueleto listo)
│       └── FaultManager.cpp        ← Implementar en FASE 6
│
├── lib/                            ← Librerías locales (vacío por ahora)
├── test/                           ← Tests PlatformIO Unity (FASE 8)
│
├── .vscode/
│   ├── settings.json               ← Configuración IDE (IntelliSense, formato)
│   ├── extensions.json             ← Extensiones recomendadas
│   └── launch.json                 ← Debug ST-Link con cortex-debug
│
├── platformio.ini                  ← Configuración de build y upload
├── .clang-format                   ← Estilo de código (se aplica al guardar)
├── .gitignore
│
├── AGENT.md    ← 📋 Contexto para el agente de IA
├── MEMORY.md   ← 🧠 Estado persistente del proyecto
├── TASKS.md    ← ✅ Backlog completo por fases
├── RULES.md    ← 📏 Reglas del proyecto
└── SRS.md      ← 📄 Especificación de requisitos
```

---

## Comandos PlatformIO Más Usados

```bash
pio run                          # Compilar
pio run --target upload          # Compilar y flashear (ST-Link)
pio run --target upload_dfu      # Flashear por USB DFU (BOOT + RST)
pio device monitor               # Monitor serie (USB VCP, 115200 baud)
pio test                         # Ejecutar tests unitarios
pio run --target clean           # Limpiar artefactos de compilación
pio lib install "Simple FOC"     # Instalar librería manualmente
pio pkg update                   # Actualizar plataforma y librerías
```

---

## DFU Bootloader (flashear sin ST-Link)

Si no tienes ST-Link disponible, puedes flashear por USB DFU:

1. Mantén pulsado el botón **BOOT** (PB8).
2. Pulsa y suelta el botón **RESET** (PG10).
3. Suelta **BOOT**.
4. El gripper aparece como dispositivo DFU en el PC.
5. Cambia `upload_protocol = dfu` en `platformio.ini`.
6. Ejecuta `pio run --target upload`.

---

## Resolución de Problemas

### PlatformIO no compila / error de toolchain
```bash
# Reinstalar la plataforma STM32
pio platform install ststm32

# Limpiar y recompilar
pio run --target clean && pio run
```

### Error: Unknown board ID `genericSTM32G474CE`
Este error se produce al compilar debido a que PlatformIO no reconoce la placa `genericSTM32G474CE` en combinacion con el framework Arduino. 
Para solucionarlo hay que modificar el archivo `platformio.ini`:
1. Cambiar la placa a `board = nucleo_g474re`. Esta placa Nucleo tiene el mismo microcontrolador subyacente y soporta el framework Arduino.
2. Comentar o eliminar la línea `debug_server_arguments` ya que está obsoleta y lanza advertencias.


### ST-Link no detecta la placa
- Verifica que GND del ST-Link está conectado al GND del gripper.
- Verifica que la placa tiene alimentación (LED PWR encendido).
- Prueba `openocd -f interface/stlink.cfg -f target/stm32g4x.cfg` en terminal.
- Si OpenOCD falla: actualiza el firmware del ST-Link desde STM32CubeProgrammer.

### USB VCP no aparece en el PC
- Instala el driver STM32 VCP: https://www.st.com/en/development-tools/stsw-stm32102.html (solo Windows)
- En Linux/macOS no necesita drivers. Verifica con `ls /dev/ttyACM*`.

### IntelliSense no encuentra `Arduino.h` o headers STM32
- Espera a que PlatformIO termine la primera compilación (genera el índice de includes).
- Si persiste: `Ctrl+Shift+P` → "C/C++: Reset IntelliSense Database".

### Antigravity / PlatformIO no se comunica con el board
- Este es el problema conocido documentado en la comunidad. PlatformIO en Antigravity
  no está oficialmente soportado. Si la compilación funciona pero el upload falla,
  intenta usar `pio run --target upload` desde el **terminal integrado** en lugar del
  botón de la UI de PlatformIO.

---

## Hardware de Referencia

| Componente | Parte | Datasheet |
|---|---|---|
| MCU | STM32G474CEU6 | [ST.com](https://www.st.com/resource/en/reference_manual/rm0440-stm32g4-series-advanced-armbased-32bit-mcus-stmicroelectronics.pdf) |
| Gate Driver | DRV8316CRRGFR | [TI.com](https://www.ti.com/lit/ds/symlink/drv8316.pdf) |
| Encoder | MT6701QT | [Magntek](https://www.magntek.com.cn/upload/MT6701_Rev.1.8.pdf) |
| RS-485 | MAX3485 | [Analog](https://www.analog.com/media/en/technical-documentation/data-sheets/MAX3483-MAX3491.pdf) |
| SimpleFOC | v2.x | [docs.simplefoc.com](https://docs.simplefoc.com) |

---

*Diseñado por Tknika — Licensed under CC BY-SA 4.0*
