#include "BillyBassMotor.h"

// Constructor
BillyBassMotor::BillyBassMotor(uint8_t pin1, uint8_t pin2)
    : _motor(pin1, pin2), _currentSpeed(0) {}

// Basic Motor Control
void BillyBassMotor::begin() { 
    _motor.setSpeed(0); 
}

void BillyBassMotor::setSpeed(uint8_t speed) { 
    _currentSpeed = speed; 
    _motor.setSpeed(speed); 
}

uint8_t BillyBassMotor::getSpeed() const { 
    return _currentSpeed; 
}

void BillyBassMotor::forward() { 
    _motor.forward(); 
}

void BillyBassMotor::backward() { 
    _motor.backward(); 
}

void BillyBassMotor::stop() { 
    _motor.halt(); 
    _currentSpeed = 0; 
} 