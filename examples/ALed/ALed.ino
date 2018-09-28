#include <ALed.h>

#define TEST_ON_OFF true
#define TEST_BLINK  true
#define TEST_PULSE  false
#define TEST_FLASH  false

// Use on-board LED
#define LED_PIN 13

ALed led;

void setup()
{
    led.Setup(LED_PIN);

    #if TEST_ON_OFF
    led.On();
    delay(500);
    led.Off();
    #endif

    #if TEST_BLINK
    led.Blink(200, 2000);
    #elif TEST_PULSE
    led.Pulse(200, 2000);
    #elif TEST_FLASH
    led.Flash(200, 2000);
    #endif
}

void loop()
{
    led.Update();
}
