# Product Context: BTBillyBass

## Problem Statement
Users want to control and automate a Big Mouth Billy Bass animatronic for entertainment and interactive purposes. The system needs to provide both manual control for precise movements and automated responses to audio input, making the fish appear to "sing" along with music or speech.

## Target Users
- Hobbyists: Want to experiment with animatronic control and automation
- Entertainers: Need the fish to respond to music or speech for performances
- Developers: Want to integrate the fish into larger interactive projects

## User Experience Goals
- Simple and intuitive command interface
- Responsive and natural-looking movements
- Easy calibration process
- Reliable audio response
- Predictable behavior

## Key Features
- Manual Control: Direct control of mouth and body movements with timing options
- Yap Mode: Audio-responsive automatic movements
  - Audio calibration without movement
  - Simple ready indication (single upward movement)
  - Responsive mouth movements to sound
  - Dynamic head movements based on audio patterns
- Position Holding: Ability to maintain positions with reduced power
- Safety Features: Motor protection and command validation

## User Flows
1. Manual Control Flow:
   - Send command through serial monitor
   - Fish responds with corresponding movement
   - Optional timing parameters for movement duration
   - Release or hold position as needed

2. Yap Mode Flow:
   - Enter yap mode command
   - System performs audio calibration (3 seconds)
   - Single upward movement indicates ready state
   - Fish automatically responds to audio input
   - Exit with stop command when done 