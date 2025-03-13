/*
 * Alexa-Enabled Big Mouth Billy Bass
 * =================================
 * Controls a Big Mouth Billy Bass to respond to audio input.
 * - High frequencies (vocals) → mouth movements
 * - Low frequencies (bass) → body movements
 * - States: WAITING → TALKING → FLAPPING
 * 
 * Hardware: D3/D5 (mouth motor), D6/D9 (body motor), A0 (audio input)
 * Credits: Donald Bell, jswett77, Jordan Bunker
 * License: MIT
 */

#include <MX1508.h> 
#include "arduinoFFT.h"

// ==================== CONFIGURATION ====================
// Fish states (don't change these)
#define STATE_WAITING  0
#define STATE_TALKING  1
#define STATE_FLAPPING 2

// Hardware Configuration
struct {
  // Pin Configuration
  const uint8_t SOUND_PIN = A0;         // Analog input for audio
  const uint8_t BODY_MOTOR_PINS[2] = {6, 9};  // Body/tail motor control pins
  const uint8_t MOUTH_MOTOR_PINS[2] = {5, 3}; // Mouth motor control pins
  
  // Motor Speed Settings
  const uint8_t MOUTH_OPEN_SPEED = 255;  // Max speed for opening mouth
  const uint8_t MOUTH_CLOSE_SPEED = 200; // Speed for closing mouth
  const uint8_t TAIL_FLAP_SPEED = 180;   // Speed for tail flapping (backward() moves the tail)
  const uint8_t BODY_FORWARD_SPEED = 255; // Speed for body forward movement
} hw;

// Audio Processing Configuration
struct {
  // FFT Settings
  const uint16_t SAMPLES = 64;             // FFT sample size (power of 2)
  const uint16_t SAMPLING_FREQUENCY = 1000; // Sampling frequency in Hz
  
  // Frequency Thresholds
  const uint8_t SILENCE_THRESHOLD = 10;    // Threshold for detecting silence
  const uint8_t HIGH_FREQ_THRESHOLD = 5;   // Threshold for high frequencies (vocals)
  const uint8_t LOW_FREQ_THRESHOLD = 8;    // Threshold for low frequencies (bass)
  const uint8_t BEAT_THRESHOLD = 8;        // Threshold for beat detection
  const uint8_t MAX_SOUND_VOLUME = 100;    // Maximum expected sound volume
  
  // Frequency Bands
  const uint8_t LOW_FREQ_CUTOFF = 5;       // Upper limit for low frequency band
  const uint8_t HIGH_FREQ_START = 6;       // Lower limit for high frequency band
} audio;

// Behavior Configuration
struct {
  // Timing (milliseconds)
  const uint16_t BEAT_COOLDOWN_MS = 150;   // Minimum time between beat responses
  const uint16_t IDLE_TIMEOUT_MS = 1000;   // Time before idle animation triggers
  const uint16_t FLAP_DURATION_MS = 500;   // Duration of flapping animation
  const uint16_t QUICK_FLAP_DURATION_MS = 200; // Duration for a quick flap
  const uint16_t BEAT_FLAP_DURATION_MS = 50;   // Duration for beat-triggered flap
  
  // Random Behavior
  const uint8_t RANDOM_FLAP_THRESHOLD = 10;       // % chance of random flap when quiet
  const uint8_t CONSECUTIVE_QUIET_THRESHOLD = 5;  // Frames of quiet before random actions
  const uint8_t HEAVY_BASS_FLAP_CHANCE = 40;      // % chance of tail flap for heavy bass
  
  // Idle Behavior (seconds)
  const uint8_t MIN_IDLE_TIME_S = 5;       // Min time between idle animations
  const uint8_t MAX_IDLE_TIME_S = 15;      // Max time between idle animations
  
  // Movement Thresholds
  const uint16_t MIN_RANDOM_INTERVAL_MS = 2000;   // Min time between random movements
  const uint8_t SOFT_BASS_THRESHOLD = 5;          // Threshold for soft bass detection
  const uint8_t MEDIUM_BASS_THRESHOLD = 15;       // Threshold for medium bass detection
} behavior;

// ==================== GLOBAL VARIABLES ====================
// FFT
double vReal[64], vImag[64]; // Using the constant value directly for array size
ArduinoFFT<double> FFT = ArduinoFFT<double>(vReal, vImag, 64, 1000);

// Audio Analysis
int highFreqMagnitude = 0, lowFreqMagnitude = 0, lastLowMag = 0, soundVolume = 0;
bool beatDetected = false;
int consecutiveQuietFrames = 0;

// Motors & State
MX1508 bodyMotor(hw.BODY_MOTOR_PINS[0], hw.BODY_MOTOR_PINS[1]);  // Controls body/tail movement
MX1508 mouthMotor(hw.MOUTH_MOTOR_PINS[0], hw.MOUTH_MOTOR_PINS[1]); // Controls mouth movement
int bodySpeed = 0, fishState = STATE_WAITING;
bool talking = false, dancing = false;

// Timing
unsigned long currentTime = 0, mouthActionTime = 0, bodyActionTime = 0;
unsigned long lastActionTime = 0, lastFFTTime = 0, lastBeatTime = 0;

// ==================== SETUP & MAIN LOOP ====================
void setup() {
  bodyMotor.setSpeed(0); 
  mouthMotor.setSpeed(0);
  pinMode(hw.SOUND_PIN, INPUT);
  Serial.begin(9600);
  Serial.println("BTBillyBass initialized. Waiting for audio input...");
}

void loop() {
  currentTime = millis();
  if (currentTime - lastFFTTime > 100) {
    performFFTAnalysis();
    lastFFTTime = currentTime;
  }
  SMBillyBass();
}

// ==================== AUDIO ANALYSIS ====================
void performFFTAnalysis() {
  // Sample audio input
  for (int i = 0; i < audio.SAMPLES; i++) {
    vReal[i] = analogRead(hw.SOUND_PIN);
    vImag[i] = 0;
    delayMicroseconds(1000000/audio.SAMPLING_FREQUENCY);
  }

  // Process FFT
  FFT.windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.compute(FFT_FORWARD);
  FFT.complexToMagnitude();

  // Calculate frequency bands
  lowFreqMagnitude = highFreqMagnitude = 0;
  int lowBandCount = 0, highBandCount = 0;
  
  for (int i = 1; i < audio.LOW_FREQ_CUTOFF; i++) {
    lowFreqMagnitude += vReal[i];
    lowBandCount++;
  }
  lowFreqMagnitude = lowBandCount > 0 ? lowFreqMagnitude / lowBandCount : 0;
  
  for (int i = audio.HIGH_FREQ_START; i < audio.SAMPLES/2; i++) {
    highFreqMagnitude += vReal[i];
    highBandCount++;
  }
  highFreqMagnitude = highBandCount > 0 ? highFreqMagnitude / highBandCount : 0;

  // Beat detection
  static int lastHighMag = 0;
  beatDetected = (lowFreqMagnitude > audio.LOW_FREQ_THRESHOLD && 
                 (lowFreqMagnitude > (lastLowMag * 1.3) || lowFreqMagnitude > audio.BEAT_THRESHOLD * 2) &&
                 currentTime - lastBeatTime > behavior.BEAT_COOLDOWN_MS);
  
  if (beatDetected) {
    lastBeatTime = currentTime;
    Serial.println("BEAT DETECTED!");
  }

  // Track quiet periods & apply smoothing
  consecutiveQuietFrames = (lowFreqMagnitude < audio.LOW_FREQ_THRESHOLD && highFreqMagnitude < audio.HIGH_FREQ_THRESHOLD) ? 
                           consecutiveQuietFrames + 1 : 0;
  
  lowFreqMagnitude = (lowFreqMagnitude * 0.7) + (lastLowMag * 0.3);
  highFreqMagnitude = (highFreqMagnitude * 0.7) + (lastHighMag * 0.3);
  lastLowMag = lowFreqMagnitude;
  lastHighMag = highFreqMagnitude;
  
  soundVolume = max(lowFreqMagnitude, highFreqMagnitude);
  
  // Debug output
  Serial.print("Low: ");
  Serial.print(lowFreqMagnitude);
  Serial.print(" | High: ");
  Serial.println(highFreqMagnitude);
}

// ==================== STATE MACHINE ====================
void SMBillyBass() {
  // Handle beat detection and random flapping
  if (beatDetected) respondToBeat();
  if (shouldPerformRandomFlap()) performRandomFlap();

  // State machine
  switch (fishState) {
    case STATE_WAITING: handleWaitingState(); break;
    case STATE_TALKING: handleTalkingState(); break;
    case STATE_FLAPPING: flapTail(); fishState = STATE_WAITING; break;
  }
}

void respondToBeat() {
  // Quick tail flap in response to beat detection
  bodyMotor.setSpeed(hw.TAIL_FLAP_SPEED);
  moveTail();  // Flap the tail (backward movement)
  delay(behavior.BEAT_FLAP_DURATION_MS);
  returnTail(); // Return tail to normal position
  lastActionTime = currentTime;
}

bool shouldPerformRandomFlap() {
  return consecutiveQuietFrames > behavior.CONSECUTIVE_QUIET_THRESHOLD && 
         random(100) < behavior.RANDOM_FLAP_THRESHOLD && 
         currentTime - lastActionTime > behavior.MIN_RANDOM_INTERVAL_MS;
}

void performRandomFlap() {
  bodyMotor.setSpeed(hw.TAIL_FLAP_SPEED);
  moveTail();  // Flap the tail (backward movement)
  delay(behavior.QUICK_FLAP_DURATION_MS);
  returnTail(); // Return tail to normal position
  lastActionTime = currentTime;
  consecutiveQuietFrames = 0;
  Serial.println("RANDOM FLAP!");
}

void handleWaitingState() {
  // Check for high frequencies (vocals)
  if (highFreqMagnitude > audio.HIGH_FREQ_THRESHOLD && currentTime > mouthActionTime) {
    talking = true;
    mouthActionTime = currentTime + map(constrain(highFreqMagnitude, audio.HIGH_FREQ_THRESHOLD, audio.MAX_SOUND_VOLUME), 
                                      audio.HIGH_FREQ_THRESHOLD, audio.MAX_SOUND_VOLUME, 30, 150);
    fishState = STATE_TALKING;
  }
  
  // Check for low frequencies (bass)
  if (lowFreqMagnitude > audio.LOW_FREQ_THRESHOLD && currentTime > bodyActionTime) {
    dancing = true;
    bodyActionTime = currentTime + map(constrain(lowFreqMagnitude, audio.LOW_FREQ_THRESHOLD, audio.MAX_SOUND_VOLUME), 
                                     audio.LOW_FREQ_THRESHOLD, audio.MAX_SOUND_VOLUME, 50, 300);
    fishState = STATE_TALKING;
  }
  else if (currentTime > mouthActionTime + 50 && currentTime > bodyActionTime + 50) {
    bodyMotor.halt();
    mouthMotor.halt();
  }
  
  // Check for idle timeout
  if (currentTime - lastActionTime > behavior.IDLE_TIMEOUT_MS) {
    lastActionTime = currentTime + random(behavior.MIN_IDLE_TIME_S, behavior.MAX_IDLE_TIME_S) * 1000L;
    fishState = STATE_FLAPPING;
  }
}

void handleTalkingState() {
  // Handle mouth movement
  if (currentTime < mouthActionTime && talking) {
    openMouth();
    lastActionTime = currentTime;
  } else {
    closeMouth();
    talking = false;
  }
  
  // Handle body movement
  if (currentTime < bodyActionTime && dancing) {
    articulateBody(true);
    lastActionTime = currentTime;
  } else {
    articulateBody(false);
    dancing = false;
  }
  
  // Return to waiting state if both actions are complete
  if (!talking && !dancing) fishState = STATE_WAITING;
}

// ==================== MOTOR CONTROL ====================
// Mouth Control Functions
void openMouth() {
  mouthMotor.halt();
  int scaledSpeed = map(constrain(highFreqMagnitude, audio.HIGH_FREQ_THRESHOLD, audio.MAX_SOUND_VOLUME), 
                        audio.HIGH_FREQ_THRESHOLD, audio.MAX_SOUND_VOLUME, 150, hw.MOUTH_OPEN_SPEED);
  mouthMotor.setSpeed(scaledSpeed);
  mouthMotor.forward();
  
  Serial.print("High: ");
  Serial.print(highFreqMagnitude);
  Serial.print(" | Mouth: ");
  Serial.println(scaledSpeed);
}

void closeMouth() {
  mouthMotor.halt();
  mouthMotor.setSpeed(hw.MOUTH_CLOSE_SPEED);
  mouthMotor.backward();
}

// Body/Tail Control Functions
void moveTail() {
  // backward() activates the tail flapping motion
  bodyMotor.backward();
}

void returnTail() {
  // forward() returns the tail to normal position
  bodyMotor.forward();
}

void articulateBody(bool active) {
  if (active) {
    bodySpeed = map(constrain(lowFreqMagnitude, audio.LOW_FREQ_THRESHOLD, audio.MAX_SOUND_VOLUME), 
                   audio.LOW_FREQ_THRESHOLD, audio.MAX_SOUND_VOLUME, 120, hw.BODY_FORWARD_SPEED);
    
    if (lowFreqMagnitude < audio.LOW_FREQ_THRESHOLD + behavior.SOFT_BASS_THRESHOLD) {
      // Gentle movement for soft bass
      bodyMotor.setSpeed(bodySpeed);
      returnTail(); // Use forward() for gentle movement
    } else if (lowFreqMagnitude < audio.LOW_FREQ_THRESHOLD + behavior.MEDIUM_BASS_THRESHOLD) {
      // Medium movement for moderate bass
      bodyMotor.setSpeed(bodySpeed);
      returnTail(); // Use forward() for medium movement
    } else if (random(100) < behavior.HEAVY_BASS_FLAP_CHANCE) {
      // Chance of tail flap for heavy bass
      bodyMotor.setSpeed(hw.TAIL_FLAP_SPEED);
      moveTail(); // Use backward() for tail flap
      Serial.println("BASS-TRIGGERED FLAP!");
    } else {
      bodyMotor.setSpeed(bodySpeed);
      returnTail(); // Use forward() for normal movement
    }
    
    Serial.print("Low: ");
    Serial.print(lowFreqMagnitude);
    Serial.print(" | Body: ");
    Serial.println(bodySpeed);
  } else if (currentTime > bodyActionTime) {
    bodyMotor.halt();
  }
}

void flapTail() {
  bodyMotor.setSpeed(hw.TAIL_FLAP_SPEED);
  moveTail(); // Use backward() for tail flap
  delay(behavior.FLAP_DURATION_MS);
  bodyMotor.halt();
}