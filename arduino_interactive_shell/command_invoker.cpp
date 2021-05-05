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
    int32_t int_argument;
    char str_argument[MAX_COMMAND_SIZE];
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
    if (strlen(command) == 0) {
      Serial.println(F("Received empty command"));
      command_line.argument_type = NONE;
      return;
    }

    char *first_space;
    first_space = strchr(command, ' ');

    if (first_space == NULL) {
      Serial.println(F("No argument"));
      command_line.argument_type = NONE;
      strlcpy(command_line.function_name, command, MAX_COMMAND_SIZE);
      return;
    }

    strlcpy(command_line.function_name, command, first_space - command + 1);
    strlcpy(command_line.str_argument, first_space + 1, MAX_COMMAND_SIZE - (first_space - command) - 1);

    char *end;
    command_line.int_argument = strtol(command_line.str_argument, &end, 0); // Accepts 123 or 0xFF00FF

    if (*end) {
      command_line.argument_type = STRING;
    } else {
      command_line.argument_type = INT32;
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
      Serial.print(F("  "));
      Serial.print(commands[i].name);
      Serial.print(commands[i].doc);
      Serial.println(F("."));
    }
  }

  String getValue(String data, char separator, int index) {
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
      if (data.charAt(i) == separator || i == maxIndex) {
        found++;
        strIndex[0] = strIndex[1] + 1;
        strIndex[1] = (i == maxIndex) ? i + 1 : i;
      }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
  }

  /*
   * Tries to find the corresponding callback for a given command. Name and parameter type should fit.
   */
  void execute(const char *command_str) {
    String myString = String(command_str);
    String x_str = getValue(myString, ' ', 0);
    String op_str = getValue(myString, ' ', 1);
    String y_str = getValue(myString, ' ', 2);
    long x = x_str.toInt();
    long y = y_str.toInt();
    long result;
    if (op_str == "*") {
      Serial.println("Multiplication!");
      result = x * y;
    } else if (op_str == "+") {
      Serial.println("Addition!");
      result = x + y;
    } else if (op_str == "-") {
      Serial.println("Soustraction!");
      result = x - y;
    } else if (op_str == "/") {
      Serial.println("Division!");
      result = x / y;
    } else {
      Serial.println("Aucune idée. :(");
      return;
    }
    Serial.print(" = ");
    Serial.println(result);
    led_effects::showNumber(result);
  }
}
