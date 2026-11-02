#ifndef LedControl_h
#define LedControl_h

#include <Arduino.h>
#include <avr/pgmspace.h>

class LedControl {
   private:
    void setGroups(uint8_t groups);

   public:
    LedControl();

    void showFace(uint8_t number);
};

#endif
