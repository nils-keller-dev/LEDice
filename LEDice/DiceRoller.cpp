#include "DiceRoller.h"

DiceRoller::DiceRoller(LedControl* matrix) : matrix(matrix) {}

void DiceRoller::rollSingleDice(uint8_t maximum, bool isDisplay) {
    uint8_t number = isDisplay ? maximum : random(maximum) + 1;
    maximum < 10 ? rollDot(number) : rollDecimal(number);
}

void DiceRoller::rollMultipleDice(uint8_t count, uint8_t maximum,
                                  bool isDisplay) {
    uint8_t numbers[count];

    for (uint8_t i = 0; i < count; i++) {
        numbers[i] = isDisplay ? maximum - 1 : random(maximum);
    }

    for (uint8_t i = 0; i <= 2; i++) {
        matrix->setRow(7 - i, pgm_read_byte(&smallDot[numbers[0]][i]) |
                                  pgm_read_byte(&smallDot[numbers[1]][i]) << 5);
    }

    if (count > 2) {
        for (uint8_t i = 0; i < 3; i++) {
            uint8_t row = pgm_read_byte(&smallDot[numbers[2]][i]);
            if (count == 4) row |= pgm_read_byte(&smallDot[numbers[3]][i]) << 5;
            matrix->setRow(2 - i, row);
        }
    }
}

void DiceRoller::rollDecimal(uint8_t number) {
    uint8_t digits[2] = {0, 0};
    uint8_t index = 1;

    while (number > 0) {
        digits[index--] = number % 10;
        number /= 10;
    }

    for (uint8_t i = 0; i < 8; i++) {
        uint8_t row =
            (digits[0] != 0) ? pgm_read_byte(&decimal[digits[0]][i]) : 0;
        row |= pgm_read_byte(&decimal[digits[1]][i]) << 5;
        matrix->setRow(7 - i, row);
    }
}

void DiceRoller::rollDot(uint8_t number) {
    for (uint8_t i = 0; i <= 7; i++) {
        matrix->setRow(7 - i, pgm_read_byte(&dot[number - 1][i]));
    }
}
