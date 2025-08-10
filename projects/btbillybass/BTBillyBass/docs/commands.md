# Command Reference

Type these single-character commands in the Serial Monitor (9600 baud).

## Movement
- `o`: Open mouth
- `c`: Close mouth
- `f`: Flap tail
- `b`: Body forward
- `r`: Reset to home position
- `s`: Singing motion (coordinated sequence)

## Calibration
- `t`: Set mouth timing (open,close in ms)
- `y`: Set body timing (forward,back in ms)
- `m`: Set mouth speed (0–180)
- `n`: Set body speed (0–180)
- `p`: Print current settings

## Modes & Info
- `a`: Toggle audio reactivity
- `l`: Toggle manual/auto mode
- `d`: Toggle debug mode
- `h` or `?`: Show help menu

## Speed Nudge
- `+`: Increase internal motor speed by 5
- `-`: Decrease internal motor speed by 5

Notes:
- Audio-reactive behavior runs when audio is enabled and manual mode is OFF.
- Safety limits apply (e.g., max movement time, cooldowns).