# System Patterns: BTBillyBass

## Architecture Overview
The system is built on an Arduino platform using a state-based architecture with non-blocking motor control and event-driven audio processing. It uses the MX1508 library for motor control and implements memory-efficient string handling using PROGMEM.

## Key Components
- Command Parser: Processes serial input and validates commands
- Motor Controller: Manages motor states and movements using MX1508 drivers
- Audio Processor: Handles audio input and calibration for yap mode
- State Manager: Maintains system state and coordinates components
- Timer System: Manages timed actions without blocking

## Design Patterns
- State Pattern: Used for motor and system state management
- Command Pattern: Implements serial command processing
- Observer Pattern: Audio monitoring and response system
- Factory Pattern: Motor parameter management and initialization
- Non-blocking Design: All long-running operations are non-blocking

## Data Flow
1. Command Input:
   - Serial input → Command Parser → State Manager → Motor Controller
   - Parameters validated and processed at each stage

2. Audio Processing:
   - Analog Input → Audio Processor → State Manager → Motor Controller
   - Continuous monitoring with debounce and threshold checks

3. Motor Control:
   - State Manager → Motor Controller → MX1508 Driver → Physical Motors
   - Includes timing, speed control, and position management

## Technical Decisions
- Use of PROGMEM: Optimizes memory usage for string constants
- Non-blocking Design: Ensures responsive system behavior
- State-based Architecture: Simplifies complex behavior management
- Debounce Implementation: Prevents rapid motor state changes
- Parameter Validation: Ensures safe motor operation 