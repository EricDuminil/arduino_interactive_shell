/***************************************************************************************************
 * Small interactive shell via Serial interface for ESP32 and ESP8266, based on the command pattern.
 ***************************************************************************************************/

#include "command_invoker.h"
#include "led_effects.h"

/**
 * Setup
 */

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  led_effects::setupRing();

  Serial.println(F("Bonjour! Que puis-je calculer pour vous?"));
  Serial.print(F("> "));
}

/*
 * Saves bytes from Serial.read() until enter is pressed, and tries to run the corresponding command.
 *   http://www.gammon.com.au/serial
 */
void processSerialInput(const byte input_byte) {
  static char input_line[MAX_COMMAND_SIZE];
  static unsigned int input_pos = 0;
  switch (input_byte) {
  case '\n': // end of text
    Serial.println();
    input_line[input_pos] = 0;
    command_invoker::execute(input_line);
    input_pos = 0;
    Serial.print(F("> "));
    break;
  case '\r': // discard carriage return
    break;
  case '\b': // backspace
    if (input_pos > 0) {
      input_pos--;
      Serial.print(F("\b \b"));
    }
    break;
  default:
    // keep adding if not full ... allow for terminating null byte
    if (input_pos < (MAX_COMMAND_SIZE - 1)) {
      input_line[input_pos++] = input_byte;
      Serial.print((char) input_byte);
    }
    break;
  }
}

/**
 * Loop and wait for serial input. Commands could also come from webserver or MQTT, for example.
 */

void loop() {
  while (Serial.available() > 0) {
    processSerialInput(Serial.read());
  }
  delay(50);
}
