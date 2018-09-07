#ifndef _ABUTTON_H_
#define _ABUTTON_H_

#include <Arduino.h>

#ifndef ABUTTON_MIN_DOWN
// Only register a button click if it is depressed for 50ms
#define ABUTTON_MIN_DOWN 50
#endif

struct AButton {
    // General configuration
    byte pin;
    bool enabled;
    void (*callback)();

    AButton()
    {
        pin = 0;
        enabled = false;
    }

    void Setup(const byte buttonPin)
    {
        if (enabled)        return;    // Already setup
        if (buttonPin == 0) return;    // Invalid pin

        // @todo: More comprehensively ensure pin is within valid range

        pin = buttonPin;
        pinMode(pin, INPUT_PULLUP);

        enabled = true;
    }

    void Register(void (*onPress)())
    {
        callback = onPress;
    }

    bool Pressed()
    {
        if (!enabled) return false;

        return (digitalRead(pin) == HIGH)
            ? true
            : false;
    }

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
            if (Elapsed(startPressed) >= ABUTTON_MIN_DOWN) {
                callback();
            }
            wasPressed = false;
        }
    }

private:
    unsigned long maxLong = -1lu;

    unsigned long Elapsed(const unsigned long lastCounter)
    {
        unsigned long endCounter = millis();

        return (lastCounter <= endCounter)
            ? (endCounter - lastCounter)
            : (maxLong - lastCounter + endCounter);
    }
};

#endif
