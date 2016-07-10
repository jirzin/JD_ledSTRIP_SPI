#include <TimerOne.h>
#include <SPI.h>
#include "Neophob_LPD6803.h"

/*
Unlike software SPI which is configurable, hardware SPI works only on very specific pins.

On the Arduino Uno, Duemilanove, etc., clock = pin 13 and data = pin 11.
For the Arduino Mega, clock = pin 52, data = pin 51.
For the ATmega32u4 Breakout Board and Teensy, clock = pin B1, data = B2.

Alternately, on most boards you can use the 6-pin programming
header for SPI output as well, in which case clock = pin 3 and data = pin 4.

src: http://forums.adafruit.com/viewtopic.php?f=47&t=24256
thanks phil!
*/

#define LED_MODULES 240

Neophob_LPD6803 strip = Neophob_LPD6803(LED_MODULES);

const int len = 12; // length of light streak
int streakColors[len][3] = {
  {63, 14, 0},  // 0
  {63, 7, 0},
  {55, 3, 0},
  {25, 2, 0},
  {15, 1, 0},
  {10, 0, 0},
  {7, 0, 0},
  {5, 0, 0},
  {3, 0, 0},
  {2, 0, 0},
  {1, 0, 0},
  {0, 0, 0} // end state = black
};

const int switchPin = 8;

typedef enum {LISTENING, STREAK} ProgState;
ProgState ps = LISTENING;

void setup() {
  pinMode(switchPin, INPUT_PULLUP);
  strip.setCPUmax(100);
  strip.begin(SPI_CLOCK_DIV16);
  strip.show();
}

void loop() {
  switch (ps) {
    case LISTENING:
      if (!digitalRead(switchPin)) {
        ps = STREAK;
      }
      break;
    case STREAK:
      colorWipeB(20);
      delay(1000);
      ps = LISTENING;
      break;
  }
}

void colorWipeA(uint8_t wait) {
  for (int i = 0; i < strip.numPixels(); i++) {
    for (int l = 0; l < len; l++) {
      strip.setPixelColor(i - l, Color(streakColors[l][0], streakColors[l][1], streakColors[l][2]));
    }
    strip.show();
    delay(wait);
  }
}

void colorWipeB(uint8_t wait) {
  for (int i = strip.numPixels(); i > 0; i--) {
    for (int l = 0; l < len; l++) {
      strip.setPixelColor(i + l, Color(streakColors[l][0], streakColors[l][1], streakColors[l][2]));
    }
    strip.show();
    delay(wait);
  }
}


void stripBlack() {
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, Color(0, 0, 0));
    strip.show();
    delayMicroseconds(1);
  }
}

unsigned int Color(byte R, byte G, byte B)
{
  return ( ((unsigned int)B & 0x1F ) << 10 | ((unsigned int)R & 0x1F) << 5 | (unsigned int)G & 0x1F);
}

