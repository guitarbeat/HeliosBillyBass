#include "StateMachine.h"
#include "../utils/Debug.h"
#include <Arduino.h>

/**
 * Updates sound input from analog pin
 * Reads the current sound level and stores it in the global soundVolume variable
 */
void updateSoundInput() {
    // Read analog value from sound sensor
    soundVolume = analogRead(SOUND_PIN);
    
#if DEBUG
    // Only print debug info if sound is above threshold to avoid console spam
    if (soundVolume > SILENCE_THRESHOLD) {
        DEBUG_PRINT(F("Sound: "));
        DEBUG_PRINTLN(soundVolume);
    }
#endif
}

/**
 * State machine for Billy Bass
 * Manages the fish's behavior based on current state, sound input, and timing
 */
void stateMachineBillyBass() {
    uint8_t prevState = fishState;
    
    switch (fishState) {
        case STATE_WAITING: // Waiting for input
            // Check for sound above threshold
            if (soundVolume > SILENCE_THRESHOLD && currentTime > mouthActionTime) { 
                talking = true; 
                mouthActionTime = currentTime + PAUSE_TIME;
                fishState = STATE_TALKING; // Transition to talking state
            } 
            // Stop motors if beyond scheduled talking time
            else if (currentTime > mouthActionTime + PAUSE_TIME) { 
                billy.mouthMotor.stop();
                billy.bodyMotor.stop();
            }
            
            // Check if fish has been idle too long
            if (currentTime - lastActionTime > IDLE_TIMEOUT) { 
                // Schedule next random flap
                lastActionTime = currentTime + random(FLAP_INTERVAL_MIN, FLAP_INTERVAL_MAX); 
                fishState = STATE_FLAPPING; // Transition to flapping state
            }
            break;

        case STATE_TALKING: // Talking (mouth moving)
            if (currentTime < mouthActionTime) { 
                // If we have a scheduled mouth action in the future
                if (talking) { 
                    // Open mouth and articulate body
                    billy.openMouth(); 
                    lastActionTime = currentTime;
                    billy.articulateBody(true);
                }
            }
            else { 
                // Close mouth and stop articulating
                billy.closeMouth();
                billy.articulateBody(false);
                talking = false;
                fishState = STATE_WAITING; // Return to waiting state
            }
            break;

        case STATE_FLAPPING: // Flapping (random movement)
            // Perform a single flap
            billy.flap();
            fishState = STATE_WAITING; // Return to waiting state
            break;
            
        default:
            // Invalid state, reset to waiting
            fishState = STATE_WAITING;
            break;
    }
    
    // Log state transitions
    if (prevState != fishState) {
        debugStateTransition(prevState, fishState);
    }
} 