#include <Arduino.h>
#include <AStepper.h>

#define ASTEPPER_AUTOMATE   // Comment out to use explicit Update() method
#define ALSO_BLINK          // Uncomment to also blink on-board LED (to demonstrate non-blocking)

bool forward;
AStepper motor;

#ifdef ALSO_BLINK
#include <ALed.h>
ALed led;
#endif

void setup()
{
    forward = true;

    #ifdef ALSO_BLINK
    led.Setup(13);          // On-board LED - pin 13
    led.Blink(500, 2000);   // Blink every 0.5 secs starting in 2 seconds
    #endif

    // Stepper motor on pins 8, 9, 10, 11
    //  Strides per rev = 64
    //  Gear ration = 64
    motor.Setup(11, 10, 9, 8, (64 * 64));

    #ifdef ASTEPPER_AUTOMATE
    motor.Automate();
    #endif

    // Rotate 2.5 revolutions at 6 RPM
    motor.Rotate(2.5, forward, 6);
}

void loop()
{
    #ifdef ALSO_BLINK
    led.Update();
    #endif

    #ifndef ASTEPPER_AUTOMATE
    motor.Update();
    #endif

    if (!motor.active) {
        forward = !forward;
        delay(2000);    // NOTE: This will stop LED from blinking for the duration
        motor.Rotate(2.5, forward); // Use default speed (6 RPM)
    }
}
