# BTBillyBass Quick Start Guide

## 🚀 Getting Started in 5 Minutes

This guide will help you get your Big Mouth Billy Bass up and running quickly!

## 📋 Prerequisites

### Hardware Required
- [ ] Arduino board (Uno/Nano recommended)
- [ ] 2x MX1508 motor drivers
- [ ] Big Mouth Billy Bass toy
- [ ] Microphone or sound sensor
- [ ] Breadboard and jumper wires
- [ ] Power supply (5V, 2A minimum)

### Software Required
- [ ] Arduino IDE (1.8.x or 2.x)
- [ ] MX1508 library (install via Library Manager)

## 🔧 Hardware Setup

### Step 1: Disassemble Billy Bass
1. Remove the back panel of the fish
2. Locate the original motor connections
3. Disconnect the original control board
4. Keep the motors and mechanical parts intact

### Step 2: Connect Motors to MX1508 Drivers
```
Mouth Motor:
├── Motor + → MX1508 A+ (Driver 1)
├── Motor - → MX1508 A- (Driver 1)
└── Driver 1 IN1 → Arduino D5
└── Driver 1 IN2 → Arduino D3

Body Motor:
├── Motor + → MX1508 A+ (Driver 2)
├── Motor - → MX1508 A- (Driver 2)
└── Driver 2 IN1 → Arduino D6
└── Driver 2 IN2 → Arduino D9
```

### Step 3: Connect Audio Input
```
Microphone/Sensor:
├── VCC → 5V
├── GND → GND
└── Signal → Arduino A0
```

### Step 4: Power Connections
```
Power Supply:
├── 5V → Arduino VIN
├── 5V → MX1508 VCC (both drivers)
└── GND → Arduino GND
└── GND → MX1508 GND (both drivers)
```

## 💻 Software Setup

### Step 1: Install Libraries
1. Open Arduino IDE
2. Go to **Tools → Manage Libraries**
3. Search for "MX1508"
4. Install the MX1508 library

### Step 2: Download and Upload Code
1. Download the BTBillyBass project
2. Open `BTBillyBass.ino` in Arduino IDE
3. Select your Arduino board
4. Click **Upload**

### Step 3: Open Serial Monitor
1. Go to **Tools → Serial Monitor**
2. Set baud rate to **9600**
3. You should see: `🐟 BTBillyBass initialized!`

## 🎮 First Test

### Basic Movement Test
Type these commands in the Serial Monitor:

```
o    # Open mouth
c    # Close mouth
f    # Flap tail
b    # Body forward
r    # Reset position
```

You should see the fish respond to each command!

### Audio Test
```
a    # Enable audio reactivity
# Now speak or make noise near the microphone
# The fish should respond to sound!
```

## ⚙️ Calibration (Optional)

If movements seem too fast/slow or don't complete properly:

### Step 1: Check Current Settings
```
p    # Print current settings
```

### Step 2: Adjust Timing
```
t    # Mouth timing setup
# Enter open time (ms): 400
# Enter close time (ms): 400

y    # Body timing setup  
# Enter forward time (ms): 800
# Enter back time (ms): 800
```

### Step 3: Adjust Speed
```
m    # Mouth speed setup
# Enter speed (0-180): 100

n    # Body speed setup
# Enter speed (0-180): 100
```

## 🎯 Common Commands

### Movement Controls
| Command | Action |
|---------|--------|
| `o` | Open mouth |
| `c` | Close mouth |
| `f` | Flap tail |
| `b` | Body forward |
| `r` | Reset position |
| `s` | Singing motion |

### Mode Controls
| Command | Action |
|---------|--------|
| `a` | Toggle audio reactivity |
| `l` | Toggle manual/auto mode |
| `d` | Toggle debug mode |
| `h` | Show help menu |

### Calibration
| Command | Action |
|---------|--------|
| `t` | Set mouth timing |
| `y` | Set body timing |
| `m` | Set mouth speed |
| `n` | Set body speed |
| `p` | Print settings |

## 🔧 Troubleshooting

### Fish Won't Move
1. **Check power**: Ensure 5V power supply is connected
2. **Check connections**: Verify motor wires are secure
3. **Check pins**: Confirm Arduino pins match the diagram
4. **Try reset**: Type `r` to reset motors

### Audio Not Working
1. **Check microphone**: Ensure it's connected to A0
2. **Enable audio**: Type `a` to enable audio reactivity
3. **Check threshold**: Lower values in `SILENCE_THRESHOLD` if needed
4. **Test with multimeter**: Verify microphone output

### Erratic Movement
1. **Check mechanical**: Tighten any loose connections
2. **Reduce speed**: Lower speed values (try 50-100)
3. **Increase timing**: Use longer movement times
4. **Check power**: Ensure stable 5V supply

### Serial Commands Not Working
1. **Check baud rate**: Must be 9600
2. **Check Serial Monitor**: Ensure it's open
3. **Clear buffer**: Close and reopen Serial Monitor
4. **Check upload**: Re-upload the sketch

## 🎉 Next Steps

### Advanced Features
- **Custom Sequences**: Modify the code to create new movements
- **Audio Processing**: Add filters or frequency analysis
- **External Control**: Add Bluetooth or WiFi control
- **LED Effects**: Add lighting effects to the fish

### Customization Ideas
- **Voice Recognition**: Add speech-to-text capabilities
- **Music Sync**: Synchronize with music beats
- **Remote Control**: Add IR or RF remote control
- **Multiple Fish**: Control multiple Billy Bass units

## 📚 Additional Resources

- **Full Documentation**: See `README.md` for detailed information
- **Technical Docs**: See `TECHNICAL_DOCS.md` for architecture details
- **Code Examples**: Check the source files for implementation examples
- **Community**: Join Arduino forums for support

## 🆘 Need Help?

If you're stuck:
1. **Check the troubleshooting section above**
2. **Enable debug mode**: Type `d` for detailed information
3. **Check the full documentation**: See `README.md`
4. **Ask the community**: Arduino forums are very helpful

---

**Happy Fishing! 🐟**

*This quick start guide covers the basics. For advanced features and customization, refer to the full documentation.*