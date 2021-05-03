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
      void (*voidFunction)();
      void (*intFunction)(int32_t);
      void (*strFunction)(char*);
    };
    const char *doc;
    input_type parameter_type;
  };

  struct CommandLine {
    char function_name[MAX_COMMAND_SIZE];
    input_type argument_type;
    union {
      int32_t int_argument;
      char str_argument[MAX_COMMAND_SIZE];
    };
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

  void defineCommand(const char *name, void (*function)(), const __FlashStringHelper *doc_fstring) {
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
   * Tries to split a string command (e.g. 'mqtt 60' or 'show_csv') into
   * a CommandLine struct (function_name, argument_type and argument)
   */
  void parseCommand(const char *command, CommandLine &command_line) {
    char split_command[MAX_COMMAND_SIZE];
    strlcpy(split_command, command, MAX_COMMAND_SIZE);
    char *part1;
    char *arg;
    part1 = strtok(split_command, " ");
    if (!part1) {
      Serial.println(F("Received empty command"));
      command_line.argument_type = NONE;
      return;
    }
    strlcpy(command_line.function_name, part1, MAX_COMMAND_SIZE);
    arg = strtok(NULL, "");
    if (arg) {
      char *end;
      command_line.int_argument = strtol(arg, &end, 10);
      if (*end) {
        strlcpy(command_line.str_argument, arg, MAX_COMMAND_SIZE);
        command_line.argument_type = STRING;
      } else {
        command_line.argument_type = INT32;
      }
    } else {
      command_line.argument_type = NONE;
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
  void execute(const char *command_str) {
    CommandLine input;
    parseCommand(command_str, input);
    for (uint8_t i = 0; i < commands_count; i++) {
      if (!strcmp(input.function_name, commands[i].name) && input.argument_type == commands[i].parameter_type) {
        Serial.print(F("Calling : "));
        Serial.print(input.function_name);
        switch (input.argument_type) {
        case NONE:
          Serial.println(F("()"));
          commands[i].voidFunction();
          return;
        case INT32:
          Serial.print(F("("));
          Serial.print(input.int_argument);
          Serial.println(F(")"));
          commands[i].intFunction(input.int_argument);
          return;
        case STRING:
          Serial.print(F("('"));
          Serial.print(input.str_argument);
          Serial.println(F("')"));
          commands[i].strFunction(input.str_argument);
          return;
        }
      }
    }
    Serial.print(F("'"));
    Serial.print(command_str);
    Serial.println(F("' not supported. Available commands :"));
    listAvailableCommands();
  }
}
