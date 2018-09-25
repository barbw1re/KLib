#ifndef _ASERVO_H_
#define _ASERVO_H_

#include <Arduino.h>
#include <Servo.h>

// Degree range
#define ASERVO_MIN  0
#define ASERVO_MAX  180

struct AServo {
    byte pin;
    Servo servo;
    bool enabled;

    unsigned int position;

    /**
     * Constructor - set AServo as inactive
     */
    AServo()
    {
        position = ASERVO_MAX + 1;
        enabled  = false;
    }

    /**
     * Setup - initialise servo motor
     */
    void Setup(const byte servoPin)
    {
        if (enabled) return;

        pin = servoPin;
        servo.attach(pin);
        enabled = true;

        Reset();
    }

    /**
     * Reset - move to 0-degrees position.
     */
    void Reset()
    {
        if (!enabled)      return;
        if (position == 0) return;

        MoveTo(0);
    }

    /**
     * Forwards - move 1 degree forwards.
     */
    bool Forward()
    {
        if (!enabled)               return false;
        if (position >= ASERVO_MAX) return false;

        MoveTo(position + 1);

        return (position < ASERVO_MAX);
    }

    /**
     * Backwards - move 1 degree backwards.
     */
    bool Backward()
    {
        if (!enabled)               return false;
        if (position <= ASERVO_MIN) return false;

        MoveTo(position - 1);

        return (position > ASERVO_MIN);
    }

    /**
     * MoveTo - move to specified position (and store new position).
     */
    void MoveTo(unsigned int degrees)
    {
        if (!enabled) return;

        if (degrees <= ASERVO_MIN) {
            degrees = ASERVO_MIN;
        }
        else if (degrees >= ASERVO_MAX) {
            degrees = ASERVO_MAX;
        }

        position = degrees;
        servo.write(position);
    }
};

#endif
