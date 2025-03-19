# BTBillyBass

An Arduino-based controller for a Big Mouth Billy Bass animatronic fish with audio reactivity.

## Project Overview

This project transforms a Big Mouth Billy Bass animatronic fish into an interactive device that responds to audio input. The fish can move its mouth and body in response to sound, or be controlled manually through serial commands.

## Hardware Requirements

- Arduino board (tested on Uno/Nano)
- 2x MX1508 motor drivers
- Big Mouth Billy Bass toy
- Audio input (microphone or sound sensor) connected to A0

## Pin Configuration

- **A0**: Audio input
- **D3/D5**: Mouth motor
- **D6/D9**: Body/tail motor

## Features

- Audio-reactive movement
- Manual control via serial commands
- Calibration system for timing and speed
- State machine for autonomous behavior
- Debug mode for troubleshooting

## Initial Calibration

### Default Settings
```cpp
// Movement Timing
mouthOpenTime = 400ms    // Default mouth open duration
mouthCloseTime = 400ms   // Default mouth close duration
bodyForwardTime = 800ms  // Default body forward time
bodyBackTime = 800ms     // Default body return time

// Speed Settings
mouthSpeed = 100         // Default mouth motor speed (0-180)
bodySpeed = 100         // Default body motor speed (0-180)
```

### Calibration Process

1. **Initial Setup**:
   - Upload the code with default settings
   - Open Serial Monitor (9600 baud)
   - Type 'd' to enable debug mode
   - Type 'p' to view current settings

2. **Basic Movement Test**:
   ```
   o -> c  : Test mouth (open/close, 400ms each)
   f -> b  : Test body (flap/forward, 800ms each)
   r      : Reset position
   ```

3. **Adjust Settings** (if needed):
   - Mouth Timing: `t` (enter open,close times in ms)
   - Body Timing: `y` (enter forward,back times in ms)
   - Mouth Speed: `m` (enter speed 0-180)
   - Body Speed: `n` (enter speed 0-180)

4. **Fine-Tuning Tips**:
   - Start with lower speeds (around 100)
   - Use shorter durations initially (200-300ms)
   - Increase values gradually until movement is smooth
   - Test each movement thoroughly before proceeding

## Commands

### Movement Controls
- **o**: Open mouth
- **c**: Close mouth
- **f**: Flap tail
- **b**: Body forward
- **r**: Reset position
- **s**: Singing motion

### Calibration Commands
- **t**: Set mouth timing (open,close)
- **y**: Set body timing (forward,back)
- **m**: Set mouth speed
- **n**: Set body speed
- **p**: Print current settings

### Mode Controls
- **a**: Toggle audio reactivity
- **l**: Toggle manual/auto mode
- **d**: Toggle debug mode
- **h**: Show help menu

## Getting Started

1. Connect the hardware according to the pin configuration
2. Upload the sketch to your Arduino board
3. Open the Serial Monitor at 9600 baud
4. Follow the Initial Calibration process above
5. Type 'h' to see all available commands

## Development

The project follows a modular architecture with clear separation of concerns:

- **Core**: Central functionality and state management
- **Commands**: Serial command processing
- **Drivers**: Hardware abstraction layer
- **Utils**: Utility functions and helpers

## License

This project is open source and available under the MIT License. 