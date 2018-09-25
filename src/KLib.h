#ifndef _KLIB_H_
#define _KLIB_H_

/**
 * Common helpers - maybe enable via #ifdef
 */

/**
 * KLIB_Elapsed - calculate and return the elapsed time between the two provided
 *                counters, accounting for unsigned long overflow.
 */
unsigned long KLIB_Elapsed(const unsigned long endCounter, const unsigned long lastCounter)
{
    static const unsigned long maxLong = -1lu;

    return (lastCounter <= endCounter)
        ? (endCounter - lastCounter)
        : (maxLong - lastCounter + endCounter);
}

// Always include ADebug
// The KLIB_ADEBUG define is used to enable/disable it
#include "ADebug.h"

#ifdef KLIB_ABUTTON
#include "AButton.h"
#endif

#ifdef KLIB_ALED
#include "ALed.h"
#endif

#ifdef KLIB_ASERVO
#include "AServo.h"
#endif

#ifdef KLIB_ASTEPPER
#include "AStepper.h"
#endif

#ifdef KLIB_ASTORAGE
#include "AStorage.h"
#endif

#endif
