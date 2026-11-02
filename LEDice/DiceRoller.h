#include <Arduino.h>
#include <avr/pgmspace.h>

#include "LedControl.h"

class DiceRoller {
   public:
    DiceRoller(LedControl* led);
    void start();
    void update(unsigned long currentTime);

   private:
    LedControl* led;
    unsigned long rollStartTime;
    uint8_t lastNumber = 0;
    uint8_t currentAnimationStep;
    bool isRolling;

    uint16_t calculateDelay(uint8_t index);
    void displayRandom();
};
