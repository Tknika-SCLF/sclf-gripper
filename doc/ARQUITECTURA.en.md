# Project Architecture: Antigravity IDE + PlatformIO (SCLF Gripper)

The repository is divided into four strictly isolated areas of responsibility (domains): Cognition (AI), Physics (Hardware), Environment (IDE/Compiler), and Execution (C++ Code).

```plaintext
/sclf-gripper/
├── .agent/              <- Cognition (AI, Rules and Skills)
├── .vscode/             <- Editor configuration and Debugger
├── doc/                 <- Architecture and technical documentation
├── examples/            <- Development phases (Phase 0 - Phase 7)
├── hardware/            <- Electronic design (Schematics, PCB, BOM)
├── images/              <- Documentation images
├── production/          <- Manufacturing files (Gerber, Pick&Place)
├── scripts/             <- Automation scripts (Python/Bash)
├── src/                 <- Modular Source Code
│   ├── main.cpp
│   ├── motor/           <- SimpleFOC control
│   ├── encoder/         <- MT6701 driver
│   ├── comms/           <- RS-485 and VCP
│   ├── faults/          <- Safety manager
│   └── config/          <- Hardware configuration (pins.h)
├── .clang-format        <- Code style rules
├── AGENT.md             <- AI parametric instructions
├── MEMORY.md            <- Architectural decision log
├── platformio.ini       <- Build engine (PlatformIO)
├── README.md            <- General project introduction
├── RULES.md             <- Critical development and safety rules
├── SRS.md               <- Software Requirements Specification
└── TASKS.md             <- Roadmap and development status
```

## 1. Cognition Domain (AI Engine)

Defines the reasoning limits, safety protocols, and procedural capabilities of the autonomous agent.

*   `.agent/rules/hardware_safety.md`: Contains the unbreakable laws of the project. The agent constantly evaluates this file (`always_on: true`). It blocks the use of synchronous code (`delay`) and requires hardware validation before suggesting logic.
*   `.agent/skills/foc-hardware-setup/SKILL.md`: Defines the step-by-step Standard Operating Procedure (SOP). The AI must analyze KiCad schematics and extract hardware mapping empirically before writing code.
*   `AGENT.md`: Establishes the AI's parametric role (Senior FOC Firmware Engineer).
*   `MEMORY.md`: Log of architectural decisions already made.
*   `RULES.md`: Critical development and safety rules.
*   `SRS.md`: Software technical requirements specification.
*   `TASKS.md`: Project status and roadmap tracking.

## 2. Physical Domain (Hardware and Reference)

Source of absolute truth. The agent has only analytical read permissions over this section.

*   `hardware/`: Original electronic design files (KiCad schematics and PCB) and bill of materials (`bom.csv`).
*   `production/`: Files intended for manufacturing (Gerber, Pick&Place).
*   `doc/`: Technical documentation and architecture guides storage.

## 3. Environment and Compilation Domain

Infrastructure required to transition code from software to the microcontroller without dependency errors.

*   `.vscode/`: Editor interface and debugger configuration.
*   `platformio.ini`: The project's build engine. Defines the exact target, framework, and dependencies.

## 4. Execution Domain (Source Code)

Final product resulting from development iterations.

*   `src/`: Modularly organized directory. Separates responsibilities for easier maintenance.
*   `examples/`: Development milestones divided by phases, allowing each component to be validated in isolation.
*   `lib/`: Container for low-level or external C++ libraries.
