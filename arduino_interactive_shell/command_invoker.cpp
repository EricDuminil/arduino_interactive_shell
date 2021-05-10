#include "command_invoker.h"

namespace command_invoker {
  /*
   * Tries to find the corresponding callback for a given command. Name and parameter type should fit.
   */
  void execute(const char *command_str) {
    int error;
    double result = te_interp(command_str, &error);
    if (error){
        Serial.print(" ");
        for(int i=0; i<error; i++){
          Serial.print(" ");
        }
        Serial.println("↑");
        Serial.println("Je n'ai pas compris la question. :(");
        return;
    }
    Serial.printf(" = %.10g\n", result);
    led_effects::showBinaryNumber(result);
//    led_effects::showNumber(result);
  }
}
