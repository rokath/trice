/*! \file triceTick.h
\brief trices for tool evaluation
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/


#ifndef TRICETICK_H_
#define TRICETICK_H_

#ifdef __cplusplus
extern "C" {
#endif

#if defined( __arm__ )       /* Defined by GNU C and RealView */ \
    || defined( __thumb__ )  /* Defined by GNU C and RealView in Thumb mode */ \
    || defined( _ARM )       /* Defined by ImageCraft C */ \
    || defined( _M_ARM )     /* Defined by Visual Studio */ \
    || defined( _M_ARMT )    /* Defined by Visual Studio in Thumb mode */ \
    || defined( __arm )      /* Defined by Diab */ \
    || defined( __ICCARM__ ) /* IAR */ \
	|| defined( __CC_ARM )   /* ARM's (RealView) compiler */ \
    || defined( __ARM__ )    /* TASKING VX ARM toolset C compiler */ \
    || defined( __CARM__ )   /* TASKING VX ARM toolset C compiler */ \
    || defined( __CPARM__ )  /* TASKING VX ARM toolset C++ compiler */
#define SYSTICKVAL32 (*(volatile uint32_t*)0xE000E018UL)
#else
#error "unknown architecture"
#endif


#define SYSTICKVAL16 ((uint16_t)SYSTICKVAL32)

#ifdef __cplusplus
}
#endif

#endif /* TRICETICK_H_ */
