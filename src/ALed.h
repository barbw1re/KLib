#ifndef _KLIB_ALED_H_
#define _KLIB_ALED_H_

#include <Arduino.h>
#include "KLib.h"

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
     * Constructor - initialise core values.
     */
    ALed()
    {
        pin      = 0;
        enabled  = false;
        blinking = false;
    }

    /**
     * Setup - configure ALed.
     */
    void Setup(const byte ledPin)
    {
        if (enabled)     return;    // Already setup
        if (ledPin == 0) return;    // Invalid pin

        // @todo: More comprehensively ensure pin is within valid range

        pin = ledPin;
        pinMode(pin, OUTPUT);

        digitalWrite(pin, LOW);
        active = false;

        enabled = true;
    }

    /**
     * Update - check if ALed should toggle and do so if required.
     */
    void Update()
    {
        if (!enabled)  return;
        if (!blinking) return;

        unsigned long elapsed = KLIB_Elapsed(millis(), blinkStarted);

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

    /**
     * On - turn ALed on.
     */
    void On()
    {
        // @todo: Decide (and implement) how this should behave if mid-blink

        if (enabled && !active) {
            digitalWrite(pin, HIGH);
            active = true;
        }
    }

    /**
     * Off - turn ALed off.
     */
    void Off()
    {
        // @todo: Decide (and implement) how this should behave if mid-blink

        if (enabled && active) {
            digitalWrite(pin, LOW);
            active = false;
        }
    }

    /**
     * Blink - initiate a blink of ALed.
     */
    void Blink(const unsigned int interval, const unsigned int delay)
    {
        if (!enabled)  return;
        if (blinking)  return;
        if (!interval) return;

        blinking = true;
        flashing = false;

        blinkPulse    = 0;
        blinkDelay    = delay;
        blinkInterval = interval;
        blinkStarted  = millis();

        if (delay == 0) {
            Toggle();
        }
    }

    /**
     * Pulse - initiate pulse-flashing of ALed.
     */
    void Pulse(const unsigned int interval, const unsigned int delay)
    {
        Blink(interval, delay);
        blinkPulse = delay;
    }

    /**
     * Flash - initiate flashing of ALed.
     */
    void Flash(const unsigned int interval, const unsigned int delay)
    {
        Blink(interval, delay);
        flashing = true;
    }

private:
    /**
     * Toggle - Switch ALed between on and off.
     */
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
};

#endif
