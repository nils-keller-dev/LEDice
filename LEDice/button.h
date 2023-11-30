class Button {
  private:
    uint8_t pin;
    uint16_t state;

  public:
    Button(uint8_t p) {
      pin = p;
    }

    void begin() {
      state = 0;
      pinMode(pin, INPUT_PULLUP);
    }

    bool isPressed() {
      state = (state << 1) | digitalRead(pin) | 0xfe00;
      return (state == 0xff00);
    }
};
