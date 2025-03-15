// arduino-lint-disable
/*
 * Big Mouth Billy Bass - Audio Reactive Control
 * ============================================
 * Controls a Big Mouth Billy Bass animatronic fish using an Arduino and MX1508 motor drivers.
 * Features mouth and body/tail movement that reacts to audio input.
 * 
 * Hardware:
 * - Arduino board (tested on Uno/Nano)
 * - 2x MX1508 motor drivers
 * - Big Mouth Billy Bass toy
 * - Audio input connected to A0
 * 
 * Pins:
 * - A0: Audio input
 * - D3/D5: Mouth motor
 * - D6/D9: Body/tail motor
 * 
 * Commands:
 * o: Open mouth    c: Close mouth
 * f: Flap tail     b: Body forward
 * s: Singing mode  r: Reset position
 * +/-: Speed up/down
 * a: Toggle audio reactivity mode
 * m: Toggle manual/auto mode
 * d: Toggle debug mode
 * h: Help menu
 */

// Include all module headers
#include "src/core/Config.h"
#include "src/core/BillyBass.h"
#include "src/core/StateMachine.h"
#include "src/commands/CommandSystem.h"
#include "src/utils/Debug.h"

// Define global state variables
uint8_t fishState = 0;                 // Current state of the fish (0=waiting, 1=talking, 2=flapping)
bool audioReactivityEnabled = true;    // Whether audio reactivity is enabled
bool manualMode = false;               // Whether manual mode is enabled
bool debugMode = false;                // Whether debug mode is enabled
uint16_t soundVolume = 0;              // Current sound volume
bool talking = false;                  // Whether the fish is currently talking

// Define timing variables
unsigned long currentTime = 0;         // Current time in milliseconds
unsigned long mouthActionTime = 0;     // Time for next mouth action
unsigned long bodyActionTime = 0;      // Time for next body action
unsigned long lastActionTime = 0;      // Time of last action

void setup() {
    // Initialize serial communication
    Serial.begin(9600);
    
    // Configure pins
    pinMode(SOUND_PIN, INPUT);
    
    // Initialize Billy Bass
    billy.begin();
    
    // Show welcome message and menu
    Serial.println(F("\n=== BTBillyBass Initialized ==="));
    Serial.println(F("Audio-reactive animatronic fish control"));
    Serial.println(F("Type 'h' for help menu"));
    printMenu();
}

void loop() {
    // Update current time
    currentTime = millis();
    
    // Process serial commands if available
    if (Serial.available() > 0) {
        char input = Serial.read();
        processCommand(input);
    }
    
    // Run audio reactive mode if enabled and not in manual mode
    if (audioReactivityEnabled && !manualMode) {
        updateSoundInput();
        stateMachineBillyBass();
    }
} 