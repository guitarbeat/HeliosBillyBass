#ifndef BILLYBASSMOTOR_H
#define BILLYBASSMOTOR_H

#include <MX1508.h>
#include "../core/Config.h"

/**
 * Controls DC motors using the MX1508 driver
 * Handles basic and advanced motor operations
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
    
    // Advanced Motor Control
    void rampSpeed(uint8_t targetSpeed, bool isForward);  // Gradually increase speed
    void smoothStop();                                    // Gradually decrease speed to stop

private:
    MX1508 _motor;           // Motor driver instance
    uint8_t _currentSpeed;   // Current motor speed
};

#endif // BILLYBASSMOTOR_H 