#include <Arduino.h>    // For Serial notification

// Make sure you add defines to enable required modules before including KLib.h
#define KLIB_ALED       // Comment out to disable ALed module
#define KLIB_ASTORAGE   // Comment out to disable AStorage module

#include <KLib.h>

#include "ALed.h"       // ALed example code

void setup()
{
    Serial.begin(9600);

    ALED_SETUP();

    #ifdef KLIB_ASTORAGE
    AStorage storage;
    Serial.print(F("Available storage: "));
    Serial.print(storage.capacity);
    Serial.println(F(" bytes"));
    #endif
}

void loop()
{
    ALED_LOOP();
}
