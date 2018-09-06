#ifndef _ALED_H_
#define _ALED_H_

#ifndef KLIB_ALED
#define ALED_SETUP()
#define ALED_LOOP()
#else

#include <Arduino.h>

ALed led;

void ALED_SETUP()
{
    led.Setup(LED_BUILTIN);     // Use on-board LED
    led.Flash(200, 5000);       // After 5 seconds, blink one time for 200ms
}

void ALED_LOOP()
{
    led.Update();
    if (!led.blinking) {
        // Start continuous blinking
        led.Blink(200, 1000);   // After 1 second, blink for 200ms
    }
}

#endif

#endif
