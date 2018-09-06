# KLib

A library providing general abstractions of common Arduino functionality.


## General Usage

The library functionality is available upon including the main header file:

```
#include <KLib.h>
```

Doing this will not actually include (or provide) any functionality, as in order to be conservative with resources all modules are disabled by default. To include the functionality of a module, a `#define` is required before `#include <KLib.h>`. For example, to include the ALed LED functionality, your sketch should have:

```
#define KLIB_ALED
#include <KLib.h>
```

After which point the ALed struct and functionality will be available to you.


## Available Modules

Following are the modules provided by this library.

