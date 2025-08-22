# Technical Overview

BTBillyBass is a modular Arduino project that drives a Big Mouth Billy Bass with audio reactivity.

## Architecture

- Main sketch: `BTBillyBass.ino`
- Core logic: `src/core/` (`BillyBass`, `StateMachine`, `Config`)
- Drivers: `src/drivers/` (`BillyBassMotor` using `MX1508`)
- Utilities: `src/utils/`

## Pins

- A0: Audio input
- D3/D5: Mouth motor (MX1508)
- D6/D9: Body motor (MX1508)

## Key Components

- `BillyBass`: high-level movements (`openMouth`, `closeMouth`, `flapTail`, `bodyForward`, `singingMotion`, `resetMotorsToHome`)
- `BillyBassMotor`: MX1508 control with safety and ramping
- `StateMachine`: WAITING, TALKING, FLAPPING
- `Config`: limits and calibration, e.g. `MAX_MOVEMENT_TIME=1000`, `MAX_SPEED=180`, `SILENCE_THRESHOLD=12`

## Calibration Structure

```
struct MovementCalibration {
  uint16_t mouthOpenTime = 400;
  uint16_t mouthCloseTime = 400;
  uint16_t bodyForwardTime = 600; // defaults; overridden to 800 on setup
  uint16_t bodyBackTime = 600;    // defaults; overridden to 800 on setup
  uint8_t mouthSpeed = 150;       // defaults; overridden to 100 on setup
  uint8_t bodySpeed = 120;        // defaults; overridden to 100 on setup
};
```

Runtime defaults set in `initializeCalibration()`:
- Mouth: 400/400 ms
- Body: 800/800 ms
- Speeds: mouth 100, body 100

## Serial Protocol

- Movement: `o`, `c`, `f`, `b`, `r`, `s`
- Calibration: `t`, `y`, `m`, `n`, `p`
- Modes: `a`, `l`, `d`, `h`
- Speed nudge: `+`, `-` (affects internal motor speed in `BillyBass`)

See full details in `TECHNICAL_DOCS.md`.