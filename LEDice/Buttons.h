#include <Bounce2.h>

class Buttons {
  private : Bounce2::Button btnRoll;
  private : Bounce2::Button btnMode;

  private : bool bothPressed = false;
  private : bool skipNextRollRelease = false;
  private : bool skipNextModeRelease = false;

  public : Buttons(uint8_t rollPin, uint8_t modePin, bool rollPressed, bool modePressed) {
    skipNextRollRelease = rollPressed;
    skipNextModeRelease = modePressed;
    if (skipNextRollRelease && skipNextModeRelease) bothPressed = true;

    btnRoll = Bounce2::Button();
    btnMode = Bounce2::Button();
    btnRoll.attach(rollPin, INPUT_PULLUP);
    btnMode.attach(modePin, INPUT_PULLUP);
    btnRoll.interval(5);
    btnMode.interval(5);
    btnRoll.setPressedState(LOW);
    btnMode.setPressedState(LOW);
  }

  void update() {
    btnRoll.update();
    btnMode.update();

    if (!btnRoll.isPressed() && !btnMode.isPressed()) {
      bothPressed = false;
    }
  }

  bool wasRollReleased() {
    if (btnRoll.released()) {
      if (!skipNextRollRelease) {
        return true;
      }
      skipNextRollRelease = false;
    }
    return false;
  }

  bool wasModeReleased() {
    if (btnMode.released()) {
      if (!skipNextModeRelease) {
        return true;
      }
      skipNextModeRelease = false;
    }
    return false;
  }

  bool wereBothPressed() {
    if (btnRoll.isPressed() && btnMode.isPressed() && !bothPressed) {
      bothPressed = true;
      skipNextRollRelease = true;
      skipNextModeRelease = true;
      return true;
    }

    return false;
  }
};