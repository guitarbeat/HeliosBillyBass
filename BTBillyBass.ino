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