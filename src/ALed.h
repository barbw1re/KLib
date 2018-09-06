#ifndef _KLIB_ALED_H_
#define _KLIB_ALED_H_

#include <Arduino.h>

struct ALed {
    // General configuration
    byte pin;
    bool enabled;

    // Current state
    bool active;
    bool blinking;
    bool flashing;                  // This is a one-time blink

    // Blink data
    unsigned int blinkDelay;        // Number of milliseconds to wait before blinking
    unsigned int blinkPulse;        // Number of milliseconds to wait between blinking (for pulse mode)
    unsigned int blinkInterval;     // Number of milliseconds to blink
    unsigned long blinkStarted;     // Value of millis() when blink started

    /**
     * Constructor - initialise some values
     */
    ALed()
    {
        pin = 0;
        enabled = false;
        blinking = false;
    }

    void Setup(const byte ledPin)
    {
        if (active)      return;    // Already setup
        if (ledPin == 0) return;    // Invalid pin

        // @todo: More comprehensively ensure pin is within valid range

        pin = ledPin;
        pinMode(pin, OUTPUT);

        digitalWrite(pin, LOW);

        active  = false;
        enabled = true;
    }

    void Update()
    {
        if (!enabled)  return;
        if (!blinking) return;

        unsigned long elapsed = Elapsed(blinkStarted);

        if (blinkDelay > 0) {
            if (elapsed >= blinkDelay) {
                blinkDelay = 0;
                blinkStarted = millis();
                Toggle();
            }
            return;
        }
        if (elapsed < blinkInterval) {
            return;
        }

        // Blink interval completed
        Toggle();

        if (flashing) {
            blinking = false;
            return;
        }

        // Reset blinking
        blinkDelay = blinkPulse;
        blinkStarted = millis();
    }

    void On()
    {
        // @todo: Decide (and implement) how this should behave if mid-blink

        if (enabled && !active) {
            digitalWrite(pin, HIGH);
            active = true;
        }
    }

    void Off()
    {
        // @todo: Decide (and implement) how this should behave if mid-blink

        if (enabled && active) {
            digitalWrite(pin, LOW);
            active = false;
        }
    }

    void Blink(const unsigned int interval, const unsigned int delay)
    {
        if (!enabled)  return;
        if (blinking)  return;
        if (!interval) return;

        blinking = true;
        flashing = false;

        blinkPulse = 0;
        blinkDelay = delay;
        blinkInterval = interval;
        blinkStarted = millis();

        if (delay == 0) {
            Toggle();
        }
    }

    void Pulse(const unsigned int interval, const unsigned int delay)
    {
        Blink(interval, delay);
        blinkPulse = delay;
    }

    void Flash(const unsigned int interval, const unsigned int delay)
    {
        Blink(interval, delay);
        flashing = true;
    }

private:
    unsigned long maxLong = -1lu;

    void Toggle()
    {
        if (!enabled) return;

        if (active) {
            Off();
        }
        else {
            On();
        }
    }

    unsigned long Elapsed(const unsigned long lastCounter)
    {
        unsigned long endCounter = millis();

        return (lastCounter <= endCounter)
            ? (endCounter - lastCounter)
            : (maxLong - lastCounter + endCounter);
    }
};

#endif
