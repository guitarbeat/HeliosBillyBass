# Troubleshooting
[← Back to docs index](./index.md)

Use this guide to diagnose and fix common issues. Run quick tests early and often.

## Quick checks
- Power: 5V, 2A+ supply, common ground for all modules
- Wiring: Pins 6/9 → body, 5/3 → mouth, A0 → audio in
- Serial: 9600 baud, line endings set correctly

## Motor issues

### Motors not moving
- Check power rails and polarity
- Verify MX1508 wiring (IN pins to correct Arduino pins)
- Test commands: `body forward 500`, `mouth open 200`

### Erratic or wrong direction
- Swap motor polarity on OUT terminals to correct direction
- Shorten durations for testing
- Inspect for loose connections

### Holding/timeout behavior
- After a timed action, motors may hold briefly, then release after timeout
- Adjust `HOLD_TIMEOUT` and speeds in code if needed

## Audio issues

### No response to audio
- Verify microphone circuit and connections to A0
- Start auto-calibration with `yap`
- Set manual threshold: `yap 50` (more sensitive), `yap 100` (less sensitive)

### Overly sensitive or noisy
- Ensure minimum threshold is respected in code
- Add basic filtering (0.1μF cap) and check grounding
- Use an amplifier (LM386) for weak microphones

## Power and stability
- Ensure current capacity is sufficient (motors can spike)
- Add decoupling capacitors near drivers
- Consider separate supply for motors vs. logic

## Diagnostics
```bash
help                    # List commands
body forward 100        # Quick motor test
mouth open 50           # Quick mouth test
yap                     # Test audio mode
stop                    # Emergency stop
```

## When to check the code
- Update default timings or speeds for your specific hardware
- Review `docs/API.md` for tunables and safety constraints
- Confirm pin mappings match your wiring