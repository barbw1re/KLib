#include <Arduino.h>

#define KLIB_ASERVO
#include <KLib.h>

AServo servo;
unsigned long timer;

void setup()
{
    servo.Setup(9);     // Initialise servo on pin 9.
                        // Will also reset motor to 0-degrees position

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
