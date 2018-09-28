#ifndef _ABUTTON_H_
#define _ABUTTON_H_

#include <Arduino.h>
#include "KLib.h"

#ifndef ABUTTON_MIN_DOWN
// Only register a button click if it is depressed for 50ms
#define ABUTTON_MIN_DOWN 50
#endif

struct AButton {
    // General configuration
    byte pin;
    bool enabled;
    void (*callback)();

    /**
     * Constructir - initialise struct data.
     */
    AButton()
    {
        pin      = 0;
        enabled  = false;
        callback = NULL;
    }

    /**
     * Setup - configure AButton.
     */
    void Setup(const byte buttonPin)
    {
        if (enabled)        return;    // Already setup
        if (buttonPin == 0) return;    // Invalid pin

        // @todo: More comprehensively ensure pin is within valid range

        pin = buttonPin;
        pinMode(pin, INPUT_PULLUP);

        enabled = true;
    }

    /**
     * Register - Provide callback function for AButton press handling.
     */
    void Register(void (*onPress)())
    {
        callback = onPress;
    }

    /**
     * Pressed - is AButton currently depressed?
     */
    bool Pressed()
    {
        if (!enabled) return false;

        return (digitalRead(pin) == HIGH)
            ? true
            : false;
    }

    /**
     * Update - check if AButton has completed a press action
     *          and if so fire callback (if configured to do so).
     */
    void Update()
    {
        if (!enabled)  return;
        if (!callback) return;

        static bool wasPressed = false;
        static unsigned long startPressed = 0;

        if (Pressed()) {
            if (!wasPressed) {
                wasPressed = true;
                startPressed = millis();
            }
        }
        else if (wasPressed) {
            if (KLIB_Elapsed(millis(), startPressed) >= ABUTTON_MIN_DOWN) {
                callback();
            }
            wasPressed = false;
        }
    }
};

#endif
