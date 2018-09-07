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


## ADebug Module

Debugging assistance is provided by the ADebug module. The functionality is available as a selection of macros which allows the module to be enabled and disabled by means of a `#define`. This means that When you want to disable debugging you do not need to remove a bunch of `Serial.print()` calls from you sketch. It is enabled by placing before the `#include <ADebug.h>` or `#include <KLib.h>`:

```
#define KLIB_ADEBUG
```


### Debug Macros

The primary behaviour of the macros are to output messages through the Serial Monitor. They are:

* `ADEBUG_SETUP(baud)` - Initialise the ADebug module and specify the Serial baud rate.
* `ADEBUG_ASSERT(condition)` - If condition fails (is false), an error message will be output including the offending file name and line number.
* `ADEBUG_WRITE(message)` - Write a fixed message to the serial monitor. This will automatically append a new-line.
* `ADEBUG_PRINT(format, ...)` - Write a message to the serial monitor using `printf()` format. This will automatically append a new-line.
* `ADEBUG_START_BLOCK()` - Start a timer to profile a block of code and push onto timing stack. This timer uses `micros()` so is more granular than `millis()`.
* `ADEBUG_END_BLOCK(name)` - End a timer block and print out the provided name and the time inside the block.


### Notes

**Important**: Calling these macros should be performed as though they were functions so terminate them with a semi-colon!

The `ADEBUG_START_BLOCK()` and `ADEBUG_END_BLOCK()` are stack-based, so care should be taken to ensure all child blocks are ended before trying to end the parent block.

The macros `ADEBUG_WRITE()` and `ADEBUG_END_BLOCK()` expect a `__FlashStringHelper` be provided to them so you should use the `F()` macro when calling them, such as:

```
ADEBUG_WRITE(F("Started Blink Code"));
```


### Resources Used

Enabling ADebug will incur the following resource costs:

* Program storage: 2,408 bytes (about 7.5% of 32,256 bytes on Arduino Uno)
* Dynamic memory: 271 bytes (about 13% of 2,048 bytes on Arduino Uno)


Dynamic memory can be reduced by tuning buffer sizes. This can be done by adding either or both of the following `#include` lines to your sketch (before `#include <ADebug.h>` or `#include <KLib.h>`):

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

To improve consistency in identifying button presses, the button signal will be "debounced" and is only identified as a legitimate press when the button is held down for at least 50ms. This value may be modified by adding the following `#define` (before #including AButton.h or KLib.h):

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

**NOTE**: This means that you can get unexpected results if you change the value of `capacity` - so sensible behaviour is advised.

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
