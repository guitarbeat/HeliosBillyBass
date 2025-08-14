# Project Brief: BTBillyBass

## Purpose
Control a Big Mouth Billy Bass animatronic using serial commands and audio input for interactive and automated movements.

## Core Requirements
- Serial command interface for manual control of mouth and body movements
- Audio-responsive mode ("yap" mode) for automatic mouth and head movements
- Precise motor control with timing and position holding capabilities
- Memory-efficient implementation using PROGMEM strings
- Simple calibration process for audio sensitivity

## Success Criteria
- Responsive and accurate motor control
- Natural-looking movements in response to audio
- Stable and reliable operation
- Clear and intuitive command interface
- Efficient resource usage on Arduino

## Constraints
- Arduino board with at least 4 PWM pins required
- MX1508 H-bridge motor drivers needed
- 5V power supply for motors
- Limited memory available on Arduino
- Serial communication speed fixed at 9600 baud

## Stakeholders
- End User: Controls the Billy Bass through serial commands
- Developer: Maintains and enhances the codebase
- Hardware Integrator: Ensures proper connection of motors and components 