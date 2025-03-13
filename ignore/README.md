# Alexa-Enabled Big Mouth Billy Bass

![Billy Bass with Alexa](https://via.placeholder.com/800x400?text=Billy+Bass+with+Alexa)

## Project Overview

This project transforms a classic Big Mouth Billy Bass animatronic fish into a voice-responsive device that works with Amazon Alexa. The fish's mouth and body movements are synchronized with audio input, creating the illusion that the fish is speaking and responding to commands.

### Features

- Audio-reactive mouth movements that sync with speech
- Randomized body movements for natural-looking animation
- State-based control system for different fish behaviors
- Idle animations when no audio is detected
- Integration with Amazon Alexa (in progress)

### Tech Stack

- Arduino-based control system
- Custom MX1508 motor driver library
- H-bridge motor controllers
- Analog audio input processing
- Amazon Alexa integration (planned)

## Hardware Requirements

- Big Mouth Billy Bass animatronic fish
- Arduino board (Uno, Nano, or similar)
- MX1508 or similar H-bridge motor driver
- Audio input circuit (details in wiring section)
- Power supply (5-12V depending on your motors)
- Jumper wires and connectors
- Optional: Amazon Echo device for Alexa integration

## Software Dependencies

- Arduino IDE
- [MX1508 Library](https://github.com/yourusername/MX1508) - For controlling the H-bridge motor drivers
- Additional libraries for Alexa integration (to be added)

## Installation

1. Clone this repository:
   ```
   git clone https://github.com/yourusername/BTBillyBass.git
   ```

2. Install the required libraries:
   - Download the MX1508 library and place it in your Arduino libraries folder
   - Additional steps for Alexa integration will be added

3. Connect your hardware according to the wiring diagram
   
4. Upload the BTBillyBass.ino sketch to your Arduino

## Wiring Diagram

```
Arduino Pin | Connection
------------|------------
D3          | Mouth Motor Input 2
D5          | Mouth Motor Input 1
D6          | Body Motor Input 1
D9          | Body Motor Input 2
A0          | Audio Input
GND         | Common Ground
5V          | Logic Power
```

![Wiring Diagram](https://via.placeholder.com/800x600?text=Wiring+Diagram)

## How It Works

### State Machine

The fish operates using a simple state machine with three states:

1. **WAITING (State 0)**: The fish is idle, monitoring audio input. If no activity is detected for a while, it transitions to the "flapping" state.

2. **TALKING (State 1)**: When audio is detected above the silence threshold, the fish opens its mouth and makes random body movements to appear animated.

3. **FLAPPING (State 2)**: A brief animation where the fish flaps to show it's "bored" before returning to the waiting state.

### Audio Processing

The system monitors analog input on pin A0, which should be connected to an audio source. When the audio level exceeds the defined silence threshold, the fish's mouth opens. As the audio level drops, the mouth closes, creating a synchronized talking effect.

### Motor Control

Two motors are controlled via the MX1508 H-bridge driver:
- **Mouth Motor**: Controls the opening and closing of the fish's mouth
- **Body Motor**: Controls the body/tail movement for added personality

## Configuration

You can adjust the following parameters in the code:

- `silence`: Threshold for audio detection (default: 12)
- Motor speeds for mouth and body movements
- Timing parameters for animations
- Random ranges for body movements

## Alexa Integration (Coming Soon)

Instructions for integrating with Amazon Alexa will be added in a future update. This will allow the fish to respond to voice commands and act as an Alexa device.

## Troubleshooting

### Common Issues

- **Fish not responding to audio**: Check your audio input circuit and the silence threshold value
- **Erratic movements**: Verify your motor connections and power supply
- **Motors not moving**: Ensure the H-bridge is correctly wired and receiving power

## Credits

This project builds upon work by:

- [Donald Bell](https://www.instructables.com/id/Animate-a-Billy-Bass-Mouth-With-Any-Audio-Source/) - Original audio-reactive Billy Bass concept
- [jswett77](https://github.com/jswett77/big_mouth/blob/master/billy.ino) - State-based Billy Bass control
- Jordan Bunker - MX1508 library and state machine implementation

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## Future Plans

- Complete Alexa integration
- Add support for additional voice assistants (Google Assistant, Siri)
- Improve animation patterns
- Add LED effects
- Create a 3D-printable mounting system 