#include <Arduino.h>

#define KLIB_ASERVO
#include <KLib.h>

AServo servo;
unsigned long timer;

void setup()
{
    // Initialise servo on pin 9.
    // Valid range is from 15-degrees to 165-degrees.
    // Will reset motor to 0-degrees position (15-degrees).
    servo.Setup(9, 15, 165);

    timer = millis();   // Start the timer
}

void loop()
{
    static bool reset = false;

    const unsigned long now = millis();
    if (KLIB_Elapsed(now, timer) >= 1000) {
        // Time to move
        if (reset) {
            servo.Reset();
        }
        else {
            reset = !servo.Forward();
        }
        timer = now;
    }
}
