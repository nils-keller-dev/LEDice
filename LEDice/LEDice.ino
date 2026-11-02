#include "ButtonDebounce.h"
#include <EEPROM.h>
#include "DiceRoller.h"
#include "LedControl.h"

#define ROLL_PIN 4
#define ROLL_TIME 300

LedControl* led;
ButtonDebounce* button;
DiceRoller* diceRoller;

void setup() {
  seedRandom();

  led = new LedControl();
  diceRoller = new DiceRoller(led);
  button = new ButtonDebounce(ROLL_PIN);

  diceRoller->start();
}

void seedRandom() {
  uint32_t seed;
  EEPROM.get(0, seed);
  randomSeed(seed);
  EEPROM.put(0, seed * 1103515245UL + 12345UL);
}

void loop() {
  button->update();
  diceRoller->update(millis());

  if (button->pressed()) {
    randomSeed(random() ^ micros());
    diceRoller->start();
  }
}
