#ifndef LED_EFFECTS_H_INCLUDED
#define LED_EFFECTS_H_INCLUDED
#include <Arduino.h>

// Adafruit NeoPixel (Arduino library for controlling single-wire-based LED pixels and strip)
// https://github.com/adafruit/Adafruit_NeoPixel
// Documentation : http://adafruit.github.io/Adafruit_NeoPixel/html/class_adafruit___neo_pixel.html
#include "src/lib/Adafruit_NeoPixel/Adafruit_NeoPixel.h"
#define MAX_BRIGHTNESS 120

namespace color {
  const uint32_t red = 0xFF0000;
  const uint32_t green = 0x00FF00;
  const uint32_t blue = 0x0000FF;
  const uint32_t black = 0x000000;
  const uint32_t magenta = 0xFF00FF;
  const uint32_t beautiful = 0xFF0015;
}

namespace led_effects {
  void setupOnBoardLED();
  void onBoardLEDOff();
  void onBoardLEDOn();
  void LEDsOff();

  void setupRing();
  void showBinaryNumber(long n);
  void showNumber(long n);
}
#endif
