
/*
  The MIT License (MIT)

  Copyright (c) 2013 thomasfredericks

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
  Main code by Thomas O Fredericks (tof@t-o-f.info)
  Previous contributions by Eric Lowry, Jim Schimpf and Tom Harkaway
  * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef Bounce2_h
#define Bounce2_h

#include <inttypes.h>

#include "Arduino.h"

class Bounce2 {
   public:
    Bounce2();

    void attach(int pin, int mode);
    void attach(int pin);
    bool update();
    bool read() const;
    bool changed() const;
    bool isPressed() const;
    bool released() const;

   private:
    static const uint8_t DEBOUNCED_STATE = 0b00000001;
    static const uint8_t UNSTABLE_STATE = 0b00000010;
    static const uint8_t CHANGED_STATE = 0b00000100;

    void changeState();
    void setStateFlag(const uint8_t flag);
    void unsetStateFlag(const uint8_t flag);
    void toggleStateFlag(const uint8_t flag);
    bool getStateFlag(const uint8_t flag) const;
    bool readCurrentState();
    void setPinMode(int pin, int mode);
    void begin();

    unsigned long previous_millis;
    uint8_t state;
    uint8_t pin;
};

#endif