#include "DiceRoller.h"

DiceRoller::DiceRoller(LedControl* led) : led(led) {}

void DiceRoller::start() {
    isRolling = true;
    rollStartTime = millis();
    currentAnimationStep = 0;
}

void DiceRoller::update(unsigned long currentTime) {
    if (!isRolling) return;

    if (currentAnimationStep < 7) {
        if (currentTime - rollStartTime > calculateDelay(currentAnimationStep)) {
            currentAnimationStep++;
            displayRandom();
        }
    } else {
        isRolling = false;
    }
}

uint16_t DiceRoller::calculateDelay(uint8_t x) { return 15 * x * x + 65 * x; }

void DiceRoller::displayRandom() {
    uint8_t number;
    do {
        number = random(6) + 1;
    } while (number == lastNumber);

    lastNumber = number;

    led->showFace(number);
}
