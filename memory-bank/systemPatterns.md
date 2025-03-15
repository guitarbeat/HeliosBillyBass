# System Patterns: BTBillyBass

## Architecture Overview

The BTBillyBass project follows a modular architecture with clear separation of concerns. The system is built around a state machine that manages the fish's behavior based on audio input and timing. The architecture consists of several key components organized into logical subfolders that work together to create a responsive and interactive animatronic fish.

## Key Components

- **Main Controller (BTBillyBass.ino)**: Central orchestration point that initializes components and manages the main program loop.
- **Core Components (src/core/)**:
  - **Configuration (Config.h)**: Centralized configuration parameters including pin definitions and global variable declarations.
  - **Constants (Constants.h)**: Centralized constant definitions including state definitions, timing values, and motor settings.
  - **BillyBass Controller (BillyBass.h/cpp)**: High-level control interface for the fish's movements and behaviors.
  - **State Machine (StateMachine.h/cpp)**: Manages the fish's behavior states (waiting, talking, flapping) based on audio input and timing.
- **Drivers (src/drivers/)**:
  - **Motor Control (BillyBassMotor.h/cpp)**: Low-level motor control abstraction for managing the physical motors.
- **Commands (src/commands/)**:
  - **Command System (CommandSystem.h/cpp)**: Processes serial commands using function pointers for efficient command mapping.
- **Utilities (src/utils/)**:
  - **Debug Utilities (Debug.h)**: Centralized debug functionality for consistent logging.

## Design Patterns

- **State Pattern**: Used in the StateMachine to manage different behavioral states of the fish.
- **Command Pattern**: Implemented in the CommandSystem for processing serial commands.
- **Singleton Pattern**: Used for global state management across components.
- **Factory Method**: Used in motor control initialization.
- **Observer Pattern**: Implicit in the audio reactivity system where state changes based on audio input.
- **Early Return Pattern**: Used to reduce nesting and improve code readability.
- **Conditional Compilation**: Used for debug output to reduce code size in production.
- **Centralized Logging Pattern**: Consolidated state transition logging to reduce code duplication.
- **Minimal Interface Pattern**: Removed unused methods to simplify class interfaces.
- **Standardized Naming Pattern**: Used consistent naming conventions for command functions.
- **Helper Method Pattern**: Added helper methods to reduce code duplication.
- **Lookup Table Pattern**: Used arrays for pattern settings to simplify complex methods.
- **Named Constants Pattern**: Used named constants for bit masks and state values.
- **Facade Pattern**: The BillyBass class provides a simplified interface to the underlying motor control.
- **Module Pattern**: Organized related functionality into logical subfolders.
- **Dependency Injection**: Components receive their dependencies through constructor parameters or global variables.

## Data Flow

1. **Input Processing**:
   - Audio input is captured through an analog pin and processed by the StateMachine
   - Serial commands are received and processed by the CommandSystem

2. **State Management**:
   - The StateMachine determines the appropriate state based on inputs
   - State transitions trigger different behaviors

3. **Motor Control**:
   - High-level commands from BillyBass are translated to motor movements
   - BillyBassMotor handles the low-level PWM control of the motors

4. **Feedback Loop**:
   - Debug information is sent back through Serial for monitoring
   - State information is maintained for decision making

## Technical Decisions

- **Modular Code Structure**: Code is split into logical components with clear responsibilities to improve maintainability and readability.
- **Subfolder Organization**: Source files are organized into logical subfolders (core, drivers, commands, utils) for better navigation and maintainability.
- **Header File Organization**: Each component has a header file with proper include guards to prevent multiple inclusions.
- **Function Pointer Command System**: Using function pointers for command mapping instead of if-else chains for better performance and maintainability.
- **State-Based Behavior**: Using a state machine to manage the fish's behavior provides a clear and maintainable way to handle complex interactions.
- **Debug Mode**: Comprehensive debugging capability with conditional compilation to reduce code size in production.
- **Constants Extraction**: Magic numbers replaced with named constants for better code readability and maintenance.
- **Global State Management**: Using global variables for state that needs to be shared across components, with clear documentation of their purpose.
- **Simplified Documentation**: Reduced verbosity in header files for better readability while maintaining clarity.
- **Streamlined Debug Output**: Standardized debug messages with shorter, clearer format.
- **Consolidated Constants**: Grouped related constants for better organization and maintainability.
- **Early Returns**: Used early returns in complex methods to reduce nesting and improve readability.
- **Redundancy Elimination**: Systematically identified and removed redundant code patterns.
- **Minimal Interfaces**: Removed unused methods to simplify class interfaces and reduce program size.
- **Centralized State Logging**: Consolidated state transition logging to a single location for better maintainability.
- **Constants Separation**: Separated constants from configuration for better organization and maintainability.
- **Helper Methods**: Added helper methods to reduce code duplication and improve maintainability.
- **Pattern Arrays**: Used arrays for pattern settings to simplify complex methods and improve readability.
- **Bit Mask Constants**: Used named constants for bit masks to improve code readability and maintainability.
- **Centralized Debug Utilities**: Created a dedicated Debug.h file for consistent logging across the codebase.
- **Relative Include Paths**: Used relative include paths to maintain proper dependencies between components.

## File References

- `BTBillyBass.ino`: Main Arduino sketch file
- `src/core/Config.h`: Configuration header with pin definitions and global variable declarations
- `src/core/Constants.h`: Constants header with state definitions, timing values, and motor settings
- `src/core/BillyBass.h`: High-level control header
- `src/core/BillyBass.cpp`: High-level control implementation
- `src/core/StateMachine.h`: State machine header
- `src/core/StateMachine.cpp`: State machine implementation
- `src/drivers/BillyBassMotor.h`: Motor control header
- `src/drivers/BillyBassMotor.cpp`: Motor control implementation
- `src/commands/CommandSystem.h`: Command processing header
- `src/commands/CommandSystem.cpp`: Command processing implementation
- `src/utils/Debug.h`: Debug utilities header

## Movement Patterns

1. Basic Movements:
   - Open/Close Mouth: Controlled movement with position tracking
   - Flap Tail/Body Forward: Coordinated movement with state management

2. Complex Sequences:
   - Singing Motion: Coordinated mouth and body movements
   - Reset Sequence: Safe return to home position

3. Speed Control:
   - Speed Adjustment: 5-unit increments within defined limits
