/*
 * Alexa-Enabled Big Mouth Billy Bass
 * =================================
 * 
 * This Arduino sketch controls a Big Mouth Billy Bass animatronic fish to respond to 
 * audio input and integrate with Amazon Alexa. The fish's mouth and body movements are 
 * synchronized with audio, creating the illusion that the fish is speaking.
 * 
 * HARDWARE CONNECTIONS:
 * - Pins D3, D5: Mouth motor control (via MX1508 H-bridge)
 * - Pins D6, D9: Body motor control (via MX1508 H-bridge)
 * - Pin A0: Audio input (from Alexa or other audio source)
 * 
 * STATE MACHINE:
 * - State 0: WAITING - Monitoring audio input, transitions to flapping if idle
 * - State 1: TALKING - Mouth and body animations active when audio detected
 * - State 2: FLAPPING - Idle animation when no activity detected for a while
 * 
 * CONFIGURATION:
 * - Adjust 'silence' variable to set audio detection threshold
 * - Motor speeds can be adjusted in the openMouth(), closeMouth(), and articulateBody() functions
 * - Random timing parameters can be adjusted in the articulateBody() function
 * 
 * ALEXA INTEGRATION:
 * - Current version responds to any audio input above threshold
 * - Future versions will include direct Alexa API integration
 * 
 * CREDITS:
 * This code builds upon work by:
 * - Donald Bell: https://www.instructables.com/id/Animate-a-Billy-Bass-Mouth-With-Any-Audio-Source/
 * - jswett77: https://github.com/jswett77/big_mouth/blob/master/billy.ino
 * - Jordan Bunker <jordan@hierotechnics.com> 2019
 * 
 * LICENSE: MIT License (https://opensource.org/licenses/MIT)
 */

#include <MX1508.h>

// Motor control setup
MX1508 bodyMotor(6, 9); // Sets up an MX1508 controlled motor on PWM pins 6 and 9
MX1508 mouthMotor(5, 3); // Sets up an MX1508 controlled motor on PWM pins 5 and 3

// Audio input configuration
int soundPin = A0;       // Sound input pin
int silence = 12;        // Threshold for "silence". Anything below this level is ignored.

// Motor control variables
int bodySpeed = 0;       // Body motor speed initialized to 0
int soundVolume = 0;     // Variable to hold the analog audio value
int fishState = 0;       // Variable to indicate the state Billy is in (0=waiting, 1=talking, 2=flapping)
bool talking = false;    // Indicates whether the fish should be talking or not

// Timing variables for animation scheduling
long currentTime;        // Current time in milliseconds
long mouthActionTime;    // Scheduled time for mouth action to end
long bodyActionTime;     // Scheduled time for body action to end
long lastActionTime;     // Time of the last action

/**
 * Setup function - initializes pins and serial communication
 */
void setup() {
  // Initialize motor speeds to zero
  bodyMotor.setSpeed(0); 
  mouthMotor.setSpeed(0);

  // Configure audio input pin
  pinMode(soundPin, INPUT);

  // Initialize serial communication for debugging
  Serial.begin(9600);
  
  // Optional: Print startup message
  Serial.println("BTBillyBass initialized. Waiting for audio input...");
}

/**
 * Main program loop
 */
void loop() {
  currentTime = millis();     // Update current time
  updateSoundInput();         // Read audio input level
  SMBillyBass();              // Run state machine to control fish behavior
}

/**
 * State Machine for Billy Bass behavior control
 * Controls transitions between waiting, talking, and flapping states
 */
void SMBillyBass() {
  switch (fishState) {
    case 0: // WAITING STATE - Monitoring audio input
      if (soundVolume > silence) { // If audio detected above threshold
        if (currentTime > mouthActionTime) { // And if no mouth action is scheduled
          talking = true;
          mouthActionTime = currentTime + 100; // Schedule mouth action
          fishState = 1; // Transition to talking state
        }
      } else if (currentTime > mouthActionTime + 100) { // If beyond scheduled talking time
        bodyMotor.halt();
        mouthMotor.halt();
      }
      
      // Check if fish has been idle too long
      if (currentTime - lastActionTime > 1500) {
        // Schedule next idle animation (30-60 seconds from now)
        lastActionTime = currentTime + floor(random(30, 60)) * 1000L;
        fishState = 2; // Transition to flapping state
      }
      break;

    case 1: // TALKING STATE - Animating mouth and body
      if (currentTime < mouthActionTime) { // If mouth action is scheduled
        if (talking) {
          openMouth();
          lastActionTime = currentTime;
          articulateBody(true);
        }
      }
      else { // End of mouth action
        closeMouth();
        articulateBody(false);
        talking = false;
        fishState = 0; // Return to waiting state
      }
      break;

    case 2: // FLAPPING STATE - Idle animation
      flap();
      fishState = 0; // Return to waiting state
      break;
  }
}

/**
 * Reads the audio input level from the sound pin
 */
int updateSoundInput() {
  soundVolume = analogRead(soundPin);
  return soundVolume; // Return value for potential future use
}

/**
 * Opens the fish's mouth by controlling the mouth motor
 */
void openMouth() {
  mouthMotor.halt();        // Stop the mouth motor
  mouthMotor.setSpeed(220); // Set the mouth motor speed
  mouthMotor.forward();     // Open the mouth
}

/**
 * Closes the fish's mouth by controlling the mouth motor
 */
void closeMouth() {
  mouthMotor.halt();        // Stop the mouth motor
  mouthMotor.setSpeed(180); // Set the mouth motor speed
  mouthMotor.backward();    // Close the mouth
}

/**
 * Controls the body movements of the fish
 * 
 * @param talking Boolean indicating if the fish is currently "talking"
 */
void articulateBody(bool talking) {
  if (talking) { // If fish is talking
    if (currentTime > bodyActionTime) { // And if no body action is scheduled
      // Create random movement patterns
      int r = floor(random(0, 8)); // Random number between 0 and 7
      
      if (r < 1) {
        // No body movement
        bodySpeed = 0;
        bodyActionTime = currentTime + floor(random(500, 1000));
        bodyMotor.forward();
      } else if (r < 3) {
        // Slow body movement
        bodySpeed = 150;
        bodyActionTime = currentTime + floor(random(500, 1000));
        bodyMotor.forward();
      } else if (r == 4) {
        // Medium body movement
        bodySpeed = 200;
        bodyActionTime = currentTime + floor(random(500, 1000));
        bodyMotor.forward();
      } else if (r == 5) {
        // Tail raise movement
        bodySpeed = 0;
        bodyMotor.halt();
        bodyMotor.setSpeed(255);
        bodyMotor.backward();
        bodyActionTime = currentTime + floor(random(900, 1200));
      } else {
        // Full speed head raise
        bodySpeed = 255;
        bodyMotor.forward();
        bodyActionTime = currentTime + floor(random(1500, 3000));
      }
    }

    bodyMotor.setSpeed(bodySpeed);
  } else {
    // If not talking and beyond scheduled body action time
    if (currentTime > bodyActionTime) {
      bodyMotor.halt();
      bodyActionTime = currentTime + floor(random(20, 50));
    }
  }
}

/**
 * Performs a flapping animation (tail raise) when the fish is idle
 */
void flap() {
  bodyMotor.setSpeed(180); // Set body motor to medium-high speed
  bodyMotor.backward();    // Raise the tail
  delay(500);              // Wait for dramatic effect
  bodyMotor.halt();        // Stop the motor
}
