#ifndef CONFIG_H
#define CONFIG_H

#include "Constants.h"

// Debug Configuration
#define DEBUG 0  // Set to 1 to enable debug output, 0 to disable

// Pin Configuration
const uint8_t MOUTH_PIN1 = 5;
const uint8_t MOUTH_PIN2 = 3;
const uint8_t BODY_PIN1 = 6;
const uint8_t BODY_PIN2 = 9;
const uint8_t SOUND_PIN = A0;

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

// State Definitions
const uint8_t STATE_WAITING = 0;
const uint8_t STATE_TALKING = 1;
const uint8_t STATE_FLAPPING = 2;

// Global state variables (declared as extern here, defined in main file)
extern uint8_t fishState;
extern bool audioReactivityEnabled;
extern bool manualMode;
extern bool debugMode;
extern uint16_t soundVolume;
extern bool talking;

// Timing variables
extern unsigned long currentTime;
extern unsigned long mouthActionTime;
extern unsigned long bodyActionTime;
extern unsigned long lastActionTime;

#endif // CONFIG_H 