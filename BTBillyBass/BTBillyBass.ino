/*
 * BTBillyBass - Serial Monitor Control System
 * 
 * Control a Big Mouth Billy Bass animatronic using serial commands with precise timing.
 * Features include:
 *  - Direct motor control for body and mouth movements
 *  - Timed actions with automatic position holding
 *  - Memory-efficient implementation using PROGMEM strings
 *  - Command parsing with flexible duration parameters
 * 
 * Hardware Requirements:
 *  - Arduino board with at least 4 PWM pins
 *  - MX1508 H-bridge motor drivers
 *  - Big Mouth Billy Bass (or compatible animatronic)
 *  - 5V power supply for motors
 * 
 * Pin Configuration:
 *  - Body motor: Pins 6, 9 (controls head/tail movement)
 *  - Mouth motor: Pins 5, 3 (controls mouth opening/closing)
 * 
 * For detailed command list, enter 'help' in the Serial Monitor.
 * 
 * Original code by Jordan Bunker <jordan@hierotechnics.com> 2019
 * Modified for manual control with timed actions and position holding.
 * License: MIT License (https://opensource.org/licenses/MIT)
 */

#include <MX1508.h>
#include <avr/pgmspace.h> // For PROGMEM string storage

// Define FPSTR if it doesn't exist (compatibility for older Arduino versions)
#ifndef FPSTR
#define FPSTR(pstr_pointer) (reinterpret_cast<const __FlashStringHelper *>(pstr_pointer))
#endif

// ======= HARDWARE CONFIGURATION =======
// Motor control pins and setup
MX1508 bodyMotor(6, 9);  // Controls head/tail movement on PWM pins 6 and 9
MX1508 mouthMotor(5, 3);  // Controls mouth movement on PWM pins 5 and 3

// Audio input configuration
const int SOUND_PIN = A0;                // Sound input on analog pin A0
int silenceThreshold = 12;               // Default threshold for "silence" - anything below this is ignored
const float AUDIO_DROP_FACTOR = 0.7;     // Factor to detect audio level drops (70% of previous level)
const float LOUD_SOUND_FACTOR = 1.5;     // Factor to detect loud sounds (150% of threshold)

// Motor speed settings
const int BODY_SPEED = 150;    // Maximum speed for body movements (was 200)
const int MOUTH_SPEED = 100;   // Increased speed for mouth movements (was 100)
const int HOLD_DIVISOR = 2;    // Divides speed when holding position to reduce power

// Default timing values based on testing
const int DEFAULT_MOUTH_OPEN_TIME = 100;     
const int DEFAULT_BODY_FORWARD_TIME = 700;  // Longer time for body forward for full calibration (was 500ms)
const int DEFAULT_BODY_BACKWARD_TIME = 1000;  // Longer time for body backward (was 500ms)
const int DEFAULT_TAIL_ONLY_TIME = 500;      // Increased time for tail movement (was 300ms)
const int HOLD_TIMEOUT = 1000;               // Time to hold before auto-releasing (1 second)

// Yap mode configuration
const int MOUTH_DEBOUNCE_TIME = 100;      // Minimum time between mouth movements (ms)
const int HEAD_MOVEMENT_INTERVAL = 6000; // Reduced time between random head movements (was 5000ms)
const int LOUD_SOUND_INTERVAL = 500;     // Time window to count consecutive loud sounds (ms)
const int CONSECUTIVE_SOUNDS_THRESHOLD = 3; // Number of consecutive sounds to trigger head movement
const int HEAD_RETURN_DELAY = 500;       // Time to wait before returning head to forward position

// ======= STATE MANAGEMENT =======
// Serial command processing
char inputBuffer[32];          // Buffer for incoming commands (32 byte limit)
int inputIndex = 0;            // Index for current position in buffer
boolean stringComplete = false; // Flag for complete command received
boolean yapMode = false;       // Flag to track if in yap mode

// Timing variables
unsigned long currentTime = 0;
unsigned long mouthActionEndTime = 0;
unsigned long bodyActionEndTime = 0;
boolean mouthTimedAction = false;
boolean bodyTimedAction = false;

// Hold state tracking
unsigned long mouthHoldStartTime = 0;
unsigned long bodyHoldStartTime = 0;
boolean mouthHolding = false;
boolean bodyHolding = false;

// Motor direction tracking
boolean mouthOpenDirection = true;  // true = open, false = close
boolean bodyForwardDirection = true; // true = forward, false = backward

// Motor type identifiers for generic functions
#define BODY_MOTOR 0
#define MOUTH_MOTOR 1

// ======= MESSAGE STRINGS (STORED IN FLASH) =======
// Command response messages - all unused messages removed

// Help text - concise version only
const char HELP_TITLE[] PROGMEM = "\n== Billy Bass Controls ==";
const char HELP_BODY_CMDS[] PROGMEM = "Body: forward, backward, release, hold";
const char HELP_MOUTH_CMDS[] PROGMEM = "Mouth: open, close, release, hold";
const char HELP_TIME_INFO[] PROGMEM = "Add time (ms) to commands: 'body forward 1000'";
const char HELP_YAP_CMD[] PROGMEM = "Yap: activates audio-responsive mode ('stop' to exit)";
const char HELP_STOP_CMD[] PROGMEM = "Stop: halts all motors";
const char HELP_END[] PROGMEM = "========================";

// ======= SYSTEM INITIALIZATION =======
void setup() {
  // Initialize motors in safe state
  bodyMotor.setSpeed(0);
  mouthMotor.setSpeed(0);
  bodyMotor.halt();
  mouthMotor.halt();
  
  // Initialize random seed for head movements
  randomSeed(analogRead(A1)); // Use unconnected analog pin for randomness
  
  // Setup serial communication
  Serial.begin(9600);
  
  // Configure audio input pin
  pinMode(SOUND_PIN, INPUT);
  
  // Display help information on startup
  printHelp();
}

// ======= MAIN PROGRAM LOOP =======
void loop() {
  currentTime = millis();
  
  // Check for yap mode
  if (yapMode) {
    processYap();
  } else {
    checkTimedActions();
    checkHoldTimeouts(); // Check if we need to release holds
  }
  
  // Process completed commands
  if (stringComplete) {
    processCommand();
    inputIndex = 0;
    inputBuffer[0] = '\0';
    stringComplete = false;
  }
  
  // Read incoming serial data
  while (Serial.available() && inputIndex < 31) {
    char inChar = (char)Serial.read();
    inputBuffer[inputIndex++] = inChar;
    inputBuffer[inputIndex] = '\0';
    
    if (inChar == '\n' || inChar == '\r') {
      inputBuffer[--inputIndex] = '\0';
      stringComplete = true;
    }
  }
}

// ======= MOTOR TIMING MANAGEMENT =======
// Checks if timed motor actions have completed and transitions to hold state
void checkTimedActions() {
  // Handle mouth timed actions
  if (mouthTimedAction && currentTime >= mouthActionEndTime) {
    transitionToHold(MOUTH_MOTOR, mouthMotor, MOUTH_SPEED, mouthOpenDirection, &mouthTimedAction);
  }
  
  // Handle body timed actions
  if (bodyTimedAction && currentTime >= bodyActionEndTime) {
    transitionToHold(BODY_MOTOR, bodyMotor, BODY_SPEED, bodyForwardDirection, &bodyTimedAction);
  }
}

// Check if we need to release any holds that have timed out
void checkHoldTimeouts() {
  if (mouthHolding && (currentTime - mouthHoldStartTime >= HOLD_TIMEOUT)) {
    releaseMotor(MOUTH_MOTOR);
    mouthHolding = false;
    Serial.println("Mouth hold: timeout");
  }
  if (bodyHolding && (currentTime - bodyHoldStartTime >= HOLD_TIMEOUT)) {
    releaseMotor(BODY_MOTOR);
    bodyHolding = false;
    Serial.println("Body hold: timeout");
  }
}

// Helper function to transition a motor to hold state after a timed action
void transitionToHold(uint8_t motorType, MX1508 &motor, int speed, boolean direction, 
                      boolean *timedAction) {
  *timedAction = false;
  
  // For body motor (both forward and backward directions), just release instead of holding
  if (motorType == BODY_MOTOR) {
    motor.setSpeed(0);
    motor.release();
    bodyHolding = false;
    // Minimal output - just a single line
    Serial.println(direction ? "Body forward: complete" : "Body backward: complete");
  } else {
    // For mouth motor, continue with hold behavior
    motor.setSpeed(speed / HOLD_DIVISOR);
    direction ? motor.forward() : motor.backward();
    mouthHolding = true;
    mouthHoldStartTime = currentTime;
    // Minimal output for mouth actions too
    Serial.println(direction ? "Mouth open: complete (holding)" : "Mouth closed: complete");
  }
}

// ======= STRING UTILITY FUNCTIONS =======
// Checks if two strings are equal
bool strEqual(const char* str1, const char* str2) {
  return strcmp(str1, str2) == 0;
}

// Checks if a string starts with a given prefix
bool strStartsWith(const char* str, const char* prefix) {
  return strncmp(str, prefix, strlen(prefix)) == 0;
}

// Prints a string stored in program memory
void printProgmemString(const char* str) {
  char c;
  while ((c = pgm_read_byte(str++))) Serial.print(c);
  Serial.println();
}

// ======= COMMAND PROCESSING =======
// Helper function to parse duration from commands
int parseDuration(const char* cmdPrefix) {
  int duration = 0;
  char format[32];
  snprintf(format, sizeof(format), "%s %%d", cmdPrefix);
  sscanf(inputBuffer, format, &duration);
  return duration;
}

void processCommand() {
  // Convert input to lowercase for case-insensitive commands
  for (int i = 0; inputBuffer[i]; i++) inputBuffer[i] = tolower(inputBuffer[i]);
  
  // Extract command parts - no need to echo received command
  char command[20] = {0};
  sscanf(inputBuffer, "%s", command);
  
  // Parse and execute commands
  if (strStartsWith(command, "body")) {
    if (strEqual(inputBuffer, "body forward")) {
      // Move body forward for default time, then release (position held by gears)
      timedMotorAction(BODY_MOTOR, true, DEFAULT_BODY_FORWARD_TIME);
    }
    else if (strStartsWith(inputBuffer, "body forward ")) {
      // Custom timed forward movement
      timedMotorAction(BODY_MOTOR, true, parseDuration("body forward"));
    }
    else if (strEqual(inputBuffer, "body backward")) {
      // Move body backward for default time
      timedMotorAction(BODY_MOTOR, false, DEFAULT_BODY_BACKWARD_TIME);
    }
    else if (strStartsWith(inputBuffer, "body backward ")) {
      int duration = parseDuration("body backward");
      timedMotorAction(BODY_MOTOR, false, duration);
      
      // Just indicate if it's a tail-only movement without extra explanation
      if (duration <= DEFAULT_TAIL_ONLY_TIME) {
        Serial.println("Tail-only");
      }
    }
    else if (strEqual(inputBuffer, "body release")) {
      // Release body motor (useful after tail-only movements)
      releaseMotor(BODY_MOTOR);
    }
    else if (strEqual(inputBuffer, "body hold")) {
      // Hold body in current position
      holdMotor(BODY_MOTOR, "Body: holding");
    }
  }
  else if (strStartsWith(command, "mouth")) {
    if (strEqual(inputBuffer, "mouth open")) {
      // Open mouth for default time, then hold
      timedMotorAction(MOUTH_MOTOR, true, DEFAULT_MOUTH_OPEN_TIME);
    }
    else if (strStartsWith(inputBuffer, "mouth open ")) {
      // Custom timed mouth opening
      timedMotorAction(MOUTH_MOTOR, true, parseDuration("mouth open"));
    }
    else if (strEqual(inputBuffer, "mouth close") || strStartsWith(inputBuffer, "mouth close ")) {
      // For mouth close, just release as it has a spring-loaded design
      releaseMotor(MOUTH_MOTOR);
    }
    else if (strEqual(inputBuffer, "mouth release")) {
      // Release mouth motor (allows spring to close it)
      releaseMotor(MOUTH_MOTOR);
    }
    else if (strEqual(inputBuffer, "mouth hold")) {
      // Hold mouth in current position
      holdMotor(MOUTH_MOTOR, "Mouth: holding");
    }
  }
  else if (strEqual(inputBuffer, "stop")) {
    // Stop all motors and exit yap mode
    Serial.println("Stopped");
    mouthTimedAction = bodyTimedAction = false;
    yapMode = false;
    bodyMotor.halt();
    mouthMotor.halt();
  }
  else if (strEqual(inputBuffer, "yap")) {
    // Start yap mode with default threshold
    startYapMode(silenceThreshold);
  }
  else if (strStartsWith(inputBuffer, "yap ")) {
    // Start yap mode with custom threshold
    int threshold = parseDuration("yap");
    startYapMode(threshold);
  }
  else if (strEqual(inputBuffer, "help")) {
    printHelp();
  }
  else {
    Serial.println("Unknown command. Type 'help'.");
  }
}

// ======= GENERIC MOTOR CONTROL FUNCTIONS =======
// Helper to access motor variables based on type
void getMotorParams(uint8_t motorType, MX1508 **motor, int *speed, boolean **timedAction, 
                   boolean **direction, unsigned long *actionEndTime) {
  if (motorType == BODY_MOTOR) {
    *motor = &bodyMotor;
    *speed = BODY_SPEED;
    *timedAction = &bodyTimedAction;
    *direction = &bodyForwardDirection;
    *actionEndTime = bodyActionEndTime;
  } else { // MOUTH_MOTOR
    *motor = &mouthMotor;
    *speed = MOUTH_SPEED;
    *timedAction = &mouthTimedAction;
    *direction = &mouthOpenDirection;
    *actionEndTime = mouthActionEndTime;
  }
}

// Generic function to control motors for both body and mouth
void setMotorDirection(uint8_t motorType, boolean isForward) {
  // Use short direct message instead of PROGMEM string
  Serial.println(motorType == BODY_MOTOR ? 
    (isForward ? "Body: forward" : "Body: backward") : 
    (isForward ? "Mouth: opening" : "Mouth: closing"));
    
  MX1508 *motor; int speed; boolean *timedAction, *direction; unsigned long endTime;
  getMotorParams(motorType, &motor, &speed, &timedAction, &direction, &endTime);
  
  *timedAction = false;
  motor->setSpeed(speed);
  isForward ? motor->forward() : motor->backward();
  *direction = isForward;
}

// Generic function to release motors
void releaseMotor(uint8_t motorType) {
  // Use short direct message instead of PROGMEM string
  Serial.println(motorType == BODY_MOTOR ? "Body: released" : "Mouth: released");
  
  MX1508 *motor; int speed; boolean *timedAction, *direction; unsigned long endTime;
  getMotorParams(motorType, &motor, &speed, &timedAction, &direction, &endTime);
  
  *timedAction = false;
  motor->setSpeed(0);
  motor->release();
  
  // Clear hold state
  if (motorType == BODY_MOTOR) {
    bodyHolding = false;
  } else {
    mouthHolding = false;
  }
}

// Generic function to hold motor position
void holdMotor(uint8_t motorType, const char* holdMsg) {
  // Use the simplified holdMsg directly
  Serial.println(holdMsg);
  
  MX1508 *motor; int speed; boolean *timedAction, *direction; unsigned long endTime;
  getMotorParams(motorType, &motor, &speed, &timedAction, &direction, &endTime);
  
  *timedAction = false;
  motor->setSpeed(speed / HOLD_DIVISOR);
  *direction ? motor->forward() : motor->backward();
  
  // Set hold state
  if (motorType == BODY_MOTOR) {
    bodyHolding = true;
    bodyHoldStartTime = currentTime;
  } else {
    mouthHolding = true;
    mouthHoldStartTime = currentTime;
  }
}

// Generic function for timed motor actions
void timedMotorAction(uint8_t motorType, boolean isForward, int duration) {
  // Simplified output for timed actions
  Serial.print(motorType == BODY_MOTOR ? 
    (isForward ? "Body forward: " : "Body backward: ") : 
    (isForward ? "Mouth open: " : "Mouth close: "));
  Serial.print(duration);
  Serial.println("ms");
  
  MX1508 *motor; int speed; boolean *timedAction, *direction; unsigned long endTime;
  getMotorParams(motorType, &motor, &speed, &timedAction, &direction, &endTime);
  
  motor->setSpeed(speed);
  isForward ? motor->forward() : motor->backward();
  *direction = isForward;
  *timedAction = true;
  if (motorType == BODY_MOTOR)
    bodyActionEndTime = currentTime + duration;
  else
    mouthActionEndTime = currentTime + duration;
}

// ======= YAP MODE FUNCTIONS =======
// Start yap mode with specified threshold
void startYapMode(int threshold) {
  silenceThreshold = threshold;
  
  Serial.print("Yap mode activated with threshold: ");
  Serial.println(silenceThreshold);
  
  // First calibrate the audio level
  calibrateAudio();
  
  // Single upward movement to indicate calibration is complete
  timedMotorAction(BODY_MOTOR, true, DEFAULT_BODY_FORWARD_TIME / 2);
  delay(600); // Wait for movement to complete
  
  yapMode = true;
} 


// Auto-calibrate audio levels by sampling ambient noise
void calibrateAudio() {
  Serial.println("Calibrating audio levels (3s)...");
  
  // Take multiple samples to find the ambient noise level
  long sumLevels = 0;
  int sampleCount = 100;
  int maxLevel = 0;
  
  for (int i = 0; i < sampleCount; i++) {
    int level = analogRead(SOUND_PIN);
    sumLevels += level;
    if (level > maxLevel) maxLevel = level;
    delay(30); // Sample over 3 seconds
  }
  
  int avgLevel = sumLevels / sampleCount;
  
  // Set threshold to be slightly higher than the ambient level
  silenceThreshold = avgLevel + 10;
  
  Serial.print("Ambient level: ");
  Serial.print(avgLevel);
  Serial.print(" | Max: ");
  Serial.print(maxLevel);
  Serial.print(" | Threshold: ");
  Serial.println(silenceThreshold);
}

// Process audio input for yap mode
void processYap() {
  // Read audio level
  int audioLevel = analogRead(SOUND_PIN);
  
  // Check for serial input (to allow stop command)
  if (Serial.available()) {
    return; // Let the main loop handle the command
  }
  
  // Process mouth movements based on audio
  processMouthMovements(audioLevel);
  
  // Process head movements based on audio patterns
  processHeadMovements(audioLevel);
}

// Handle mouth movements in response to audio
void processMouthMovements(int audioLevel) {
  static boolean mouthIsOpen = false;
  static unsigned long lastMouthAction = 0;
  static int lastAudioLevel = 0;
  
  // Avoid too frequent mouth actions with a small delay
  if (currentTime - lastMouthAction > MOUTH_DEBOUNCE_TIME) {
    // Open mouth when audio rises above threshold
    if (audioLevel > silenceThreshold && !mouthIsOpen) {
      mouthMotor.setSpeed(MOUTH_SPEED);
      mouthMotor.forward();
      mouthIsOpen = true;
      lastMouthAction = currentTime;
      
      // Minimal debugging output
      Serial.print("A:");
      Serial.println(audioLevel);
    }
    // Close mouth when audio drops
    else if ((audioLevel < silenceThreshold || audioLevel < lastAudioLevel * AUDIO_DROP_FACTOR) && mouthIsOpen) {
      mouthMotor.setSpeed(0);
      mouthMotor.release(); // Use spring to close mouth
      mouthIsOpen = false;
      lastMouthAction = currentTime;
    }
    
    lastAudioLevel = audioLevel;
  }
}

// Handle dynamic head movements
void processHeadMovements(int audioLevel) {
  // Static variables to maintain state between function calls
  static unsigned long lastHeadMovement = 0;
  static unsigned long headMovementEndTime = 0;
  static unsigned long returnToForwardTime = 0;
  static boolean headMoving = false;
  static boolean returningToForward = false;
  static boolean headDirection = true; // true = forward/up, false = backward/down
  static int consecutiveLoudSounds = 0;
  static unsigned long lastLoudSound = 0;
  
  // Detect sustained loud audio for head movement triggering
  if (audioLevel > silenceThreshold * LOUD_SOUND_FACTOR) {
    if (currentTime - lastLoudSound < LOUD_SOUND_INTERVAL) {
      consecutiveLoudSounds++;
    } else {
      consecutiveLoudSounds = 1;
    }
    lastLoudSound = currentTime;
  }
  
  // Non-blocking head movement state machine
  if (headMoving) {
    // If movement is complete, stop and release
    if (currentTime >= headMovementEndTime) {
      bodyMotor.setSpeed(0);
      bodyMotor.release();
      headMoving = false;
      
      // Schedule return to forward position if we moved backward
      if (!headDirection) {
        returningToForward = true;
        returnToForwardTime = currentTime + HEAD_RETURN_DELAY;
      }
    }
  } 
  // Handle return to forward position after backward movement
  else if (returningToForward) {
    if (currentTime >= returnToForwardTime) {
      // Move back to forward position
      bodyMotor.setSpeed(BODY_SPEED);
      bodyMotor.forward();
      headMovementEndTime = currentTime + DEFAULT_BODY_FORWARD_TIME;
      headMoving = true;
      returningToForward = false;
      headDirection = true;
      Serial.println("Head: returning to up position");
    }
  }
  // Decide if we should start a new head movement
  else if (!headMoving && !returningToForward) {
    boolean shouldMoveHead = false;
    
    // Move head based on audio patterns or timed intervals
    if (consecutiveLoudSounds >= CONSECUTIVE_SOUNDS_THRESHOLD) {
      shouldMoveHead = true;
      consecutiveLoudSounds = 0;
    } 
    else if (currentTime - lastHeadMovement > HEAD_MOVEMENT_INTERVAL) {
      shouldMoveHead = true;
    }
    
    if (shouldMoveHead) {
      // Decide movement direction - increase chance of tail/backward movement for more dynamics
      if (!headDirection || random(10) < 4) { // 40% chance to go backward if already forward (was 30%)
        // Move head backward/down with more varied durations
        headDirection = false;
        int duration = 400 + random(300); // 400-700ms (was 300-500ms)
        bodyMotor.setSpeed(BODY_SPEED);
        bodyMotor.backward();
        headMovementEndTime = currentTime + duration;
        Serial.println("Head: down");
      } else {
        // Small head forward/up adjustment
        headDirection = true;
        int duration = 300 + random(200); // 300-500ms (was 200-400ms)
        bodyMotor.setSpeed(BODY_SPEED);
        bodyMotor.forward();
        headMovementEndTime = currentTime + duration;
        Serial.println("Head: up");
      }
      
      headMoving = true;
      lastHeadMovement = currentTime;
    }
  }
}

// ======= HELP DISPLAY FUNCTION =======
void printHelp() {
  // Print concise help only
  printProgmemString(HELP_TITLE);
  printProgmemString(HELP_BODY_CMDS);
  printProgmemString(HELP_MOUTH_CMDS);
  printProgmemString(HELP_TIME_INFO);
  printProgmemString(HELP_YAP_CMD);
  printProgmemString(HELP_STOP_CMD);
  printProgmemString(HELP_END);
}