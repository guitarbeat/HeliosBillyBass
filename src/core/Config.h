#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>  // For standard Arduino types and A0 constant
#include <stdint.h>   // For uint8_t and uint16_t types

// ===== Debug Configuration =====
#define DEBUG 1  // Enable debug output to help diagnose issues

// ===== Safety Configuration =====
const uint16_t MAX_MOTOR_ON_TIME = 500;      // Reduced from 1000ms to 500ms for safety
const uint16_t MOTOR_COOLDOWN_TIME = 1000;   // Reduced from 2000ms to 1000ms to allow quicker response
const uint8_t MAX_CONSECUTIVE_MOVES = 5;     // Increased from 3 to 5 to allow for correction attempts
const uint16_t RAMP_TIME = 100;             // Reduced from 200ms to 100ms for quicker response
const uint8_t POSITION_TIMEOUT = 100;       // Time to wait for position confirmation

// ===== Pin Configuration =====
const uint8_t MOUTH_PIN1 = 5;
const uint8_t MOUTH_PIN2 = 3;
const uint8_t BODY_PIN1 = 6;
const uint8_t BODY_PIN2 = 9;
const uint8_t SOUND_PIN = A0;

// ===== Motor & Movement Settings =====
const uint8_t DEFAULT_SPEED = 35;           // Reduced from 60 to 35 for safer starting speed
const uint8_t MIN_SPEED = 0;
const uint8_t MAX_SPEED = 180;              // Reduced from 255 to 180 for safety
const uint8_t MOUTH_SPEED = 150;            // Reduced from 200 to 150
const uint8_t FLAP_SPEED = 120;             // Reduced from 180 to 120

// ===== Timing Constants (ms) =====
const uint16_t DEFAULT_DURATION = 800;
const uint16_t PAUSE_TIME = 300;
const uint16_t IDLE_TIMEOUT = 1500;
const uint16_t FLAP_INTERVAL_MIN = 30000;
const uint16_t FLAP_INTERVAL_MAX = 60000;
const uint16_t MAX_MOVEMENT_TIME = 1000;    // Maximum time for any single movement (ms)

// ===== Movement Calibration =====
// These can be adjusted via serial commands
struct MovementCalibration {
    uint16_t mouthOpenTime = 400;    // Time to run motor when opening mouth (ms)
    uint16_t mouthCloseTime = 400;   // Time to run motor when closing mouth (ms)
    uint16_t bodyForwardTime = 600;  // Time to run motor for body forward (ms)
    uint16_t bodyBackTime = 600;     // Time to run motor for body back (ms)
    uint8_t mouthSpeed = 150;        // Speed for mouth movement (0-255)
    uint8_t bodySpeed = 120;         // Speed for body movement (0-255)
};

// ===== Audio Settings =====
const uint16_t SILENCE_THRESHOLD = 12;

// ===== State Definitions =====
const uint8_t STATE_WAITING = 0;
const uint8_t STATE_TALKING = 1;
const uint8_t STATE_FLAPPING = 2;

// ===== Motor State Bit Masks =====
const uint8_t MOUTH_OPEN_BIT = 0x01;
const uint8_t BODY_MOVED_BIT = 0x02;

// ===== Emergency Override =====
const bool ALLOW_EMERGENCY_CLOSE = true;    // Always allow mouth to close regardless of safety checks

// ===== State Structures =====
// Fish state struct
struct FishState {
    uint8_t state;                  // Current state
    bool audioReactivityEnabled;    // Whether audio reactivity is enabled
    bool manualMode;                // Whether manual mode is enabled
    bool talking;                   // Whether the fish is currently talking
    uint16_t soundVolume;           // Current sound volume
};

// Timing struct
struct TimingVars {
    unsigned long current;          // Current time in milliseconds
    unsigned long mouthAction;      // Time for next mouth action
    unsigned long bodyAction;       // Time for next body action
    unsigned long lastAction;       // Time of last action
};

// Global variables
extern FishState fishState;
extern TimingVars timing;
extern MovementCalibration calibration;
extern bool debugMode;

#endif // CONFIG_H 