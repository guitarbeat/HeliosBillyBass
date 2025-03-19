# Technical Context: Billy Bass Control System

## Technology Stack

- **Hardware**:
  - Arduino Nano/Uno
  - 2x MX1508 motor drivers
  - Big Mouth Billy Bass animatronic
  - Audio input circuit
- **Software**:
  - Arduino C++
  - MX1508 motor driver library
  - Custom motor control classes

## Development Environment

- **Arduino IDE**: Used for code development, compilation, and uploading to the Arduino board
- **Serial Monitor**: For debugging and command input at 9600 baud rate
- **Debug output enabled**: For detailed output during development

## Dependencies

- **MX1508 Library**: Simple driver for the MX1508 motor controller
- **Arduino Standard Library**: Standard Arduino libraries for basic functionality
- **AVR Standard Library**: Standard libraries for AVR processors

## Technical Constraints

- **Memory Limitations**: Arduino has limited program memory (32KB on Uno) and RAM (2KB on Uno)
- **Processing Power**: Limited by Arduino's 16MHz processor
- **PWM Resolution**: Arduino's PWM is limited to 8-bit resolution (0-255)
- **Analog Input Resolution**: 10-bit resolution (0-1023) for sound input
- **Serial Communication**: Limited to 9600 baud for reliable communication
- **Maximum motor speed**: 180 (reduced from 255)
- **Maximum movement time**: 1000ms
- **PWM pins required for motor control**: For controlling the motors

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

### Compilation

- **Arduino IDE**: Used for compiling the code
- **Standard Arduino AVR compilation**: For generating the binary file
- **Debug mode enabled by default**: For detailed output during development

### Upload Process

1. **Select Arduino Nano/Uno board**: For the specific hardware
2. **Choose correct COM port**: For connecting the Arduino to the computer
3. **Upload sketch**: The binary file is transferred to the Arduino board
4. **Open Serial Monitor at 9600 baud**: For verifying correct operation and debugging

### Initial Setup

1. **Upload code**: The binary file is uploaded to the Arduino board
2. **Open Serial Monitor**: For sending commands and verifying initial setup
3. **Send 'h' for help menu**: For accessing available commands and options
4. **Calibrate movements**:
   - **Start with low speeds**: For testing and adjusting
   - **Test each movement**: To ensure functionality
   - **Adjust timing as needed**: For optimal performance
   - **Save working values**: For consistent results

### Testing Process

1. **Enable debug output**: For detailed information during testing
2. **Test each movement individually**: To verify timing and speeds
3. **Verify timing and speeds**: Ensure correct operation
4. **Adjust calibration as needed**: For fine-tuning performance

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

## Motor Control System

### Direct Control Implementation

```cpp
// Basic motor control pattern
void BillyBass::openMouth() {
    if (!isMouthOpen()) {
        mouthMotor.setSpeed(calibration.mouthSpeed);
        mouthMotor.forward();
        delay(calibration.mouthOpenTime);
        mouthMotor.halt();
        _motorState |= MOUTH_OPEN_BIT;
    }
}
```

### Calibration System

- **Movement Timing**:
  - Mouth open/close: 0-1000ms
  - Body forward/back: 0-1000ms
- **Speed Control**:
  - Mouth: 0-180
  - Body: 0-180
- **Default Values**:
  - Mouth timing: 400ms
  - Body timing: 600ms
  - Mouth speed: 150
  - Body speed: 120

### Serial Commands

``` mermaid
Basic Movement:
o - Open mouth
c - Close mouth
f - Flap tail
b - Body forward
r - Reset position

Configuration:
+ - Speed up
- - Speed down
a - Toggle audio mode
m - Toggle manual mode
d - Toggle debug mode
```
