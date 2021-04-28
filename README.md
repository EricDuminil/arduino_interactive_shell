# Minimalist Arduino interactive shell

Small interactive shell via Serial interface, written in C++ for ESP32 and ESP8266, based on the command pattern.

This library makes it possible to define commands, and execute them directly over `Serial`:

```
Console is ready!
> test
'test' not supported. Available commands :
  double 123 (Doubles value).
  led 1/0 (LED on/off).
  reset (restarts the microcontroller).
> led 1
Calling : led(1)
> led 0
Calling : led(0)
> double 12345 
Calling : double(12345)
12345 * 2 = 24690
> double
'double' not supported. Available commands :
  double 123 (Doubles value).
  led 1/0 (LED on/off).
  reset (restarts the microcontroller).
> reset
Calling : reset()
```

Commands could also be sent via web interface or MQTT.

The corresponding libraries just need to `#include 'command_invoker.h'`, and can be independent from each others.

## Hardware Requirements

* [ESP8266](https://en.wikipedia.org/wiki/ESP8266) or [ESP32](https://en.wikipedia.org/wiki/ESP32) microcontroller (this project has been tested with *ESP8266 ESP-12 WIFI* and *TTGO ESP32 SX1276 LoRa*)

## Software Requirements

* [PlatformIO](https://platformio.org/)
or
* [Arduino IDE](https://www.arduino.cc/en/software)

## Installation


### PlatformIO

PlatformIO can be run from [VSCODE](https://platformio.org/install/ide?install=vscode), [Eclipse CDT](https://www.eclipse.org/cdt/) or console:

```bash
make upload board=esp8266 && make monitor # For ESP8266
```
```bash
make upload board=esp32 && make monitor # For ESP32
```

### Arduino IDE

* Add your board to the [board manager](https://github.com/esp8266/Arduino#installing-with-boards-manager). Either ESP8266:

        http://arduino.esp8266.com/stable/package_esp8266com_index.json

  or ESP32:

        https://dl.espressif.com/dl/package_esp32_index.json

* Choose the correct board in *Tools > Board > ...*
* Choose the correct *Flash size* (e.g. "Flash Size : 4MB (1MB FS, OTA:~1019kB)" for *ESP8266 ESP-12 WIFI*)

* *Verify*
* *Upload*
* *Tools > Serial Monitor*


## Usage

Once the microcontroller runs, commands can be sent directly from Arduino IDE Serial Monitor or PlatformIO monitor.

If an unknown command is sent via Serial, a list of known commands will be displayed.

If a command expects an integer, trying to call it without argument will not run anything.

## Adding commands

It is possible to add new commands by including `'command_invoker.h'` and calling:

    command_invoker::defineCommand("command_name", voidFunctionName, F(" (documentation)"));
    command_invoker::defineIntCommand("command_name", intFunctionName, F(" parameter (documentation)"));

For example:

    sensor_console::defineCommand("free", util::showFreeSpace, F(" (Displays available heap space)"));
    sensor_console::defineIntCommand("set_time", ntp::setLocalTime, F(" 1618829570 (Sets time to the given UNIX time)"));


## Author

 * Eric Duminil

## License

Copyright © 2021, Eric Duminil

[GPLv3](https://choosealicense.com/licenses/gpl-3.0/)
