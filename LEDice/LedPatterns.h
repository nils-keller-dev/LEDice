#define GROUP_A _BV(PB0)
#define GROUP_B _BV(PB1)
#define GROUP_C _BV(PB2)
#define GROUP_D _BV(PB3)
#define GROUP_MASK (GROUP_A | GROUP_B | GROUP_C | GROUP_D)

static const uint8_t FACES[7] PROGMEM = {
    GROUP_A,                              // ⚀
    GROUP_B,                              // ⚁
    GROUP_A | GROUP_B,                    // ⚂
    GROUP_B | GROUP_C,                    // ⚃
    GROUP_A | GROUP_B | GROUP_C,          // ⚄
    GROUP_B | GROUP_C | GROUP_D           // ⚅
};