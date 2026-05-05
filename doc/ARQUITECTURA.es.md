# Arquitectura del Proyecto: Antigravity IDE + PlatformIO (SCLF Gripper)

El repositorio está dividido en cuatro áreas de responsabilidad (dominios) estrictamente aisladas: Cognición (IA), Física (Hardware), Entorno (IDE/Compilador) y Ejecución (Código C++).

```plaintext
/sclf-gripper/
├── .agent/              <- Cognición (IA, Reglas y Skills)
├── .vscode/             <- Configuración del editor y Debugger
├── doc/                 <- Arquitectura y documentación técnica
├── examples/            <- Fases de desarrollo (Phase 0 - Phase 7)
├── hardware/            <- Diseño electrónico (Esquemáticos, PCB, BOM)
├── images/              <- Imágenes para documentación
├── production/          <- Archivos para fabricación (Gerber, Pick&Place)
├── scripts/             <- Scripts de automatización (Python/Bash)
├── src/                 <- Código Fuente Modular
│   ├── main.cpp
│   ├── motor/           <- Control SimpleFOC
│   ├── encoder/         <- Driver MT6701
│   ├── comms/           <- RS-485 y VCP
│   ├── faults/          <- Gestor de seguridad
│   └── config/          <- Configuración de hardware (pins.h)
├── .clang-format        <- Reglas de estilo de código
├── AGENT.md             <- Instrucciones paramétricas para la IA
├── MEMORY.md            <- Registro de decisiones arquitectónicas
├── platformio.ini       <- Motor de construcción (PlatformIO)
├── README.md            <- Introducción general al proyecto
├── RULES.md             <- Reglas críticas de desarrollo y seguridad
├── SRS.md               <- Especificación de Requisitos de Software
└── TASKS.md             <- Hoja de ruta y estado del desarrollo (Roadmap)
```

## 1. Dominio de Cognición (Motor de IA)

Define los límites de razonamiento, protocolos de seguridad y capacidades procedimentales del agente autónomo.

*   `.agent/rules/hardware_safety.md`: Contiene las leyes inquebrantables del proyecto. El agente evalúa este archivo constantemente (`always_on: true`). Bloquea el uso de código síncrono (`delay`) y exige validación de hardware antes de sugerir lógica.
*   `.agent/skills/foc-hardware-setup/SKILL.md`: Define el Procedimiento Estándar de Operación (SOP) paso a paso. La IA debe analizar los esquemáticos de KiCad y extraer el mapeo de hardware de forma empírica antes de escribir código.
*   `AGENT.md`: Establece el rol paramétrico de la IA (Ingeniero Senior de Firmware FOC).
*   `MEMORY.md`: Registro de decisiones arquitectónicas ya tomadas.
*   `RULES.md`: Reglas críticas de desarrollo y seguridad.
*   `SRS.md`: Especificación de requisitos técnicos del software.
*   `TASKS.md`: Seguimiento del estado del proyecto y la hoja de ruta (roadmap).

## 2. Dominio Físico (Hardware y Referencia)

Fuente de verdad absoluta. El agente solo tiene permisos de lectura analítica sobre esta sección.

*   `hardware/`: Archivos originales del diseño electrónico (Esquemáticos KiCad y PCB) y lista de materiales (`bom.csv`).
*   `production/`: Archivos destinados a la fabricación (Gerber, Pick&Place).
*   `doc/`: Almacén de documentación técnica y guías de arquitectura.

## 3. Dominio de Entorno y Compilación

Infraestructura necesaria para transicionar el código desde el software al microcontrolador sin errores de dependencia.

*   `.vscode/`: Configuración de la interfaz del editor y el depurador (debugger).
*   `platformio.ini`: El motor de construcción del proyecto. Define el target exacto, el framework y las dependencias.

## 4. Dominio de Ejecución (Código Fuente)

Producto final resultante de las iteraciones de desarrollo.

*   `src/`: Directorio organizado de forma modular. Separa las responsabilidades para facilitar el mantenimiento.
*   `examples/`: Hitos de desarrollo por fases, permitiendo validar cada componente de forma aislada.
*   `lib/`: Contenedor para librerías de bajo nivel o externas.
