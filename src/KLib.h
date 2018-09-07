#ifndef _KLIB_H_
#define _KLIB_H_

// Always include ADebug
// The KLIB_ADEBUG define is used to enable/disable it
#include "ADebug.h"

#ifdef KLIB_ABUTTON
#include "AButton.h"
#endif

#ifdef KLIB_ALED
#include "ALed.h"
#endif

#ifdef KLIB_ASTORAGE
#include "AStorage.h"
#endif

#endif
