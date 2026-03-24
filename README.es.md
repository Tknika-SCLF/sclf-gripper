# SCLF Gripper v1.0 — Firmware

[ Euskara ](README.md) | [ English ](README.en.md) | [ Español ](README.es.md)

> Firmware del controlador de motor para el SCLF Gripper v1.0 de Tknika.
> STM32G474CEU6 · SimpleFOC · PlatformIO · IDE Antigravity
> Licencia: CC BY-SA 4.0

---

> ⚠️ **IMPORTANTE:** Antes de empezar a trabajar con este proyecto, por favor lee siempre el archivo `TASKS.md`. Allí encontrarás el estado actual exacto del proyecto y los siguientes pasos a realizar entre desarrolladores.

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

**2a.** Ve a: https://github.com/microsoft/vscode-cpptools/releases/latest

Descarga el archivo correspondiente a tu sistema operativo:

| Sistema | Archivo a descargar |
|---|---|
| Windows (64-bit) | `cpptools-windows-x64.vsix` |
| macOS (Apple Silicon) | `cpptools-macOS-arm64.vsix` |
| macOS (Intel) | `cpptools-macOS-x64.vsix` |
| Linux (64-bit) | `cpptools-linux-x64.vsix` |

**2b.** En Antigravity: `Extensiones (Ctrl+Shift+X)` → icono `···` (tres puntos) → **Instalar desde VSIX** → selecciona el archivo `.vsix` descargado.

---

### Paso 3 — Instalar PlatformIO

**3a.** Ve a: https://github.com/platformio/platformio-vscode-ide/releases/latest

Abre la sección **"Assets"** de la última versión y descarga el archivo (ejemplo: `platformio-ide-3.3.3.vsix`).

**3b.** En Antigravity: `Extensiones (Ctrl+Shift+X)` → `···` → **Instalar desde VSIX** → selecciona el archivo `.vsix` descargado.

**3c.** Reinicia Antigravity. PlatformIO instalará sus dependencias automáticamente la primera vez (puede tardar de 2 a 5 minutos).

> ⚠️ **Nota Importante:** PlatformIO dentro de Antigravity no tiene soporte oficial. Si tienes problemas de compilación o subida, consulta la sección de [Solución de Problemas](#solución-de-problemas) al final de este documento.

---

### Paso 4 — Instalar cortex-debug (para depurar con ST-Link)

> 💡 **¿Qué es Open VSX?** Es un registro de extensiones de código abierto gestionado por la Fundación Eclipse. Es el repositorio oficial y seguro que utilizan Antigravity y VSCodium para evitar las restricciones de Microsoft.

**4a.** Ve a: https://open-vsx.org/extension/marus25/cortex-debug

En el lado derecho, en el menú desplegable **"Version"**, selecciona la última versión estable que NO diga **"Pre-release"** (ejemplo: `1.12.1`). Luego, haz clic en el botón morado **`DOWNLOAD`** que aparece debajo para obtener el archivo `.vsix`.

**4b.** Instálalo como en el paso anterior (Instalar desde VSIX).

**4c.** Configurar OpenOCD:
> 💡 **Buena noticia:** ¡No necesitas descargar o instalar nada manualmente! PlatformIO descarga OpenOCD automáticamente la primera vez que compilas o subes el proyecto a la placa. Su ubicación en Windows suele ser:
`C:\Users\TU_USUARIO\.platformio\packages\tool-openocd\bin\openocd.exe`

*(Si Cortex-Debug da un error diciendo que no encuentra OpenOCD, ve a la configuración de Antigravity y establece esta ruta exacta en la opción `cortex-debug.openocdPath`).*

---

### Paso 5 — Instalar Git

Para poder clonar el proyecto necesitas tener Git instalado.

**5a.** Ve a: https://git-scm.com/downloads

**5b.** Descarga e instala la versión para tu sistema operativo (puedes mantener las opciones por defecto durante la instalación).

---

### Paso 6 — Abrir el Proyecto

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

### Paso 7 — Primera Compilación

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

### Paso 8 — Conectar ST-Link y Flashear

> ⚠️ **NUEVO PC:** Si es la primera vez que usas este ordenador, necesitas instalar los drivers USB del ST-Link para que Windows lo detecte. Tienes dos opciones:
> 
> **Opción A (Oficial de ST):** Descárgalos desde [STSW-LINK009](https://www.st.com/en/development-tools/stsw-link009.html) (requiere crear una cuenta gratuita).
> 
> **Opción B (El "Truco" Rápido - Zadig):** Si no quieres registrarte, puedes usar Zadig. 
> 1. Descarga e inicia [Zadig](https://zadig.akeo.ie/) (es portable, no requiere instalación).
> 2. Conecta tu ST-Link al USB del ordenador.
> 3. En Zadig, ve a **Options** → **List All Devices** (Listar todos los dispositivos).
> 4. En el menú desplegable, selecciona **"STM32 STLink"** (o similar).
> 5. Asegúrate de que el Driver Destino (el recuadro verde de la derecha) sea **"WinUSB"**.
> 6. Haz clic en **Install Driver / Replace Driver** y espera a que termine. ¡Listo! Ya será detectado por PlatformIO/OpenOCD.
> 
> **Opción C (Usuarios de WSL2):** Si estás ejecutando PlatformIO dentro de WSL (Ubuntu/Linux en Windows), *Linux ya tiene los drivers*, PERO Windows no "pasa" el USB a WSL por defecto. 
> 1. En PowerShell (como Admin) en Windows, instala usbipd: `winget install --interactive --exact dorssel.usbipd-win`
> 2. Cierra y abre PowerShell. Conecta el ST-Link.
> 3. Ejecuta `usbipd list` y busca el VID:PID del ST-Link (ej. `0483:3748`).
> 4. Conéctalo a WSL con `usbipd bind --busid <BUSID>` y luego `usbipd attach --wsl --busid <BUSID>`. (Ahora Linux verá `/dev/ttyUSB` o `/dev/stlinkv2`).

Conecta el ST-Link al puerto SWD del SCLF Gripper:

| Pin ST-Link | Pad Gripper | Señal |
|---|---|---|
| SWDIO | DIO | PA13 |
| SWCLK | CLK | PA14 |
| RST | RST | PG10 |
| GND | GND | — |
| 3.3V | 3V3 | (alimenta solo el STM32, no el encoder ni driver) |

> ⚠️ **Alimentación:**
> - El pin 3.3V del ST-Link **SÍ es suficiente** para flashear el STM32 y ejecutar código básico (blink LED).
> - 💡 **¡NUEVO DESCUBRIMIENTO!**: El encoder MT6701 **también se alimenta con la línea de 3.3V**. Por lo tanto, puedes probar la Fase 1.1 (Encoder) y Fase 1.4 (RS-485) usando EXCLUSIVAMENTE el ST-Link.
> - **PERO** el driver DRV8316 y los motores requieren que el circuito principal reciba **24V externos** (vía pogo-pins o bornera). Las fases 1.2, 1.3 y 2+ están bloqueadas si no hay 24V.

Flashear:
```bash
pio run --target upload
```

**Salida esperada:** `[SUCCESS]` y el LED D4 (PC6) empezará a parpadear.

#### Pre-test recomendado: LED Heartbeat

Antes de intentar tests complejos, verifica que el STM32 arranca:

1. Cambia `build_src_filter` en `platformio.ini` a:
   ```ini
   build_src_filter = +<../examples/fase0_led_heartbeat>
   ```
2. Flashea con `pio run --target upload`.
3. El LED D4 debe parpadear a 5Hz (200ms). Si no parpadea → problema de alimentación o cableado ST-Link.
4. Restaura el `build_src_filter` original cuando quieras volver a la Fase 1.1.

---

### Paso 9 — Verificar Encoder (Fase 1.1)

> 💡 **Nota:** ¡El encoder funciona perfectamente solo con los 3.3V del ST-Link! No requiere la fuente de 24V externa.

El firmware actual de `fase1_1_mt6701_test` usa el LED D4 como diagnóstico visual (sin Serial):

1. Asegúrate de que `build_src_filter` está así en `platformio.ini`:
   ```ini
   build_src_filter = +<../examples/fase1_1_mt6701_test> +<encoder/>
   ```
2. Flashea: `pio run --target upload`
3. Observa el LED D4:

| Comportamiento LED | Significado |
|---|---|
| 3 destellos rápidos al arrancar | `setup()` completado OK |
| LED lento (700ms, ~0.7Hz) | Motor en primera mitad del giro (0°-180°) |
| LED rápido (80ms, ~6Hz) | Motor en segunda mitad del giro (180°-360°) |
| **Cambia al girar el motor** ✅ | **¡Encoder MT6701 funciona!** |
| Siempre rápido sin cambiar | SPI devuelve 0xFFFF → encoder sin alimentación |

✅ **Si el LED cambia al girar el motor, la Fase 1.1 está superada.**

### Paso 9b — Verificación del Monitor Serie (USB VCP)

> ✅ **ESTADO:** Funciona correctamente. El firmware incluye un bloque `SystemClock_Config` override en `main.cpp` que activa el oscilador interno HSI48 necesario para el periférico USB de los STM32G4.

1. Conecta el cable USB-C entre el Gripper y el PC.
2. Abre el Monitor Serie de PlatformIO (icono del enchufe 🔌 o paleta de comandos) o ejecuta:
   ```bash
   pio device monitor --baud 115200
   ```
3. Deberías ver la salida del test (ej. `MT6701 Raw (con o sin 24V): 0` o el valor que cambie al girar).

### Paso 9c — Verificar Comunicaciones RS-485 (Fase 1.4)

> 💡 **Nota:** Esta fase también funciona perfectamente solo con los 3.3V del ST-Link.

1. Cambia el `build_src_filter` en `platformio.ini` a:
   ```ini
   build_src_filter = +<../examples/fase1_4_rs485_ping/> +<comms/>
   ```
2. Flashea: `pio run --target upload`
3. Abre el Monitor Serie de PlatformIO (`pio device monitor --baud 115200`).
4. Escribe un comando válido para el ID 1, por ejemplo: `1:T:hola` (y pulsa Enter).
5. El firmware inyectará ese comando en el búfer de recepción RS-485 (Simulador Local), el parser lo procesará como un comando válido y responderá enviando la cadena `1:ACK:T` físicamente por el bus RS-485 (mostrado también en el monitor).
6. ✅ **Si recibes el ACK, el driver RS-485 y el parser de comunicaciones operan al 100%.**

---

### Paso 10 — Configurar el Agente de IA

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
├── examples/
│   ├── fase0_led_heartbeat/        ← Pre-test: LED blink sin dependencias (verificar MCU)
│   ├── fase1_1_mt6701_test/        ← Fase 1.1: Test encoder MT6701 vía SPI (debug por LED)
│   ├── fase1_2_drv_spi/            ← Fase 1.2: Test comunicación SPI con DRV8316
│   ├── fase1_3_open_loop_v_control/← Fase 1.3: Control open-loop voltaje
│   ├── fase3_2_gripper_kinematics/ ← Fase 3.2: Cinemática del gripper
│   └── fase4_2_rs485/              ← Fase 4.2: Comunicación RS-485
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

### Explicación de la Arquitectura Modular

Este proyecto está desarrollado para ser desacoplado y organizado:
* **`main.cpp` ("El Director"):** Contiene únicamente el bucle de control FOC y la configuración inicial. No se abarrota con código espagueti.
* **Archivos `.cpp` específicos ("Los Músicos"):** Cada chip tiene su archivo dedicado para sus funciones más complejas (ej. `MT6701.cpp` para leer la posición directamente manejando errores, o `DRV8316.cpp` para informar de temperatura o cortocircuitos).
* **De esta manera:** `main.cpp` da comandos básicos, y cada módulo sabe exactamente cómo tocar su instrumento, haciendo que futuros cambios de hardware sean increíblemente fáciles.

### Seguridad de Hardware y Desarrollo (⚠️ MUY IMPORTANTE)

Al desarrollar este proyecto en Antigravity, se establecen [Reglas Estrictas (RULES.md)](RULES.md):
1. **Prohibido Bloquear:** Nunca uses `delay()`, `HAL_Delay`, o bucles `while()` infinitos dentro de la ejecución FOC. SimpleFOC requiere el 100% de atención del MCU para calcular la corriente en tiempo real. Poner el MCU a "dormir" puede resultar en quemar el motor o el driver en un segundo.
2. **Herramienta `foc-hardware-setup` (Skill):** Antes de escribir código, nuestro asistente de IA tiene una herramienta específica para leer esquemáticos eléctricos de KiCad directamente (`.kicad_sch`) y generar el archivo `pins.h` perfecto sin margen de error humano. ¡Es tu amiga, úsala para verificar la asignación final de pines del proyecto!
3. **Magia de Cortex-Debug:** Con un clon de ST-Link v2 u original, la placa puede ser controlada y pausada en vivo. Si quieres descubrir por qué una variable toma cierto valor en el programa, pon un Breakpoint (punto de interrupción) en Antigravity, ¡y el código se pausará directamente en el STM32!

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

> 💡 **Nota sobre usar DFU sin ST-Link (pin PA9):**
> En los STM32 antiguos (como F1/F4), el pin `PA9` es necesario para detectar la conexión USB (VBUS sensing). En nuestro diseño, `PA9` está conectado a la Fase B del driver del motor (`PIN_BH`). ¡No te preocupes! La familia **STM32G4 tiene el sensor VBUS deshabilitado** en su Bootloader interno de fábrica (AN2606). Por lo tanto, el flasheo DFU funcionará perfectamente utilizando únicamente los pines de datos `PA11` y `PA12`, sin necesidad de cambios en el hardware.
> 
> **⚠️ PRECAUCIÓN sobre la Depuración (Debugging):**
> Ten en cuenta que el método USB DFU es una "subida a ciegas": sirve **únicamente** para volcar la imagen del código (`.bin`) en la memoria del MCU. Cuando estés en fase de desarrollo y necesites investigar un comportamiento inesperado del motor con SimpleFOC, **es indispensable usar un ST-Link (o sonda SWD)**. Un ST-Link se conecta directamente al núcleo ARM Cortex mediante `PA13/PA14`, permitiéndote pausar la ejecución (Breakpoints), leer valores de variables en tiempo real y analizar el código línea por línea en VS Code. El puerto USB no permite hacer esto.

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

### LED no parpadea después de flashear
- **Asegúrate de tener el override de reloj:** El framework STM32duino con las flags `-DUSBCON` (activadas por defecto en este proyecto) intenta inicializar el USB CDC **antes** del `setup()`. Si falta la función `SystemClock_Config(void)` activando HSI48 al final de tu archivo `main.cpp`, el firmware se cuelga en silencio.
- Prueba con `examples/fase0_led_heartbeat` (solo LED, sin dependencias) asegurándote de que compila y sube bien. Si tampoco parpadea → problema de alimentación o cableado.

### Serial Monitor no muestra datos (pantalla en blanco)
- Asegúrate de conectar al puerto COM correcto. El ST-Link genera un COM separado: ese COM siempre estará en silencio. Busca el COM del "STM32 Virtual COM Port" en el Administrador de dispositivos.
- Si acaba de flashear, Windows puede tardar un segundo en reconocer el nuevo COM del USB. Si PlatformIO abrió el monitor en el puerto antiguo, se quedará "colgado". Cancela en la terminal con `Ctrl+C` y vuelve a ejecutar `pio device monitor`.
- El `monitor_port` en `platformio.ini` NO debe tener valor fijo de Linux (`/dev/ttyACM0`). En Windows déjalo vacío para autodetección.

### IntelliSense no encuentra `Arduino.h` o cabeceras STM32
- Espera a que PlatformIO termine su primera compilación (genera el índice de includes).
- Si el problema persiste: `Ctrl+Shift+P` → "C/C++: Reset IntelliSense Database".

### Antigravity / PlatformIO no comunica con la placa
- Este es un problema conocido documentado por la comunidad. PlatformIO en Antigravity
  no tiene soporte oficial. Si la compilación funciona pero la subida falla,
  intenta ejecutar `pio run --target upload` **desde el terminal interno** en lugar de
  usar el botón UI de PlatformIO.

### PlatformIO pide constantemente "ms-vscode.cpptools" (Cirugía de dependencias)
PlatformIO a veces exige la instalación de la extensión de Microsoft aunque ya esté instalada a mano. Para solucionarlo:
1. Cierra Antigravity por completo.
2. Abre el explorador de archivos y ve a: `%USERPROFILE%\.antigravity\extensions\` (Windows) o `~/.antigravity/extensions/` (Linux/macOS).
3. Entra en la carpeta que empieza por `platformio.platformio-ide-...`.
4. Abre el archivo `package.json` con un editor de texto.
5. Busca `"extensionDependencies": ["ms-vscode.cpptools"]`.
6. Déjalo vacío de esta manera: `"extensionDependencies": []`
7. Guarda el archivo y reinicia Antigravity.

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
