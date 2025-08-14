# Technical Context: BTBillyBass

## Technology Stack

### Hardware
- Arduino Nano/Uno
- MX1508 motor drivers
- DC motors from Billy Bass
- Audio input sensor

### Software
- Arduino IDE
- Custom motor control library
- State machine implementation
- Calibration system

## Development Environment

### Setup Requirements
- Arduino IDE
- Serial Monitor (9600 baud)
- MX1508 library

### Pin Configuration
- A0: Audio input
- D3/D5: Mouth motor
- D6/D9: Body motor

## Dependencies

### Hardware Dependencies
- MX1508 H-bridge modules
- DC motors (original from Billy Bass)
- Audio sensor/microphone

### Software Dependencies
- MX1508 library
- Arduino standard libraries

## Technical Constraints

### Motor Control
- Speed range: 0-180
- Maximum movement time: 1000ms
- PWM control via MX1508

### Audio Processing
- Analog input on A0
- Silence threshold: 12
- Sample rate: System dependent

## Build & Deployment

### Initial Setup
1. Upload code with default settings
2. Enable debug mode ('d')
3. View current settings ('p')
4. Test basic movements
5. Calibrate as needed

### Calibration Process

1. **Default Settings**:
   ```cpp
   // Movement Timing
   mouthOpenTime = 400ms    // Quick mouth movements
   mouthCloseTime = 400ms
   bodyForwardTime = 800ms  // Slower body movements for more dramatic effect
   bodyBackTime = 800ms
   
   // Speed Settings
   mouthSpeed = 100         // Moderate speeds for smooth movement
   bodySpeed = 100
   ```

2. **Movement Characteristics**:
   - Mouth movements: Quick and responsive (400ms)
   - Body movements: Slower, more dramatic (800ms)
   - Motor speeds: Moderate (100/180) for smooth operation
   - All movements can be fine-tuned via serial commands

2. **Testing Sequence**:
   ```
   Basic Movements:
   o -> c  : Test mouth
   f -> b  : Test body
   r      : Reset position
   
   Calibration:
   t      : Mouth timing
   y      : Body timing
   m      : Mouth speed
   n      : Body speed
   ```

3. **Fine-Tuning Guidelines**:
   - Start with lower speeds (~100)
   - Use shorter durations (200-300ms)
   - Increase gradually
   - Test thoroughly

## Command Interface

### Movement Commands
```
o - Open mouth
c - Close mouth
f - Flap tail
b - Body forward
r - Reset position
s - Singing motion
```

### Calibration Commands
```
t - Set mouth timing
y - Set body timing
m - Set mouth speed
n - Set body speed
p - Print settings
```

### Mode Commands
```
l - Manual/auto toggle
a - Audio reactivity
d - Debug output
h - Help menu
```

## Debug System

### Debug Output
- Motor movement status
- Current settings
- State changes
- Audio levels

### Debug Commands
```
d - Toggle debug mode
p - Print current settings
```

## Safety Considerations

### Motor Protection
- Speed limits (0-180)
- Movement time limits
- Smooth stop capability

### Error Handling
- Invalid command handling
- Out-of-range value protection
- Emergency stop capability
