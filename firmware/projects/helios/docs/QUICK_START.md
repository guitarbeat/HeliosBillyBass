# BTBillyBass Quick Start Guide
[← Back to docs index](./index.md)

## 🚀 Get Up and Running in 10 Minutes

This guide will help you get your BTBillyBass system operational quickly. For detailed information, see the full documentation.

## Prerequisites

### Required Hardware
- Arduino board (Uno, Nano, Mega, etc.)
- 2x MX1508 motor drivers
- Big Mouth Billy Bass (or compatible)
- 5V, 2A+ power supply
- Microphone with basic circuit
- Breadboard and jumper wires

### Required Software
- Arduino IDE
- MX1508 library

## Quick Setup

### 1. Install Software (2 minutes)
```bash
# Install Arduino IDE from arduino.cc
# Install MX1508 library via Library Manager
```

### 2. Basic Wiring (5 minutes)
```
Arduino Pin 6,9 ──→ MX1508 Driver 1 ──→ Body Motor
Arduino Pin 5,3 ──→ MX1508 Driver 2 ──→ Mouth Motor
Arduino Pin A0 ──→ Microphone Circuit
5V Power ──→ Both Motor Drivers
```

### 3. Upload Code (2 minutes)
1. Open `BTBillyBass.ino` in Arduino IDE
2. Select your board and port
3. Click Upload

### 4. Test Basic Functions (1 minute)
1. Open Serial Monitor (9600 baud)
2. Type `help` to see commands
3. Test: `body forward`, `mouth open`

## Essential Commands

### Basic Motor Control
```bash
body forward     # Move head/tail forward
body backward    # Move head/tail backward
mouth open       # Open mouth
mouth close      # Close mouth
stop             # Stop all motors
```

### Audio Mode
```bash
yap              # Start audio-responsive mode
stop             # Exit audio mode
```

### Custom Timing
```bash
body forward 1000    # Move forward for 1 second
mouth open 200       # Open mouth for 200ms
```

## Quick Troubleshooting

### Motors Not Working?
1. Check power connections
2. Verify motor driver wiring
3. Test: `body forward 500`

### Audio Not Responding?
1. Check microphone circuit
2. Run: `yap` (auto-calibration)
3. Adjust: `yap 50` (lower threshold)

### System Acting Strange?
1. Use: `stop` (emergency stop)
2. Check all connections
3. Restart Arduino

## Configuration Tips

### Motor Speed Adjustment
```cpp
// In BTBillyBass.ino, modify these values:
const int BODY_SPEED = 200;    // 0-255
const int MOUTH_SPEED = 250;   // 0-255
```

### Timing Adjustment
```cpp
// Modify default timings:
const int DEFAULT_MOUTH_OPEN_TIME = 120;      // ms
const int DEFAULT_BODY_FORWARD_TIME = 800;    // ms
```

### Audio Sensitivity
```bash
yap 30     # More sensitive
yap 100    # Less sensitive
```

## Common Use Cases

### Manual Control Mode
```bash
# Basic movements
body forward
mouth open
body backward
mouth close

# Timed movements
body forward 1500
mouth open 300
```

### Audio-Responsive Mode
```bash
# Start audio mode
yap

# Billy Bass will automatically:
# - Open mouth when it hears sound
# - Move head based on audio patterns
# - Return to neutral after silence

# Stop audio mode
stop
```

### Performance Mode
```bash
# Quick movements for testing
body forward 200
mouth open 100
body backward 200
```

## Safety Notes

### Emergency Stop
- Always use `stop` command to halt all motors
- Keep hands clear of moving parts
- Disconnect power when not in use

### Power Requirements
- Use 5V, 2A+ power supply
- Don't exceed motor specifications
- Monitor for overheating

## Next Steps

### For Beginners
1. Master basic commands
2. Experiment with timing values
3. Test audio mode in different environments
4. Read full documentation for advanced features

### For Advanced Users
1. Modify motor speeds for your Billy Bass
2. Adjust audio processing parameters
3. Add external control interfaces
4. Integrate with other systems

### For Developers
1. Review API documentation
2. Extend command processing
3. Add new motor types
4. Implement custom audio algorithms

## Support Resources

### Documentation
- [Docs Index](./README.md)
- [Command Reference](./COMMANDS.md)
- [Troubleshooting](./TROUBLESHOOTING.md)
- [Hardware Setup](./HARDWARE_SETUP.md)
- [API Reference](./API.md)
- [Project Overview](../README.md)

### Testing Commands
```bash
help                    # Show all commands
body forward 100        # Quick test
mouth open 50           # Quick test
yap                     # Audio test
stop                    # Emergency stop
```

### Performance Checklist
- [ ] Motors respond to commands
- [ ] Audio mode activates
- [ ] Movements are smooth
- [ ] No overheating
- [ ] Stable operation

---

**Need Help?** Check the troubleshooting section in the full documentation or test with basic commands first.

**Happy Fishing! 🐟**