# KLib

A library providing general abstractions of common Arduino functionality.


## General Usage

The library functionality is available upon including the main header file:

```
#include <KLib.h>
```

Doing this will not actually include (or provide) any functionality, as in order to be conservative with resources all modules are disabled by default. To include the functionality of a module, a `#define` is required before `#include <KLib.h>`. For example, to include the ALed LED functionality, your sketch should have:

```
#define KLIB_ALED
#include <KLib.h>
```

After which point the ALed struct and functionality will be available to you.


## Available Modules

Following are the modules provided by this library.


### ALed

This module provides LED functionality. It is enabled via:

```
#define KLIB_ALED
```


#### Function Overview

* `Setup(const byte pin)` - Initialise LED on specified pin
* `Update()` - Call in main loop to ensure non-blocking `Flash()` and `Blink()` (see below)
* `On()` - Turn LED on
* `Off()` - Turn LED off
* `Flash(const unsigned int interval, const unsigned int delay)` - After waiting `delay` milliseconds, toggle the LED, wait for `interval` milliseconds, and toggle the LED again. This is a one-time `Blink()` (see below). **Note**: Current implementation for `interval` and `delay` are limited to about 65 seconds as they are `unsigned int` values.
* `Blink(const unsigned int interval, const unsigned int delay)` - After waiting `delay` milliseconds, toggle the LED, wait for `interval` milliseconds, toggle the LED again, wait for `interval` milliseconds, toggle the LED again, … you get the picture. This is contrasted with `Pulse()` (see below) by it not waiting the `delay` duration for each *blink* - it is only used as an initial delay. **Note**: As with `Flash()`, the current implementation for `interval` and `delay` are limited to about 65 seconds as they are `unsigned int` values.
* `Pulse(const unsigned int interval, const unsigned int delay)` - After waiting `delay` milliseconds, toggle the LED, wait for `interval` milliseconds, toggle the LED again and restart sequence; that is wait for `delay` milliseconds, toggle the LED, wait for `interval` milliseconds, … you get the picture. **Note**: The current implementation for `interval` and `delay` in this are also limited to about 65 seconds as they are `unsigned int` values.


#### Notes

While there is no specific function to stop blinking or pulsing, you can stop it yourself by setting `blinking` to `false` as in:

```
ALed led;
led.Setup(13);
led.Blink(200, 0);      // Blink immediately - will toggle LED
delay(250);             // Wait a bit
led.Update();           // Interval passed - will toggle LED again
led.blinking = false;   // Disable manually
delay(250);             // Wait a bit
led.Update();           // Interval passed - LED unchanged (as no longer blinking)
```


#### Known Issues and TODOs

There is only a very naive validation of the pin provided to `Setup()` and unless you provide `0` the module will think it is connected to a legitimate LED. This could be improved.

Calling `On()` or `Off()` does not care if the LED is currently Blinking/Flashing/Pulsing so doing so may affect (invert) your programs concept of **On** and **Off**.

