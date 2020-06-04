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

#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <util_string.h>

////////////////////////////////////////////////////////////////////////////////
int __strnicmp(const char * cs,const char * ct, size_t count)
{
  register signed char __res = 0;

  while (count) {
    if ((__res = toupper( *cs ) - toupper( *ct++ ) ) != 0 || !*cs++)
      break;
    count--;
  }

  return __res;
}

////////////////////////////////////////////////////////////////////////////////
int __stricmp(const char * cs,const char * ct)
{
  register signed char __res = 0;

  while (1) {
    if ((__res = toupper( *cs ) - toupper( *ct++ )) != 0 || !*cs++)
      break;
  }

  return __res;
}

////////////////////////////////////////////////////////////////////////////////
char *__strnstr(const char *cs, const char *ct, size_t n)
{
    const char *search;
    const char *search2;
    const char *start = cs;

    if (*ct)
    {
        for ( ; cs - start < n; cs++ )
        {
            for (search=ct, search2=cs; *search && *search==*search2; ++search, ++search2);               /* look if string ct is in cs   */
            if (!*search)          /* if found the string in ct,   */
            {
                return( (char *)cs );   /* return pointer       */
            }
        }
    }
    else
    {
        return( (char *)cs );
    }
    return NULL;                 /* no matching string found     */
}

////////////////////////////////////////////////////////////////////////////////
char *__strdup(const char *s)
{
    char *result;

    result = (char *)malloc(strlen(s) + 1);
    if (result != NULL)
    {
        strcpy(result, s);
    }

    return result;
}

