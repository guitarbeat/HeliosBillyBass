# BTBillyBass - Arduino Animatronic Control System

Documentation: [Quick Start](docs/QUICK_START.md) • [Hardware Setup](docs/HARDWARE_SETUP.md) • [Commands](docs/COMMANDS.md) • [API](docs/API.md) • [Troubleshooting](docs/TROUBLESHOOTING.md)

## Overview
BTBillyBass is an Arduino-based control system for Big Mouth Billy Bass animatronics. It provides manual serial control and an audio-responsive mode for natural movements, with precise motor timing and memory-efficient design.

## Features
- Manual control via serial commands
- Audio-responsive “yap” mode with auto-calibration
- Timed actions with automatic hold and safety timeout
- Efficient memory use (PROGMEM) and non-blocking architecture

## Get started
- New users: read the [Quick Start](docs/QUICK_START.md)
- Building hardware: follow [Hardware Setup](docs/HARDWARE_SETUP.md)
- Running commands: see [Command Reference](docs/COMMANDS.md)

## Learn more
- Internals and tunables: [API Reference](docs/API.md)
- Fix issues: [Troubleshooting](docs/TROUBLESHOOTING.md)
- Changes: [Changelog](CHANGELOG.md)

## Development
- Entry point: `BTBillyBass.ino`
- Key areas: hardware configuration, state management, motor/audio processing, and command handling
- See [API Reference](docs/API.md) for functions, constants, and state

## License
MIT License - See LICENSE file for details.

## Credits
- Original Code: Jordan Bunker <jordan@hierotechnics.com> (2019)
- Enhancements: Manual control with timed actions and documentation suite
- Hardware: Big Mouth Billy Bass animatronic

## Support
- Use `help` in Serial Monitor to list commands
- Start with basic tests: `body forward 500`, `mouth open 200`, `yap`
- For wiring and power guidance, see [Hardware Setup](docs/HARDWARE_SETUP.md)