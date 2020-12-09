/* ------------------------------------------------------------
**
**  Copyright (c) 2013-2015 Altium Limited
**
**  This software is the proprietary, copyrighted property of
**  Altium Ltd. All Right Reserved.
**
**  SVN revision information:
**  $Rev: 14907 $:
**  $Date: 2015-01-19 13:30:51 +0100 (Mon, 19 Jan 2015) $:
**
** ------------------------------------------------------------
*/

#ifndef __STRING_UTIL_H
#define __STRING_UTIL_H

#include <stdio.h>
#include <stdint.h>

////////////////////////////////////////////////////////////////////////////////
#define strncasecmp     __strnicmp
#define strcasecmp      __stricmp
#define strnstr         __strnstr
#define strdup          __strdup

////////////////////////////////////////////////////////////////////////////////
extern int __strnicmp(const char * cs,const char * ct,size_t count);
extern int __stricmp(const char * cs,const char * ct);
extern char *__strnstr(const char *cs, const char *ct, size_t n);
extern char *__strdup(const char *s);



#endif // __STRING_UTIL_H
