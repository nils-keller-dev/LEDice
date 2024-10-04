#ifndef DiceRoller_h
#define DiceRoller_h

#include <Arduino.h>
#include <avr/pgmspace.h>

#include "LedControl.h"
#include "LedPatterns.h"

class DiceRoller {
   public:
    DiceRoller(LedControl* matrix);
    void startRoll(uint8_t count, uint8_t maximum, bool isDisplay);
    void updateRoll(unsigned long currentTime);

   private:
    LedControl* matrix;
    unsigned long rollStartTime;
    uint8_t currentAnimationStep;
    uint8_t lastNumbers[4] = {0, 0, 0, 0};
    uint8_t currentAnimationSteps[4];
    uint8_t delayOffsets[4];
    bool isRolling;
    uint8_t count;
    uint8_t maximum;
    bool isDisplay;

    uint16_t calculateDelay(uint8_t index);
    void rollSingleDice();
    void rollDecimal(uint8_t number);
    void rollDot(uint8_t number);
    void rollMultipleDots(uint8_t numbers[4]);
};

#endif