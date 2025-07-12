# BTBillyBass - Arduino Animatronic Control System

## Overview

BTBillyBass is an Arduino-based control system for Big Mouth Billy Bass animatronics. It provides both manual control via serial commands and automatic audio-responsive movements. The system features precise motor control, timing management, and intelligent audio processing for natural-looking movements.

## Features

### 🎯 Core Functionality
- **Manual Control**: Serial command interface for precise motor control
- **Audio-Responsive Mode**: Automatic mouth and head movements based on audio input
- **Timed Actions**: Configurable movement durations with automatic position holding
- **Memory Efficient**: Uses PROGMEM for string storage to conserve Arduino memory
- **Auto-Calibration**: Intelligent audio level calibration for optimal sensitivity

### 🎮 Control Modes
- **Manual Mode**: Direct control via serial commands
- **Yap Mode**: Audio-responsive automation with configurable sensitivity
- **Hold Mode**: Position holding with automatic timeout

### 🎵 Audio Processing
- Real-time audio level monitoring
- Adaptive threshold calibration
- Energy-based movement triggering
- Silence detection and neutral position return

## Hardware Requirements

### Essential Components
- **Arduino Board**: Any Arduino with at least 4 PWM pins (Uno, Nano, Mega, etc.)
- **MX1508 H-bridge Motor Drivers**: 2x for body and mouth motor control
- **Big Mouth Billy Bass**: Or compatible animatronic fish
- **5V Power Supply**: For motor operation
- **Audio Input Circuit**: Microphone with amplification (connected to A0)

### Pin Configuration
| Component | Arduino Pin | Function |
|-----------|-------------|----------|
| Body Motor | Pins 6, 9 | Head/tail movement control |
| Mouth Motor | Pins 5, 3 | Mouth opening/closing control |
| Audio Input | Pin A0 | Sound level detection |
| Random Seed | Pin A1 | Unconnected pin for random number generation |

## Software Features

### Motor Control System
- **Body Motor**: Controls head/tail movements with forward/backward positioning
- **Mouth Motor**: Controls mouth opening/closing with spring-assisted closing
- **Speed Control**: Configurable speeds for different movement types
- **Position Holding**: Automatic holding with timeout protection

### Audio Processing Engine
- **Real-time Monitoring**: Continuous audio level sampling
- **Adaptive Calibration**: Automatic threshold adjustment based on ambient noise
- **Energy Accumulation**: Tracks audio energy for movement decisions
- **Peak Detection**: Identifies important audio moments for enhanced movements

### Command Interface
- **Serial Communication**: 9600 baud rate
- **Case-Insensitive**: Commands work regardless of capitalization
- **Flexible Timing**: Custom duration parameters for all movements
- **Help System**: Built-in command reference

## Installation & Setup

### 1. Hardware Assembly
```bash
# Connect motors to MX1508 drivers
Body Motor → MX1508 Driver 1 → Arduino Pins 6, 9
Mouth Motor → MX1508 Driver 2 → Arduino Pins 5, 3

# Connect audio input
Microphone → Amplifier → Arduino Pin A0

# Power connections
5V Supply → MX1508 Drivers → Motors
```

### 2. Software Installation
1. Install Arduino IDE
2. Install required libraries:
   - MX1508 Motor Driver Library
3. Open `BTBillyBass.ino` in Arduino IDE
4. Select appropriate board and port
5. Upload code to Arduino

### 3. Initial Testing
1. Open Serial Monitor (9600 baud)
2. Type `help` to see available commands
3. Test basic movements: `body forward`, `mouth open`
4. Test yap mode: `yap`

## Usage Guide

### Manual Control Commands

#### Body Movements
```bash
body forward [duration]    # Move head/tail forward (default: 800ms)
body backward [duration]   # Move head/tail backward (default: 1000ms)
body release               # Release body motor
body hold                  # Hold current position
```

#### Mouth Movements
```bash
mouth open [duration]      # Open mouth (default: 120ms)
mouth close                # Close mouth (spring-assisted)
mouth release              # Release mouth motor
mouth hold                 # Hold current position
```

#### System Commands
```bash
yap [threshold]            # Start audio-responsive mode
stop                       # Stop all motors and exit yap mode
help                       # Display command reference
```

### Audio-Responsive Mode (Yap Mode)

#### Activation
```bash
yap              # Start with default threshold
yap 50           # Start with custom threshold (50)
```

#### Behavior
- **Mouth Movements**: Opens when audio detected, closes when audio drops
- **Head Movements**: 
  - Starts in singing position (head down)
  - Moves to neutral after extended silence
  - Responds to important audio moments with forward movements
  - Varies between singing positions based on audio energy

#### Calibration
- Automatic 3-second calibration on startup
- Adjusts threshold based on ambient noise level
- Minimum threshold prevents over-sensitivity

### Timing Parameters

#### Default Timings
- **Mouth Open**: 120ms
- **Body Forward**: 800ms
- **Body Backward**: 1000ms
- **Tail Movement**: 400ms
- **Hold Timeout**: 500ms

#### Custom Timing Examples
```bash
body forward 1500    # Move forward for 1.5 seconds
mouth open 200       # Open mouth for 200ms
body backward 500    # Move backward for 500ms
```

## Technical Details

### Motor Control Architecture
- **MX1508 Drivers**: H-bridge control for bidirectional motor operation
- **PWM Speed Control**: Variable speed for different movement types
- **Position Holding**: Reduced power mode for maintaining positions
- **Safety Features**: Automatic timeout and release mechanisms

### Audio Processing Algorithm
```cpp
// Key parameters
const float AUDIO_DROP_FACTOR = 0.7;     // 70% drop detection
const float LOUD_SOUND_FACTOR = 1.5;     // 150% threshold for loud sounds
const int MOUTH_DEBOUNCE_TIME = 110;     // Minimum time between movements
const int SILENCE_TIMEOUT = 3000;        // 3s silence before neutral position
```

### Memory Management
- **PROGMEM Strings**: All help text and messages stored in program memory
- **Efficient Buffers**: 32-byte command buffer with overflow protection
- **Static Variables**: State management without dynamic allocation

### State Machine Design
- **Non-blocking Operation**: All movements are non-blocking
- **Timing Management**: Precise millisecond timing for movements
- **Mode Switching**: Seamless transition between manual and yap modes

## Troubleshooting

### Common Issues

#### Motors Not Responding
1. Check power supply connections
2. Verify MX1508 driver connections
3. Test individual motor commands
4. Check Serial Monitor for error messages

#### Audio Not Working
1. Verify microphone and amplifier connections
2. Check audio input on pin A0
3. Run calibration: `yap` command
4. Adjust threshold if needed: `yap [value]`

#### Unusual Movements
1. Check motor wiring polarity
2. Verify pin assignments
3. Test with shorter durations
4. Use `stop` command to reset system

### Performance Optimization
- **Speed Adjustment**: Modify `BODY_SPEED` and `MOUTH_SPEED` constants
- **Timing Tuning**: Adjust default timing values for your specific Billy Bass
- **Audio Sensitivity**: Fine-tune `silenceThreshold` for your environment

## Development

### Code Structure
```
BTBillyBass.ino
├── Hardware Configuration
├── State Management
├── Motor Control Functions
├── Audio Processing
├── Command Processing
└── Utility Functions
```

### Key Functions
- `setup()`: System initialization
- `loop()`: Main program loop
- `processCommand()`: Serial command parsing
- `processYap()`: Audio-responsive mode
- `timedMotorAction()`: Timed movement control
- `calibrateAudio()`: Audio level calibration

### Extending the System
- **Additional Motors**: Add new motor types to the system
- **New Commands**: Extend command processing in `processCommand()`
- **Audio Features**: Enhance audio processing in yap mode
- **External Control**: Add Bluetooth or WiFi connectivity

## License

MIT License - See LICENSE file for details.

## Credits

- **Original Code**: Jordan Bunker <jordan@hierotechnics.com> 2019
- **Modifications**: Enhanced for manual control with timed actions
- **Hardware**: Big Mouth Billy Bass animatronic
- **Libraries**: MX1508 motor driver library

## Support

For issues, questions, or contributions:
1. Check the troubleshooting section
2. Review the command reference (`help`)
3. Test with basic commands first
4. Verify hardware connections

---

**Happy Fishing! 🐟**