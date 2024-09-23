#include "Buttons.h"
#include "DiceRoller.h"
#include "LedControl.h"

#define ROLL_PIN PB4
#define MODE_PIN PB3
#define RND_PIN A1
#define ROLL_TIME 300
#define BLINK_INTERVAL 500

const uint8_t diceSidesCount[6] PROGMEM = {4, 6, 8, 10, 12, 20};

enum State { ROLLING, SELECTING };

LedControl* matrix;
Buttons* buttons;
DiceRoller* diceRoller;

State currentState = ROLLING;
uint8_t selectedDice = 1;
uint8_t multipleNumber = 1;

bool isRolling = false;
unsigned long rollStartTime = 0;
unsigned long lastBlinkTime = 0;
bool isBlinkingOn = true;

void rollDice(bool isDisplay = false) {
    matrix->clearDisplay();
    uint8_t maximum = pgm_read_byte(&diceSidesCount[selectedDice]);

    if (multipleNumber == 1 || selectedDice >= 3) {
        diceRoller->rollSingleDice(maximum, isDisplay);
    } else {
        diceRoller->rollMultipleDice(multipleNumber, maximum, isDisplay);
    }
}

void setup() {
    pinMode(RND_PIN, INPUT);
    randomSeed(analogRead(RND_PIN));

    delay(100);

    matrix = new LedControl();
    diceRoller = new DiceRoller(matrix);

    uint8_t intensity = 0;
    pinMode(ROLL_PIN, INPUT_PULLUP);
    pinMode(MODE_PIN, INPUT_PULLUP);
    bool rollPressed = !digitalRead(ROLL_PIN);
    bool modePressed = !digitalRead(MODE_PIN);

    if (rollPressed) intensity += 5;
    if (modePressed) intensity += 10;
    matrix->setIntensity(intensity);

    buttons = new Buttons(ROLL_PIN, MODE_PIN, rollPressed, modePressed);

    rollDice();
}

void loop() {
    buttons->update();

    switch (currentState) {
        case ROLLING:
            handleRolling(millis());

            if (buttons->wasRollReleased()) {
                startRoll();
            }
            if (buttons->wasModeReleased() && selectedDice <= 2) {
                handleModePress();
                rollDice(true);
            }
            if (buttons->wereBothPressed()) {
                currentState = SELECTING;
                lastBlinkTime = 0;
                isBlinkingOn = false;
                multipleNumber = 1;
            }
            break;

        case SELECTING:
            handleBlinking(millis());

            if (buttons->wereBothPressed()) {
                confirmSelection();
                rollDice(true);
                return;
            }

            if (buttons->wasModeReleased()) {
                selectNextDice();
                updateSelectedDice();
            } else if (buttons->wasRollReleased()) {
                selectPreviousDice();
                updateSelectedDice();
            }

            break;
    }
}

void startRoll() {
    matrix->clearDisplay();
    rollStartTime = millis();
    isRolling = true;
}

void handleRolling(unsigned long currentTime) {
    if (isRolling && currentTime - rollStartTime >= ROLL_TIME) {
        rollDice();
        isRolling = false;
    }
}

void handleBlinking(unsigned long currentTime) {
    if (currentTime - lastBlinkTime >= BLINK_INTERVAL) {
        lastBlinkTime = currentTime;
        isBlinkingOn = !isBlinkingOn;
        isBlinkingOn ? rollDice(true) : matrix->clearDisplay();
    }
}

void confirmSelection() {
    currentState = ROLLING;
    lastBlinkTime = 0;
    isBlinkingOn = false;
}

void updateSelectedDice() {
    lastBlinkTime = 0;
    isBlinkingOn = false;
}

void selectNextDice() {
    selectedDice = (selectedDice + 1) % 6;
    multipleNumber = 1;
}

void selectPreviousDice() {
    selectedDice = (selectedDice + 5) % 6;
    multipleNumber = 1;
}

void handleModePress() { multipleNumber = (multipleNumber % 4) + 1; }
