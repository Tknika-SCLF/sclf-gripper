# SCLF Gripper v1.0 — Firmware

[ Euskara ](README.eu.md) | [ English ](README.md) | [ Español ](README.es.md)

> Firmware del controlador de motor para el SCLF Gripper v1.0 de Tknika.
> STM32G474CEU6 · SimpleFOC · PlatformIO · IDE Antigravity
> Licencia: CC BY-SA 4.0

---

## Inicio Rápido

Si ya tienes el entorno configurado:

```bash
# 1. Compilar
pio run

# 2. Flashear (con ST-Link conectado)
pio run --target upload

# 3. Monitor Serie (USB-C del Gripper al PC)
pio device monitor --baud 115200
```

---

## Configuración del Entorno desde Cero

### Paso 1 — Instalar Antigravity

Descarga e instala Antigravity desde su sitio web oficial. Es un fork de VSCode/Windsurf y funciona de manera idéntica.

---

### Paso 2 — Instalar C/C++ Tools

> ⚠️ PlatformIO **no está disponible en Open VSX** (el marketplace de Antigravity). Ambas extensiones deben instalarse manualmente desde archivos `.vsix`.

**2a.** Ve a: https://github.com/microsoft/vscode-cpptools/releases

Descarga el archivo correspondiente a tu sistema operativo:

| Sistema | Archivo a descargar |
|---|---|
| Windows (64-bit) | `cpptools-win32-x64.vsix` |
| macOS (Apple Silicon) | `cpptools-osx-arm64.vsix` |
| macOS (Intel) | `cpptools-osx-x64.vsix` |
| Linux (64-bit) | `cpptools-linux-x64.vsix` |

**2b.** En Antigravity: `Extensiones (Ctrl+Shift+X)` → icono `···` (tres puntos) → **Instalar desde VSIX** → selecciona el archivo `.vsix` descargado.

---

### Paso 3 — Instalar PlatformIO

**3a.** Ve a: https://github.com/platformio/platformio-vscode-ide/releases

Descarga el archivo más reciente: `platformio-ide-X.X.X.vsix`

**3b.** En Antigravity: `Extensiones (Ctrl+Shift+X)` → `···` → **Instalar desde VSIX** → selecciona `platformio-ide-X.X.X.vsix`.

**3c.** Reinicia Antigravity. PlatformIO instalará sus dependencias automáticamente la primera vez (puede tardar de 2 a 5 minutos).

> ⚠️ **Nota Importante:** PlatformIO dentro de Antigravity no tiene soporte oficial. Si tienes problemas de compilación o subida, consulta la sección de [Solución de Problemas](#solución-de-problemas) al final de este documento.

---

### Paso 4 — Instalar cortex-debug (para depurar con ST-Link)

**4a.** Ve a: https://github.com/Marus/cortex-debug/releases

Descarga: `cortex-debug-X.X.X.vsix`

**4b.** Instálalo como en el paso anterior (Instalar desde VSIX).

**4c.** Instala también OpenOCD, si no lo tienes. Normalmente PlatformIO lo incluye:
```
~/.platformio/packages/tool-openocd/
```

---

### Paso 5 — Abrir el Proyecto

```bash
# Clona o copia la carpeta del firmware
# Abre Antigravity en la carpeta raíz (donde se encuentra platformio.ini)
```

En Antigravity: `Archivo → Abrir Carpeta` → selecciona la carpeta `SCLF_Gripper_v1_0_firmware/`.

PlatformIO detectará automáticamente `platformio.ini` y descargará lo siguiente:
- Plataforma `ststm32`
- Framework `framework-arduinoststm32`
- Toolchain `toolchain-gccarmnoneeabi`
- Biblioteca `SimpleFOC`

---

### Paso 6 — Primera Compilación

En la barra inferior de Antigravity, haz clic en el icono ✓ (Compilar) de PlatformIO, o desde el terminal:

```bash
pio run
```

**Salida esperada:**
```
[SUCCESS] Took X.XX seconds
RAM:   [=         ]  X.X% (used XXXX bytes from 131072 bytes)
Flash: [=         ]  X.X% (used XXXXX bytes from 524288 bytes)
```

Si da error al compilar → consulta [Solución de Problemas](#solución-de-problemas).

---

### Paso 7 — Conectar ST-Link y Flashear

Conecta el ST-Link al puerto SWD del SCLF Gripper:

| Pin ST-Link | Pad Gripper | Señal |
|---|---|---|
| SWDIO | DIO | PA13 |
| SWCLK | CLK | PA14 |
| RST | RST | PG10 |
| GND | GND | — |
| 3.3V | — | (alimentar la placa por separado a 24V) |

> ⚠️ **No alimentes la placa desde el ST-Link**. Usa una fuente de alimentación externa de 24V (del robot o de laboratorio).

Flashear:
```bash
pio run --target upload
```

**Salida esperada:** El LED (PC6) empezará a parpadear cada 500ms.

---

### Paso 8 — Verificar USB VCP

1. Conecta un cable USB-C entre el Gripper y el PC.
2. Abre el monitor serie de PlatformIO (icono 🔌 en la barra inferior) o:
   ```bash
   pio device monitor --baud 115200
   ```
3. Deberías ver esto:
   ```
   ========================================
     SCLF Gripper v1.0 — Firmware FASE 0
   ========================================
     MCU: STM32G474CEU6 @ 170 MHz
     Estado: SMOKE TEST (Sin FOC, sin motor)
   ========================================
     Pines inicializados correctamente.
   [0s] loops/s≈XXXXX  LED=ON
   [1s] loops/s≈XXXXX  LED=OFF
   ```

✅ **Si ves esto, el entorno está 100% operativo.** Actualiza `MEMORY.md` y marca las tareas de la FASE 0 como completadas en `TASKS.md`.

---

### Paso 9 — Configurar el Agente de IA

Para trabajar con el asistente de IA integrado en Antigravity:

1. Abre el panel de chat del agente (icono de IA en la barra lateral).
2. Al inicio de cada sesión, pega esto:

```
Lee estos archivos de contexto antes de ayudarme:
- AGENT.md   (hardware, pines, arquitectura)
- MEMORY.md  (estado persistente, parámetros calibrados)
- TASKS.md   (la tarea en la que estoy trabajando ahora)
- RULES.md   (reglas que el código generado debe respetar)
```

3. Luego, describe qué tarea quieres abordar de `TASKS.md`.

---

## Estructura del Proyecto

```
SCLF_Gripper_v1_0_firmware/
│
├── src/
│   ├── main.cpp                    ← Punto de entrada. FASE 0: smoke test
│   ├── config/
│   │   └── pins.h                  ← TODOS los pines. Verificados con el esquemático.
│   ├── encoder/
│   │   ├── MT6701.h                ← API del encoder (esqueleto listo)
│   │   └── MT6701.cpp              ← Implementar en FASE 1.1
│   ├── motor/
│   │   ├── DRV8316.h               ← API del driver de puerta (esqueleto listo)
│   │   └── DRV8316.cpp             ← Implementar en FASE 1.2
│   ├── comms/
│   │   ├── RS485.h                 ← API RS-485 (esqueleto listo)
│   │   └── RS485.cpp               ← Implementar en FASE 1.4
│   └── faults/
│       ├── FaultManager.h          ← API del gestor de fallos (esqueleto listo)
│       └── FaultManager.cpp        ← Implementar en FASE 6
│
├── lib/                            ← Bibliotecas locales (actualmente vacía)
├── test/                           ← Tests Unitarios PlatformIO (FASE 8)
│
├── .vscode/
│   ├── settings.json               ← Config IDE (IntelliSense, formato)
│   ├── extensions.json             ← Extensiones recomendadas
│   └── launch.json                 ← Depurar con ST-Link y cortex-debug
│
├── platformio.ini                  ← Configuración de compilación y subida
├── .clang-format                   ← Estilo de código (aplicado al guardar)
├── .gitignore
│
├── AGENT.md    ← 📋 Contexto para el agente de IA
├── MEMORY.md   ← 🧠 Estado persistente del proyecto
├── TASKS.md    ← ✅ Backlog completo por fases
├── RULES.md    ← 📏 Reglas del proyecto
└── SRS.md      ← 📄 Especificación de Requisitos de Software
```

---

## Comandos más usados de PlatformIO

```bash
pio run                          # Compilar
pio run --target upload          # Compilar y flashear (ST-Link)
pio run --target upload_dfu      # Flashear vía USB DFU (BOOT + RST)
pio device monitor               # Monitor serie (USB VCP, 115200 baudios)
pio test                         # Ejecutar tests unitarios
pio run --target clean           # Limpiar artefactos de compilación
pio lib install "Simple FOC"     # Instalar librería manualmente
pio pkg update                   # Actualizar plataforma y librerías
```

---

## Bootloader DFU (Flashear sin ST-Link)

Si no tienes un ST-Link disponible, puedes flashear vía USB DFU:

1. Mantén presionado el botón **BOOT** (PB8).
2. Presiona y suelta el botón **RESET** (PG10).
3. Suelta el botón **BOOT**.
4. El Gripper aparecerá como un dispositivo DFU en el PC.
5. Cambia a `upload_protocol = dfu` en el archivo `platformio.ini`.
6. Ejecuta `pio run --target upload`.

---

## Solución de Problemas

### PlatformIO falla al compilar / error de toolchain
```bash
# Reinstalar la plataforma STM32
pio platform install ststm32

# Limpiar y volver a compilar
pio run --target clean && pio run
```

### ST-Link no detecta la placa
- Verifica que el GND del ST-Link esté conectado al GND del Gripper.
- Verifica que la placa tenga alimentación (LED PWR encendido).
- Prueba ejecutar `openocd -f interface/stlink.cfg -f target/stm32g4x.cfg` en el terminal.
- Si OpenOCD falla: actualiza el firmware del ST-Link vía STM32CubeProgrammer.

### USB VCP no aparece en el PC
- Instala el driver STM32 VCP: https://www.st.com/en/development-tools/stsw-stm32102.html (Solo Windows)
- No se necesita driver en Linux/macOS. Verifica con `ls /dev/ttyACM*`.

### IntelliSense no encuentra `Arduino.h` o cabeceras STM32
- Espera a que PlatformIO termine su primera compilación (genera el índice de includes).
- Si el problema persiste: `Ctrl+Shift+P` → "C/C++: Reset IntelliSense Database".

### Antigravity / PlatformIO no comunica con la placa
- Este es un problema conocido documentado por la comunidad. PlatformIO en Antigravity
  no tiene soporte oficial. Si la compilación funciona pero la subida falla,
  intenta ejecutar `pio run --target upload` **desde el terminal interno** en lugar de
  usar el botón UI de PlatformIO.

---

## Hardware de Referencia

| Componente | Pieza | Hoja de datos (Datasheet) |
|---|---|---|
| MCU | STM32G474CEU6 | [ST.com](https://www.st.com/resource/en/reference_manual/rm0440-stm32g4-series-advanced-armbased-32bit-mcus-stmicroelectronics.pdf) |
| Driver Motor | DRV8316CRRGFR | [TI.com](https://www.ti.com/lit/ds/symlink/drv8316.pdf) |
| Encoder | MT6701QT | [Magntek](https://www.magntek.com.cn/upload/MT6701_Rev.1.8.pdf) |
| RS-485 | MAX3485 | [Analog](https://www.analog.com/media/en/technical-documentation/data-sheets/MAX3483-MAX3491.pdf) |
| SimpleFOC | v2.x | [docs.simplefoc.com](https://docs.simplefoc.com) |

---

*Diseñado por Tknika — Licenciado bajo CC BY-SA 4.0*
