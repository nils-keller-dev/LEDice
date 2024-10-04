#include "DiceRoller.h"

DiceRoller::DiceRoller(LedControl* matrix) : matrix(matrix) {}

void DiceRoller::startRoll(uint8_t count, uint8_t maximum, bool isDisplay) {
    this->count = count;
    this->maximum = maximum;
    this->isDisplay = isDisplay;
    isRolling = true;
    rollStartTime = millis();
    currentAnimationStep = 0;

    if (!isDisplay) {
        for (uint8_t i = 0; i < count; i++) {
            currentAnimationSteps[i] = 0;
            delayOffsets[i] = random(0, 255);
        }

        if (count == 1) {
            delayOffsets[0] = 0;
        } else {
            rollMultipleDots(lastNumbers);
        }
    }
}

void DiceRoller::updateRoll(unsigned long currentTime) {
    if (!isRolling) return;

    if (isDisplay) {
        if (count == 1) {
            maximum < 10 ? rollDot(maximum) : rollDecimal(maximum);
        } else {
            for (uint8_t i = 0; i < count; i++) {
                lastNumbers[i] = maximum - 1;
            }
            rollMultipleDots(lastNumbers);
        }
        isRolling = false;
        return;
    }

    bool allFinished = true;

    for (uint8_t i = 0; i < count; i++) {
        if (currentAnimationSteps[i] < 7) {
            allFinished = false;

            if (currentTime - rollStartTime >
                delayOffsets[i] + calculateDelay(currentAnimationSteps[i])) {
                currentAnimationSteps[i]++;
                if (count == 1) {
                    rollSingleDice();
                } else {
                    uint8_t number;
                    do {
                        number = random(maximum);
                    } while (number == lastNumbers[i]);
                    lastNumbers[i] = number;

                    rollMultipleDots(lastNumbers);
                }
            }
        }
    }

    isRolling = !allFinished;
}

uint16_t DiceRoller::calculateDelay(uint8_t x) { return 15 * x * x + 65 * x; }

void DiceRoller::rollSingleDice() {
    uint8_t number;
    do {
        number = random(maximum) + 1;
    } while (number == lastNumbers[0]);

    lastNumbers[0] = number;

    maximum < 10 ? rollDot(number) : rollDecimal(number);
}

void DiceRoller::rollDecimal(uint8_t number) {
    uint8_t digits[2] = {0, 0};
    uint8_t index = 1;

    while (number > 0) {
        digits[index--] = number % 10;
        number /= 10;
    }

    for (uint8_t i = 0; i < 8; i++) {
        uint8_t row =
            (digits[0] != 0) ? pgm_read_byte(&decimal[digits[0]][i]) : 0;
        row |= pgm_read_byte(&decimal[digits[1]][i]) << 5;
        matrix->setRow(7 - i, row);
    }
}

void DiceRoller::rollDot(uint8_t number) {
    for (uint8_t i = 0; i <= 7; i++) {
        matrix->setRow(7 - i, pgm_read_byte(&dot[number - 1][i]));
    }
}

void DiceRoller::rollMultipleDots(uint8_t numbers[4]) {
    for (uint8_t i = 0; i <= 2; i++) {
        matrix->setRow(7 - i, pgm_read_byte(&smallDot[numbers[0]][i]) |
                                  pgm_read_byte(&smallDot[numbers[1]][i]) << 5);
    }

    if (count > 2) {
        for (uint8_t i = 0; i < 3; i++) {
            uint8_t row = pgm_read_byte(&smallDot[numbers[2]][i]);
            if (count == 4) row |= pgm_read_byte(&smallDot[numbers[3]][i]) << 5;
            matrix->setRow(2 - i, row);
        }
    }
}
