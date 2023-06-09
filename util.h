#include "defs.h"
#include <sys/types.h>

#ifndef __UTIL_H__
#define __UTIL_H__

#ifndef _VA_LIST
typedef __builtin_va_list va_list;
#define _VA_LIST
#endif
#define va_start(ap, param) __builtin_va_start(ap, param)
#define va_end(ap) __builtin_va_end(ap)
#define va_arg(ap, type) __builtin_va_arg(ap, type)

/* GCC always defines __va_copy, but does not define va_copy unless in c99 mode
 * or -ansi is not specified, since it was not part of C90.
 */
#ifndef __va_copy
#define __va_copy(d, s) __builtin_va_copy(d, s)
#endif

#if __STDC_VERSION__ >= 199901L || __cplusplus >= 201103L ||                   \
    !defined(__STRICT_ANSI__)
#define va_copy(dest, src) __builtin_va_copy(dest, src)
#endif

#ifndef __GNUC_VA_LIST
#define __GNUC_VA_LIST 1
typedef __builtin_va_list __gnuc_va_list;
#endif

extern int pscanf(const char *path, const char *fmt, ...);
extern void exec_command(const char *command, char *buffer);

#endif
