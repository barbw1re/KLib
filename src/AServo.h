#ifndef _ASERVO_H_
#define _ASERVO_H_

#include <Arduino.h>
#include <Servo.h>

// Default degree range
#define ASERVO_MIN  0
#define ASERVO_MAX  180

struct AServo {
    // General configuration
    byte pin;
    Servo servo;
    bool enabled;

    // Acceptable rotation range
    unsigned int minPosition;
    unsigned int maxPosition;

    // Degree substitution map
    unsigned int *positionMap;

    // Current rotation state
    unsigned int position;

    /**
     * Constructor - set AServo as inactive
     */
    AServo()
    {
        enabled     = false;
        positionMap = NULL;
    }

    /**
     * Setup - initialise servo motor
     */
    void Setup(const byte servoPin, unsigned int min = ASERVO_MIN, unsigned int max = ASERVO_MAX)
    {
        if (enabled) return;

        if (max > ASERVO_MAX) max = ASERVO_MAX;
        if (min > max)        min = max;

        minPosition = min;
        maxPosition = max;

        pin = servoPin;
        servo.attach(pin);

        enabled = servo.attached();

        position = max;
        Reset();
    }

    /**
     * Reset - move to 0-degrees position.
     */
    void Reset()
    {
        if (!enabled)      return;
        if (position == 0) return;

        JumpTo(0);
    }

    /**
     * Forwards - move 1 degree forwards.
     */
    bool Forward()
    {
        if (!enabled)                return false;
        if (position >= maxPosition) return false;

        JumpTo(position + 1);

        return (position < maxPosition);
    }

    /**
     * Backwards - move 1 degree backwards.
     */
    bool Backward()
    {
        if (!enabled)                return false;
        if (position <= minPosition) return false;

        JumpTo(position - 1);

        return (position > minPosition);
    }

    /**
     * JumpTo - jump to specified position (and store new position).
     */
    void JumpTo(unsigned int degrees)
    {
        if (!enabled) return;

        if (degrees < minPosition) {
            degrees = minPosition;
        }
        else if (degrees > maxPosition) {
            degrees = maxPosition;
        }

        position = degrees;

        unsigned int servoPosition = position;
        if (positionMap && positionMap[position]) {
            servoPosition = positionMap[position];
        }

        servo.write(servoPosition);
    }

    /**
     * MoveTo - slowly move to specified position (and store new position)
     */
    void MoveTo(unsigned int degrees, const unsigned int speed)
    {
        if (!enabled) return;

        if (degrees < minPosition) {
            degrees = minPosition;
        }
        else if (degrees > maxPosition) {
            degrees = maxPosition;
        }

        if (degrees == position) return;

        // @TODO: Animate this without blocking

        while (position != degrees) {
            if (position < degrees) {
                Forward();
            }
            else {
                Backward();
            }
            delay(speed);
        }
    }
};

#endif
