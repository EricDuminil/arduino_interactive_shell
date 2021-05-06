#include "led_effects.h"
/*****************************************************************
 * Configuration                                                 *
 *****************************************************************/
namespace config {
  const uint8_t max_brightness = MAX_BRIGHTNESS;
}

#if defined(ESP8266)
// NeoPixels on GPIO05, aka D1 on ESP8266.
const int NEOPIXELS_PIN = 5;
#elif defined(ESP32)
// NeoPixels on GPIO23 on ESP32. To avoid conflict with LoRa_SCK on TTGO.
const int NEOPIXELS_PIN = 23;
#endif

const int NUMPIXELS = 12;
//NOTE: One value has been prepended, to make calculations easier and avoid out of bounds index.
const uint16_t CO2_TICKS[NUMPIXELS + 1] = { 0, 500, 600, 700, 800, 900, 1000, 1200, 1400, 1600, 1800, 2000, 2200 }; // [ppm]
// const uint16_t CO2_TICKS[NUMPIXELS + 1] = { 0, 400, 500, 600, 700, 800, 900, 1000, 1100, 1200, 1300, 1400, 1500, 1600, 1800, 2000, 2200 }; // [ppm]
// For a given LED, which color should be displayed? First LED will be pure green (hue angle 120°),
// last 4 LEDs will be pure red (hue angle 0°), LEDs in-between will be yellowish.
// For reference, this python code can be used to generate the array
//    NUMPIXELS = 12
//    RED_LEDS = 4
//    hues = [ (2**16-1) // 3 * max(NUMPIXELS - RED_LEDS - i, 0) // (NUMPIXELS - RED_LEDS) for i in range(NUMPIXELS) ]
//    '{' + ', '.join([str(hue) + ('U' if hue else '') for hue in hues]) + '}; // [hue angle]'
const uint16_t LED_HUES[NUMPIXELS] = { 21845U, 19114U, 16383U, 13653U, 10922U, 8191U, 5461U, 2730U, 0, 0, 0, 0 }; // [hue angle]
// const uint16_t LED_HUES[NUMPIXELS] = { 21845U, 20024U, 18204U, 16383U, 14563U, 12742U, 10922U, 9102U, 7281U, 5461U, 3640U, 1820U, 0, 0, 0, 0 }; // [hue angle]
Adafruit_NeoPixel pixels(NUMPIXELS, NEOPIXELS_PIN, NEO_GRB + NEO_KHZ800);

namespace led_effects {
  //On-board LED on D4, aka GPIO02
  const int ONBOARD_LED_PIN = 2;

  void setupOnBoardLED() {
    pinMode(ONBOARD_LED_PIN, OUTPUT);
  }

  void onBoardLEDOff() {
    //NOTE: OFF is LOW on ESP32 and HIGH on ESP8266 :-/
#ifdef ESP8266
    digitalWrite(ONBOARD_LED_PIN, HIGH);
#else
    digitalWrite(ONBOARD_LED_PIN, LOW);
#endif
  }

  void onBoardLEDOn() {
#ifdef ESP8266
    digitalWrite(ONBOARD_LED_PIN, LOW);
#else
    digitalWrite(ONBOARD_LED_PIN, HIGH);
#endif
  }

  void LEDsOff() {
    pixels.clear();
    pixels.show();
    onBoardLEDOff();
  }

  void setupRing() {
    pixels.begin();
    pixels.setBrightness(config::max_brightness);
    LEDsOff();
  }

  void showBinaryNumber(long n) {
    pixels.clear();
    for (int bit = 0; bit < NUMPIXELS; ++bit) {
      if (n >> bit & 1) {
        pixels.setPixelColor(bit, color::beautiful);
      }
    }
    pixels.show();
    delay(50);
  }

  void redAlert() {
    pixels.clear();
    for (int i = 0; i < 11; i++) {
      pixels.setBrightness(static_cast<int>(config::max_brightness * (1 - i * 0.1)));
      delay(50);
      pixels.fill(color::beautiful);
      pixels.show();
    }
  }

  void showNumber(long n) {
    for (int i = 0; i < n / NUMPIXELS; i++) {
      redAlert();
    }
    pixels.clear();
    pixels.show();
    delay(200);
    pixels.setBrightness(config::max_brightness);
    if (n % NUMPIXELS){
      pixels.fill(color::beautiful, 0, n % NUMPIXELS);
    }
    pixels.show();
    delay(50);
  }
}
