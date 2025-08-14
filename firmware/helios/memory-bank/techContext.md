# Technical Context: BTBillyBass

## Technology Stack
- Hardware Platform: Arduino
- Motor Control: MX1508 H-bridge motor drivers
- Input Processing: Analog audio input
- Communication: Serial interface at 9600 baud
- Memory Management: PROGMEM for string storage

## Development Environment
- Arduino IDE
- Serial Monitor: For command input and debugging
- Libraries:
  - MX1508: Motor driver control
  - avr/pgmem.h: Program memory management

## Dependencies
- MX1508 Library: Latest version - Motor driver control
- AVR Core: Required for PROGMEM functionality
- Hardware Dependencies:
  - Arduino board with 4+ PWM pins
  - MX1508 H-bridge motor drivers (2x)
  - 5V power supply
  - Audio input circuit

## Technical Constraints
- PWM Pin Requirement: Minimum 4 pins
- Memory Limitations: String storage in PROGMEM
- Power Requirements: 5V for motors
- Serial Communication: Fixed at 9600 baud
- Audio Input: Analog reading limitations

## Build & Deployment
1. Hardware Setup:
   - Connect motors to MX1508 drivers
   - Wire drivers to Arduino PWM pins
   - Setup audio input circuit
   - Connect power supply

2. Software Deployment:
   - Install required libraries
   - Upload code via Arduino IDE
   - Test via Serial Monitor
   - Calibrate audio sensitivity if needed 