#include "Bounce2.h"

Bounce2::Bounce2() : previous_millis(0), state(0), pin(0) {}

void Bounce2::attach(int pin) {
    this->pin = pin;
    begin();
}

void Bounce2::attach(int pin, int mode) {
    setPinMode(pin, mode);
    this->attach(pin);
}

void Bounce2::begin() {
    state = 0;
    if (readCurrentState()) {
        setStateFlag(DEBOUNCED_STATE | UNSTABLE_STATE);
    }
    previous_millis = millis();
}

bool Bounce2::update() {
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

inline void Bounce2::changeState() {
    toggleStateFlag(DEBOUNCED_STATE);
    setStateFlag(CHANGED_STATE);
}

bool Bounce2::read() const { return getStateFlag(DEBOUNCED_STATE); }

bool Bounce2::changed() const { return getStateFlag(CHANGED_STATE); }

bool Bounce2::isPressed() const { return read() == LOW; }

bool Bounce2::released() const { return changed() && !isPressed(); }

void Bounce2::setStateFlag(const uint8_t flag) { state |= flag; }

void Bounce2::unsetStateFlag(const uint8_t flag) { state &= ~flag; }

void Bounce2::toggleStateFlag(const uint8_t flag) { state ^= flag; }

bool Bounce2::getStateFlag(const uint8_t flag) const {
    return (state & flag) != 0;
}

bool Bounce2::readCurrentState() { return digitalRead(pin); }

void Bounce2::setPinMode(int pin, int mode) { pinMode(pin, mode); }