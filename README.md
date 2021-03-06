# KLib [![pipeline status](http://git.kdjonline.com/arduino/KLib/badges/master/pipeline.svg)](http://git.kdjonline.com/arduino/KLib/commits/master)

<img src="http://git.kdjonline.com/arduino/KLib/raw/klib-meta/KLib-Icon.png" align="right" />

A library providing general abstractions of common Arduino functionality.

Current list of modules:
* [AButton](#abutton)
* [ADebug](#adebug-module)
* [ALed](#aled)
* [AStepper](#astepper)
* [AStorage](#astorage)


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

Alternatively, you may selectively include the modules you wish to use. For example to include the ALed and AButton modules, no `#define` directives are required, simply:

```
#include <ALed.h>
#include <AButton.h> 
```

**Note**: For the ADebug module, you will still need to use the `#define KLIB_ADEBUG` to enable debugging, even if you explicitly `#include <ADebug.h>`.


## ADebug Module

Debugging assistance is provided by the ADebug module. The functionality is available as a selection of macros which allows the module to be enabled and disabled by means of a `#define`. This means that when you want to disable debugging you do not need to remove a bunch of `Serial.print()` calls from you sketch. It is enabled by placing before the `#include <ADebug.h>` or `#include <KLib.h>`:

```
#define KLIB_ADEBUG
```


### Debug Macros

The primary behaviour of the macros are to output messages through the Serial Monitor. They are:

* `ADEBUG_SETUP(baud)` - Initialise the ADebug module and specify the Serial baud rate.
* `ADEBUG_ASSERT(condition)` - If condition fails (is false), an error message will be output including the offending file name and line number.
* `ADEBUG_WRITE(message)` - Write a fixed message to the serial monitor. This will automatically append a new-line.
* `ADEBUG_PRINT(format, ...)` - Write a message to the serial monitor using `printf()` format. This will automatically append a new-line.
* `ADEBUG_START_BLOCK()` - Start a timer to profile a block of code and push onto a timing stack. This timer uses `micros()` so is more granular than `millis()`.
* `ADEBUG_END_BLOCK(name)` - End a timer block and print out the provided name and the time spent inside the block.


### Notes

**Important**: Calling these macros should be performed as though they were functions so terminate them with a semi-colon!

The `ADEBUG_START_BLOCK()` and `ADEBUG_END_BLOCK()` are stack-based, so care should be taken to ensure all child blocks are ended before trying to end the parent block.

The macros `ADEBUG_WRITE()` and `ADEBUG_END_BLOCK()` expect a `__FlashStringHelper` be provided to them so you should use the `F()` macro when calling them, such as:

```
ADEBUG_WRITE(F("Started Blink Code"));
```


### Performance Impact

It should be noted that triggering `ADEBUG_ASSERT()` will consume anywhere from 50 - 100ms to generate and output the error message. This should not be a concern, as if `ADEBUG_ASSERT()` is triggered, it should be an indication of an issue with the sketch, so the goal should be to never have an `ADEBUG_ASSERT()` fail.

**Still To Do**: Profile the other functions (especially `ADEBUG_PRINT()` which I suspect is pretty slow) to understand their performance impact.


### Resources Used

Enabling ADebug will incur the following resource costs:

* Program storage: 2,408 bytes (about 7.5% of 32,256 bytes on Arduino Uno)
* Dynamic memory: 271 bytes (about 13% of 2,048 bytes on Arduino Uno)


Dynamic memory can be reduced by tuning buffer sizes. This can be done by adding either or both of the following `#define` lines to your sketch (before `#include <ADebug.h>` or `#include <KLib.h>`):

```
#define ADEBUG_MAX_LOG_LENGTH  150
#define ADEBUG_MAX_BLOCK_DEPTH 10
```

`ADEBUG_MAX_LOG_LENGTH` defines the size of the string buffer used by `ADEBUG_PRINT()`. If you do not use this macro, feel free to set to `1` (I would not set to 0 or anything crazy like -1).

`ADEBUG_MAX_BLOCK_DEPTH` defines the code block maximum stack depth. Each entry is a `long` taking 4-bytes so if you don't nest your `ADEBUG_START_BLOCK()` and `ADEBUG_END_BLOCK()` calls (or don't use them at all) feel free to reduce this to `1` (again, I would not recommend setting to a value less than 1).


## Available Modules

Following are the modules provided by this library.


### AButton

This module provides momentary push-button functionality. It is enabled via:

```
#define KLIB_ABUTTON
```


#### Function Overview

* `Setup(const byte buttonPin)` - Initialise button on specified pin.
* `Register(void (*onPress)())` - Register a callback to be called when button is pressed.
* `Pressed()` - Returns a boolean indicating whether the button is current depressed or not.
* `Update()` - Call in main loop to ensure non-blocking button checking


#### Notes

To improve consistency in identifying button presses, the button signal will be "debounced" and is only identified as a legitimate press when the button is held down for at least 50ms. This value may be modified by adding the following `#define` (before `#include <AButton.h>` or `#include <KLib.h>`):

```
// Reduce debounce threshold to 10ms
#define ABUTTON_MIN_DOWN 10
```


### ALed

This module provides LED functionality. It is enabled via:

```
#define KLIB_ALED
```


#### Function Overview

* `Setup(const byte pin)` - Initialise LED on specified pin.
* `Update()` - Call in main loop to ensure non-blocking `Flash()`, `Blink()`, and `Pulse()` (see below).
* `On()` - Turn LED on.
* `Off()` - Turn LED off.
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


### AStepper

This module provides stepper motor control functionality. It is enabled via:

```
#define KLIB_ASTEPPER
```


#### Function Overview

* `Setup(const byte pinA, const byte pinB, const byte pinC, const byte pinD, const unsigned int stepsPerRevolution)` - Initialise stepper motor on specified pins configured for specified steps corresponding to a complete revolution.
* `Automate()` - Configure the module to maintain it's own timer for rotating the stepper motor. Calling this removes the requirement to regularly call `Update()` to ensure the non-blocking rotation occurs.
* `Rotate(const float revolutions, const bool forward = true, const unsigned int speed = 0)` - Start rotating the stepper motor (forward or backward according to `forward` parameter provided) at `speed` RPM (or a reasonable default if `0`), and continue for `revolutions` revolutions (a decimal float - for example `1.0` is a single revolution, `0.25` is a quarter revolution.
* `Stop()` - Stop stepper motor immediately.
* `Update()` - Call in main loop to ensure non-blocking motor rotation occurs (only needed when not using `Automate()` mode).


#### Notes

The stepper motor is configured to run in 8-step mode so if you are used to running in 4-step mode (a-la the standard `<Stepper.h>`) you might find your "Steps per Revolution" are only giving you 1/2 a revolution - just double it!


### AStorage

This module provides EEPROM storage (and retrieval) functionality. It is enabled via:

```
#define KLIB_ASTORAGE
```


#### Function Overview

* `Read(void *data, unsigned int size)` - Read `size` bytes from EEPROM storage into `*data`.
* `Write(const void *data, unsigned int size)` - Write `size` bytes (or as much of it that will fit) into EEPROM storage from `*data`.

You can also access the `capacity` member (unsigned int) to learn how many bytes of EEPROM storage is available.


#### Example

To populate an `int` from storage:

```
AStorage storage;
int i;
storage.Read(&i, sizeof(int));
```

To populate a whole struct:
```
struct MyData {
  int i;
  unsigned long s;
  char name[20];
};
AStorage storage;
MyData data;
storage.Read(&data, sizeof(MyData));
```


#### Notes

The `Read()` and `Write()` functions should be pretty safe (assuming you don't provide a size greater than what your data is pointing to) as the copying will stop once it has reached the specified size or the storage capacity.

**Note**: This means that you can get unexpected results if you change the value of `capacity` - so sensible behaviour is advised.

If you try to read more than the storage capacity, all additional data will be set to `'\0'`.

If you try to write data which is the same as the currently stored value, it will not perform the write (thus not impacting the limited write lifespan of the EEPROM) so there is no downside to writing a struct where only some of the values have changed.



#### Limitations and TODOs

Currently all `Read()` and `Write()` calls are performed against the start of the EEPROM storage so you are unable to do something like:

```
AStorage storage;
int a, b;
storage.Read(&a, sizeof(int));
storage.Read(&b, sizeof(int));
```

To achieve this you will need to create a struct containing ints `a` and `b`.

If an elegant way of specifying an offset presents itself (such as something like `storage.ReadFrom(100, &c, sizeof(int));`), it may be implemented.


## Versioning

We use [SemVer](http://semver.org/) for versioning. For the versions available, see the [tags on this repository](http://git.kdjonline.com/arduino/KLib/tags).


## Authors

* **Kris Johnson** - *Initial work* - [krisj](http://git.kdjonline.com/krisj)
See also the list of [contributors](http://git.kdjonline.com/arduino/KLib/graphs/master) who participated in this project.


## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.


## Acknowledgments

This would not be possible without the amazing work done by the whole [Arduino team](https://www.arduino.cc/).
