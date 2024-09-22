/*
 *    LedControl.cpp - A library for controling Leds with a MAX7219/MAX7221
 *    Copyright (c) 2007 Eberhard Fahle
 *
 *    Permission is hereby granted, free of charge, to any person
 *    obtaining a copy of this software and associated documentation
 *    files (the "Software"), to deal in the Software without
 *    restriction, including without limitation the rights to use,
 *    copy, modify, merge, publish, distribute, sublicense, and/or sell
 *    copies of the Software, and to permit persons to whom the
 *    Software is furnished to do so, subject to the following
 *    conditions:
 *
 *    This permission notice shall be included in all copies or
 *    substantial portions of the Software.
 *
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *    OTHER DEALINGS IN THE SOFTWARE.
 */

#include "LedControl.h"

#define OP_DECODEMODE 9
#define OP_INTENSITY 10
#define OP_SCANLIMIT 11
#define OP_SHUTDOWN 12
#define OP_DISPLAYTEST 15

LedControl::LedControl(uint8_t dataPin, uint8_t clkPin, uint8_t csPin) {
    SPI_MOSI = dataPin;
    SPI_CLK = clkPin;
    SPI_CS = csPin;
    pinMode(SPI_MOSI, OUTPUT);
    pinMode(SPI_CLK, OUTPUT);
    pinMode(SPI_CS, OUTPUT);
    digitalWrite(SPI_CS, HIGH);

    spiTransfer(OP_DISPLAYTEST, 0);
    setScanLimit(7);
    spiTransfer(OP_DECODEMODE, 0);
    clearDisplay();
    shutdown(true);
}

void LedControl::shutdown(bool b) { spiTransfer(OP_SHUTDOWN, !b); }

void LedControl::setScanLimit(uint8_t limit) {
    spiTransfer(OP_SCANLIMIT, limit);
}

void LedControl::setIntensity(uint8_t intensity) {
    spiTransfer(OP_INTENSITY, intensity);
}

void LedControl::clearDisplay() {
    for (uint8_t i = 0; i < 8; i++) {
        spiTransfer(i + 1, 0);
    }
}

void LedControl::setRow(uint8_t row, byte value) {
    spiTransfer(row + 1, value);
}

void LedControl::spiTransfer(volatile byte opcode, volatile byte data) {
    digitalWrite(SPI_CS, LOW);
    shiftOut(SPI_MOSI, SPI_CLK, MSBFIRST, opcode);
    shiftOut(SPI_MOSI, SPI_CLK, MSBFIRST, data);
    digitalWrite(SPI_CS, HIGH);
}
