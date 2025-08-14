# BTBillyBass API Documentation
[← Back to docs index](./index.md)
See also: [Command Reference](./COMMANDS.md).

## Table of Contents
1. [Hardware Configuration](#hardware-configuration)
2. [Constants and Parameters](#constants-and-parameters)
3. [Core Functions](#core-functions)
4. [Motor Control Functions](#motor-control-functions)
5. [Audio Processing Functions](#audio-processing-functions)
6. [Command Processing Functions](#command-processing-functions)
7. [Utility Functions](#utility-functions)
8. [State Management](#state-management)
9. [Data Structures](#data-structures)

## Hardware Configuration

### Pin Definitions
```cpp
// Motor control pins
MX1508 bodyMotor(6, 9);    // Body motor on pins 6, 9
MX1508 mouthMotor(5, 3);   // Mouth motor on pins 5, 3

// Audio input
const int SOUND_PIN = A0;   // Audio input on analog pin A0
```

### Motor Types
```cpp
#define BODY_MOTOR 0        // Body motor identifier
#define MOUTH_MOTOR 1       // Mouth motor identifier
```

## Constants and Parameters

### Motor Speed Settings
```cpp
const int BODY_SPEED = 200;     // Maximum speed for body movements (0-255)
const int MOUTH_SPEED = 250;    // Maximum speed for mouth movements (0-255)
const int HOLD_DIVISOR = 2;     // Speed reduction factor for holding positions
```

### Default Timing Values
```cpp
const int DEFAULT_MOUTH_OPEN_TIME = 120;      // Mouth open duration (ms)
const int DEFAULT_BODY_FORWARD_TIME = 800;    // Body forward duration (ms)
const int DEFAULT_BODY_BACKWARD_TIME = 1000;  // Body backward duration (ms)
const int DEFAULT_TAIL_ONLY_TIME = 400;       // Tail movement duration (ms)
const int HOLD_TIMEOUT = 500;                 // Hold timeout duration (ms)
```

### Audio Processing Parameters
```cpp
int silenceThreshold = 12;                    // Audio threshold for silence detection
const float AUDIO_DROP_FACTOR = 0.7;         // Factor for audio drop detection
const float LOUD_SOUND_FACTOR = 1.5;         // Factor for loud sound detection
const int MOUTH_DEBOUNCE_TIME = 110;         // Minimum time between mouth movements (ms)
const int SILENCE_TIMEOUT = 3000;            // Silence timeout before neutral position (ms)
const int AUDIO_QUIET_THRESHOLD = 1000;      // Time to consider audio quiet (ms)
```

### System Parameters
```cpp
const int INPUT_BUFFER_SIZE = 32;            // Serial input buffer size
const int SERIAL_BAUD_RATE = 9600;           // Serial communication baud rate
```

## Core Functions

### `setup()`
**Purpose**: System initialization and configuration

**Functionality**:
- Initializes motors in safe state
- Sets up serial communication
- Configures audio input pin
- Initializes random seed
- Displays help information
- Starts yap mode automatically after 5 seconds

**Parameters**: None

**Returns**: void

**Example**:
```cpp
void setup() {
  // Motor initialization
  bodyMotor.setSpeed(0);
  mouthMotor.setSpeed(0);
  bodyMotor.halt();
  mouthMotor.halt();
  
  // System setup
  randomSeed(analogRead(A1));
  Serial.begin(9600);
  pinMode(SOUND_PIN, INPUT);
  
  printHelp();
  delay(5000);
  startYapMode(silenceThreshold);
}
```

### `loop()`
**Purpose**: Main program loop

**Functionality**:
- Updates current time
- Processes yap mode if active
- Checks timed actions and hold timeouts
- Processes serial commands
- Reads incoming serial data

**Parameters**: None

**Returns**: void

**Example**:
```cpp
void loop() {
  currentTime = millis();
  
  if (yapMode) {
    processYap();
  } else {
    checkTimedActions();
    checkHoldTimeouts();
  }
  
  if (stringComplete) {
    processCommand();
    inputIndex = 0;
    inputBuffer[0] = '\0';
    stringComplete = false;
  }
  
  // Serial data reading
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
```

## Motor Control Functions

### `timedMotorAction(uint8_t motorType, boolean isForward, int duration)`
**Purpose**: Execute a timed motor movement

**Parameters**:
- `motorType`: BODY_MOTOR or MOUTH_MOTOR
- `isForward`: true for forward, false for backward
- `duration`: Movement duration in milliseconds

**Returns**: void

**Example**:
```cpp
// Move body forward for 1 second
timedMotorAction(BODY_MOTOR, true, 1000);

// Open mouth for 200ms
timedMotorAction(MOUTH_MOTOR, true, 200);
```

### `releaseMotor(uint8_t motorType)`
**Purpose**: Release a motor (stop and allow free movement)

**Parameters**:
- `motorType`: BODY_MOTOR or MOUTH_MOTOR

**Returns**: void

**Example**:
```cpp
releaseMotor(BODY_MOTOR);   // Release body motor
releaseMotor(MOUTH_MOTOR);  // Release mouth motor
```

### `holdMotor(uint8_t motorType, const char* holdMsg)`
**Purpose**: Hold a motor in its current position

**Parameters**:
- `motorType`: BODY_MOTOR or MOUTH_MOTOR
- `holdMsg`: Message to display when holding

**Returns**: void

**Example**:
```cpp
holdMotor(BODY_MOTOR, "Body: holding");
holdMotor(MOUTH_MOTOR, "Mouth: holding");
```

### `setMotorDirection(uint8_t motorType, boolean isForward)`
**Purpose**: Set motor direction without timing

**Parameters**:
- `motorType`: BODY_MOTOR or MOUTH_MOTOR
- `isForward`: true for forward, false for backward

**Returns**: void

**Example**:
```cpp
setMotorDirection(BODY_MOTOR, true);   // Body forward
setMotorDirection(MOUTH_MOTOR, false); // Mouth close
```

### `getMotorParams(uint8_t motorType, MX1508 **motor, int *speed, boolean **timedAction, boolean **direction, unsigned long *actionEndTime)`
**Purpose**: Get motor parameters based on motor type

**Parameters**:
- `motorType`: BODY_MOTOR or MOUTH_MOTOR
- `motor`: Pointer to motor object
- `speed`: Pointer to speed value
- `timedAction`: Pointer to timed action flag
- `direction`: Pointer to direction flag
- `actionEndTime`: Pointer to action end time

**Returns**: void

**Internal Use**: This is a helper function used by other motor control functions.

## Audio Processing Functions

### `startYapMode(int threshold)`
**Purpose**: Start audio-responsive mode

**Parameters**:
- `threshold`: Audio threshold value (0-1023)

**Returns**: void

**Functionality**:
- Sets silence threshold
- Calibrates audio levels
- Performs initial body movement
- Activates yap mode

**Example**:
```cpp
startYapMode(50);    // Start with threshold 50
startYapMode(100);   // Start with threshold 100
```

### `calibrateAudio()`
**Purpose**: Auto-calibrate audio levels

**Parameters**: None

**Returns**: void

**Functionality**:
- Samples ambient noise for 3 seconds
- Calculates average and maximum levels
- Sets threshold to average + 12 (minimum 30)
- Displays calibration results

**Example**:
```cpp
calibrateAudio();  // Automatic calibration
```

### `processYap()`
**Purpose**: Main audio processing loop

**Parameters**: None

**Returns**: void

**Functionality**:
- Reads current audio level
- Processes mouth movements
- Processes head movements
- Handles serial input interruption

**Example**:
```cpp
if (yapMode) {
  processYap();
}
```

### `processMouthMovements(int audioLevel)`
**Purpose**: Handle mouth movements based on audio

**Parameters**:
- `audioLevel`: Current audio level (0-1023)

**Returns**: void

**Functionality**:
- Opens mouth when audio exceeds threshold
- Closes mouth when audio drops
- Implements debouncing to prevent rapid movements
- Uses spring-assisted closing

**Example**:
```cpp
int audioLevel = analogRead(SOUND_PIN);
processMouthMovements(audioLevel);
```

### `processHeadMovements(int audioLevel)`
**Purpose**: Handle head movements based on audio patterns

**Parameters**:
- `audioLevel`: Current audio level (0-1023)

**Returns**: void

**Functionality**:
- Manages head position state machine
- Responds to audio energy accumulation
- Detects important audio moments
- Handles silence timeouts
- Implements non-blocking movements

**Head Positions**:
- 0: Down/back (singing position)
- 1: Partial forward (neutral)
- 2: Full forward (important moments)

**Example**:
```cpp
int audioLevel = analogRead(SOUND_PIN);
processHeadMovements(audioLevel);
```

## Command Processing Functions

### `processCommand()`
**Purpose**: Parse and execute serial commands

**Parameters**: None

**Returns**: void

**Functionality**:
- Converts input to lowercase
- Parses command and parameters
- Executes appropriate motor actions
- Handles system commands

**Supported Commands**:
```cpp
// Body commands
"body forward [duration]"
"body backward [duration]"
"body release"
"body hold"

// Mouth commands
"mouth open [duration]"
"mouth close"
"mouth release"
"mouth hold"

// System commands
"stop"
"yap [threshold]"
"help"
```

**Example**:
```cpp
if (stringComplete) {
  processCommand();
}
```

### `parseDuration(const char* cmdPrefix)`
**Purpose**: Extract duration parameter from command

**Parameters**:
- `cmdPrefix`: Command prefix to match

**Returns**: int (duration in milliseconds)

**Example**:
```cpp
int duration = parseDuration("body forward");  // Extracts duration from "body forward 1000"
```

## Utility Functions

### `strEqual(const char* str1, const char* str2)`
**Purpose**: Compare two strings for equality

**Parameters**:
- `str1`: First string
- `str2`: Second string

**Returns**: bool (true if equal)

**Example**:
```cpp
if (strEqual(command, "help")) {
  printHelp();
}
```

### `strStartsWith(const char* str, const char* prefix)`
**Purpose**: Check if string starts with prefix

**Parameters**:
- `str`: String to check
- `prefix`: Prefix to match

**Returns**: bool (true if starts with prefix)

**Example**:
```cpp
if (strStartsWith(inputBuffer, "body forward")) {
  // Handle body forward command
}
```

### `printProgmemString(const char* str)`
**Purpose**: Print string stored in program memory

**Parameters**:
- `str`: PROGMEM string pointer

**Returns**: void

**Example**:
```cpp
printProgmemString(HELP_TITLE);
```

### `printHelp()`
**Purpose**: Display command help information

**Parameters**: None

**Returns**: void

**Example**:
```cpp
printHelp();  // Display help in Serial Monitor
```

## State Management

### Timing Management Functions

#### `checkTimedActions()`
**Purpose**: Check if timed motor actions have completed

**Parameters**: None

**Returns**: void

**Functionality**:
- Checks mouth timed actions
- Checks body timed actions
- Transitions to hold state when complete

#### `checkHoldTimeouts()`
**Purpose**: Check if hold timeouts have expired

**Parameters**: None

**Returns**: void

**Functionality**:
- Checks mouth hold timeout
- Checks body hold timeout
- Releases motors when timeout reached

#### `transitionToHold(uint8_t motorType, MX1508 &motor, int speed, boolean direction, boolean *timedAction)`
**Purpose**: Transition motor from timed action to hold state

**Parameters**:
- `motorType`: BODY_MOTOR or MOUTH_MOTOR
- `motor`: Motor object reference
- `speed`: Motor speed
- `direction`: Motor direction
- `timedAction`: Pointer to timed action flag

**Returns**: void

### Global State Variables

```cpp
// Serial command processing
char inputBuffer[32];          // Input buffer
int inputIndex = 0;            // Buffer index
boolean stringComplete = false; // Command complete flag
boolean yapMode = false;       // Yap mode flag

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
boolean mouthOpenDirection = true;   // true = open, false = close
boolean bodyForwardDirection = true; // true = forward, false = backward
```

## Data Structures

### PROGMEM String Constants
```cpp
// Help text strings stored in program memory
const char HELP_TITLE[] PROGMEM = "\n== Billy Bass Controls ==";
const char HELP_BODY_CMDS[] PROGMEM = "Body: forward, backward, release, hold";
const char HELP_MOUTH_CMDS[] PROGMEM = "Mouth: open, close, release, hold";
const char HELP_TIME_INFO[] PROGMEM = "Add time (ms) to commands: 'body forward 1000'";
const char HELP_YAP_CMD[] PROGMEM = "Yap: activates audio-responsive mode ('stop' to exit)";
const char HELP_STOP_CMD[] PROGMEM = "Stop: halts all motors";
const char HELP_END[] PROGMEM = "========================";
```

### Audio Processing State
```cpp
// Static variables in processHeadMovements()
static unsigned long lastHeadMovement = 0;
static unsigned long headMovementEndTime = 0;
static boolean headMoving = false;
static int headPosition = 0;
static unsigned long audioStartTime = 0;
static unsigned long lastAudioTime = 0;
static unsigned long silenceStartTime = 0;
static boolean wasQuiet = true;
static int audioRunningAvg = 0;
static int peakAudio = 0;
static int energyAccumulator = 0;
```

## Error Handling

### Buffer Overflow Protection
```cpp
while (Serial.available() && inputIndex < 31) {
  // Prevents buffer overflow
}
```

### Motor Safety
```cpp
// Automatic timeout and release
if (mouthHolding && (currentTime - mouthHoldStartTime >= HOLD_TIMEOUT)) {
  releaseMotor(MOUTH_MOTOR);
  mouthHolding = false;
}
```

### Audio Calibration Safety
```cpp
// Minimum threshold to prevent over-sensitivity
int minimumThreshold = 30;
silenceThreshold = max(calculatedThreshold, minimumThreshold);
```

## Performance Considerations

### Memory Optimization
- All strings stored in PROGMEM
- Efficient buffer management
- Static variables for state tracking
- No dynamic memory allocation

### Timing Precision
- Non-blocking operations
- Millisecond precision timing
- Efficient state machine design
- Minimal processing overhead

### Audio Processing Efficiency
- Running average calculations
- Peak detection with decay
- Energy accumulation
- Debouncing for movement control