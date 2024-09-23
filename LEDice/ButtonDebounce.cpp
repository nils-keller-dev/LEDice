#include "ButtonDebounce.h"

ButtonDebounce::ButtonDebounce(int pin, int mode)
    : previous_millis(0), state(0), pin(pin) {
    pinMode(pin, mode);
    this->pin = pin;
    state = 0;
    if (readCurrentState()) {
        setStateFlag(DEBOUNCED_STATE | UNSTABLE_STATE);
    }
    previous_millis = millis();
}

bool ButtonDebounce::update() {
    unsetStateFlag(CHANGED_STATE);
    bool currentState = readCurrentState();

    if (currentState != getStateFlag(UNSTABLE_STATE)) {
        previous_millis = millis();
        toggleStateFlag(UNSTABLE_STATE);
    } else if (millis() - previous_millis >= 5) {
        if (currentState != getStateFlag(DEBOUNCED_STATE)) {
            previous_millis = millis();
            changeState();
        }
    }

    return changed();
}

inline void ButtonDebounce::changeState() {
    toggleStateFlag(DEBOUNCED_STATE);
    setStateFlag(CHANGED_STATE);
}

bool ButtonDebounce::read() const { return getStateFlag(DEBOUNCED_STATE); }

bool ButtonDebounce::changed() const { return getStateFlag(CHANGED_STATE); }

bool ButtonDebounce::isPressed() const { return read() == LOW; }

bool ButtonDebounce::released() const { return changed() && !isPressed(); }

void ButtonDebounce::setStateFlag(const uint8_t flag) { state |= flag; }

void ButtonDebounce::unsetStateFlag(const uint8_t flag) { state &= ~flag; }

void ButtonDebounce::toggleStateFlag(const uint8_t flag) { state ^= flag; }

bool ButtonDebounce::getStateFlag(const uint8_t flag) const {
    return (state & flag) != 0;
}

bool ButtonDebounce::readCurrentState() { return digitalRead(pin); }
