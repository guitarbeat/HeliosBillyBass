# File References: BTBillyBass

This document serves as a comprehensive index that maps all project files to their corresponding Memory Bank documentation.

## Source Files

| File | Description | Memory Bank References |
|------|-------------|------------------------|
| `BTBillyBass.ino` | Main Arduino sketch file | [systemPatterns.md](systemPatterns.md), [techContext.md](techContext.md), [activeContext.md](activeContext.md) |
| `src/core/Config.h` | Configuration header with pin definitions and global variables | [systemPatterns.md](systemPatterns.md), [techContext.md](techContext.md) |
| `src/core/Constants.h` | Constants header with state definitions and timing values | [systemPatterns.md](systemPatterns.md), [techContext.md](techContext.md) |
| `src/drivers/BillyBassMotor.h` | Motor control header with simplified interface | [systemPatterns.md](systemPatterns.md), [techContext.md](techContext.md) |
| `src/drivers/BillyBassMotor.cpp` | Motor control implementation with optimized methods | [systemPatterns.md](systemPatterns.md), [techContext.md](techContext.md) |
| `src/core/BillyBass.h` | High-level control header with streamlined documentation | [systemPatterns.md](systemPatterns.md), [techContext.md](techContext.md) |
| `src/core/BillyBass.cpp` | High-level control implementation with improved logic | [systemPatterns.md](systemPatterns.md), [techContext.md](techContext.md) |
| `src/commands/CommandSystem.h` | Command processing header with simplified structure | [systemPatterns.md](systemPatterns.md), [techContext.md](techContext.md) |
| `src/commands/CommandSystem.cpp` | Command processing implementation with condensed actions | [systemPatterns.md](systemPatterns.md), [techContext.md](techContext.md) |
| `src/core/StateMachine.h` | State machine header with minimal documentation | [systemPatterns.md](systemPatterns.md), [techContext.md](techContext.md) |
| `src/core/StateMachine.cpp` | State machine implementation with optimized transitions | [systemPatterns.md](systemPatterns.md), [techContext.md](techContext.md) |
| `src/utils/Debug.h` | Debug utilities for consistent logging | [systemPatterns.md](systemPatterns.md), [techContext.md](techContext.md) |
| `README.md` | Project documentation and structure overview | All memory bank files |

## Configuration Files

| File | Description | Memory Bank References |
|------|-------------|------------------------|
| `.arduino-lint.yml` | Arduino linting configuration | [techContext.md](techContext.md) |
| `.gitignore` | Version control exclusions | [techContext.md](techContext.md) |

## Documentation Files

| File | Description | Related Files |
|------|-------------|---------------|
| `memory-bank/projectbrief.md` | Project overview and requirements | All project files |
| `memory-bank/productContext.md` | Product context and user experience | All project files |
| `memory-bank/systemPatterns.md` | System architecture and design patterns | All source files |
| `memory-bank/techContext.md` | Technical details and constraints | All source files |
| `memory-bank/activeContext.md` | Current focus and recent changes | All source files |
| `memory-bank/progress.md` | Project progress and known issues | All source files |
| `memory-bank/fileReferences.md` | File reference index | All project files |
| `.projectrules` | Project rules and patterns | All project files |

## Module Relationships

- **Main Controller** (`BTBillyBass.ino`): Integrates all modules
- **Core** (`src/core/`): Contains central functionality
  - **Configuration** (`Config.h`): Referenced by all modules
  - **Constants** (`Constants.h`): Referenced by all modules
  - **BillyBass** (`BillyBass.h/cpp`): High-level fish control
  - **StateMachine** (`StateMachine.h/cpp`): Behavior state management
- **Drivers** (`src/drivers/`): Hardware abstraction layer
  - **BillyBassMotor** (`BillyBassMotor.h/cpp`): Motor control
- **Commands** (`src/commands/`): Command processing
  - **CommandSystem** (`CommandSystem.h/cpp`): Serial command handling
- **Utils** (`src/utils/`): Utility functions
  - **Debug** (`Debug.h`): Debug utilities

## File Dependency Graph

```
BTBillyBass.ino
├── src/core/Config.h
│   └── src/core/Constants.h
├── src/core/BillyBass.h
│   ├── src/drivers/BillyBassMotor.h
│   └── src/core/Config.h
├── src/core/StateMachine.h
│   ├── src/core/Config.h
│   └── src/core/BillyBass.h
├── src/commands/CommandSystem.h
│   └── src/core/BillyBass.h
└── src/utils/Debug.h
    └── src/core/Config.h
```

## Recent Optimizations

- **Subfolder Structure**: Organized source files into logical subfolders
- **Debug Utilities**: Created centralized debug utilities for consistent logging
- **Include Path Updates**: Updated include paths to reflect the new folder structure
- **README Documentation**: Added comprehensive README with project structure documentation
- **Config.h**: Separated configuration from constants for better organization
- **Constants.h**: Created dedicated file for constants to improve maintainability
- **BillyBassMotor.h/cpp**: Simplified motor control methods and removed unused functionality
- **BillyBass.h/cpp**: Added helper methods and used arrays for pattern settings
- **CommandSystem.h/cpp**: Improved menu display and standardized command function names
- **StateMachine.h/cpp**: Consolidated state transition logging and simplified state management
- **Bit Masks**: Replaced magic bit masks with named constants for better readability
