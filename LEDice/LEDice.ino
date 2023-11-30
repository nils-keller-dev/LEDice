#include <LedControl.h>
#include "button.h"

#define CS_PIN  PB0
#define CLK_PIN PB1
#define DIN_PIN PB2
#define MAX_SEG 1
#define ROLL_PIN PB4
#define NUMBER_DICE_PIN A3
#define RND_PIN A0
#define ROLL_TIME 300

const uint8_t dice[6][8] = {
  { B00000000, B00000000, B00000000, B00011000, B00011000, B00000000, B00000000, B00000000 },
  { B11000000, B11000000, B00000000, B00000000, B00000000, B00000000, B00000011, B00000011 },
  { B11000000, B11000000, B00000000, B00011000, B00011000, B00000000, B00000011, B00000011 },
  { B11000011, B11000011, B00000000, B00000000, B00000000, B00000000, B11000011, B11000011 },
  { B11000011, B11000011, B00000000, B00011000, B00011000, B00000000, B11000011, B11000011 },
  { B11000011, B11000011, B00000000, B11000011, B11000011, B00000000, B11000011, B11000011 }
};

const uint8_t smallDice[6][3] = {
  { B000, B010, B000 },
  { B100, B000, B001 },
  { B100, B010, B001 },
  { B101, B000, B101 },
  { B101, B010, B101 },
  { B101, B101, B101 },
};

LedControl* matrix;
bool diceRolling = false;
uint32_t buttonPressTime = 0;
uint8_t numberDice;

Button btnRollDice(ROLL_PIN);
Button btnNumberDice(NUMBER_DICE_PIN);

void rollDice(bool setup = false) {
  if (!setup) {
    matrix->clearDisplay(0);
    delay(ROLL_TIME);
  }

  if (numberDice == 0) {
    rollSingleDice();
  } else {
    rollMultipleDice();
  }
}

void setup() {
  matrix = new LedControl(DIN_PIN, CLK_PIN, CS_PIN, MAX_SEG);
  matrix->clearDisplay(0);

  randomSeed(analogRead(RND_PIN));

  btnRollDice.begin();
  btnNumberDice.begin();

  matrix->shutdown(0, false);

  uint8_t intensity = 0;

  if (!digitalRead(ROLL_PIN)) {
    intensity += 5;
  }

  if (!digitalRead(NUMBER_DICE_PIN)) {
    intensity += 10;
  }

  matrix->setIntensity(0, intensity);

  rollDice(true);
}

void changeNumberDice() {
  numberDice = (numberDice + 1) % 4;
}

void rollSingleDice() {
  uint8_t number = random(6);

  for (uint8_t i = 0; i <= 7; i++) {
    matrix->setRow(0, i, dice[number][i]);
  }
}

void rollMultipleDice() {
  uint8_t numbers[numberDice + 1];

  for (uint8_t i = 0; i <= numberDice; i++) {
    numbers[i] = random(6);
  }

  for (uint8_t i = 0; i <= 2; i++) {
    matrix->setRow(0, i, smallDice[numbers[0]][i] | smallDice[numbers[1]][i] << 5);
  }

  if (numberDice < 2) {
    return;
  }

  for (uint8_t i = 0; i <= 2; i++) {
    uint8_t rowValue = smallDice[numbers[2]][i];
    if (numberDice == 3) {
      rowValue |= smallDice[numbers[3]][i] << 5;
    }
    matrix->setRow(0, i + 5, rowValue);
  }
}

void loop() {
  if (btnRollDice.isPressed()) {
    rollDice();
  }

  if (btnNumberDice.isPressed()) {
    changeNumberDice();
    rollDice();
  }
}
