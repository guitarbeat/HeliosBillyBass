#ifndef CONSTANTS_H
#define CONSTANTS_H

// State Definitions
const uint8_t STATE_WAITING = 0;
const uint8_t STATE_TALKING = 1;
const uint8_t STATE_FLAPPING = 2;

// Motor & Movement Settings
const uint8_t DEFAULT_SPEED = 60;
const uint8_t MIN_SPEED = 0;
const uint8_t MAX_SPEED = 255;
const uint8_t MOUTH_SPEED = 200;
const uint8_t FLAP_SPEED = 180;

// Timing Constants (ms)
const uint16_t DEFAULT_DURATION = 800;
const uint16_t PAUSE_TIME = 300;
const uint16_t IDLE_TIMEOUT = 1500;
const uint16_t FLAP_INTERVAL_MIN = 30000;
const uint16_t FLAP_INTERVAL_MAX = 60000;

// Audio Settings
const uint16_t SILENCE_THRESHOLD = 12;

// Motor State Bit Masks
const uint8_t MOUTH_OPEN_MASK = 0x01;
const uint8_t BODY_MOVED_MASK = 0x02;

#endif // CONSTANTS_H 