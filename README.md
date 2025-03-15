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
- State machine for autonomous behavior
- Configurable settings
- Debug mode for troubleshooting

## Project Structure

```
BTBillyBass/
├── BTBillyBass.ino       # Main Arduino sketch
├── memory-bank/          # Project documentation
├── src/                  # Source code
│   ├── core/             # Core functionality
│   │   ├── BillyBass.cpp # High-level fish control
│   │   ├── BillyBass.h   # High-level fish control header
│   │   ├── Config.h      # Configuration settings
│   │   ├── Constants.h   # Constant definitions
│   │   ├── StateMachine.cpp # State machine implementation
│   │   └── StateMachine.h   # State machine header
│   ├── commands/         # Command processing
│   │   ├── CommandSystem.cpp # Command implementation
│   │   └── CommandSystem.h   # Command system header
│   ├── drivers/          # Hardware drivers
│   │   ├── BillyBassMotor.cpp # Motor control implementation
│   │   └── BillyBassMotor.h   # Motor control header
│   └── utils/            # Utility functions
│       └── Debug.h       # Debug utilities
└── .arduino-lint.yml     # Arduino linting configuration
```

## Commands

- **o**: Open mouth
- **c**: Close mouth
- **f**: Flap tail
- **b**: Body forward
- **r**: Reset to home position
- **s**: Singing motion
- **+/-**: Increase/decrease speed
- **a**: Toggle audio reactivity
- **m**: Toggle manual/auto mode
- **d**: Toggle debug mode
- **h**: Show help menu

## Getting Started

1. Connect the hardware according to the pin configuration
2. Upload the sketch to your Arduino board
3. Open the Serial Monitor at 9600 baud
4. Type 'h' to see the available commands

## Development

The project follows a modular architecture with clear separation of concerns:

- **Core**: Central functionality and state management
- **Commands**: Serial command processing
- **Drivers**: Hardware abstraction layer
- **Utils**: Utility functions and helpers

## License

This project is open source and available under the MIT License. 