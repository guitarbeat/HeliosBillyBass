# BTBillyBass Hardware Setup Guide
[← Back to docs index](./index.md)

## Table of Contents
1. [Component List](#component-list)
2. [Wiring Diagram](#wiring-diagram)
3. [Step-by-Step Assembly](#step-by-step-assembly)
4. [Power Requirements](#power-requirements)
5. [Audio Input Circuit](#audio-input-circuit)
6. [Testing and Calibration](#testing-and-calibration)
7. [Troubleshooting](#troubleshooting)

## Component List

### Essential Components
| Component | Quantity | Description | Notes |
|-----------|----------|-------------|-------|
| Arduino Board | 1 | Uno, Nano, Mega, or compatible | Must have 4+ PWM pins |
| MX1508 H-bridge Driver | 2 | Motor driver modules | 2A per channel, 5V logic |
| DC Motors | 2 | 5V DC motors from Billy Bass | Original motors recommended |
| 5V Power Supply | 1 | 2A+ power supply | Separate from Arduino power |
| Microphone | 1 | Electret microphone | With preamplifier circuit |
| Breadboard | 1 | For prototyping | Optional for permanent setup |
| Jumper Wires | 20+ | Male-to-male and male-to-female | Various lengths |

### Optional Components
| Component | Quantity | Description | Notes |
|-----------|----------|-------------|-------|
| Perfboard | 1 | For permanent circuit | Alternative to breadboard |
| Enclosure | 1 | Project box | For protection and mounting |
| Power Switch | 1 | Toggle or push-button | For easy power control |
| LED Indicators | 2-3 | Status LEDs | Power, activity, error |
| Resistors | 10+ | 220Ω, 1kΩ, 10kΩ | For LED and audio circuits |

## Wiring Diagram

### Pin Assignment Overview
```
Arduino Pin Layout:
┌─────────────────┐
│ 3  - Mouth Motor │
│ 5  - Mouth Motor │
│ 6  - Body Motor  │
│ 9  - Body Motor  │
│ A0 - Audio Input │
│ A1 - Random Seed │
└─────────────────┘
```

### Detailed Wiring Connections

#### Motor Control Circuit
```
Arduino Pin 6 ──→ MX1508 Driver 1 (IN1)
Arduino Pin 9 ──→ MX1508 Driver 1 (IN2)
Arduino Pin 5 ──→ MX1508 Driver 2 (IN1)
Arduino Pin 3 ──→ MX1508 Driver 2 (IN2)

5V Power ──→ MX1508 Driver 1 (VCC)
5V Power ──→ MX1508 Driver 2 (VCC)
GND ──→ MX1508 Driver 1 (GND)
GND ──→ MX1508 Driver 2 (GND)

MX1508 Driver 1 (OUT1) ──→ Body Motor (+)
MX1508 Driver 1 (OUT2) ──→ Body Motor (-)
MX1508 Driver 2 (OUT1) ──→ Mouth Motor (+)
MX1508 Driver 2 (OUT2) ──→ Mouth Motor (-)
```

#### Audio Input Circuit
```
Microphone (+) ──→ 10kΩ Resistor ──→ Arduino A0
Microphone (-) ──→ GND
Arduino A0 ──→ 1kΩ Resistor ──→ GND (pull-down)
```

### Complete Circuit Diagram
```
                    MX1508 Driver 1
Arduino Pin 6 ──→ IN1 ──→ OUT1 ──→ Body Motor (+)
Arduino Pin 9 ──→ IN2 ──→ OUT2 ──→ Body Motor (-)
                    VCC ──→ 5V Power
                    GND ──→ GND

                    MX1508 Driver 2
Arduino Pin 5 ──→ IN1 ──→ OUT1 ──→ Mouth Motor (+)
Arduino Pin 3 ──→ IN2 ──→ OUT2 ──→ Mouth Motor (-)
                    VCC ──→ 5V Power
                    GND ──→ GND

Audio Input Circuit:
Microphone ──→ 10kΩ ──→ Arduino A0 ──→ 1kΩ ──→ GND
```

## Step-by-Step Assembly

### Step 1: Prepare the Arduino
1. **Verify Arduino Board**: Ensure your Arduino has at least 4 PWM pins
2. **Install Libraries**: Install the MX1508 library in Arduino IDE
3. **Test Basic Functionality**: Upload a simple blink sketch to verify operation

### Step 2: Set Up Power Distribution
1. **Connect Power Supply**: 
   - Connect 5V power supply positive to breadboard power rail
   - Connect power supply negative to breadboard ground rail
2. **Add Power Switch** (optional):
   - Connect switch between power supply and power rail
3. **Add Status LED** (optional):
   - Connect LED with 220Ω resistor to power rail

### Step 3: Install Motor Drivers
1. **Mount MX1508 Drivers**:
   - Place drivers on breadboard with proper orientation
   - Ensure proper pin alignment
2. **Connect Power to Drivers**:
   - Connect VCC pins to 5V power rail
   - Connect GND pins to ground rail
3. **Connect Arduino Control Pins**:
   - Pin 6 → Driver 1 IN1
   - Pin 9 → Driver 1 IN2
   - Pin 5 → Driver 2 IN1
   - Pin 3 → Driver 2 IN2

### Step 4: Connect Motors
1. **Identify Motor Wires**:
   - Body motor: Controls head/tail movement
   - Mouth motor: Controls mouth opening/closing
2. **Connect to Drivers**:
   - Body motor (+) → Driver 1 OUT1
   - Body motor (-) → Driver 1 OUT2
   - Mouth motor (+) → Driver 2 OUT1
   - Mouth motor (-) → Driver 2 OUT2

### Step 5: Set Up Audio Input
1. **Build Audio Circuit**:
   - Connect microphone positive to 10kΩ resistor
   - Connect resistor to Arduino A0
   - Connect microphone negative to ground
   - Add 1kΩ pull-down resistor from A0 to ground
2. **Test Audio Input**:
   - Use Arduino IDE Serial Monitor to verify audio readings

### Step 6: Final Connections
1. **Connect Arduino Power**:
   - Arduino VIN → 5V power rail (if using external power)
   - Arduino GND → ground rail
2. **Verify All Connections**:
   - Double-check all wire connections
   - Ensure proper polarity for motors
   - Verify power connections

## Power Requirements

### Power Specifications
- **Voltage**: 5V DC
- **Current**: 2A minimum (1A per motor driver)
- **Power**: 10W minimum

### Power Supply Options
1. **USB Power Supply**: 5V, 2A+ USB adapter
2. **Bench Power Supply**: Adjustable 5V supply
3. **Battery Pack**: 4x AA batteries in series (6V, use voltage regulator)

### Power Distribution
```
5V Power Supply
    │
    ├── Arduino (if using external power)
    ├── MX1508 Driver 1
    ├── MX1508 Driver 2
    └── Audio Circuit
```

### Voltage Regulation (if needed)
If using higher voltage power supply:
```
6V+ Power ──→ LM7805 ──→ 5V Output
                │
                └── 0.1μF Capacitor (input)
                └── 0.1μF Capacitor (output)
```

## Audio Input Circuit

### Basic Audio Circuit
```
Microphone ──→ 10kΩ Resistor ──→ Arduino A0
                │
                └── 1kΩ Pull-down ──→ GND
```

### Enhanced Audio Circuit (Recommended)
```
Microphone ──→ 10μF Capacitor ──→ 10kΩ Resistor ──→ Arduino A0
                │
                └── 1kΩ Pull-down ──→ GND
                │
                └── 0.1μF Capacitor ──→ GND (noise filtering)
```

### Audio Amplification (Optional)
For better sensitivity:
```
Microphone ──→ LM386 Amplifier ──→ Arduino A0
                │
                └── 5V Power
                └── GND
```

### Component Specifications
- **Microphone**: Electret condenser microphone
- **Capacitor**: 10μF electrolytic, 0.1μF ceramic
- **Resistors**: 10kΩ, 1kΩ (1/4W)
- **Amplifier**: LM386 (if using amplification)

## Testing and Calibration

### Initial Testing
1. **Power Test**:
   ```bash
   # Check power supply voltage
   # Should read 5V ±0.2V
   ```

2. **Motor Test**:
   ```bash
   # Upload test sketch
   # Verify motors respond to basic commands
   ```

3. **Audio Test**:
   ```bash
   # Check audio input readings
   # Should vary with sound levels
   ```

### Motor Calibration
1. **Test Individual Motors**:
   ```bash
   body forward 500    # Test body motor
   mouth open 200      # Test mouth motor
   ```

2. **Adjust Timing**:
   - Modify timing constants in code if needed
   - Test with different durations

3. **Speed Adjustment**:
   - Adjust `BODY_SPEED` and `MOUTH_SPEED` constants
   - Test for optimal movement

### Audio Calibration
1. **Run Auto-Calibration**:
   ```bash
   yap    # Starts automatic calibration
   ```

2. **Manual Threshold Adjustment**:
   ```bash
   yap 50     # Set custom threshold
   yap 100    # Higher threshold for noisy environments
   ```

3. **Test Audio Response**:
   - Speak or play music near microphone
   - Verify mouth and head movements
   - Adjust threshold if needed

## Troubleshooting

### Common Issues

#### Motors Not Moving
**Symptoms**: Motors don't respond to commands
**Solutions**:
1. Check power connections
2. Verify motor driver connections
3. Test individual motor commands
4. Check Arduino pin assignments

#### Erratic Motor Behavior
**Symptoms**: Motors move unexpectedly or in wrong direction
**Solutions**:
1. Check motor wire polarity
2. Verify driver connections
3. Test with shorter durations
4. Check for loose connections

#### Audio Not Working
**Symptoms**: No response to audio input
**Solutions**:
1. Check microphone connections
2. Verify audio circuit components
3. Test audio input with Serial Monitor
4. Run audio calibration

#### Power Issues
**Symptoms**: System resets or motors stall
**Solutions**:
1. Check power supply capacity
2. Verify voltage levels
3. Add power filtering capacitors
4. Use separate power for motors

### Diagnostic Commands
See also: [Command Reference](./COMMANDS.md).
```bash
help                    # Display available commands
body forward 100        # Quick motor test
mouth open 50           # Quick mouth test
yap                     # Test audio mode
stop                    # Emergency stop
```

### Testing Checklist
- [ ] Power supply provides 5V, 2A+
- [ ] All ground connections are common
- [ ] Motor drivers are properly connected
- [ ] Motors respond to basic commands
- [ ] Audio input shows varying readings
- [ ] Yap mode activates and responds
- [ ] All movements are smooth and controlled

### Performance Optimization
1. **Motor Speed**: Adjust for your specific Billy Bass
2. **Timing Values**: Fine-tune for natural movements
3. **Audio Sensitivity**: Optimize for your environment
4. **Power Supply**: Ensure adequate current capacity

## Safety Considerations

### Electrical Safety
- Use appropriate power supply
- Avoid short circuits
- Check connections before powering on
- Use proper wire gauge for current

### Mechanical Safety
- Secure motors properly
- Avoid pinching wires
- Test movements in safe area
- Monitor for overheating

### Operational Safety
- Use `stop` command for emergency stop
- Monitor system during operation
- Keep hands clear of moving parts
- Disconnect power when not in use