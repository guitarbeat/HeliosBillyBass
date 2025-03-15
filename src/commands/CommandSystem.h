#ifndef COMMANDSYSTEM_H
#define COMMANDSYSTEM_H

#include "../core/BillyBass.h"

// Command structure for mapping characters to functions
typedef struct {
    char command;
    void (*action)();
    const char* description;
} CommandInfo;

// Menu and command processing
void printMenu();
bool processCommand(char cmd);

// External declarations
extern const CommandInfo COMMANDS[];
extern const int NUM_COMMANDS;

// Command actions
void openMouthAction();
void closeMouthAction();
void flapTailAction();
void bodyForwardAction();
void singingAction();
void resetAction();
void stopMotorsAction();
void increaseSpeedAction();
void decreaseSpeedAction();
void toggleAudioAction();
void toggleModeAction();
void toggleDebugAction();
void helpAction();

#endif // COMMANDSYSTEM_H 