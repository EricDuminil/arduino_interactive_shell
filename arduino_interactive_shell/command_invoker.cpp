#include "command_invoker.h"

namespace command_invoker {
  /*
   * Tries to find the corresponding callback for a given command. Name and parameter type should fit.
   */
  void execute(const char *command_str) {
    int error;
    double result = te_interp(command_str, &error);
    if (error){
        Serial.println("Je n'ai pas compris la question. :(");
        return;
    }
    Serial.print(" = ");
    Serial.println(result);
    led_effects::showBinaryNumber(result);
//    led_effects::showNumber(result);
  }
}
