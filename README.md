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


    Console is ready!
    > double 456
    Calling : double(456)
    456 * 2 = 912
    > double
    'double' not supported. Available commands :
      double 123 (Doubles the input value).
      led 1/0 (LED on/off).
      reset (restarts the microcontroller).
    > led 0
    Calling : led(0)
    > reset
    Calling : reset()
    ets Jun  8 2016 00:22:57

    rst:0xc (SW_CPU_RESET),boot:0x17 (SPI_FAST_FLASH_BOOT)
    configsip: 0, SPIWP:0xee
    clk_drv:0x00,q_drv:0x00,d_drv:0x00,cs0_drv:0x00,hd_drv:0x00,wp_drv:0x00
    mode:DIO, clock div:2
    load:0x3fff0018,len:4
    load:0x3fff001c,len:1044
    load:0x40078000,len:8896
    load:0x40080400,len:5828
    entry 0x400806ac
    Calling : led(1)
    Console is ready!
    >

Commands can also be run directly from C++ code, e.g. with string literals:

    command_invoker::execute("led 1");

or from a webserver query string (be careful!):

    void handleWebServerCommand() {
      if (!shouldBeAllowed()) {
        return http.requestAuthentication(DIGEST_AUTH);
      }
      http.sendHeader("Location", "/");
      http.send(303);
      command_invoker::execute(http.arg("send").c_str());
    }

## Adding commands

It is possible to add new commands by including `'command_invoker.h'` and calling:

    command_invoker::defineCommand("command_name", voidFunctionName, F(" (documentation)"));
    command_invoker::defineIntCommand("command_name", intFunctionName, F(" parameter (documentation)"));

For example:

    sensor_console::defineCommand("free", util::showFreeSpace, F(" (Displays available heap space)"));
    sensor_console::defineIntCommand("set_time", ntp::setLocalTime, F(" 1618829570 (Sets time to the given UNIX time)"));

## License

Copyright © 2021, Eric Duminil

[GPLv3](https://choosealicense.com/licenses/gpl-3.0/)
