#ifndef BILLYBASSMOTOR_H
#define BILLYBASSMOTOR_H

#include <MX1508.h>
#include "../core/Config.h"

/**
 * Controls DC motors using the MX1508 driver
 * Handles basic and advanced motor operations with safety features
 */
class BillyBassMotor {
public:
    // Constructor
    BillyBassMotor(uint8_t pin1, uint8_t pin2);
    
    // Basic Motor Control
    void begin();                    // Initialize the motor
    void setSpeed(uint8_t speed);    // Set motor speed (0-255)
    uint8_t getSpeed() const;        // Get current speed
    void forward();                  // Set direction to forward
    void backward();                 // Set direction to backward
    void stop();                     // Stop the motor
    void halt();                     // Stops the motor
    
    // Advanced Motor Control
    void rampSpeed(uint8_t targetSpeed, bool isForward);  // Gradually increase speed
    void smoothStop();                                    // Gradually decrease speed to stop
    
    // Safety Features
    bool isMoving() const;           // Check if motor is currently moving
    bool isSafeToMove() const;       // Check if it's safe to move the motor
    void emergencyStop();            // Immediately stop motor
    bool needsCooldown() const;      // Check if motor needs cooldown
    unsigned long getRunTime() const; // Get current run time
    void resetRunTime();             // Reset the run time counter
    
    // Emergency Override
    void forceMove(uint8_t speed, bool isForward);  // Force movement ignoring safety
    
private:
    MX1508 _motor;                   // Motor driver instance
    uint8_t _currentSpeed;           // Current motor speed
    bool _isMoving;                  // Current movement state
    unsigned long _moveStartTime;     // When current movement started
    unsigned long _lastMoveTime;      // When last movement ended
    uint8_t _consecutiveMoves;        // Count of consecutive movements
    
    void updateSafetyMetrics();      // Update safety-related metrics
};

#endif // BILLYBASSMOTOR_H 