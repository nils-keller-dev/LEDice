#include "LedControl.h"
#include "LedPatterns.h"

LedControl::LedControl() {
    DDRB |= GROUP_MASK;
}

void LedControl::setGroups(uint8_t groups) {
    PORTB = (PORTB & ~GROUP_MASK) | (groups & GROUP_MASK);
}

void LedControl::showFace(uint8_t number) {
    setGroups(pgm_read_byte(&FACES[number - 1]));
}