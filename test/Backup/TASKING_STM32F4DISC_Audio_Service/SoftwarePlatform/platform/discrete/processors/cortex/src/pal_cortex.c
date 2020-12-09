/* ------------------------------------------------------------
**
**  Copyright (c) 2013-2015 Altium Limited
**
**  This software is the proprietary, copyrighted property of
**  Altium Ltd. All Right Reserved.
**
**  SVN revision information:
**  $Rev: 15022 $:
**  $Date: 2015-04-22 10:52:38 +0200 (Wed, 22 Apr 2015) $:
**
** ------------------------------------------------------------
*/

#include <pal.h>
#include <pal_arch_cortex_variant_cmsis.h>
#include "pal_cfg.h"

#include <time.h>
#if ( __POSIX_KERNEL__ != 0 )
#include <posix_time.h>
#endif

// timer period in microseconds
#if ( __POSIX_TIMERS > 0 )
# include "posix_threads_cfg.h"
# define    PAL_TIMERSPERIODUSCS    ((POSIX_THREADS_OSTICKMICROSCS)*(1ULL))
#else
# ifndef    PAL_TIMERS_BASE
#  define PAL_TIMERS_BASE           10000
# endif
# define    PAL_TIMERSPERIODUSCS    ((PAL_TIMERS_BASE)*(1ULL))
#endif


#ifndef __CPU_ARMV6M__
uint32_t            _pal_cortex_nvic_intlinesnum;
#endif
volatile uint64_t   _pal_systick_num_ticks;

//#if ( (__TIMERS__ > 0) || (__POSIX_TIMERS > 0 ) )
#if ( __POSIX_KERNEL__ != 0 )
static volatile int _pal_timer_enadis_nesting_level = 0;    // first enable done with start not with enable
#endif

bool     pal_architecture_interrupts_init( void )
{
//    pal_architecture_interrupts_disable();
#ifndef __CPU_ARMV6M__
    cortex_copy_and_switch_vector_table();
    cortex_install_interrupt_handlers();
#endif
    return true;
}

void pal_architecture_timer_interrupt_enable(void)
{
    unsigned int ctrl;

#if ( __POSIX_KERNEL__ != 0 )
    bool bb;

    bb = pal_architecture_interrupts_disable_if_enabled();
    if (_pal_timer_enadis_nesting_level-- == 1)
    {
#endif
        ctrl = SysTick->CTRL;
        if (ctrl & SysTick_CTRL_COUNTFLAG_Msk)
        {
            SCB->ICSR = SCB_ICSR_PENDSTSET_Msk;
        }

        SysTick->CTRL = (ctrl & ~SysTick_CTRL_COUNTFLAG_Msk) | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;

        /*
         * cond3 test revealed this extra test is necessary,
         * otherwise we end up with a counting SysTick but no more interrupts
         */
        ctrl = SysTick->CTRL;
        if (ctrl & SysTick_CTRL_COUNTFLAG_Msk)
        {
            SCB->ICSR = SCB_ICSR_PENDSTSET_Msk;
        }
#if ( __POSIX_KERNEL__ != 0 )
    }
    if (bb)
    {
        pal_architecture_interrupts_enable();
    }
#endif
}

void pal_architecture_timer_interrupt_start(void)
{
    unsigned int ctrl;

    ctrl = SysTick->CTRL;
    if (ctrl & SysTick_CTRL_COUNTFLAG_Msk)
    {
        SCB->ICSR = SCB_ICSR_PENDSTSET_Msk;
    }

    SysTick->CTRL = (ctrl & ~SysTick_CTRL_COUNTFLAG_Msk) | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;

    ctrl = SysTick->CTRL;
    if (ctrl & SysTick_CTRL_COUNTFLAG_Msk)
    {
        SCB->ICSR = SCB_ICSR_PENDSTSET_Msk;
    }
}


void pal_architecture_timer_interrupt_disable(void)
{
#if ( __POSIX_KERNEL__ != 0 )
    bool bb;

    bb = pal_architecture_interrupts_disable_if_enabled();
    if (_pal_timer_enadis_nesting_level++ == 0)
    {
#endif
        SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
#if ( __POSIX_KERNEL__ != 0 )
    }
    if (bb)
    {
        pal_architecture_interrupts_enable();
    }
#endif
}

void pal_architecture_timer_interrupt_stop(void)
{
    SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
}

// Use systick to build our own system clock
bool pal_architecture_timer_interrupt_init(void)
{
    // set reload value such that systick period will be PAL_TIMERSPERIODUSCS us.
    uint32_t systick_reload = (PAL_CLOCKHZ / 1000000) * PAL_TIMERSPERIODUSCS;

    /* SysTick Current Value Register: A write of any value clears the field to 0, and also clears the SYST_CSR.COUNTFLAG bit to 0. */
#ifdef SYSTICK_CALIBRATION_VALUE
    SysTick->LOAD = SYSTICK_CALIBRATION_VALUE;
    SysTick->VAL = SYSTICK_CALIBRATION_VALUE;
#else
    SysTick->LOAD = systick_reload - 1;
    SysTick->VAL = systick_reload - 1;
#endif
    // Make sure the systick timer uses the processor freerunning clock (FCLK) as a reference
    // PAL_CLOCKHZ = FCLK = SystemCoreClock/SystemFrequency variable is specified in the CMSIS standard
    SysTick->CTRL |= 0x4;

    return true;
}

void pal_architecture_timer_interrupt_acknowledge(void)
{
    SysTick->CTRL;     // read to clear
    SCB->ICSR = SCB_ICSR_PENDSTCLR_Msk;
}


#if ( __POSIX_KERNEL__ != 0 )

/* suppress 'interrupt function "" does not save register "" warnings */
#pragma warning 750

/* avoid stack pops before function call */
#pragma optimize O0

#ifdef __CPU_ARMV6M__
void __interrupt(15) __frame() SysTick_Handler(void)
{
    /* store context */
    /* bottom part already on the stack */
    /* save pc */
    __asm("push     {lr}");
    /* make room for align0 and pc */
    __asm("sub  sp, sp, #4");
    /* use align0 as a back up of the status bits (in case they where overwritten by incomplete context restore) (epsr reads as zero)*/
    __asm("ldr  r0, [sp,#36]" : : : "r0");  // saved epsr to align0
    __asm("str  r0, [sp,#0]");

    /* top part of context and pc */
    __asm("mov      r0, r11");
    __asm("push     {r0}");
    __asm("mov      r0, r10");
    __asm("push     {r0}");
    __asm("mov      r0, r9");
    __asm("push     {r0}");
    __asm("mov      r0, r8");
    __asm("push     {r0}");
    __asm("mov      r0, r7");
    __asm("push     {r0}");
    __asm("mov      r0, r6");
    __asm("push     {r0}");
    __asm("mov      r0, r5");
    __asm("push     {r0}");
    __asm("mov      r0, r4");
    __asm("push     {r0}");

    /* store sp in _posix_current_thread->sp ( first field )    */
    if (_posix_current_thread != NULL)
    {
        __asm("mov      r1, sp": : : "r1");
        __asm("str      r1, [%0,#0]": : "r"(_posix_current_thread) : "r1");
    }
    else
    {
        __asm("mov      r1, sp": : : "r1");
        __asm("str      r1, [%0,#0]": : "r"(&_posix_irq_sp) : "r1");
    }

#if ( POSIX_THREADS_DEBUG != 0 )
    /* store $ra in _posix_resume_address                       */
    __asm("mov      r1, lr": : : "r1");
    __asm("str      r1, [%0,#0]" : : "r"(&_posix_resume_address) : "r1");
    /* store $sp incase needed later */
    __asm("mov      r1, sp": : : "r1");
    __asm("adds     r1, r1, #72": : : "r1");
    __asm("str      r1, [%0,#0]": : "r"(&_posix_user_stack) : "r1");
#endif

    /* switch to interrupt stack */
    __asm("ldr      r1, [%0,#0]": : "r"(&_posix_interrupt_stack_top) : "r1");
    __asm("mov      sp, r1": : : "r1");

    /* increment num_ticks for clock() */
    _pal_systick_num_ticks++;

    /* increment _posix_irq_level */
    _posix_irq_level++;

    // TODO: posix interrupts off, global interrupts on

    /* call posix clock interrupt handler */
    posix_clock_interrupt_handler();

    // TODO: global interrupts off, posix interrupts on

    /* decrement _posix_irq_level */
    _posix_irq_level--;

    /* get stack pointer */
    if (_posix_current_thread != NULL)
    {
        __asm("ldr      r1, [%0,#0]": : "r"(_posix_current_thread) : "r1");
        __asm("mov      sp, r1");
    }
    else
    {
        __asm("ldr      r1, [%0,#0]": : "r"(&_posix_irq_sp) : "r1");
        __asm("mov      sp, r1");
    }

    /* restore context */
    /* top part of context, pc and auto magically bottom part */
//    __asm("ldmia    sp!, {r4, r5, r6, r7, r8, r9, r10, r11}");
    __asm("pop      {r4, r5, r6, r7}");
    __asm("pop      {r1}");
    __asm("mov      r8, r1");
    __asm("pop      {r1}");
    __asm("mov      r9, r1");
    __asm("pop      {r1}");
    __asm("mov      r10, r1");
    __asm("pop      {r1}");
    __asm("mov      r11, r1");
    /* load pc, adjust sp */
    __asm("ldr      r0, [sp,#0]");         //align0 to apsr
    __asm("str      r0, [sp,#36]");
    __asm("ldr      r0, [sp,#4]");      // load pc (0xfffffff9)
    __asm("add      sp, sp, #8");       // adjust stack pointer to ->r0
    __asm("bx       r0");
}
#else
void __interrupt(15) __frame() SysTick_Handler(void)
{
    /* store context */
    /* bottom part already on the stack */
#if __FPU_VFP__
	/* save upper floating point context (check for bit 5 in lr 0xffffffEx) */
	__asm("tst lr, #16");
	__asm("bne skip_save_fp_upper");
	__asm("vpush.32 {s16-s31}");
	__asm("skip_save_fp_upper:");
#endif
    /* make room for align0 and pc */
    __asm("sub  sp, sp, #8");
    /* save pc */
    __asm("str  lr, [sp,#4]");
    /* use align0 as a back up of the status bits (in case they where overwritten by incomplete context restore) (epsr reads as zero)*/
#if __FPU_VFP__
    __asm("tst lr, #16");
	__asm("bne epsr_from_offset36");
    __asm("ldr  r0, [sp,#100]" : : : "r0");  // saved epsr to align0, offset includes s16-s31 block
    __asm("str  r0, [sp,#0]");
    __asm("b    skip_epsr_from_offset36");
   	__asm("epsr_from_offset36:");
#endif
    __asm("ldr  r0, [sp,#36]" : : : "r0");  // saved epsr to align0, offset without s16-s31 block
    __asm("str  r0, [sp,#0]");
   	__asm("skip_epsr_from_offset36:");

    /* top part of context and pc */
    __asm("stmdb    sp!, {r4, r5, r6, r7, r8, r9, r10, r11}");

    /* store sp in _posix_current_thread->sp ( first field )    */
    if (_posix_current_thread != NULL)
    {
        __asm("str      sp, [%0,#0]": : "r"(_posix_current_thread));
    }
    else
    {
        __asm("str      sp, [%0,#0]": : "r"(&_posix_irq_sp));
    }

#if ( POSIX_THREADS_DEBUG != 0 )
    /* store $ra in _posix_resume_address                       */
    __asm("str      lr, [%0,#0]" : : "r"(&_posix_resume_address));
    /* store $sp incase needed later */
    __asm("mov		r0, sp": : : "r0");
#if __FPU_VFP__
    __asm("tst      lr, #16");
	__asm("bne      user_stack_72");
    __asm("add		r0, r0, #136": : : "r0");
    __asm("b        skip_user_stack_72");
    __asm("user_stack_72:");
#endif
    __asm("add		r0, r0, #72": : : "r0");
    __asm("skip_user_stack_72:");
    __asm("str      r0, [%0,#0]": : "r"(&_posix_user_stack) : "r0");
#endif

    /* switch to interrupt stack */
    __asm("ldr  sp, [%0,#0]": : "r"(&_posix_interrupt_stack_top));

    /* increment num_ticks for clock() */
    _pal_systick_num_ticks++;

    /* increment _posix_irq_level */
    _posix_irq_level++;

    // TODO: posix interrupts off, global interrupts on

    /* call posix clock interrupt handler */
    posix_clock_interrupt_handler();

    // TODO: global interrupts off, posix interrupts on

    /* decrement _posix_irq_level */
    _posix_irq_level--;

    /* get stack pointer */
    if (_posix_current_thread != NULL)
    {
        __asm("ldr      sp, [%0,#0]": : "r"(_posix_current_thread));
    }
    else
    {
        __asm("ldr      sp, [%0,#0]": : "r"(&_posix_irq_sp));
    }

    /* restore context */
    /* top part of context, pc and auto magically bottom part */
    __asm("ldmia    sp!, {r4, r5, r6, r7, r8, r9, r10, r11}");

    /* load pc, adjust sp */
#if __FPU_VFP__
    __asm("ldr      r0, [sp,#4]");      // load pc (0xfffffff9 e9)
    __asm("tst      r0, #16");
    __asm("bne      aprs_from_offset36");
    __asm("ldr      r0, [sp,#0]");         //align0 to apsr
    __asm("str      r0, [sp,#100]");
    __asm("ldr      r0, [sp,#4]");      // load pc (0xfffffff9)
    __asm("b        skip_aprs_from_offset36");
    __asm("aprs_from_offset36:");
#endif
    __asm("ldr      r0, [sp,#0]");         //align0 to apsr
    __asm("str      r0, [sp,#36]");
    __asm("ldr      r0, [sp,#4]");      // load pc (0xfffffff9)
    __asm("skip_aprs_from_offset36:");
    __asm("add      sp, sp, #8");       // adjust stack pointer to ->r0
#if __FPU_VFP__
	/* restore upper floating point context (check for bit 5 in lr 0xffffffEx) */
	__asm("tst		r0, #16");
	__asm("bne		skip_restore_fp_upper");
	__asm("vpop.32	{s16-s31}");
	__asm("skip_restore_fp_upper:");
#endif
    __asm("bx       r0");
}
#endif

/* undo suppress 'interrupt function "" does not save register "" warnings */
#pragma warning restore

#pragma endoptimize

#else
void __interrupt(15) SysTick_Handler(void)
{
    _pal_systick_num_ticks++;

#if (__TIMERS__ > 0)
    pal_timers_common_handler();
#else
    pal_architecture_timer_interrupt_acknowledge();
#endif
}
#endif


clock_t clock(void)
{
    uint64_t ticks;
    uint32_t current;
    uint64_t ret_clock;
#if ( __POSIX_KERNEL__ != 0 )
    static uint64_t prev_clock;
#endif

    // while loop ensures determining clk is not interrupted
    // by systick/posix_threads (changed systick_num_ticks)
    do
    {
        ticks = _pal_systick_num_ticks;
        current = SysTick->VAL;
    } while (ticks != _pal_systick_num_ticks);
    // clk = PAL_TIMERSPERIODUSCS * PAL_CLOCKHZ / 1000000 *(ticks + (systick_reload - current)/systick_reload)
    // with systick_reload = (PAL_CLOCKHZ / 1000000) * PAL_TIMERSPERIODUSCS -->
    ret_clock = (clock_t)(PAL_TIMERSPERIODUSCS * PAL_CLOCKHZ / 1000000 * (ticks + 1) - current);

#if ( __POSIX_KERNEL__ != 0 )
    /* detect and avoid the past */
    if (ret_clock < prev_clock)
    {
    	return prev_clock;
    }
    prev_clock = ret_clock;
#endif

    return ret_clock;
}


#if ((defined __CM3_CMSIS_VERSION) && (__CM3_CMSIS_VERSION <= 0x20000))
 // CMSIS 2.0
 #ifndef InterruptType_ICTR_INTLINESNUM_Msk
  #define InterruptType_ICTR_INTLINESNUM_Msk IntType_ICTR_INTLINESNUM_Msk
 #endif
 #ifndef InterruptType_ICTR_INTLINESNUM_Pos
  #define InterruptType_ICTR_INTLINESNUM_Pos IntType_ICTR_INTLINESNUM_Pos
 #endif
#endif

bool pal_architecture_processor_init(void)
{
#ifndef __CPU_ARMV6M__
#if ((defined __CM3_CMSIS_VERSION) && (__CM3_CMSIS_VERSION >= 0x20010)) || ((defined __CM4_CMSIS_VERSION) && (__CM4_CMSIS_VERSION >= 0x20010))
    _pal_cortex_nvic_intlinesnum = (SCnSCB->ICTR & SCnSCB_ICTR_INTLINESNUM_Msk) >> SCnSCB_ICTR_INTLINESNUM_Pos;
#else
    _pal_cortex_nvic_intlinesnum = (InterruptType->ICTR & InterruptType_ICTR_INTLINESNUM_Msk) >> InterruptType_ICTR_INTLINESNUM_Pos;
# endif
#endif

    pal_architecture_variant_init();
    return true;
}



bool pal_architecture_clock_init(void)
{
#if ( (__TIMERS__ == 0) && (__POSIX_TIMERS == 0) )
    pal_architecture_timer_interrupt_init();
    pal_architecture_timer_interrupt_enable();
#endif
    return true;
}

/**
 * @brief
 *      Configure interrupt type
 *
 * This function sets the interrupt priority for a given interrupt. It should be
 * used by drivers during initialization to configure the interrupts priority in the
 * controller.
 *
 * @param number
 *      Interrupt number
 * @param pre
        Preemption priority
 * @param sub
        Sub priority
 * @param cfg
 *      Interrupt type
 *
 * @return true to indicate success, or false otherwise.
 */
bool pal_architecture_interrupt_priority(uint32_t number, uint32_t preemptionpriority, uint32_t subpriority)
{

    uint32_t tmppriority = 0x00, tmppre = 0x00, tmpsub = 0x0F;

    /* Compute the Corresponding IRQ Priority --------------------------------*/
    tmppriority = (0x700 - ((SCB->AIRCR) & (uint32_t)0x700))>> 0x08;
    tmppre = (0x4 - tmppriority);
    tmpsub = tmpsub >> tmppriority;

    tmppriority = preemptionpriority << tmppre;
    tmppriority |=  subpriority & tmpsub;
    tmppriority = tmppriority << 0x04;

    NVIC->IP[number] = (uint8_t)tmppriority;
    return true;
}
