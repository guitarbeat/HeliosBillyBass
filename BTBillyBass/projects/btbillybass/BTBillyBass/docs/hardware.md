# Hardware Guide

## Parts
- Arduino Uno/Nano
- 2x MX1508 dual H-bridge motor drivers
- Big Mouth Billy Bass (original motors retained)
- Microphone/sound sensor module
- Breadboard, jumpers, 5V/2A power supply

## Pins
- A0: audio input
- D3/D5: mouth motor (MX1508 IN2/IN1)
- D6/D9: body/tail motor (MX1508 IN1/IN2)

## Wiring
- Mouth motor → MX1508 A+/A- (Driver 1)
  - Driver 1 IN1 → D5
  - Driver 1 IN2 → D3
- Body motor → MX1508 A+/A- (Driver 2)
  - Driver 2 IN1 → D6
  - Driver 2 IN2 → D9
- Microphone/sensor
  - VCC → 5V
  - GND → GND
  - SIG → A0
- Power
  - 5V → Arduino VIN, MX1508 VCC (both)
  - GND → Arduino GND, MX1508 GND (both)

Ensure all grounds are common.