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
 * FREQUENCY ANALYSIS:
 * - High frequencies (vocals) control mouth movements
 * - Low frequencies (bass) control body movements
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
#include "arduinoFFT.h"

// FFT configuration
#define SAMPLES 64             // Must be a power of 2
#define SAMPLING_FREQUENCY 1000 // Hz, must be less than 10000 due to ADC

// FFT variables
double vReal[SAMPLES];
double vImag[SAMPLES];

// Create FFT object with the correct template syntax for newer library versions
ArduinoFFT<double> FFT = ArduinoFFT<double>(vReal, vImag, SAMPLES, SAMPLING_FREQUENCY);

int highFreqMagnitude = 0;     // Magnitude of high frequencies (vocals)
int lowFreqMagnitude = 0;      // Magnitude of low frequencies (bass)
int highFreqThreshold = 5;     // Threshold for high frequency detection (lowered for more sensitivity)
int lowFreqThreshold = 8;      // Threshold for low frequency detection (lowered for more sensitivity)

// Beat detection variables
int lastLowMag = 0;            // Previous low frequency magnitude
int beatThreshold = 8;         // Threshold for beat detection (LOWERED from 15 to 8)
bool beatDetected = false;     // Flag for beat detection
long lastBeatTime = 0;         // Time of last detected beat
int consecutiveQuietFrames = 0; // Count of consecutive frames with low audio

// Frequency band definitions
int lowFreqCutoff = 5;         // Bin number for low frequency cutoff (lower for more bass sensitivity)
int highFreqStart = 6;         // Bin number where high frequencies start (just after low frequencies)

// Motor control setup
MX1508 bodyMotor(6, 9); // Sets up an MX1508 controlled motor on PWM pins 6 and 9
MX1508 mouthMotor(5, 3); // Sets up an MX1508 controlled motor on PWM pins 5 and 3

// Audio input configuration
int soundPin = A0;       // Sound input pin
int silence = 10;        // Threshold for "silence". Lowered to make it more sensitive
int maxSoundVolume = 100; // Maximum expected sound volume for scaling

// Motor control variables
int bodySpeed = 0;       // Body motor speed initialized to 0
int soundVolume = 0;     // Variable to hold the analog audio value
int fishState = 0;       // Variable to indicate the state Billy is in (0=waiting, 1=talking, 2=flapping)
bool talking = false;    // Indicates whether the fish should be talking or not
bool dancing = false;    // Indicates whether the fish should be dancing to bass
int mouthOpenSpeed = 255; // Maximum mouth open speed (increased for more exaggerated movement)
int mouthCloseSpeed = 200; // Mouth close speed (increased for more responsive closing)

// Timing variables for animation scheduling
long currentTime;        // Current time in milliseconds
long mouthActionTime;    // Scheduled time for mouth action to end
long bodyActionTime;     // Scheduled time for body action to end
long lastActionTime;     // Time of the last action
long lastFFTTime;        // Time of the last FFT calculation

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
  Serial.println("FFT enabled - High frequencies control mouth, low frequencies control body");
}

/**
 * Main program loop
 */
void loop() {
  currentTime = millis();     // Update current time
  
  // Perform FFT analysis every 100ms
  if (currentTime - lastFFTTime > 100) {
    performFFTAnalysis();
    lastFFTTime = currentTime;
  }
  
  SMBillyBass();              // Run state machine to control fish behavior
}

/**
 * Performs FFT analysis on audio input to separate frequency bands
 */
void performFFTAnalysis() {
  // Sample the audio input
  for (int i = 0; i < SAMPLES; i++) {
    vReal[i] = analogRead(soundPin);
    vImag[i] = 0;
    delayMicroseconds(1000000/SAMPLING_FREQUENCY);
  }

  // Perform FFT
  FFT.windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.compute(FFT_FORWARD);
  FFT.complexToMagnitude();

  // Calculate average magnitude for low frequencies (bass)
  lowFreqMagnitude = 0;
  int lowBandCount = 0;
  for (int i = 1; i < lowFreqCutoff; i++) { // First few bins for bass frequencies
    lowFreqMagnitude += vReal[i];
    lowBandCount++;
  }
  lowFreqMagnitude = lowBandCount > 0 ? lowFreqMagnitude / lowBandCount : 0;

  // Calculate average magnitude for high frequencies (vocals and everything else)
  highFreqMagnitude = 0;
  int highBandCount = 0;
  for (int i = highFreqStart; i < SAMPLES/2; i++) { // All remaining bins up to Nyquist frequency
    highFreqMagnitude += vReal[i];
    highBandCount++;
  }
  highFreqMagnitude = highBandCount > 0 ? highFreqMagnitude / highBandCount : 0;

  // Beat detection - RELAXED conditions for more frequent detection
  beatDetected = false;
  if (lowFreqMagnitude > lowFreqThreshold && 
      (lowFreqMagnitude > (lastLowMag * 1.3) || lowFreqMagnitude > beatThreshold * 2) && // Reduced ratio from 1.5 to 1.3, added alternative condition
      currentTime - lastBeatTime > 150) { // Reduced from 200ms to 150ms
    beatDetected = true;
    lastBeatTime = currentTime;
    
    // Debug output for beat detection
    Serial.println("BEAT DETECTED!");
  }

  // Track quiet periods for potential random flapping
  if (lowFreqMagnitude < lowFreqThreshold && highFreqMagnitude < highFreqThreshold) {
    consecutiveQuietFrames++;
  } else {
    consecutiveQuietFrames = 0;
  }

  // Apply some smoothing to prevent jitter (optional)
  static int lastHighMag = 0;
  lowFreqMagnitude = (lowFreqMagnitude * 0.7) + (lastLowMag * 0.3);
  highFreqMagnitude = (highFreqMagnitude * 0.7) + (lastHighMag * 0.3);
  lastLowMag = lowFreqMagnitude;
  lastHighMag = highFreqMagnitude;

  // Update sound volume for backward compatibility
  soundVolume = max(lowFreqMagnitude, highFreqMagnitude);

  // Debug output
  Serial.print("Low Freq: ");
  Serial.print(lowFreqMagnitude);
  Serial.print(" | High Freq: ");
  Serial.println(highFreqMagnitude);
}

/**
 * State Machine for Billy Bass behavior control
 * Controls transitions between waiting, talking, and flapping states
 */
void SMBillyBass() {
  // Handle beat detection - immediate response to beats
  if (beatDetected) {
    // Dramatic response to beat - quick tail flap
    bodyMotor.setSpeed(255);
    bodyMotor.backward();
    delay(50); // Very short delay for quick response
    bodyMotor.forward();
    lastActionTime = currentTime;
  }

  // Random flapping during quiet periods (NEW)
  if (consecutiveQuietFrames > 5 && random(100) < 10 && currentTime - lastActionTime > 2000) {
    // Occasional random flap during quiet periods
    bodyMotor.setSpeed(180);
    bodyMotor.backward();
    delay(200);
    bodyMotor.forward();
    lastActionTime = currentTime;
    consecutiveQuietFrames = 0;
    Serial.println("RANDOM FLAP!");
  }

  switch (fishState) {
    case 0: // WAITING STATE - Monitoring audio input
      // Check for high frequencies (vocals) to trigger mouth movement
      if (highFreqMagnitude > highFreqThreshold) {
        if (currentTime > mouthActionTime) {
          talking = true;
          // Make mouth action time proportional to high frequency magnitude
          // More responsive - shorter duration for quicker reactions
          int mouthDuration = map(constrain(highFreqMagnitude, highFreqThreshold, 100), 
                                 highFreqThreshold, 100, 30, 150);
          mouthActionTime = currentTime + mouthDuration;
          fishState = 1; // Transition to talking state
        }
      } 
      // Check for low frequencies (bass) to trigger body movement
      if (lowFreqMagnitude > lowFreqThreshold) { // Changed from else if to if - allow simultaneous detection
        if (currentTime > bodyActionTime) {
          dancing = true;
          // Make body action time proportional to low frequency magnitude
          // More responsive - shorter duration for quicker reactions
          int bodyDuration = map(constrain(lowFreqMagnitude, lowFreqThreshold, 100), 
                               lowFreqThreshold, 100, 50, 300);
          bodyActionTime = currentTime + bodyDuration;
          fishState = 1; // Transition to talking/dancing state
        }
      }
      else if (currentTime > mouthActionTime + 50 && currentTime > bodyActionTime + 50) { // Reduced delay
        bodyMotor.halt();
        mouthMotor.halt();
      }
      
      // Check if fish has been idle too long - REDUCED idle time for more frequent flapping
      if (currentTime - lastActionTime > 1000) { // Reduced from 1500ms to 1000ms
        // Schedule next idle animation (5-15 seconds from now - reduced for more activity)
        lastActionTime = currentTime + floor(random(5, 15)) * 1000L; // Reduced from 10-20 to 5-15 seconds
        fishState = 2; // Transition to flapping state
      }
      break;

    case 1: // TALKING/DANCING STATE - Animating mouth and body based on frequency
      // Handle mouth movement for high frequencies
      if (currentTime < mouthActionTime && talking) {
        openMouth();
        lastActionTime = currentTime;
      } else {
        closeMouth();
        talking = false;
      }
      
      // Handle body movement for low frequencies
      if (currentTime < bodyActionTime && dancing) {
        articulateBody(true);
        lastActionTime = currentTime;
      } else {
        articulateBody(false);
        dancing = false;
      }
      
      // Return to waiting state if both actions are complete
      if (!talking && !dancing) {
        fishState = 0;
      }
      break;

    case 2: // FLAPPING STATE - Idle animation
      flap();
      fishState = 0; // Return to waiting state
      break;
  }
}

/**
 * Opens the fish's mouth by controlling the mouth motor
 * Now scales the opening speed based on high frequency magnitude for more expressiveness
 */
void openMouth() {
  mouthMotor.halt();        // Stop the mouth motor
  
  // Scale mouth opening speed based on high frequency magnitude
  // More dramatic - wider range of speeds
  int scaledSpeed = map(constrain(highFreqMagnitude, highFreqThreshold, 100), 
                        highFreqThreshold, 100, 
                        150, mouthOpenSpeed);
                        
  mouthMotor.setSpeed(scaledSpeed); // Set the mouth motor speed based on high frequency
  mouthMotor.forward();     // Open the mouth
  
  // Debug output
  Serial.print("High Freq: ");
  Serial.print(highFreqMagnitude);
  Serial.print(" | Mouth Speed: ");
  Serial.println(scaledSpeed);
}

/**
 * Closes the fish's mouth by controlling the mouth motor
 */
void closeMouth() {
  mouthMotor.halt();        // Stop the mouth motor
  mouthMotor.setSpeed(mouthCloseSpeed); // Increased speed for more responsive closing
  mouthMotor.backward();    // Close the mouth
}

/**
 * Controls the body movements of the fish
 * Now responds to low frequency magnitude for bass response
 * 
 * @param active Boolean indicating if the fish should be moving
 */
void articulateBody(bool active) {
  if (active) { // If fish should be moving
    // Scale body movement based on low frequency magnitude
    // More dramatic - wider range of speeds
    bodySpeed = map(constrain(lowFreqMagnitude, lowFreqThreshold, 100), 
                    lowFreqThreshold, 100, 
                    120, 255);
    
    // Choose movement pattern based on bass intensity
    // More reactive - lower thresholds
    if (lowFreqMagnitude < lowFreqThreshold + 5) {
      // Gentle movement for soft bass
      bodyMotor.setSpeed(bodySpeed);
      bodyMotor.forward();
    } else if (lowFreqMagnitude < lowFreqThreshold + 15) {
      // Medium movement for moderate bass
      bodyMotor.setSpeed(bodySpeed);
      bodyMotor.forward();
    } else {
      // Full tail flap for heavy bass drops - INCREASED probability of tail flap
      if (random(100) < 40) { // 40% chance of tail flap (increased from implicit 0%)
        bodyMotor.setSpeed(255);
        bodyMotor.backward();
        Serial.println("BASS-TRIGGERED FLAP!");
      } else {
        bodyMotor.setSpeed(bodySpeed);
        bodyMotor.forward();
      }
    }
    
    // Debug output
    Serial.print("Low Freq: ");
    Serial.print(lowFreqMagnitude);
    Serial.print(" | Body Speed: ");
    Serial.println(bodySpeed);
  } else {
    // If not active and beyond scheduled body action time
    if (currentTime > bodyActionTime) {
      bodyMotor.halt();
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

/**
 * Legacy function for backward compatibility
 * Now just a wrapper around the FFT analysis
 */
int updateSoundInput() {
  // FFT analysis is now done in performFFTAnalysis()
  return soundVolume;
}
