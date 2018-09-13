#ifndef _ASTEPPER_H_
#define _ASTEPPER_H_

#include <Arduino.h>
#include "KLib.h"

#define ASTEPPER_STEP_COUNT    8
#define ASTEPPER_DEFAULT_SPEED 12

struct AStepper;
static AStepper *astep_stepper; // Global pointer for interrupt access

struct AStepper {
    // General configuration
    byte pins[4];
    unsigned int stepsPerRev;
    bool enabled;

    // Motor state
    bool active;                // Is motor currently rotating

    // Rotating state
    bool stepForward;
    unsigned long stepDelay;
    unsigned long stepCounter;
    unsigned long stepsRemaining;

    /**
     * Constructor - Set AStepper as inactive.
     */
    AStepper()
    {
        active        = false;
        enabled       = false;
        astep_stepper = NULL;
    }

    /**
     * Setup - configure AStepper.
     */
    void Setup(const byte pinA, const byte pinB, const byte pinC, const byte pinD, const unsigned int stepsPerRevolution)
    {
        pins[0] = pinA;
        pins[1] = pinB;
        pins[2] = pinC;
        pins[3] = pinD;

        stepsPerRev = stepsPerRevolution;

        // Validate configuration
        if (!stepsPerRev) return;

        for (byte pin = 0; pin < 4; pin++) {
            if (!pins[pin]) return;
        }

        // Initialise pins
        for (byte pin = 0; pin < 4; pin++) {
            pinMode(pins[pin], OUTPUT);
            digitalWrite(pins[pin], 0);
        }

        enabled = true;
    }

    /**
     * Automate - initialise self-timed interrupt for rotation.
     */
    void Automate()
    {
        if (astep_stepper) return;

        // Setup a TIMER0_COMPA interrupt
        // Will fire once a millisecond,
        // whenever the counter value passes 0xAF
        OCR0A = 0xAF;
        TIMSK0 |= _BV(OCIE0A);
        astep_stepper = this;
    }

    /**
     * Rotate - trigger AStepper to start rotating the specified number of
     *          revolutions, in the specified direction, at the specified speed.
     */
    void Rotate(const float revolutions, const bool forward, const unsigned int speed = 0)
    {
        stepCounter = 0;
        stepForward = forward;
        stepsRemaining = revolutions * stepsPerRev;
        stepDelay = 60L * 1000L * 1000L / stepsPerRev / (speed ? speed : ASTEPPER_DEFAULT_SPEED);

        active = true;

        Update();
    }

    /**
     * Stop - immediately stop rotating and disable AStepper.
     */
    void Stop()
    {
        if (!enabled) return;
        if (!active)  return;

        active = false;

        for (byte pin = 0; pin < 4; pin++) {
            digitalWrite(pins[pin], 0);
        }
    }

    /**
     * Update - check if AStepper should move and if so do.
     */
    void Update()
    {
        if (!enabled) return;
        if (!active)  return;

        if (!stepsRemaining) {
            Stop();
            return;
        }

        if (KLIB_Elapsed(micros(), stepCounter) >= stepDelay) {
            // Time for next step
            stepCounter = micros();
            Step(stepForward);
            stepsRemaining--;
        }
    }

private:
    /**
     * Step - move AStepper one step in the specified direction.
     */
    void Step(const bool forward)
    {
        if (!enabled) return;

        // 1st 4-bits, counter-clockwise pins; 2nd 4-bits, clockwise pins
        static const byte stepMap[8] = {0x11, 0x93, 0x82, 0xc6, 0x44, 0x6c, 0x28, 0x39};

        static byte currentStep = 0;

        byte pinMap = (forward ? stepMap[currentStep] : (stepMap[currentStep] >> 4));

        for (byte pin = 0; pin < 4; pin++) {
            // Set pins correctly according to step being performed
            const byte mask = bit(pin);
            digitalWrite(pins[pin], ((pinMap & mask) == mask ? true : false));
        }

        if (++currentStep >= ASTEPPER_STEP_COUNT) {
            currentStep = 0;
        }
    }
};

/**
 * Timer interrupt handler.
 */
SIGNAL(TIMER0_COMPA_vect)
{
    if (astep_stepper) {
        astep_stepper->Update();
    }
}

#endif
