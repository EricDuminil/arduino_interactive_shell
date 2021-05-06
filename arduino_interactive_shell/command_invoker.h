#ifndef COMMAND_INVOKER_H_INCLUDED
#define COMMAND_INVOKER_H_INCLUDED
#include <Arduino.h>
#include "led_effects.h"
#include "tinyexpr.h"

#define MAX_COMMAND_SIZE 30

namespace command_invoker {
  void execute(const char *command_line);
}

#endif
