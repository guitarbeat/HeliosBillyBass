# System Patterns: BTBillyBass

## Core Architecture

The BTBillyBass project follows a layered architecture with clear separation of concerns:

1. Hardware Layer: MX1508 motor drivers and physical components
2. Motor Control Layer: Direct motor control with calibration
3. Movement Coordination Layer: High-level movement patterns
4. User Interface Layer: Serial command processing

## Component Structure

### Core Components

- **Main Controller (BTBillyBass.ino)**:
  - Command processing
  - Calibration management
  - State tracking
  - Audio input handling

- **Configuration (src/core/)**:
  - Config.h: Movement limits, calibration defaults
  - BillyBass.h/cpp: Movement coordination
  - StateMachine.h/cpp: Audio reaction behavior

- **Hardware Interface (src/drivers/)**:
  - BillyBassMotor.h/cpp: Direct motor control
  - Basic movement operations
  - Speed control

### Movement System

#### Calibration Framework

```cpp
struct MovementCalibration {
    uint16_t mouthOpenTime;    // Open duration (ms)
    uint16_t mouthCloseTime;   // Close duration (ms)
    uint16_t bodyForwardTime;  // Forward motion time (ms)
    uint16_t bodyBackTime;     // Return motion time (ms)
    uint8_t mouthSpeed;        // Mouth motor speed (0-180)
    uint8_t bodySpeed;         // Body motor speed (0-180)
};
```

#### Command System

1. Movement Commands:
   ```
   o - Open mouth
   c - Close mouth
   f - Flap tail
   b - Body forward
   r - Reset position
   ```

2. Calibration Commands:
   ```
   t - Set mouth timing
   y - Set body timing
   m - Set mouth speed
   n - Set body speed
   p - Print settings
   ```

3. Mode Commands:
   ```
   l - Manual/auto toggle
   a - Audio reactivity
   d - Debug output
   h - Help menu
   ```

### Motor Operations

- **Direct Control**:
  - Speed setting (0-180)
  - Forward/backward movement
  - Immediate halt
  - Timed movements

- **Movement Types**:
  - Basic: open/close, forward/back
  - Complex: singing, flapping
  - Audio-reactive: dynamic response

### State Management

- **Position Tracking**:
  ```cpp
  _motorState |= MOUTH_OPEN_BIT;   // Set mouth open
  _motorState &= ~MOUTH_OPEN_BIT;  // Set mouth closed
  ```

- **Mode Management**:
  ```cpp
  fishState.manualMode = !fishState.manualMode;
  fishState.audioReactivityEnabled = !fishState.audioReactivityEnabled;
  ```

## Implementation Patterns

### Motor Control

1. Direct Movement:
   ```cpp
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

2. State Tracking:
   ```cpp
   bool BillyBass::isMouthOpen() const {
       return (_motorState & MOUTH_OPEN_BIT) != 0;
   }
   ```

### Command Processing

```cpp
void processCommand(char cmd) {
    switch (cmd) {
        case 'o': billy.openMouth(); break;
        case 'c': billy.closeMouth(); break;
        case 't': // Set timing...
        case 'm': // Set speed...
        // etc...
    }
}
```

## Technical Decisions

### Simplified Control

- Removed safety layers
- Direct motor commands
- Calibration-based timing
- User-adjustable settings

### Movement Calibration

- Independent timing control
- Separate speed settings
- Real-time adjustment
- Persistent settings

### Command Interface

- Single-character commands
- Interactive calibration
- Immediate feedback
- Mode toggles

## Future Extensibility

### Planned Features

- Configuration persistence
- Additional movement patterns
- Enhanced audio response
- Testing framework

### Design Flexibility

- Modular components
- Clear interfaces
- Documented patterns
- Easy calibration
