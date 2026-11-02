#include "ButtonDebounce.h"
#include "DiceRoller.h"
#include "LedControl.h"

#define ROLL_PIN 4
#define ROLL_TIME 300

LedControl* led;
ButtonDebounce* button;
DiceRoller* diceRoller;

void setup() {
  // randomSeed(analogRead(RND_PIN));

  led = new LedControl();
  diceRoller = new DiceRoller(led);
  button = new ButtonDebounce(ROLL_PIN);

  diceRoller->start();
}

void loop() {
  button->update();
  diceRoller->update(millis());

  if (button->pressed()) {
    randomSeed(random() ^ micros());
    diceRoller->start();
  }
}
