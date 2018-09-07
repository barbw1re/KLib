#include <Arduino.h>    // For Serial notification

// Make sure you add defines to enable required modules before including KLib.h
#define KLIB_ABUTTON    // Comment out to disable AButton module
#define KLIB_ALED       // Comment out to disable ALed module
#define KLIB_ASTORAGE   // Comment out to disable AStorage module

// Comment out to disable ADebug module
// This does not require you to remove debug macros from your code
// but they will not be compiled into the build.
#define KLIB_ADEBUG
// You can also fine-tune the amount of dynamic memory used by ADebug
// by tweaking these follow defines (set to their default values).
//#define ADEBUG_MAX_LOG_LENGTH  150
//#define ADEBUG_MAX_BLOCK_DEPTH 10

#include <KLib.h>

#include "ALed.h"       // ALed example code

#ifdef KLIB_ABUTTON
const byte buttonPin = 12;
AButton button;
void pressed()
{
    Serial.println(F("Button Pressed!"));
}
#endif

void setup()
{
    ADEBUG_SETUP(9600);

    ADEBUG_START_BLOCK();

    #ifndef KLIB_ADEBUG
    Serial.begin(9600);
    #endif

    ADEBUG_START_BLOCK();
    ALED_SETUP();
    ADEBUG_END_BLOCK(F("LED Setup()"));

    #ifdef KLIB_ABUTTON
    ADEBUG_START_BLOCK();
    button.Setup(buttonPin);
    button.Register(&pressed);
    ADEBUG_END_BLOCK(F("Button Setup()"));
    #endif

    #ifdef KLIB_ASTORAGE
    AStorage storage;
    Serial.print(F("Available storage: "));
    Serial.print(storage.capacity);
    Serial.println(F(" bytes"));
    #endif

    ADEBUG_END_BLOCK(F("setup()"));
}

void loop()
{
    static int counter = 0;

    ADEBUG_START_BLOCK();

    ALED_LOOP();

    #ifdef KLIB_ABUTTON
    button.Update();
    #endif

    // Trigger assertion every 100 loops
    counter++;
    ADEBUG_ASSERT(counter > 100);
    if (counter > 100) {
        counter = 0;
    }

    ADEBUG_END_BLOCK(F("loop()"));
}
