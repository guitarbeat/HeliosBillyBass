#include "BillyBass.h"
#include "../utils/Debug.h"
#include <Arduino.h>

// Global instance definition
BillyBass billy;

// Constructor
BillyBass::BillyBass() : 
    mouthMotor(MOUTH_PIN1, MOUTH_PIN2),
    bodyMotor(BODY_PIN1, BODY_PIN2),
    _motorSpeed(DEFAULT_SPEED),
    _movementDuration(DEFAULT_DURATION),
    _motorState(0),
    _bodySpeed(0) {}

// Initialization
void BillyBass::begin() {
    mouthMotor.begin();
    bodyMotor.begin();
    resetMotorsToHome();
}

// Basic Movement Commands
void BillyBass::openMouth() {
    if (!isMouthOpen()) {
        mouthMotor.setSpeed(MOUTH_SPEED);
        mouthMotor.forward();
        _motorState |= MOUTH_OPEN_MASK;
        
        DEBUG_PRINTLN(F("Mouth opened"));
    }
}

void BillyBass::closeMouth() {
    if (isMouthOpen()) {
        mouthMotor.setSpeed(MOUTH_SPEED);
        mouthMotor.backward();
        _motorState &= ~MOUTH_OPEN_MASK;
        
        DEBUG_PRINTLN(F("Mouth closed"));
    }
}

void BillyBass::flapTail() {
    if (!isBodyMoved()) {
        bodyMotor.setSpeed(FLAP_SPEED);
        bodyMotor.backward();
        _motorState |= BODY_MOVED_MASK;
        
        DEBUG_PRINTLN(F("Tail flapped"));
    }
}

void BillyBass::bodyForward() {
    if (isBodyMoved()) {
        bodyMotor.setSpeed(_motorSpeed);
        bodyMotor.forward();
        _motorState &= ~BODY_MOVED_MASK;
        
        DEBUG_PRINTLN(F("Body moved forward"));
    }
}

// Helper method for motor movement
void BillyBass::moveMotor(BillyBassMotor* motor, uint8_t speed, bool forward, uint16_t duration) {
    motor->setSpeed(speed);
    forward ? motor->forward() : motor->backward();
    delay(duration);
    motor->stop();
}

// Complex Movement Sequences
void BillyBass::resetMotorsToHome() {
    // Reset mouth if needed
    if (isMouthOpen()) {
        moveMotor(&mouthMotor, _motorSpeed / 2, false, DEFAULT_DURATION);
        _motorState &= ~MOUTH_OPEN_MASK;
    }
    
    // Reset body if needed
    if (isBodyMoved()) {
        moveMotor(&bodyMotor, _motorSpeed / 2, true, DEFAULT_DURATION);
        _motorState &= ~BODY_MOVED_MASK;
    }
    
    delay(PAUSE_TIME);
    
    DEBUG_PRINTLN(F("Reset to home position"));
}

void BillyBass::singingMotion() {
    resetMotorsToHome();
    
    for (byte i = 0; i < 3; i++) {
        openMouth();
        delay(PAUSE_TIME);
        closeMouth();
        delay(PAUSE_TIME);
    }
    
    flapTail();
    delay(PAUSE_TIME);
    bodyForward();
    
    DEBUG_PRINTLN(F("Singing motion completed"));
}

// Audio Reactive Methods
void BillyBass::articulateBody(bool isTalking) {
    if (!isTalking) {
        if (currentTime > bodyActionTime) {
            bodyMotor.stop();
            bodyActionTime = currentTime + random(20, 50);
            DEBUG_PRINTLN(F("Body articulation stopped"));
        }
        return;
    }
    
    // Only change movement if it's time for a new action
    if (currentTime <= bodyActionTime) return;
    
    // Movement pattern definitions
    static const uint8_t speeds[] = {0, 150, 200, 255};
    static const uint16_t durations[][2] = {
        {500, 1000},   // No/slow/medium movement
        {900, 1200},   // Tail flap
        {1500, 3000}   // Full speed
    };
    static const char* patterns[] = {
        "Body - no movement",
        "Body - slow movement",
        "Body - medium movement",
        "Body - tail flap",
        "Body - full movement"
    };
    
    // Generate random movement pattern (0-7)
    uint8_t pattern = random(8);
    uint8_t patternType;
    uint8_t durationIdx = 0;
    bool isForward = true;
    
    if (pattern == 0) {
        // No movement
        patternType = 0;
        _bodySpeed = speeds[0];
    } 
    else if (pattern <= 2) {
        // Slow movement
        patternType = 1;
        _bodySpeed = speeds[1];
    }
    else if (pattern <= 4) {
        // Medium movement
        patternType = 2;
        _bodySpeed = speeds[2];
    }
    else if (pattern == 5) {
        // Tail flap during talking
        patternType = 3;
        _bodySpeed = speeds[3];
        isForward = false;
        durationIdx = 1;
    }
    else {
        // Full speed movement
        patternType = 4;
        _bodySpeed = speeds[3];
        durationIdx = 2;
    }
    
    // Apply the movement
    bodyMotor.stop();
    bodyMotor.setSpeed(_bodySpeed);
    isForward ? bodyMotor.forward() : bodyMotor.backward();
    bodyActionTime = currentTime + random(durations[durationIdx][0], durations[durationIdx][1]);
    
    DEBUG_PRINTLN(F(patterns[patternType]));
}

void BillyBass::flap() {
    bodyMotor.setSpeed(FLAP_SPEED);
    bodyMotor.backward();
    delay(500);
    bodyMotor.stop();
    
    DEBUG_PRINTLN(F("Random flap performed"));
}

// Settings
void BillyBass::setMotorSpeed(uint8_t speed) { 
    _motorSpeed = speed; 
    
    DEBUG_PRINT(F("Motor speed set to "));
    DEBUG_PRINTLN(_motorSpeed);
}

uint8_t BillyBass::getMotorSpeed() const { 
    return _motorSpeed; 
}

void BillyBass::setMovementDuration(uint16_t duration) { 
    _movementDuration = duration; 
    
    DEBUG_PRINT(F("Movement duration set to "));
    DEBUG_PRINTLN(_movementDuration);
}

uint16_t BillyBass::getMovementDuration() const { 
    return _movementDuration; 
}

// State Checks
bool BillyBass::isMouthOpen() const { 
    return (_motorState & MOUTH_OPEN_MASK); 
}

bool BillyBass::isBodyMoved() const { 
    return (_motorState & BODY_MOVED_MASK); 
} 