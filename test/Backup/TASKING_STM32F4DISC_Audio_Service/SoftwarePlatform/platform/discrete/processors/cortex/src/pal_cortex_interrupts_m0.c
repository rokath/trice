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

#include <ctype.h>
#include <string.h>

#ifdef __CPU_ARMV6M__

#include <interrupts.h>

#include "pal.h"
#include "pal_arch_cfg.h"
#include "interrupts_cfg.h"


#if (__POSIX_KERNEL__)
static unsigned int _pal_cortex_saved_stackpointer;
#if ( POSIX_THREADS_DEBUG != 0 )
static unsigned int _pal_cortex_saved_resume_address;
#endif
#endif


#define ABC_INT_MASK(s) (1 << (s))

#define GENERATE_INTERRUPT_HANDLER( n )         ((INTERRUPTS_APPLICATION_MASK & ABC_INT_MASK( n )) == 0)

#if (__POSIX_KERNEL__)
#define __FRAME                                 __frame()
# if ( POSIX_THREADS_DEBUG != 0 )
#define INTERRUPT_HANDLER_BODY()                                                            \
    /* store context */                                                                     \
    /* bottom part already on the stack */                                                  \
    /* save align0 and pc */                                                                \
    /* save pc */                                                                           \
    __asm("push     {lr}");                                                                 \
    /* use align0 as a back up of the status bits                       */                  \
    /* (in case they where screwed up by incomplete context restore)    */                  \
    __asm("mrs      r0, apsr");                                                             \
    __asm("push     {r0}");                                                                 \
                                                                                            \
    /* top part of context and pc */                                                        \
    __asm("mov      r0, r11");                                                              \
    __asm("push     {r0}");                                                                 \
    __asm("mov      r0, r10");                                                              \
    __asm("push     {r0}");                                                                 \
    __asm("mov      r0, r9");                                                               \
    __asm("push     {r0}");                                                                 \
    __asm("mov      r0, r8");                                                               \
    __asm("push     {r0}");                                                                 \
    __asm("push     {r4, r5, r6, r7}");                                                     \
                                                                                            \
    /* store $sp */                                                                         \
    __asm("mov      r0, sp": : : "r0");                                                     \
    __asm("str      r0, [%0,#0]": : "r"(&_pal_cortex_saved_stackpointer) : "r0");           \
                                                                                            \
    /* store $ra in case this interrupt is a posix interrupt */                             \
    __asm("mov      r0, lr": : : "r0");                                                     \
    __asm("str      r0, [%0,#0]" : : "r"(&_pal_cortex_saved_resume_address): "r0");         \
                                                                                            \
    /* $sp to interrupt stack */                                                            \
    __asm("ldr      r0, [%0,#0]": : "r"(&_posix_interrupt_stack_top): "r0");                \
    __asm("mov      sp, r0");                                                               \
                                                                                            \
    cortex_m0_posix_standard_native();                                                      \
                                                                                            \
    /* load $sp */                                                                          \
    __asm("ldr      r0, [%0,#0]": : "r"(&_pal_cortex_saved_stackpointer) : "r0");           \
    __asm("mov      sp, r0");                                                               \
                                                                                            \
    /* restore context */                                                                   \
    /* top part of context, pc and auto magically bottom part */                            \
    __asm("pop      {r4, r5, r6, r7}");                                                     \
    __asm("pop      {r0}");                                                                 \
    __asm("mov      r8, r0");                                                               \
    __asm("pop      {r0}");                                                                 \
    __asm("mov      r9, r0");                                                               \
    __asm("pop      {r0}");                                                                 \
    __asm("mov      r10, r0");                                                              \
    __asm("pop      {r0}");                                                                 \
    __asm("mov      r11, r0");                                                              \
    /* load pc, adjust sp */                                                                \
    __asm("add      sp, sp, #4");                                                           \
    __asm("pop      {r0}");                                                                 \
    __asm("bx       r0");
# else
/* only difference with above is not save _pal_cortex_saved_resume_address */
#define INTERRUPT_HANDLER_BODY()                                                            \
    /* store context */                                                                     \
    /* bottom part already on the stack */                                                  \
    /* save align0 and pc */                                                                \
    /* save pc */                                                                           \
    __asm("push     {lr}");                                                                 \
    /* use align0 as a back up of the status bits                       */                  \
    /* (in case they where screwed up by incomplete context restore)    */                  \
    __asm("mrs      r0, apsr");                                                             \
    __asm("push     {r0}");                                                                 \
                                                                                            \
    /* top part of context and pc */                                                        \
    __asm("mov      r0, r11");                                                              \
    __asm("push     {r0}");                                                                 \
    __asm("mov      r0, r10");                                                              \
    __asm("push     {r0}");                                                                 \
    __asm("mov      r0, r9");                                                               \
    __asm("push     {r0}");                                                                 \
    __asm("mov      r0, r8");                                                               \
    __asm("push     {r0}");                                                                 \
    __asm("push     {r4, r5, r6, r7}");                                                     \
                                                                                            \
    /* store $sp */                                                                         \
    __asm("mov      r0, sp": : : "r0");                                                     \
    __asm("str      r0, [%0,#0]": : "r"(&_pal_cortex_saved_stackpointer) : "r0");           \
                                                                                            \
    /* $sp to interrupt stack */                                                            \
    __asm("ldr      r0, [%0,#0]": : "r"(&_posix_interrupt_stack_top): "r0");                \
    __asm("mov      sp, r0");                                                               \
                                                                                            \
    cortex_m0_posix_standard_native();                                                      \
                                                                                            \
    /* load $sp */                                                                          \
    __asm("ldr      r0, [%0,#0]": : "r"(&_pal_cortex_saved_stackpointer) : "r0");           \
    __asm("mov      sp, r0");                                                               \
                                                                                            \
    /* restore context */                                                                   \
    /* top part of context, pc and auto magically bottom part */                            \
    __asm("pop      {r4, r5, r6, r7}");                                                     \
    __asm("pop      {r0}");                                                                 \
    __asm("mov      r8, r0");                                                               \
    __asm("pop      {r0}");                                                                 \
    __asm("mov      r9, r0");                                                               \
    __asm("pop      {r0}");                                                                 \
    __asm("mov      r10, r0");                                                              \
    __asm("pop      {r0}");                                                                 \
    __asm("mov      r11, r0");                                                              \
    /* load pc, adjust sp */                                                                \
    __asm("add      sp, sp, #4");                                                           \
    __asm("pop      {r0}");                                                                 \
    __asm("bx       r0");
# endif
#else
#define __FRAME                                 
#define INTERRUPT_HANDLER_BODY()                                                              \
    uint32_t vectactive, number;                                                            \
    interrupt_entry_t   *entry, *next;                                                      \
                                                                                            \
    /* get current pending interrupt form Interrupt Control State Register  */              \
    /* bits [8-0], subtract 16 for int number                               */              \
                                                                                            \
    vectactive      = (SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk) >> SCB_ICSR_VECTACTIVE_Pos;     \
    if (vectactive < 16)                                                                    \
    {                                                                                       \
        /* internal interrupts */                                                           \
        assert(0);                                                                          \
    }                                                                                       \
    else                                                                                    \
    {                                                                                       \
        number = vectactive - 16;                                                           \
        entry = interrupt_hash_table[number % INTERRUPTS_HASH_SIZE];                        \
        while (entry != NULL)                                                               \
        {                                                                                   \
            next = entry->next;                                                             \
            if (entry->number == number)                                                    \
            {                                                                               \
                if (entry->is_native)                                                          \
                {                                                                           \
                    ((interrupt_native_handler_t)(entry->handler))();                         \
                    break;                                                                  \
                }                                                                           \
                else                                                                        \
                {                                                                           \
                    entry->handler(number, entry->context);                                 \
                }                                                                           \
            }                                                                               \
            entry = next;                                                                   \
        }                                                                                   \
        interrupt_acknowledge(number);                                                      \
    }                                                                                       \
    return;                                                                                 
#endif

#if (__POSIX_KERNEL__)
static void cortex_m0_posix_standard_native(void)
{
    int intnum;

    __asm("mrs      %0, ipsr" : "=r"(intnum));

    intnum -= 16;
    if (_posix_interrupts_bitmask & (1 << intnum))
    {
#if ( POSIX_THREADS_DEBUG != 0 )
        _posix_resume_address   = _pal_cortex_saved_resume_address;
        _posix_user_stack       = _pal_cortex_saved_stackpointer;
#endif
        /* save stack pointer */
        if (_posix_current_thread != NULL)
        {
            *(unsigned int *)_posix_current_thread = _pal_cortex_saved_stackpointer;
        }
        else
        {
            _posix_irq_sp = _pal_cortex_saved_stackpointer;
        }
        _posix_irq_level++;
        posix_interrupts_handler(intnum);
        _posix_irq_level--;

        /* get stack pointer */
        if (_posix_current_thread != NULL)
        {
            _pal_cortex_saved_stackpointer = *(unsigned int *)_posix_current_thread;
        }
        else
        {
            _pal_cortex_saved_stackpointer = _posix_irq_sp;
        }
    }
    else
    {
        interrupts_common_handler(intnum);
    }

    return;
}
#endif

/* suppress 'interrupt function "" does not save register "" warnings */
#pragma warning 750

/* avoid stack pops before function call */
#pragma optimize O0

#if GENERATE_INTERRUPT_HANDLER(0)
static void __interrupt(0+16) __FRAME pal_cortex_m0_interrupt_handler_0(void)
{
    INTERRUPT_HANDLER_BODY();
}
#endif

#if GENERATE_INTERRUPT_HANDLER(1)
static void __interrupt(1+16) __FRAME pal_cortex_m0_interrupt_handler_1(void)
{
    INTERRUPT_HANDLER_BODY();
}
#endif

#if GENERATE_INTERRUPT_HANDLER(2)
static void __interrupt(2+16) __FRAME pal_cortex_m0_interrupt_handler_2(void)
{
    INTERRUPT_HANDLER_BODY();
}
#endif

#if GENERATE_INTERRUPT_HANDLER(3)
static void __interrupt(3+16) __FRAME pal_cortex_m0_interrupt_handler_3(void)
{
    INTERRUPT_HANDLER_BODY();
}
#endif

#if GENERATE_INTERRUPT_HANDLER(4)
static void __interrupt(4+16) __FRAME pal_cortex_m0_interrupt_handler_4(void)
{
    INTERRUPT_HANDLER_BODY();
}
#endif

#if GENERATE_INTERRUPT_HANDLER(5)
static void __interrupt(5+16) __FRAME pal_cortex_m0_interrupt_handler_5(void)
{
    INTERRUPT_HANDLER_BODY();
}
#endif

#if GENERATE_INTERRUPT_HANDLER(6)
static void __interrupt(6+16) __FRAME pal_cortex_m0_interrupt_handler_6(void)
{
    INTERRUPT_HANDLER_BODY();
}
#endif

#if GENERATE_INTERRUPT_HANDLER(7)
static void __interrupt(7+16) __FRAME pal_cortex_m0_interrupt_handler_7(void)
{
    INTERRUPT_HANDLER_BODY();
}
#endif

#if GENERATE_INTERRUPT_HANDLER(8)
static void __interrupt(8+16) __FRAME pal_cortex_m0_interrupt_handler_8(void)
{
    INTERRUPT_HANDLER_BODY();
}
#endif

#if GENERATE_INTERRUPT_HANDLER(9)
static void __interrupt(9+16) __FRAME pal_cortex_m0_interrupt_handler_9(void)
{
    INTERRUPT_HANDLER_BODY();
}
#endif

#if GENERATE_INTERRUPT_HANDLER(10)
static void __interrupt(10+16) __FRAME pal_cortex_m0_interrupt_handler_10(void)
{
    INTERRUPT_HANDLER_BODY();
}
#endif

#if GENERATE_INTERRUPT_HANDLER(11)
static void __interrupt(11+16) __FRAME pal_cortex_m0_interrupt_handler_11(void)
{
    INTERRUPT_HANDLER_BODY();
}
#endif

#if GENERATE_INTERRUPT_HANDLER(12)
static void __interrupt(12+16) __FRAME pal_cortex_m0_interrupt_handler_12(void)
{
    INTERRUPT_HANDLER_BODY();
}
#endif

#if GENERATE_INTERRUPT_HANDLER(13)
static void __interrupt(13+16) __FRAME pal_cortex_m0_interrupt_handler_13(void)
{
    INTERRUPT_HANDLER_BODY();
}
#endif

#if GENERATE_INTERRUPT_HANDLER(14)
static void __interrupt(14+16) __FRAME pal_cortex_m0_interrupt_handler_14(void)
{
    INTERRUPT_HANDLER_BODY();
}
#endif

#if GENERATE_INTERRUPT_HANDLER(15)
static void __interrupt(15+16) __FRAME pal_cortex_m0_interrupt_handler_15(void)
{
    INTERRUPT_HANDLER_BODY();
}
#endif

#if GENERATE_INTERRUPT_HANDLER(16)
static void __interrupt(16+16) __FRAME pal_cortex_m0_interrupt_handler_16(void)
{
    INTERRUPT_HANDLER_BODY();
}
#endif

#if GENERATE_INTERRUPT_HANDLER(17)
static void __interrupt(17+16) __FRAME pal_cortex_m0_interrupt_handler_17(void)
{
    INTERRUPT_HANDLER_BODY();
}
#endif

#if GENERATE_INTERRUPT_HANDLER(18)
static void __interrupt(18+16) __FRAME pal_cortex_m0_interrupt_handler_18(void)
{
    INTERRUPT_HANDLER_BODY();
}
#endif

#if GENERATE_INTERRUPT_HANDLER(19)
static void __interrupt(19+16) __FRAME pal_cortex_m0_interrupt_handler_19(void)
{
    INTERRUPT_HANDLER_BODY();
}
#endif

#if GENERATE_INTERRUPT_HANDLER(20)
static void __interrupt(20+16) __FRAME pal_cortex_m0_interrupt_handler_20(void)
{
    INTERRUPT_HANDLER_BODY();
}
#endif

#if GENERATE_INTERRUPT_HANDLER(21)
static void __interrupt(21+16) __FRAME pal_cortex_m0_interrupt_handler_21(void)
{
    INTERRUPT_HANDLER_BODY();
}
#endif

#if GENERATE_INTERRUPT_HANDLER(22)
static void __interrupt(22+16) __FRAME pal_cortex_m0_interrupt_handler_22(void)
{
    INTERRUPT_HANDLER_BODY();
}
#endif

#if GENERATE_INTERRUPT_HANDLER(23)
static void __interrupt(23+16) __FRAME pal_cortex_m0_interrupt_handler_23(void)
{
    INTERRUPT_HANDLER_BODY();
}
#endif

#if GENERATE_INTERRUPT_HANDLER(24)
static void __interrupt(24+16) __FRAME pal_cortex_m0_interrupt_handler_24(void)
{
    INTERRUPT_HANDLER_BODY();
}
#endif

#if GENERATE_INTERRUPT_HANDLER(25)
static void __interrupt(25+16) __FRAME pal_cortex_m0_interrupt_handler_25(void)
{
    INTERRUPT_HANDLER_BODY();
}
#endif

#if GENERATE_INTERRUPT_HANDLER(26)
static void __interrupt(26+16) __FRAME pal_cortex_m0_interrupt_handler_26(void)
{
    INTERRUPT_HANDLER_BODY();
}
#endif

#if GENERATE_INTERRUPT_HANDLER(27)
static void __interrupt(27+16) __FRAME pal_cortex_m0_interrupt_handler_27(void)
{
    INTERRUPT_HANDLER_BODY();
}
#endif

#if GENERATE_INTERRUPT_HANDLER(28)
static void __interrupt(28+16) __FRAME pal_cortex_m0_interrupt_handler_28(void)
{
    INTERRUPT_HANDLER_BODY();
}
#endif

#if GENERATE_INTERRUPT_HANDLER(29)
static void __interrupt(29+16) __FRAME pal_cortex_m0_interrupt_handler_29(void)
{
    INTERRUPT_HANDLER_BODY();
}
#endif

#if GENERATE_INTERRUPT_HANDLER(30)
static void __interrupt(30+16) __FRAME pal_cortex_m0_interrupt_handler_30(void)
{
    INTERRUPT_HANDLER_BODY();
}
#endif

#if GENERATE_INTERRUPT_HANDLER(31)
static void __interrupt(31+16) __FRAME pal_cortex_m0_interrupt_handler_31(void)
{
    INTERRUPT_HANDLER_BODY();
}
#endif

#pragma endoptimize

/* undo suppress 'interrupt function "" dos not save register "" warnings */
#pragma warning restore

bool pal_architecture_set_native_handler(uint32_t number, void *handler)
{
assert(0);
    return true;
}

void pal_architecture_restore_default_handler(uint32_t number)
{
assert(0);
    return;
}

#endif /* ifdef __CPU_ARMV6M__ */
