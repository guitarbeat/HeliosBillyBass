/*
 * BTBillyBass - Condensed Control System - V3
 * 
 * Control a Big Mouth Billy Bass animatronic using serial commands or audio response.
 * 
 * Hardware Requirements:
 *  - Arduino board with 4 PWM pins
 *  - MX1508 H-bridge motor drivers
 *  - Big Mouth Billy Bass animatronic
 *  - 5V power supply for motors
 * 
 * Pin Configuration:
 *  - Body motor: Pins 6, 9 (controls head/tail movement)
 *  - Mouth motor: Pins 5, 3 (controls mouth opening/closing)
 * 
 * For command list, enter 'help' in the Serial Monitor.
 * 
 * Based on code by Jordan Bunker <jordan@hierotechnics.com>
 * License: MIT License
 */

#include <MX1508.h>
#include <avr/pgmspace.h>

// Motor configuration
MX1508 bodyMotor(6, 9);  // Head/tail movement
MX1508 mouthMotor(5, 3); // Mouth movement

// Pin definitions
#define SOUND_PIN A0

// Motor constants
#define BODY_MOTOR 0
#define MOUTH_MOTOR 1
#define BODY_SPEED 200
#define MOUTH_SPEED 250
#define HOLD_DIVISOR 2

// Timing constants (milliseconds)
#define MOUTH_OPEN_TIME 120
#define BODY_FORWARD_TIME 800
#define BODY_BACKWARD_TIME 1000
#define TAIL_ONLY_TIME 400
#define HOLD_TIMEOUT 500
#define MOUTH_DEBOUNCE_TIME 110
#define SILENCE_TIMEOUT 3000
#define AUDIO_QUIET_THRESHOLD 1000

// Audio constants
#define AUDIO_DROP_FACTOR 0.7
#define LOUD_SOUND_FACTOR 1.5

// State variables
char cmdBuf[32];                  // Command buffer
int cmdIndex = 0;                 // Command buffer index
boolean cmdComplete = false;      // Command complete flag
boolean yapMode = false;          // Audio response mode
int silenceThreshold = 12;        // Audio threshold

// Motor state tracking
unsigned long curTime = 0;
unsigned long mouthEndTime = 0, bodyEndTime = 0;
unsigned long mouthHoldTime = 0, bodyHoldTime = 0;
boolean mouthAction = false, bodyAction = false;
boolean mouthHolding = false, bodyHolding = false;
boolean mouthOpen = true, bodyForward = true;

// Help text (stored in flash memory)
const char HELP[] PROGMEM = 
  "\n== Billy Controls ==\n"
  "Body: forward, backward, release, hold\n"
  "Mouth: open, close, release, hold\n"
  "Add time (ms): 'body forward 1000'\n"
  "Yap: audio mode ('stop' to exit)\n"
  "Stop: halt all motors\n"
  "===================";

// ======= INITIALIZATION =======
void setup() {
  // Safety first - motors off
  bodyMotor.halt();
  mouthMotor.halt();
  
  // Setup
  randomSeed(analogRead(A1));
  Serial.begin(9600);
  pinMode(SOUND_PIN, INPUT);
  
  // Print help and start in yap mode
  printPgm(HELP);
  delay(1000);
  startYapMode(silenceThreshold);
}

// ======= MAIN LOOP =======
void loop() {
  curTime = millis();
  
  // Process modes
  if (yapMode) {
    processYap();
  } else {
    // Check for timed actions ending
    if (mouthAction && curTime >= mouthEndTime) 
      finishAction(MOUTH_MOTOR);
    if (bodyAction && curTime >= bodyEndTime) 
      finishAction(BODY_MOTOR);
    
    // Check hold timeouts
    if (mouthHolding && (curTime - mouthHoldTime >= HOLD_TIMEOUT)) {
      motorControl(MOUTH_MOTOR, 0, false);
      mouthHolding = false;
    }
    if (bodyHolding && (curTime - bodyHoldTime >= HOLD_TIMEOUT)) {
      motorControl(BODY_MOTOR, 0, false);
      bodyHolding = false;
    }
  }
  
  // Process serial commands
  if (cmdComplete) {
    runCommand();
    cmdIndex = 0;
    cmdBuf[0] = '\0';
    cmdComplete = false;
  }
  
  // Read serial input
  while (Serial.available() && cmdIndex < 31) {
    char c = (char)Serial.read();
    cmdBuf[cmdIndex++] = c;
    cmdBuf[cmdIndex] = '\0';
    
    if (c == '\n' || c == '\r') {
      cmdBuf[--cmdIndex] = '\0';
      cmdComplete = true;
    }
  }
}

// ======= COMMAND PROCESSING =======
void runCommand() {
  // Convert to lowercase
  for (int i = 0; cmdBuf[i]; i++) 
    cmdBuf[i] = tolower(cmdBuf[i]);
  
  // Extract first command word
  char cmd[16] = {0};
  sscanf(cmdBuf, "%s", cmd);
  
  // BODY COMMANDS
  if (strEq(cmd, "body")) {
    if (strEq(cmdBuf, "body forward"))
      timedAction(BODY_MOTOR, true, BODY_FORWARD_TIME);
    else if (strStart(cmdBuf, "body forward "))
      timedAction(BODY_MOTOR, true, parseInt("body forward"));
    else if (strEq(cmdBuf, "body backward"))
      timedAction(BODY_MOTOR, false, BODY_BACKWARD_TIME);
    else if (strStart(cmdBuf, "body backward ")) {
      int t = parseInt("body backward");
      timedAction(BODY_MOTOR, false, t);
      if (t <= TAIL_ONLY_TIME) Serial.println("Tail-only");
    }
    else if (strEq(cmdBuf, "body release"))
      motorControl(BODY_MOTOR, 0, false);
    else if (strEq(cmdBuf, "body hold"))
      holdMotor(BODY_MOTOR);
  }
  // MOUTH COMMANDS
  else if (strEq(cmd, "mouth")) {
    if (strEq(cmdBuf, "mouth open"))
      timedAction(MOUTH_MOTOR, true, MOUTH_OPEN_TIME);
    else if (strStart(cmdBuf, "mouth open "))
      timedAction(MOUTH_MOTOR, true, parseInt("mouth open"));
    else if (strEq(cmdBuf, "mouth close") || strStart(cmdBuf, "mouth close "))
      motorControl(MOUTH_MOTOR, 0, false);
    else if (strEq(cmdBuf, "mouth release"))
      motorControl(MOUTH_MOTOR, 0, false);
    else if (strEq(cmdBuf, "mouth hold"))
      holdMotor(MOUTH_MOTOR);
  }
  // OTHER COMMANDS
  else if (strEq(cmdBuf, "stop")) {
    Serial.println("Stopped");
    mouthAction = bodyAction = yapMode = false;
    bodyMotor.halt();
    mouthMotor.halt();
  }
  else if (strEq(cmdBuf, "yap"))
    startYapMode(silenceThreshold);
  else if (strStart(cmdBuf, "yap "))
    startYapMode(parseInt("yap"));
  else if (strEq(cmdBuf, "help"))
    printPgm(HELP);
  else
    Serial.println("Unknown command. Type 'help'.");
}

// ======= MOTOR CONTROL =======
// Unified motor control function
void motorControl(uint8_t motor, int speed, boolean isForward) {
  MX1508 &m = (motor == BODY_MOTOR) ? bodyMotor : mouthMotor;
  
  // Output message
  if (speed > 0) {
    Serial.print(motor == BODY_MOTOR ? "Body " : "Mouth ");
    Serial.println(motor == BODY_MOTOR ? 
      (isForward ? "forward" : "backward") :
      (isForward ? "open" : "close"));
  } else {
    Serial.println(motor == BODY_MOTOR ? "Body released" : "Mouth released");
  }
  
  // Control motor
  m.setSpeed(speed);
  if (speed == 0) {
    m.release();
    if (motor == BODY_MOTOR) bodyHolding = false;
    else mouthHolding = false;
  } else {
    isForward ? m.forward() : m.backward();
    if (motor == BODY_MOTOR) bodyForward = isForward;
    else mouthOpen = isForward;
  }
}

// Start a timed motor action
void timedAction(uint8_t motor, boolean isForward, int duration) {
  int speed = (motor == BODY_MOTOR) ? BODY_SPEED : MOUTH_SPEED;
  
  // Output message
  Serial.print(motor == BODY_MOTOR ? 
    (isForward ? "Body forward: " : "Body backward: ") :
    (isForward ? "Mouth open: " : "Mouth close: "));
  Serial.print(duration);
  Serial.println("ms");
  
  // Control motor
  motorControl(motor, speed, isForward);
  
  // Set timing
  if (motor == BODY_MOTOR) {
    bodyAction = true;
    bodyEndTime = curTime + duration;
  } else {
    mouthAction = true;
    mouthEndTime = curTime + duration;
  }
}

// Finish a timed action
void finishAction(uint8_t motor) {
  if (motor == BODY_MOTOR) {
    bodyAction = false;
    motorControl(BODY_MOTOR, 0, false);
    Serial.println(bodyForward ? "Body forward: complete" : "Body backward: complete");
  } else {
    mouthAction = false;
    int holdSpeed = MOUTH_SPEED / HOLD_DIVISOR;
    motorControl(MOUTH_MOTOR, holdSpeed, mouthOpen);
    mouthHolding = true;
    mouthHoldTime = curTime;
    Serial.println(mouthOpen ? "Mouth open: holding" : "Mouth closed");
  }
}

// Hold motor position
void holdMotor(uint8_t motor) {
  int speed = (motor == BODY_MOTOR ? BODY_SPEED : MOUTH_SPEED) / HOLD_DIVISOR;
  boolean dir = (motor == BODY_MOTOR) ? bodyForward : mouthOpen;
  
  Serial.println(motor == BODY_MOTOR ? "Body: holding" : "Mouth: holding");
  
  motorControl(motor, speed, dir);
  
  if (motor == BODY_MOTOR) {
    bodyHolding = true;
    bodyHoldTime = curTime;
  } else {
    mouthHolding = true;
    mouthHoldTime = curTime;
  }
}

// ======= YAP MODE FUNCTIONS =======
void startYapMode(int threshold) {
  silenceThreshold = threshold;
  Serial.print("Yap mode: threshold ");
  Serial.println(silenceThreshold);
  
  // Calibrate audio
  calibrateAudio();
  
  // Signal calibration complete
  timedAction(BODY_MOTOR, true, BODY_FORWARD_TIME / 2);
  delay(600);
  
  yapMode = true;
}

void calibrateAudio() {
  Serial.println("Calibrating audio (3s)...");
  
  long sum = 0;
  int max = 0;
  
  // Sample audio for calibration
  for (int i = 0; i < 100; i++) {
    int level = analogRead(SOUND_PIN);
    sum += level;
    if (level > max) max = level;
    delay(30);
  }
  
  int avg = sum / 100;
  silenceThreshold = avg + 12;
  
  Serial.print("Ambient: ");
  Serial.print(avg);
  Serial.print(" | Max: ");
  Serial.print(max);
  Serial.print(" | Threshold: ");
  Serial.println(silenceThreshold);
}

void processYap() {
  int audioLevel = analogRead(SOUND_PIN);
  
  // Check for serial commands
  if (Serial.available()) return;
  
  // Process audio-driven movements
  processMouth(audioLevel);
  processHead(audioLevel);
}

void processMouth(int audioLevel) {
  static boolean isOpen = false;
  static unsigned long lastAction = 0;
  static int lastLevel = 0;
  
  // Debounce mouth movements
  if (curTime - lastAction > MOUTH_DEBOUNCE_TIME) {
    // Open mouth on sound
    if (audioLevel > silenceThreshold && !isOpen) {
      mouthMotor.setSpeed(MOUTH_SPEED);
      mouthMotor.forward();
      isOpen = true;
      lastAction = curTime;
      
      // Minimal output
      Serial.print("A:");
      Serial.println(audioLevel);
    }
    // Close mouth when sound drops
    else if ((audioLevel < silenceThreshold || audioLevel < lastLevel * AUDIO_DROP_FACTOR) && isOpen) {
      mouthMotor.setSpeed(0);
      mouthMotor.release();
      isOpen = false;
      lastAction = curTime;
    }
    
    lastLevel = audioLevel;
  }
}

void processHead(int audioLevel) {
  // All static variables to maintain state
  static unsigned long moveEndTime = 0;
  static boolean moving = false;
  static int position = 0;  // 0=down, 1=partial, 2=full
  static unsigned long audioStart = 0, lastAudio = 0, silenceStart = 0;
  static boolean quiet = true;
  static int avgLevel = 0;
  static int peak = 0;
  static int energy = 0;
  
  // Update audio stats with smoothing
  avgLevel = (avgLevel * 0.8) + (audioLevel * 0.2);
  
  // Track peak with decay
  if (audioLevel > peak) {
    peak = audioLevel;
  } else if (peak > 0) {
    peak *= 0.99;
  }
  
  // Audio energy tracking
  if (audioLevel > silenceThreshold) {
    energy += audioLevel - silenceThreshold;
    lastAudio = curTime;
    
    // Start of sound detected
    if (quiet) {
      audioStart = curTime;
      quiet = false;
      
      // Move to singing position
      if (!moving && position != 0) {
        position = 0;
        bodyMotor.setSpeed(BODY_SPEED);
        bodyMotor.backward();
        moveEndTime = curTime + BODY_BACKWARD_TIME / 2;
        moving = true;
      }
    }
  } else if (curTime - lastAudio > AUDIO_QUIET_THRESHOLD) {
    energy = 0;
    
    // Start of silence detected
    if (!quiet) {
      silenceStart = curTime;
      quiet = true;
      
      // Return to neutral after silence
      if (curTime - silenceStart > SILENCE_TIMEOUT && !moving && position != 1) {
        position = 1;
        bodyMotor.setSpeed(BODY_SPEED);
        bodyMotor.forward();
        moveEndTime = curTime + BODY_FORWARD_TIME / 3;
        moving = true;
      }
    }
  }
  
  // Handle movement state machine
  if (moving) {
    // End movement
    if (curTime >= moveEndTime) {
      bodyMotor.setSpeed(0);
      bodyMotor.release();
      moving = false;
    }
  } 
  // Consider new movement when idle during audio
  else if (!moving && !quiet) {
    boolean shouldMove = false;
    int target = position;
    
    // Important audio moment
    if (audioLevel > peak * 0.9 && audioLevel > silenceThreshold * 2) {
      shouldMove = true;
      target = 2;  // Full forward
    } 
    // Random variation during regular singing
    else if (energy > 10000 && random(5) == 0) {
      shouldMove = true;
      target = (position == 0) ? 1 : 0;
      energy = 0;
    }
    
    // Execute movement if needed
    if (shouldMove && target != position) {
      position = target;
      bodyMotor.setSpeed(BODY_SPEED);
      
      if (position == 0) {
        bodyMotor.backward();
        moveEndTime = curTime + BODY_BACKWARD_TIME / 2;
      } else if (position == 1) {
        bodyMotor.forward();
        moveEndTime = curTime + BODY_FORWARD_TIME / 3;
      } else {
        bodyMotor.forward();
        moveEndTime = curTime + BODY_FORWARD_TIME / 2;
      }
      
      moving = true;
    }
  }
}

// ======= UTILITY FUNCTIONS =======
// Parse integer from command
int parseInt(const char* prefix) {
  int val = 0;
  char fmt[32];
  snprintf(fmt, sizeof(fmt), "%s %%d", prefix);
  sscanf(cmdBuf, fmt, &val);
  return val;
}

// String comparison helpers
bool strEq(const char* s1, const char* s2) {
  return strcmp(s1, s2) == 0;
}

bool strStart(const char* str, const char* prefix) {
  return strncmp(str, prefix, strlen(prefix)) == 0;
}

// Print string from program memory
void printPgm(const char* str) {
  char c;
  while ((c = pgm_read_byte(str++))) Serial.print(c);
  Serial.println();
}