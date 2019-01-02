/*! udunits_snprintf class for use with Visual Studio.

  MSVC versions less than 1900 do not implement snprintf, so we must provide our
  own.

  The following is adapted from:

  http://stackoverflow.com/questions/2915672/snprintf-and-visual-studio-2010
*/

#include "udunits2.h"

int udunits_snprintf(char* str, size_t size, const char* format, ...)
{
    int count;
    va_list ap;

    va_start(ap, format);
    count = udunits_vsnprintf(str, size, format, ap);
    va_end(ap);

    return count;
}

int udunits_vsnprintf(char* str, size_t size, const char* format, va_list ap)
{
    int count = -1;

    if (size != 0)
        count = _vsnprintf_s(str, size, _TRUNCATE, format, ap);
    if (count == -1)
        count = _vscprintf(format, ap);

    return count;
}
