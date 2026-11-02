#include <inttypes.h>
#include "Arduino.h"

class ButtonDebounce {
   public:
    ButtonDebounce(int pin, int mode = INPUT_PULLUP);

    bool update();
    bool read() const;
    bool pressed() const;

   private:
    static const uint8_t DEBOUNCED_STATE = 0b00000001;
    static const uint8_t UNSTABLE_STATE = 0b00000010;
    static const uint8_t CHANGED_STATE = 0b00000100;

    bool isPressed() const;
    bool changed() const;
    void changeState();
    void setStateFlag(const uint8_t flag);
    void unsetStateFlag(const uint8_t flag);
    void toggleStateFlag(const uint8_t flag);
    bool getStateFlag(const uint8_t flag) const;
    bool readCurrentState();

    unsigned long previous_millis;
    uint8_t state;
    uint8_t pin;
};