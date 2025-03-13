# Wiring Guide for Alexa-Enabled Billy Bass

This document provides detailed instructions for wiring your Big Mouth Billy Bass to work with an Arduino and respond to audio input from Alexa.

## Hardware Components

### Required Components:
- Big Mouth Billy Bass animatronic fish
- Arduino board (Uno, Nano, or similar)
- MX1508 dual H-bridge motor driver (or similar)
- Audio input circuit components (see below)
- Jumper wires
- Power supply (5-12V depending on your motors)
- Breadboard or perfboard for prototyping

### Optional Components:
- Amplifier circuit for audio input
- LEDs for visual feedback
- Enclosure for electronics
- Raspberry Pi or ESP32 (for advanced Alexa integration)

## Disassembling Billy Bass

1. Remove the back cover of Billy Bass (usually secured with screws)
2. Identify the two motors:
   - Mouth motor: Controls the opening and closing of the mouth
   - Body/tail motor: Controls the body and tail movement
3. Disconnect the existing control board, but keep the motors and their mechanical linkages intact
4. Optionally, remove the original speaker if you plan to use a different one

## Motor Driver Wiring

### MX1508 H-Bridge Connections:

```
MX1508 Pin | Connection
-----------|------------
VCC        | 5-12V power supply
GND        | Arduino GND
IN1        | Arduino pin D5 (Mouth motor control)
IN2        | Arduino pin D3 (Mouth motor control)
IN3        | Arduino pin D6 (Body motor control)
IN4        | Arduino pin D9 (Body motor control)
OUT1       | Mouth motor terminal 1
OUT2       | Mouth motor terminal 2
OUT3       | Body motor terminal 1
OUT4       | Body motor terminal 2
```

## Audio Input Circuit

For the fish to respond to Alexa's voice, you need to connect Alexa's audio output to the Arduino's analog input.

### Basic Audio Input Circuit:

```
Alexa Audio Out (L or R) --[10kΩ resistor]-- Arduino A0
                         |
                         |
                        GND
```

### Enhanced Audio Input Circuit (with amplification):

```
Alexa Audio Out --[10kΩ]-- |-- [10kΩ] --+-- Arduino A0
                           |            |
                         [LM358]       [100nF]
                           |            |
                          GND          GND
```

## Power Supply Considerations

- The Arduino can be powered via USB or an external 7-12V power supply
- The motors should be powered separately from the Arduino logic to prevent interference
- Use a common ground between the Arduino, motor driver, and audio circuit

## Complete Wiring Diagram

```
                                  +---------------------+
                                  |                     |
                                  |     Arduino         |
                                  |                     |
+-------------+                   |                     |
|             |                   |  D3 ----------+     |
| Alexa       |                   |               |     |
| Device      |                   |  D5 -------+  |     |
|             |                   |            |  |     |
| Audio Out --+---[Audio Circuit]-+-- A0       |  |     |
|             |                   |            |  |     |
+-------------+                   |  D6 ---+   |  |     |
                                  |        |   |  |     |
                                  |  D9 -+ |   |  |     |
                                  |      | |   |  |     |
                                  +------|-|---+--+-----+
                                         | |   |  |
                                         | |   |  |
                                  +------|-|---+--+-----+
                                  |      | |   |  |     |
                                  | IN4  | |   |  |     |
                                  | IN3  | |   |  |     |
                                  |      | |   |  |     |
                                  | MX1508  |  |     |
                                  |         |  |     |
                                  | IN1     |  |     |
                                  | IN2        |     |
                                  |                  |
                                  | OUT1 --- OUT2    |
                                  |    |       |     |
                                  | OUT3 --- OUT4    |
                                  |    |       |     |
                                  +----+-------+-----+
                                       |       |
                                       |       |
                                  +----+-------+-----+
                                  |    |       |     |
                                  |  Mouth Motor     |
                                  |                  |
                                  +------------------+
                                       |       |
                                       |       |
                                  +----+-------+-----+
                                  |    |       |     |
                                  |  Body Motor      |
                                  |                  |
                                  +------------------+
```

## Testing the Connections

1. Before connecting to Billy Bass, test the motor driver with small DC motors
2. Verify that the audio input circuit is working by monitoring the analog input value via Serial Monitor
3. Adjust the `silence` threshold in the code if needed
4. Test each motor direction and speed before final assembly

## Troubleshooting Common Wiring Issues

- **Motors not moving**: Check H-bridge connections and power supply
- **Motors moving in wrong direction**: Swap the motor wires at the H-bridge outputs
- **No audio detection**: Check audio input circuit and connections
- **Erratic behavior**: Ensure common ground between all components
- **Arduino resets when motors run**: Add capacitors across motor terminals and ensure separate power supply

## Advanced Setup (for Direct Alexa Integration)

For direct Alexa integration using a Raspberry Pi or ESP32, additional wiring will be required:

```
Raspberry Pi/ESP32 --[Serial/I2C/SPI]--> Arduino
                   |
                   +--[Microphone Array]
                   |
                   +--[Speaker]
                   |
                   +--[Optional LEDs]
```

Detailed wiring for this advanced setup will be provided in a future update. 