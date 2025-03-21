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

// Motor speed settings
const int BODY_SPEED = 200;    // Full speed for body movements
const int MOUTH_SPEED = 200;   // Full speed for mouth movements
const int HOLD_DIVISOR = 2;    // Divides speed when holding position to reduce power

// ======= STATE MANAGEMENT =======
// Serial command processing
char inputBuffer[32];          // Buffer for incoming commands (32 byte limit)
int inputIndex = 0;            // Index for current position in buffer
boolean stringComplete = false; // Flag for complete command received

// Timing variables
unsigned long currentTime = 0;
unsigned long mouthActionEndTime = 0;
unsigned long bodyActionEndTime = 0;
boolean mouthTimedAction = false;
boolean bodyTimedAction = false;

// Motor direction tracking
boolean mouthOpenDirection = true;  // true = open, false = close
boolean bodyForwardDirection = true; // true = forward, false = backward

// ======= MESSAGE STRINGS (STORED IN FLASH) =======
// Command response messages
const char MSG_BODY_FWD[] PROGMEM = "Moving body motor forward...";
const char MSG_BODY_REV[] PROGMEM = "Moving body motor backward...";
const char MSG_BODY_REL[] PROGMEM = "Releasing body motor...";
const char MSG_MOUTH_OPN[] PROGMEM = "Opening mouth...";
const char MSG_MOUTH_CLS[] PROGMEM = "Closing mouth...";
const char MSG_MOUTH_REL[] PROGMEM = "Releasing mouth motor...";
const char MSG_MOUTH_HOLD[] PROGMEM = "Holding mouth position...";
const char MSG_BODY_HOLD[] PROGMEM = "Holding body position...";
const char MSG_STOP[] PROGMEM = "Stopping all motors...";
const char MSG_UNKNOWN[] PROGMEM = "Unknown command. Type 'help' for available commands.";
const char MSG_FWD_TIME[] PROGMEM = "Moving body motor forward for ";
const char MSG_REV_TIME[] PROGMEM = "Moving body motor backward for ";
const char MSG_OPEN_TIME[] PROGMEM = "Opening mouth for ";
const char MSG_CLOSE_TIME[] PROGMEM = "Closing mouth for ";
const char MSG_MS[] PROGMEM = " milliseconds...";
const char MSG_BODY_DONE[] PROGMEM = "Body action completed";
const char MSG_MOUTH_DONE[] PROGMEM = "Mouth action completed";
const char MSG_CMD_RCVD[] PROGMEM = "Command received: ";

// Help text
const char HELP_TITLE[] PROGMEM = "\n=== Big Mouth Billy Bass Manual Control ===";
const char HELP_BASIC[] PROGMEM = "Basic Commands:";
const char HELP_BF[] PROGMEM = "- 'body forward' - Moves body motor forward (raises head, lowers tail)";
const char HELP_BB[] PROGMEM = "- 'body backward' - Moves body motor backward (lowers head, raises tail)";
const char HELP_BR[] PROGMEM = "- 'body release' - Releases body motor (free movement)";
const char HELP_BH[] PROGMEM = "- 'body hold' - Holds body in current position";
const char HELP_MO[] PROGMEM = "- 'mouth open' - Opens the mouth";
const char HELP_MC[] PROGMEM = "- 'mouth close' - Closes the mouth";
const char HELP_MR[] PROGMEM = "- 'mouth release' - Releases mouth motor (free movement)";
const char HELP_MH[] PROGMEM = "- 'mouth hold' - Holds mouth in current position";
const char HELP_STOP[] PROGMEM = "- 'stop' - Stops all motors";
const char HELP_HELP[] PROGMEM = "- 'help' - Shows this help message";
const char HELP_TIMED[] PROGMEM = "Timed Commands (specify time in milliseconds):";
const char HELP_BF_T[] PROGMEM = "- 'body forward 1000' - Moves body forward for 1 second, then holds";
const char HELP_BB_T[] PROGMEM = "- 'body backward 2000' - Moves body backward for 2 seconds, then holds";
const char HELP_MO_T[] PROGMEM = "- 'mouth open 800' - Opens mouth for 0.8 seconds, then holds open";
const char HELP_MC_T[] PROGMEM = "- 'mouth close 500' - Closes mouth for 0.5 seconds, then holds closed";
const char HELP_AUTO[] PROGMEM = "Note: After timed actions complete, position is held with reduced power.";
const char HELP_END[] PROGMEM = "==========================================\n";

// ======= SYSTEM INITIALIZATION =======
void setup() {
  // Initialize motors in safe state
  bodyMotor.setSpeed(0);
  mouthMotor.setSpeed(0);
  bodyMotor.halt();
  mouthMotor.halt();
  
  // Setup serial communication
  Serial.begin(9600);
  
  // Display help information on startup
  printHelp();
}

// ======= MAIN PROGRAM LOOP =======
void loop() {
  currentTime = millis();
  checkTimedActions();
  
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
    mouthTimedAction = false;
    mouthMotor.setSpeed(MOUTH_SPEED / HOLD_DIVISOR);
    
    if (mouthOpenDirection) {
      mouthMotor.forward();
      printProgmemString(MSG_MOUTH_DONE);
      Serial.println("Holding mouth open. Use 'stop' or another command to release.");
    } else {
      mouthMotor.backward();
      printProgmemString(MSG_MOUTH_DONE);
      Serial.println("Holding mouth closed. Use 'stop' or another command to release.");
    }
  }
  
  // Handle body timed actions
  if (bodyTimedAction && currentTime >= bodyActionEndTime) {
    bodyTimedAction = false;
    bodyMotor.setSpeed(BODY_SPEED / HOLD_DIVISOR);
    
    if (bodyForwardDirection) {
      bodyMotor.forward();
      printProgmemString(MSG_BODY_DONE);
      Serial.println("Holding body forward position. Use 'stop' or another command to release.");
    } else {
      bodyMotor.backward();
      printProgmemString(MSG_BODY_DONE);
      Serial.println("Holding body backward position. Use 'stop' or another command to release.");
    }
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
void processCommand() {
  // Convert input to lowercase for case-insensitive commands
  for (int i = 0; inputBuffer[i]; i++) inputBuffer[i] = tolower(inputBuffer[i]);
  
  // Log received command
  Serial.print(FPSTR(MSG_CMD_RCVD));
  Serial.println(inputBuffer);
  
  // Extract command parts
  char command[20] = {0};
  int position = 0, strength = 0;
  sscanf(inputBuffer, "%s %d %d", command, &position, &strength);
  
  // Parse and execute commands
  if (strStartsWith(command, "body")) {
    if (strEqual(inputBuffer, "body forward")) bodyForward();
    else if (strStartsWith(inputBuffer, "body forward ")) {
      int duration;
      sscanf(inputBuffer, "body forward %d", &duration);
      bodyForwardTimed(duration);
    }
    else if (strEqual(inputBuffer, "body backward")) bodyBackward();
    else if (strStartsWith(inputBuffer, "body backward ")) {
      int duration;
      sscanf(inputBuffer, "body backward %d", &duration);
      bodyBackwardTimed(duration);
    }
    else if (strEqual(inputBuffer, "body release")) bodyRelease();
    else if (strEqual(inputBuffer, "body hold")) bodyHold();
  }
  else if (strStartsWith(command, "mouth")) {
    if (strEqual(inputBuffer, "mouth open")) openMouth();
    else if (strStartsWith(inputBuffer, "mouth open ")) {
      int duration;
      sscanf(inputBuffer, "mouth open %d", &duration);
      openMouthTimed(duration);
    }
    else if (strEqual(inputBuffer, "mouth close")) closeMouth();
    else if (strStartsWith(inputBuffer, "mouth close ")) {
      int duration;
      sscanf(inputBuffer, "mouth close %d", &duration);
      closeMouthTimed(duration);
    }
    else if (strEqual(inputBuffer, "mouth release")) mouthRelease();
    else if (strEqual(inputBuffer, "mouth hold")) mouthHold();
  }
  else if (strEqual(inputBuffer, "stop")) stopAll();
  else if (strEqual(inputBuffer, "help")) printHelp();
  else Serial.println(FPSTR(MSG_UNKNOWN));
}

// ======= BODY MOTOR CONTROL FUNCTIONS =======
void bodyForward() {
  printProgmemString(MSG_BODY_FWD);
  bodyTimedAction = false;
  bodyMotor.setSpeed(BODY_SPEED);
  bodyMotor.forward();
  bodyForwardDirection = true;
}

void bodyBackward() {
  printProgmemString(MSG_BODY_REV);
  bodyTimedAction = false;
  bodyMotor.setSpeed(BODY_SPEED);
  bodyMotor.backward();
  bodyForwardDirection = false;
}

void bodyRelease() {
  printProgmemString(MSG_BODY_REL);
  bodyTimedAction = false;
  bodyMotor.setSpeed(0);
  bodyMotor.release();
}

void bodyHold() {
  printProgmemString(MSG_BODY_HOLD);
  bodyTimedAction = false;
  bodyMotor.setSpeed(BODY_SPEED / HOLD_DIVISOR);
  if (bodyForwardDirection) bodyMotor.forward();
  else bodyMotor.backward();
  Serial.println("Holding body position. Use 'stop' or another command to release.");
}

void bodyForwardTimed(int duration) {
  Serial.print(FPSTR(MSG_FWD_TIME));
  Serial.print(duration);
  Serial.println(FPSTR(MSG_MS));
  bodyMotor.setSpeed(BODY_SPEED);
  bodyMotor.forward();
  bodyForwardDirection = true;
  bodyTimedAction = true;
  bodyActionEndTime = currentTime + duration;
}

void bodyBackwardTimed(int duration) {
  Serial.print(FPSTR(MSG_REV_TIME));
  Serial.print(duration);
  Serial.println(FPSTR(MSG_MS));
  bodyMotor.setSpeed(BODY_SPEED);
  bodyMotor.backward();
  bodyForwardDirection = false;
  bodyTimedAction = true;
  bodyActionEndTime = currentTime + duration;
}

// ======= MOUTH MOTOR CONTROL FUNCTIONS =======
void openMouth() {
  printProgmemString(MSG_MOUTH_OPN);
  mouthTimedAction = false;
  mouthMotor.setSpeed(MOUTH_SPEED);
  mouthMotor.forward();
  mouthOpenDirection = true;
}

void closeMouth() {
  printProgmemString(MSG_MOUTH_CLS);
  mouthTimedAction = false;
  mouthMotor.setSpeed(MOUTH_SPEED);
  mouthMotor.backward();
  mouthOpenDirection = false;
}

void mouthRelease() {
  printProgmemString(MSG_MOUTH_REL);
  mouthTimedAction = false;
  mouthMotor.setSpeed(0);
  mouthMotor.release();
}

void mouthHold() {
  printProgmemString(MSG_MOUTH_HOLD);
  mouthTimedAction = false;
  mouthMotor.setSpeed(MOUTH_SPEED / HOLD_DIVISOR);
  if (mouthOpenDirection) {
    mouthMotor.forward();
    Serial.println("Holding mouth open. Use 'stop' or another command to release.");
  } else {
    mouthMotor.backward();
    Serial.println("Holding mouth closed. Use 'stop' or another command to release.");
  }
}

void openMouthTimed(int duration) {
  Serial.print(FPSTR(MSG_OPEN_TIME));
  Serial.print(duration);
  Serial.println(FPSTR(MSG_MS));
  mouthMotor.setSpeed(MOUTH_SPEED);
  mouthMotor.forward();
  mouthOpenDirection = true;
  mouthTimedAction = true;
  mouthActionEndTime = currentTime + duration;
}

void closeMouthTimed(int duration) {
  Serial.print(FPSTR(MSG_CLOSE_TIME));
  Serial.print(duration);
  Serial.println(FPSTR(MSG_MS));
  mouthMotor.setSpeed(MOUTH_SPEED);
  mouthMotor.backward();
  mouthOpenDirection = false;
  mouthTimedAction = true;
  mouthActionEndTime = currentTime + duration;
}

// ======= GLOBAL CONTROL FUNCTIONS =======
void stopAll() {
  printProgmemString(MSG_STOP);
  mouthTimedAction = bodyTimedAction = false;
  bodyMotor.halt();
  mouthMotor.halt();
}

// ======= HELP DISPLAY FUNCTION =======
void printHelp() {
  printProgmemString(HELP_TITLE);
  printProgmemString(HELP_BASIC);
  printProgmemString(HELP_BF);
  printProgmemString(HELP_BB);
  printProgmemString(HELP_BR);
  printProgmemString(HELP_BH);
  printProgmemString(HELP_MO);
  printProgmemString(HELP_MC);
  printProgmemString(HELP_MR);
  printProgmemString(HELP_MH);
  printProgmemString(HELP_STOP);
  printProgmemString(HELP_HELP);
  Serial.println("");
  printProgmemString(HELP_TIMED);
  printProgmemString(HELP_BF_T);
  printProgmemString(HELP_BB_T);
  printProgmemString(HELP_MO_T);
  printProgmemString(HELP_MC_T);
  printProgmemString(HELP_AUTO);
  printProgmemString(HELP_END);
}