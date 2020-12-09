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
**  The system startup code initializes the processor's
**  registers and the application C variables.
**
** ------------------------------------------------------------
*/

#pragma nomisrac
#pragma profiling       off                     /* prevent profiling information on cstart      */
#pragma optimize        abcefgIJKlopRsUy        /* preset optimization level                    */
#pragma tradeoff        4                       /* preset tradeoff level                        */
#pragma runtime         BCMSZ                   /* disable runtime error checking for cstart    */
#pragma warning         750                     /* do not warn about unsaved registers          */
#pragma section         .text=cstart            /* use: .text.cstart as the section name        */

#include <stdlib.h>
#include <dbg.h>

#if !defined(__CPU_ARMV6M__) && !defined(__CPU_ARMV7M__) && !defined(__CPU_ARMV7EM__)

#define Mode_USR        0x10
#define Mode_FIQ        0x11
#define Mode_IRQ        0x12
#define Mode_SVC        0x13
#define Mode_ABT        0x17
#define Mode_UND        0x1B
#define Mode_SYS        0x1F

#define I_Bit           0x80    /* when I bit is set, IRQ is disabled   */
#define F_Bit           0x40    /* when F bit is set, FIQ is disabled   */
#ifdef __THUMB__
#define T_Bit           0x20    /* when T bit is set, we're in thumb mode       */
#else
#define T_Bit           0x00    /* when T bit is not set, we're in ARM mode     */
#endif

extern  unsigned char   _lc_ub_stack[];         /* usr/sys mode stack pointer   */
extern  unsigned char   _lc_ub_stack_fiq[];
extern  unsigned char   _lc_ub_stack_irq[];
extern  unsigned char   _lc_ub_stack_svc[];
extern  unsigned char   _lc_ub_stack_abt[];
extern  unsigned char   _lc_ub_stack_und[];

#pragma weak    exit
#pragma extern  _Exit
#pragma extern  main
extern  int     main( int argc, char *argv[] );
extern  void    __init_memory( void );
extern  void    SystemInit( void );
extern  void    __init( void );
#if (__BUILD__ < 651000000)
extern  void    __init_vector_table( void );
#endif
#if     __PROF_ENABLE__
extern  void    __prof_init( void );
#endif

#ifdef __POSIX_KERNEL__
//extern  void *  _posix_boot_stack_top;
extern  void     posix_main( void );
#endif
#ifdef __SWPLATFORM__
extern  void    framework_init( void );
#endif

extern int      _APPLICATION_MODE_;     /* symbol defined in LSL file   */

#ifdef  __USE_ARGC_ARGV
#ifndef __ARGCV_BUFSIZE
#define __ARGCV_BUFSIZE         256
#endif
static  char    argcv[__ARGCV_BUFSIZE];
#endif

int test = __VERSION__;

#if (__BUILD__ >= 651000000)
void    __interrupt() __frame() Reset_Handler( void )
#else
void    __interrupt() __frame() _START( void )
#endif
{
        /*
         *      Anticipate possible ROM/RAM remapping
         *      by loading the 'real' program address.
         */
        __remap_pc();

        /*
         *      Initialize stack pointer for each mode
         *      while keeping interrupts disabled
         */
        __setcpsr_c( Mode_FIQ | F_Bit | I_Bit | T_Bit);         /* change to FIQ mode                   */
        __setsp( _lc_ub_stack_fiq );                            /* initialize FIQ stack pointer         */

        __setcpsr_c( Mode_IRQ | F_Bit | I_Bit | T_Bit);         /* change to IRQ mode                   */
        __setsp( _lc_ub_stack_irq );                            /* initialize IRQ stack pointer         */

        __setcpsr_c( Mode_SVC | F_Bit | I_Bit | T_Bit);         /* change to Supervisor mode            */
        __setsp( _lc_ub_stack_svc );                            /* initialize SVC stack pointer         */

        __setcpsr_c( Mode_ABT | F_Bit | I_Bit | T_Bit);         /* change to Abort mode                 */
        __setsp( _lc_ub_stack_abt );                            /* initialize ABT stack pointer         */

        __setcpsr_c( Mode_UND | F_Bit | I_Bit | T_Bit);         /* change to Undefined mode             */
        __setsp( _lc_ub_stack_und );                            /* initialize UND stack pointer         */

        __setcpsr_c( Mode_SYS | F_Bit | I_Bit | T_Bit);         /* change to System mode                */
        __setsp( _lc_ub_stack );                                /* initialize USR/SYS stack pointer     */

        /*
         *      call a generated function which initializes external memory
         *      based on embedded project settings for memory configuration
         */
        __init_memory();

        /*
         *      Call a user function which initializes hardware,
         *      such as ROM/RAM re-mapping or MMU configuration.
         */
        SystemInit();
        
        /*
         *      Copy initialized sections from ROM to RAM
         *      and clear uninitialized data sections in RAM.
         */
        __init();
        __asm( "_cptable_handled:" );                                   /* symbol may be used by debugger       */

#if (__BUILD__ < 651000000)
        /*
         *      Initialize or copy the vector table.
         */
        __init_vector_table();
#endif
#ifdef __POSIX__
        /*
         * run always in svc mode with interrupts disable
         */
        __setcpsr_c( Mode_SVC | F_Bit | I_Bit | T_Bit);     
        /*
         * posix stack buffer for system upbringing
         */
//      __setsp( _posix_boot_stack_top );
#else
        /*
         * switch to user-defined application mode
         * as defined through a symbol in LSL file
         */
        __setcpsr_c( (int)&_APPLICATION_MODE_ | T_Bit);
#endif
#if  __PROF_ENABLE__
        __prof_init();
#endif
#if  __SWPLATFORM__
        framework_init();
#endif
#ifdef __POSIX_KERNEL__
        posix_main();
        /* should never end up here */
        exit( 0 );
#elif defined __USE_ARGC_ARGV
        exit( main( _argcv( argcv, __ARGCV_BUFSIZE ), (char **)argcv ) );
#else
        exit( main( 0, NULL ) );
#endif
        return;
}

#else /* !defined(__CPU_ARMV6M__) && !defined(__CPU_ARMV7M__) && !defined(__CPU_ARMV7EM__) */

extern  unsigned char   _lc_ub_stack[];

#pragma weak    exit
#pragma extern  _Exit
#pragma extern  main
extern  int     main( int argc, char *argv[] );
extern  void    __init_memory( void );
extern  void    SystemInit( void );
extern  void    __init( void );
#if (__BUILD__ < 651000000)
extern  void    __init_vector_table( void );
#endif
#if     __PROF_ENABLE__
extern  void    __prof_init( void );
#endif

#ifdef __POSIX_KERNEL__
//extern  void *  _posix_boot_stack_top;
extern  void     posix_main( void );
#endif
#ifdef __SWPLATFORM__
extern  void    framework_init( void );
#endif

#ifdef  __USE_ARGC_ARGV
#ifndef __ARGCV_BUFSIZE
#define __ARGCV_BUFSIZE         256
#endif
static  char    argcv[__ARGCV_BUFSIZE];
#endif

#if (__BUILD__ >= 651000000)
void    __interrupt() __frame() Reset_Handler( void )
#else
void    __interrupt() __frame() _START( void )
#endif
{

        /*
         *      Anticipate possible ROM/RAM remapping
         *      by loading the 'real' program address.
         */
        __remap_pc();
        /*
         *      Initialize stack pointer.
         */
        __setsp( _lc_ub_stack );
        /*
         *      call a generated function which initializes external memory
         *      based on embedded project settings for memory configuration
         */
        __init_memory();
        /*
         *      Call a user function which initializes hardware,
         *      such as ROM/RAM re-mapping or MMU configuration.
         */
        SystemInit();
        /*
         *      Copy initialized sections from ROM to RAM
         *      and clear uninitialized data sections in RAM.
         */
        __init();
        __asm( "_cptable_handled:" );                                   /* symbol may be used by debugger       */

#if (__BUILD__ < 651000000)
        /*
         *      Initialize or copy the vector table.
         */
        __init_vector_table();
#endif

#ifdef __POSIX__
//      __setsp( _posix_boot_stack_top );
#endif
#if  __PROF_ENABLE__
        __prof_init();
#endif
#if  __SWPLATFORM__
        framework_init();
#endif
#ifdef __POSIX_KERNEL__
        posix_main();
        /* should never end up here */
        exit( 0 );
#elif defined __USE_ARGC_ARGV
        exit( main( _argcv( argcv, __ARGCV_BUFSIZE ), (char **)argcv ) );
#else
        exit( main( 0, NULL ) );
#endif
        return;
}

#endif
