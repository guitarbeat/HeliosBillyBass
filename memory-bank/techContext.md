# Technical Context: BTBillyBass

## Technology Stack

- **Hardware**: Arduino (Uno/Nano/Pro Mini), MX1508 motor drivers
- **Programming Language**: C++ (Arduino variant)
- **Development Environment**: Arduino IDE
- **Version Control**: Git
- **Documentation**: Markdown

## Development Environment

- **Arduino IDE**: Used for code development, compilation, and uploading to the Arduino board
- **Serial Monitor**: For debugging and command input at 9600 baud rate
- **Arduino Lint**: For code quality verification
- **Memory Bank**: Documentation system for project continuity

## Dependencies

- **Arduino Core**: Standard Arduino libraries for basic functionality
- **MX1508 Library**: Simple driver for the MX1508 motor controller

## Technical Constraints

- **Memory Limitations**: Arduino has limited program memory (32KB on Uno) and RAM (2KB on Uno)
- **Processing Power**: Limited by Arduino's 16MHz processor
- **PWM Resolution**: Arduino's PWM is limited to 8-bit resolution (0-255)
- **Analog Input Resolution**: 10-bit resolution (0-1023) for sound input
- **Serial Communication**: Limited to 9600 baud for reliable communication

## Hardware Configuration

- **Motors**: Two DC motors (mouth and tail) controlled via MX1508 motor driver
- **Audio Input**: Analog microphone or sound sensor connected to analog pin
- **Power Supply**: 5V for Arduino and motor driver

## Code Organization

The codebase is organized into modular components:

- **Main Sketch**: `BTBillyBass.ino` - Entry point and main loop
- **Source Directory**: `src/` - Contains all modular components
- **Configuration**: `Config.h` - Central configuration parameters
- **Motor Control**: `BillyBassMotor.h/cpp` - Low-level motor control
- **High-Level Control**: `BillyBass.h/cpp` - Fish behavior control
- **Command System**: `CommandSystem.h/cpp` - Serial command processing
- **State Machine**: `StateMachine.h/cpp` - Behavior state management

## Code Optimization Techniques

- **Function Pointers**: Used for command mapping to reduce code size
- **Const Variables**: Used for read-only data to save RAM
- **Conditional Compilation**: Debug code can be disabled to reduce program size
- **Modular Structure**: Organized code to improve maintainability without increasing memory usage
- **Early Returns**: Used to reduce nesting and improve code readability
- **Simplified Documentation**: Reduced verbosity while maintaining clarity
- **Streamlined Debug Output**: Standardized format for debug messages
- **Consolidated Constants**: Grouped related constants for better organization
- **Inline Functions**: Used for simple operations to reduce function call overhead

## Build & Deployment

1. **Compilation**: Arduino IDE compiles the code into a binary
2. **Upload**: Binary is uploaded to the Arduino board via USB
3. **Verification**: Serial monitor is used to verify correct operation
4. **Debugging**: Debug messages can be enabled in Config.h

## File References

- `BTBillyBass.ino`: Main Arduino sketch file
- `src/Config.h`: Configuration header
- `src/BillyBassMotor.h`: Motor control header
- `src/BillyBassMotor.cpp`: Motor control implementation
- `src/BillyBass.h`: High-level control header
- `src/BillyBass.cpp`: High-level control implementation
- `src/CommandSystem.h`: Command processing header
- `src/CommandSystem.cpp`: Command processing implementation
- `src/StateMachine.h`: State machine header
- `src/StateMachine.cpp`: State machine implementation
