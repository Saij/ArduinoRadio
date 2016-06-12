// Enable debug system
// Comment to disable debug system
#define DEBUG

#ifdef DEBUG

#include <stdarg.h>
#define PRINTF_BUF 80

void setupDebug();

void debugPrintf(const char *format, ...);

#ifdef F // check to see if F() macro is available
void debugPrintf(const __FlashStringHelper *format, ...);
#endif // F

#else // DEBUG

#define setupDebug()
#define debugPrintf(format, ...)

#endif // DEBUG
