#ifndef BILLYBASS_H
#define BILLYBASS_H

#include "../drivers/BillyBassMotor.h"
#include "Config.h"

/**
 * High-level control for the animatronic fish
 * Manages movements, sequences, and audio-reactive behavior
 */
class BillyBass {
public:
    // Constructor and initialization
    BillyBass();
    void begin();
    
    // Basic movements
    void openMouth();
    void closeMouth();
    void flapTail();
    void bodyForward();
    
    // Complex sequences
    void resetMotorsToHome();
    void singingMotion();
    void flap();  // Random flap when "bored"
    
    // Audio reactivity
    void articulateBody(bool isTalking);
    
    // Settings
    void setMotorSpeed(uint8_t speed);
    uint8_t getMotorSpeed() const;
    void setMovementDuration(uint16_t duration);
    uint16_t getMovementDuration() const;
    
    // State checks
    bool isMouthOpen() const;
    bool isBodyMoved() const;

    // Motor instances
    BillyBassMotor mouthMotor;  // Controls the mouth
    BillyBassMotor bodyMotor;   // Controls the body/tail

private:
    // Helper method for motor movement
    void moveMotor(BillyBassMotor* motor, uint8_t speed, bool forward, uint16_t duration);
    
    uint8_t _motorSpeed;        // Current speed setting
    uint16_t _movementDuration; // Current duration setting
    uint8_t _motorState;        // Bit flags for positions
    uint8_t _bodySpeed;         // Body articulation speed
};

// Global instance
extern BillyBass billy;

#endif // BILLYBASS_H 