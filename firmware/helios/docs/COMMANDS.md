# BTBillyBass Command Reference
[← Back to docs index](./index.md)

This document lists all serial commands supported by BTBillyBass. Commands are case-insensitive. Baud rate: 9600.

## Usage
- Enter commands via the Arduino Serial Monitor or another serial terminal.
- Optional duration parameters are in milliseconds.

## Body motor
```bash
body forward [duration]    # Move head/tail forward (default: 800)
body backward [duration]   # Move head/tail backward (default: 1000)
body hold                  # Hold current position (reduced power)
body release               # Release motor (coast)
```

## Mouth motor
```bash
mouth open [duration]      # Open mouth (default: 120)
mouth close                # Close mouth (spring-assisted)
mouth hold                 # Hold current position (reduced power)
mouth release              # Release motor (coast)
```

## System
```bash
yap [threshold]            # Start audio-responsive mode; auto-calibrates first
stop                       # Stop all motors and exit yap mode
help                       # Print help information
```

## Timing examples
```bash
body forward 1500
mouth open 200
body backward 500
```

## Notes
- When a timed action completes, the system transitions to hold for stability, then releases after a timeout.
- In yap mode, serial input will interrupt processing to service commands.
- For recommended default timings and tunables, see `docs/API.md`.