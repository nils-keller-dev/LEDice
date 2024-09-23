#ifndef DiceRoller_h
#define DiceRoller_h

#include <Arduino.h>
#include <avr/pgmspace.h>

#include "LedControl.h"
#include "LedPatterns.h"

class DiceRoller {
   public:
    DiceRoller(LedControl* matrix);
    void rollSingleDice(uint8_t maximum, bool isDisplay = false);
    void rollMultipleDice(uint8_t count, uint8_t maximum,
                          bool isDisplay = false);

   private:
    LedControl* matrix;

    void rollDecimal(uint8_t number);
    void rollDot(uint8_t number);
};

#endif