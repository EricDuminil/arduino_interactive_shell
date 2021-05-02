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
      void (*voidFunction)(void);
      void (*intFunction)(int32_t);
      void (*strFunction)(char*);
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

  void defineStringCommand(const char *name, void (*function)(char*), const __FlashStringHelper *doc_fstring) {
    if (addCommand(name, doc_fstring)) {
      commands[commands_count].strFunction = function;
      commands[commands_count++].parameter_type = STRING;
    }
  }

  /*
   * Tries to split a string command (e.g. 'mqtt 60' or 'show_csv') into a function_name and an argument.
   * Returns the argument type, and defines the corresponding argument.
   */
  input_type parseCommand(const char *command, char *function_name, int32_t &int_argument, char *str_argument) {
    char split_command[MAX_COMMAND_SIZE];
    strlcpy(split_command, command, MAX_COMMAND_SIZE);
    char *part1;
    char *arg;
    part1 = strtok(split_command, " ");
    if (!part1) {
      Serial.println(F("Received empty command"));
      return NONE;
    }
    strlcpy(function_name, part1, MAX_COMMAND_SIZE);
    arg = strtok(NULL, "");
    if (arg) {
      char *end;
      int_argument = strtol(arg, &end, 10);
      if (*end) {
        strlcpy(str_argument, arg, MAX_COMMAND_SIZE);
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
    char str_argument[MAX_COMMAND_SIZE];
    int32_t int_argument = 0;
    input_type argument_type;
    argument_type = parseCommand(command_line, function_name, int_argument, str_argument);
    for (uint8_t i = 0; i < commands_count; i++) {
      if (!strcmp(function_name, commands[i].name) && argument_type == commands[i].parameter_type) {
        Serial.print(F("Calling : "));
        Serial.print(function_name);
        switch (argument_type) {
        case NONE:
          Serial.println(F("()"));
          commands[i].voidFunction();
          break;
        case INT32:
          Serial.print(F("("));
          Serial.print(int_argument);
          Serial.println(F(")"));
          commands[i].intFunction(int_argument);
          break;
        case STRING:
          Serial.print(F("('"));
          Serial.print(str_argument);
          Serial.println(F("')"));
          commands[i].strFunction(str_argument);
          break;
          return;
        }
      }
    }
    Serial.print(F("'"));
    Serial.print(command_line);
    Serial.println(F("' not supported. Available commands :"));
    listAvailableCommands();
  }
}
