// Make sure you include all enable module defines before
// including KLib.h
#define KLIB_ALED   // Comment out to disable ALed module

#include <KLib.h>
#include "ALed.h"

void setup()
{
    ALED_SETUP();
}

void loop()
{
    ALED_LOOP();
}
