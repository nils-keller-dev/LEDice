// https://forum.arduino.cc/t/the-reliable-but-not-very-sexy-way-to-seed-random/65872/52

#include <avr/eeprom.h>

void reseedRandom(uint32_t* address) {
    static const uint32_t HappyPrime = 127807;
    uint32_t raw;
    unsigned long seed;

    raw = eeprom_read_dword(address);

    do {
        raw += HappyPrime;
        seed = raw & 0x7FFFFFFF;
    } while ((seed < 1) || (seed > 2147483646));

    srandom(seed);
    eeprom_write_dword(address, raw);
    random(1);
}

uint32_t reseedRandomSeed EEMEM = 0xFFFFFFFF;