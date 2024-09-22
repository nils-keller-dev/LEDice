#include "Bounce2.h"

class Buttons {
   private:
    Bounce2 btnRoll;
    Bounce2 btnMode;
    bool bothPressed = false;
    bool skipNextRollRelease = false;
    bool skipNextModeRelease = false;

   public:
    Buttons(uint8_t rollPin, uint8_t modePin, bool rollPressed,
            bool modePressed) {
        skipNextRollRelease = rollPressed;
        skipNextModeRelease = modePressed;
        if (skipNextRollRelease && skipNextModeRelease) bothPressed = true;

        btnRoll = Bounce2();
        btnMode = Bounce2();
        btnRoll.attach(rollPin, INPUT_PULLUP);
        btnMode.attach(modePin, INPUT_PULLUP);
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