#include <stdarg.h>
#include <stdio.h> /* vsnprintf */
#include <string.h>

/*
 * printf:
 *   Invokes OCALL to display the enclave buffer to the terminal.
 */
int display_message(const char* fmt, ...)
{
    char buf[BUFSIZ] = { '\0' };
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, BUFSIZ, fmt, ap);
    va_end(ap);
    return (int)strnlen(buf, BUFSIZ - 1) + 1;
}
