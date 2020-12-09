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
**  Macro definition to handle debugging output and
**  asserts. Define __SSAS_DEBUG__ to enable this
**  functionality.
**
** ------------------------------------------------------------
*/

#ifndef __UTIL_DEBUG_H
#define __UTIL_DEBUG_H

#ifdef __SSAS_DEBUG__

#include <stdio.h>
#include <stdlib.h>

#define ASSERT(x)               \
    {                           \
        if (!(x))               \
        {                       \
            printf( "\nAssertion '%s' failed in %s at line %d\n", #x, __FILE__, __LINE__); \
            abort();            \
        }                       \
    }

#define debug_printf            printf

#else

#define ASSERT(x)               do {} while (0)
#define debug_printf(...)       do {} while (0)

#endif /* __SSAS_DEBUG__ */

#endif /* __UTIL_DEBUG_H */

