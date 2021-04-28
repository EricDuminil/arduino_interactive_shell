#ifndef COMMAND_INVOKER_H_INCLUDED
#define COMMAND_INVOKER_H_INCLUDED
#include <Arduino.h>

#define MAX_COMMAND_SIZE 30

/** Other scripts can use this invoker, in order to define commands, via callbacks.
 * Those callbacks can then be used to send commands to the sensor (e.g. reset, calibrate, night mode, ...)
 * The callbacks can either have no parameter, or one int32_t parameter.
 */

namespace command_invoker {
  void defineCommand(const char *name, void (*function)(void), const __FlashStringHelper *doc_fstring);
  void defineIntCommand(const char *name, void (*function)(int32_t), const __FlashStringHelper *doc_fstring);

  void execute(const char *command_line);
}

#endif
