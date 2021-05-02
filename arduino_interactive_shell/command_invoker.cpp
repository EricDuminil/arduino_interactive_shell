#include "command_invoker.h"

namespace command_invoker {
  const uint8_t MAX_COMMANDS = 20;

  uint8_t commands_count = 0;

  enum input_type {
    NONE,
    INT32,
    STRING
  };

  struct Command {
    const char *name;
    union {
      void (*intFunction)(int32_t);
      void (*voidFunction)(void);
    };
    const char *doc;
    input_type parameter_type;
  };

  Command commands[MAX_COMMANDS];

  bool addCommand(const char *name, const __FlashStringHelper *doc_fstring) {
    if (commands_count < MAX_COMMANDS) {
      commands[commands_count].name = name;
      commands[commands_count].doc = (const char*) doc_fstring;
      return true;
    } else {
      Serial.println(F("Too many commands have been defined."));
      return false;
    }
  }

  void defineCommand(const char *name, void (*function)(void), const __FlashStringHelper *doc_fstring) {
    if (addCommand(name, doc_fstring)) {
      commands[commands_count].voidFunction = function;
      commands[commands_count++].parameter_type = NONE;
    }
  }

  void defineIntCommand(const char *name, void (*function)(int32_t), const __FlashStringHelper *doc_fstring) {
    if (addCommand(name, doc_fstring)) {
      commands[commands_count].intFunction = function;
      commands[commands_count++].parameter_type = INT32;
    }
  }

  /*
   * Tries to split a string command (e.g. 'mqtt 60' or 'show_csv') into a function_name and an argument.
   * Returns the argument type.
   */
  input_type parseCommand(const char *command, char *function_name, int32_t &argument) {
    char split_command[MAX_COMMAND_SIZE];
    strlcpy(split_command, command, MAX_COMMAND_SIZE);
    char *arg;
    char *part1;
    part1 = strtok(split_command, " ");
    if (!part1) {
      Serial.println(F("Received empty command"));
      return NONE;
    }
    strlcpy(function_name, part1, MAX_COMMAND_SIZE);
    arg = strtok(NULL, " ");
    if (arg) {
      char *end;
      argument = strtol(arg, &end, 10);
      if (*end) {
        //TODO: Return arg
        return STRING;
      } else {
        return INT32;
      }
    } else {
      return NONE;
    }
  }

  int compareCommandNames(const void *s1, const void *s2) {
    struct Command *c1 = (struct Command*) s1;
    struct Command *c2 = (struct Command*) s2;
    return strcmp(c1->name, c2->name);
  }

  void listAvailableCommands() {
    qsort(commands, commands_count, sizeof(commands[0]), compareCommandNames);
    for (uint8_t i = 0; i < commands_count; i++) {
      Serial.print("  ");
      Serial.print(commands[i].name);
      Serial.print(commands[i].doc);
      Serial.println(".");
    }
  }

  /*
   * Tries to find the corresponding callback for a given command. Name and parameter type should fit.
   */
  void execute(const char *command_line) {
    char function_name[MAX_COMMAND_SIZE];
    int32_t argument = 0;
    input_type argument_type;
    argument_type = parseCommand(command_line, function_name, argument);

    for (uint8_t i = 0; i < commands_count; i++) {
      if (!strcmp(function_name, commands[i].name) && argument_type == commands[i].parameter_type) {
        Serial.print(F("Calling : "));
        Serial.print(function_name);
        if (argument_type == INT32) {
          Serial.print(F("("));
          Serial.print(argument);
          Serial.println(F(")"));
          commands[i].intFunction(argument);
        } else {
          Serial.println(F("()"));
          commands[i].voidFunction();
        }
        return;
      }
    }
    Serial.print(F("'"));
    Serial.print(command_line);
    Serial.println(F("' not supported. Available commands :"));
    listAvailableCommands();
  }
}
