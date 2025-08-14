# Active Context: BTBillyBass

## Current Focus
Simplifying and improving the yap mode calibration process to make it more user-friendly and efficient.

## Recent Changes
- [2024-03-21] - Simplified yap mode calibration:
  - Removed calibration dance sequence
  - Added silent audio calibration
  - Implemented single upward movement as ready indicator
- [2024-03-21] - Updated documentation in Memory Bank system

## Next Steps
1. Test new calibration sequence
2. Verify audio response accuracy
3. Consider additional user feedback during calibration
4. Potential optimization of audio processing parameters

## Active Decisions
- Calibration Sequence: Simplified to audio-only with single movement indicator
- Audio Threshold: Using ambient level + 10 for noise floor
- Movement Timing: Using DEFAULT_BODY_FORWARD_TIME / 2 for ready indication
- Debounce Values: Current settings working well for most use cases

## Open Questions
- Should audio calibration duration be adjustable?
- Would visual feedback (e.g., LED) during calibration be helpful?
- Is the current audio threshold offset (+10) optimal for all environments?
- Should there be a way to recalibrate audio without exiting yap mode? 