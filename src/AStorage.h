#ifndef _ASTORAGE_H_
#define _ASTORAGE_H_

#include <Arduino.h>
#include <EEPROM.h>

struct AStorage {
    // EEPROM capacity
    unsigned int capacity;

    /**
     * Constructor - initialise AStorage capacity.
     */
    AStorage()
    {
        capacity = EEPROM.length();
    }

    /**
     * Read - copy the EEPROM contents to data.
     */
    void Read(void *data, unsigned int size)
    {
        if (size == 0) return;

        byte *bdata = (byte *) data;

        // Ensure we don't try to read more than we have available
        for (unsigned int i = 0; size > 0 && i < capacity; i++, size--) {
            *bdata++ = EEPROM.read(i);
        }

        // Zero out all reamaining space
        while (size-- > 0) {
            *bdata++ = '\0';
        }
    }

    /**
     * Write - copy the data to the EEPROM.
     */
    void Write(const void *data, unsigned int size)
    {
        if (size == 0) return;

        byte *bdata = (byte *) data;

        // Ensure we don't try to write more than we have available
        for (unsigned int i = 0; size > 0 && i < capacity; i++, size--) {
            EEPROM.update(i, (byte) *bdata++);
        }
    }
};

#endif
