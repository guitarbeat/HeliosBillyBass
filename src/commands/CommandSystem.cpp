#include "CommandSystem.h"
#include "../core/BillyBass.h"
#include <Arduino.h>

extern BillyBass billy;
extern bool audioReactivityEnabled;
extern bool manualMode;
extern bool debugMode;

// Print command menu
void printMenu() {
    Serial.println(F("\n=== BILLY BASS COMMANDS ==="));
    
    // Display commands in a more compact format
    for (int i = 0; i < NUM_COMMANDS; i++) {
        if (i > 0 && i % 3 == 0) {
            Serial.println();
        }
        Serial.print(COMMANDS[i].command);
        Serial.print(F(": "));
        Serial.print(COMMANDS[i].description);
        Serial.print(F("  "));
    }
    Serial.println();
    
    // Print current settings in a compact format
    Serial.println(F("\n=== SETTINGS ==="));
    Serial.print(F("Speed: "));
    Serial.print(billy.getMotorSpeed());
    Serial.print(F(" | Mode: "));
    Serial.print(manualMode ? F("Manual") : F("Auto"));
    Serial.print(F(" | Audio: "));
    Serial.print(audioReactivityEnabled ? F("ON") : F("OFF"));
    Serial.print(F(" | Debug: "));
    Serial.println(debugMode ? F("ON") : F("OFF"));
    
#if DEBUG
    if (debugMode) {
        Serial.println(F("\n=== DEBUG INFO ==="));
        Serial.print(F("Sound: "));
        Serial.print(soundVolume);
        Serial.print(F(" | State: "));
        Serial.print(fishState);
        Serial.print(F(" | Mouth: "));
        Serial.print(billy.isMouthOpen() ? F("Open") : F("Closed"));
        Serial.print(F(" | Body: "));
        Serial.println(billy.isBodyMoved() ? F("Moved") : F("Home"));
    }
#endif
}

// Command action implementations
void cmdOpenMouth() { billy.openMouth(); }
void cmdCloseMouth() { billy.closeMouth(); }
void cmdFlapTail() { billy.flapTail(); }
void cmdBodyForward() { billy.bodyForward(); }
void cmdReset() { billy.resetMotorsToHome(); }
void cmdSing() { billy.singingMotion(); }
void cmdIncreaseSpeed() { billy.setMotorSpeed(min(255, billy.getMotorSpeed() + 5)); }
void cmdDecreaseSpeed() { billy.setMotorSpeed(max(0, billy.getMotorSpeed() - 5)); }
void cmdToggleAudio() { audioReactivityEnabled = !audioReactivityEnabled; Serial.println(audioReactivityEnabled ? F("Audio reactivity ON") : F("Audio reactivity OFF")); }
void cmdToggleManual() { manualMode = !manualMode; Serial.println(manualMode ? F("Manual mode") : F("Auto mode")); }
void cmdToggleDebug() { debugMode = !debugMode; Serial.println(debugMode ? F("Debug ON") : F("Debug OFF")); }
void cmdHelp() { printMenu(); }

// Command mapping table
const CommandInfo COMMANDS[] = {
    {'o', cmdOpenMouth, "Open mouth"},
    {'c', cmdCloseMouth, "Close mouth"},
    {'f', cmdFlapTail, "Flap tail"},
    {'b', cmdBodyForward, "Body forward"},
    {'r', cmdReset, "Reset to home"},
    {'s', cmdSing, "Singing motion"},
    {'+', cmdIncreaseSpeed, "Increase speed"},
    {'-', cmdDecreaseSpeed, "Decrease speed"},
    {'a', cmdToggleAudio, "Toggle audio"},
    {'m', cmdToggleManual, "Toggle manual/auto"},
    {'d', cmdToggleDebug, "Toggle debug"},
    {'h', cmdHelp, "Show menu"},
    {'?', cmdHelp, "Show menu"}
};

const int NUM_COMMANDS = sizeof(COMMANDS) / sizeof(CommandInfo);

// Process a command character
bool processCommand(char cmd) {
    for (int i = 0; i < NUM_COMMANDS; i++) {
        if (COMMANDS[i].command == cmd) {
            COMMANDS[i].action();
            return true;
        }
    }
    return false;
} 