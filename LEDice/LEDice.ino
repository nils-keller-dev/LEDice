#include <LedControl.h>
#include "button.h"

#define CS_PIN PB0
#define CLK_PIN PB1
#define DIN_PIN PB2
#define MAX_SEG 1
#define ROLL_PIN PB4
#define MODE_PIN A3
#define RND_PIN A0
#define ROLL_TIME 300

const uint8_t decimal[10][8] = {
  { B010, B101, B101, B101, B101, B101, B101, B010 },
  { B001, B011, B101, B001, B001, B001, B001, B001 },
  { B010, B101, B101, B001, B010, B100, B100, B111 },
  { B110, B001, B001, B010, B001, B001, B001, B110 },
  { B100, B100, B101, B111, B001, B001, B001, B001 },
  { B111, B100, B100, B111, B001, B001, B101, B010 },
  { B010, B101, B100, B110, B101, B101, B101, B010 },
  { B111, B001, B001, B010, B010, B100, B100, B100 },
  { B010, B101, B101, B010, B101, B101, B101, B010 },
  { B010, B101, B101, B011, B001, B001, B101, B010 },
};

const uint8_t dot[8][8] = {
  { B00000000, B00000000, B00000000, B00011000, B00011000, B00000000, B00000000, B00000000 },
  { B00000011, B00000011, B00000000, B00000000, B00000000, B00000000, B11000000, B11000000 },
  { B00000011, B00000011, B00000000, B00011000, B00011000, B00000000, B11000000, B11000000 },
  { B11000011, B11000011, B00000000, B00000000, B00000000, B00000000, B11000011, B11000011 },
  { B11000011, B11000011, B00000000, B00011000, B00011000, B00000000, B11000011, B11000011 },
  { B11000011, B11000011, B00000000, B11000011, B11000011, B00000000, B11000011, B11000011 },
  { B11000011, B11000011, B00000000, B11011011, B11011011, B00000000, B11000011, B11000011 },
  { B11011011, B11011011, B00000000, B11000011, B11000011, B00000000, B11011011, B11011011 },
};

const uint8_t smallDot[6][3] = {
  { B000, B010, B000 },
  { B100, B000, B001 },
  { B100, B010, B001 },
  { B101, B000, B101 },
  { B101, B010, B101 },
  { B101, B101, B101 },
};

const uint8_t diceSidesCount[9] = { 6, 6, 6, 6, 4, 8, 10, 12, 20 };

LedControl* matrix;
bool diceRolling = false;
uint32_t buttonPressTime = 0;
uint8_t selectedDice = 0;

Button btnRoll(ROLL_PIN);
Button btnMode(MODE_PIN);

void rollDice(bool setup = false) {
  if (!setup) {
    matrix->clearDisplay(0);
    delay(ROLL_TIME);
  }

  if (selectedDice == 0 || diceSidesCount[selectedDice] != 6) {
    rollSingleDice();
  } else {
    rollMultipleDice(selectedDice + 1);
  }
}

void setup() {
  matrix = new LedControl(DIN_PIN, CLK_PIN, CS_PIN, MAX_SEG);
  matrix->clearDisplay(0);

  randomSeed(analogRead(RND_PIN));

  btnRoll.begin();
  btnMode.begin();

  matrix->shutdown(0, false);

  uint8_t intensity = 0;

  if (!digitalRead(ROLL_PIN)) {
    intensity += 5;
  }

  if (!digitalRead(MODE_PIN)) {
    intensity += 10;
  }

  matrix->setIntensity(0, intensity);

  rollDice(true);
}

void changeMode() {
  selectedDice = (selectedDice + 1) % 9;
}

void rollDecimal(uint8_t number) {
  uint8_t digits[2] = {};
  int index = 1;

  while (number > 0) {
    int digit = number % 10;
    digits[index] = digit;
    number /= 10;
    index--;
  }

  if (digits[0] != 0) {
    for (uint8_t i = 0; i < 8; i++) {
      matrix->setRow(0, i, decimal[digits[0]][i] << 5 | decimal[digits[1]][i]);
    }
  } else {
    for (uint8_t i = 0; i < 8; i++) {
      matrix->setRow(0, i, decimal[digits[1]][i]);
    }
  }
}

void rollDot(uint8_t number) {
  for (uint8_t i = 0; i <= 7; i++) {
    matrix->setRow(0, i, dot[number - 1][i]);
  }
}

void rollSingleDice() {
  uint8_t number = random(1, diceSidesCount[selectedDice] + 1);

  if (selectedDice < 6) {
    rollDot(number);
  } else {
    rollDecimal(number);
  }
}

void rollMultipleDice(uint8_t count) {
  uint8_t numbers[count];

  for (uint8_t i = 0; i <= count; i++) {
    numbers[i] = random(6);
  }

  for (uint8_t i = 0; i <= 2; i++) {
    matrix->setRow(0, i, smallDot[numbers[0]][i] | smallDot[numbers[1]][i] << 5);
  }

  if (count <= 2) {
    return;
  }

  for (uint8_t i = 0; i <= 2; i++) {
    uint8_t rowValue = smallDot[numbers[2]][i] << 5;
    if (count == 4) {
      rowValue |= smallDot[numbers[3]][i];
    }
    matrix->setRow(0, i + 5, rowValue);
  }
}

void loop() {
  if (btnRoll.isPressed()) {
    rollDice();
  }

  if (btnMode.isPressed()) {
    changeMode();
    rollDice();
  }
}
