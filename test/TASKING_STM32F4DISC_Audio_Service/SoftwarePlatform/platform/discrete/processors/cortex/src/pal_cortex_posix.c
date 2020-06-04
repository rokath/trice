/* ------------------------------------------------------------
**
**  Copyright (c) 2013-2015 Altium Limited
**
**  This software is the proprietary, copyrighted property of
**  Altium Ltd. All Right Reserved.
**
**  SVN revision information:
**  $Rev: 14940 $:
**  $Date: 2015-03-10 14:27:37 +0100 (Tue, 10 Mar 2015) $:
**
** ------------------------------------------------------------
*/

#include <assert.h>

#if ( __POSIX_KERNEL__ > 0 )

#include <string.h>
#include <stdint.h>
#include <interrupts.h>
#include "posix_threads_cfg.h"


/* interrupt stack buffer */
int32_t         _posix_interrupt_stack[(POSIX_THREADS_INTR_STACKSIZE + 3) / 4];
void   *_posix_interrupt_stack_top = (void*)&_posix_interrupt_stack[((POSIX_THREADS_INTR_STACKSIZE + 3) / 4) - 1];

#ifdef __CPU_ARMV6M__
uint32_t        _pal_enabled_posix_interrupts_bitmask;
#else
uint32_t        _pal_enabled_posix_interrupts_bitmask_array[(PAL_ARCH_MAXINTRS + 31) / 32];
#endif

#include "pal.h"
#include "pal_arch_posix.h"


/* sp pointer at first nesting level */
uint32_t    _posix_irq_sp = 0;

/* stack address to load context in svc from */
unsigned int _pal_load_ctx_from;

/* specific posix code for context switching at interrupt level */


void cortex_init_ctx(void *(*_entry)(void*), void *_arg, void (*_exit)(void*), void **__spptr)
{
    hal_ctx_t *context;

    // TODO: check if we need to align stack for 8 bytes boundary

    context = (hal_ctx_t *)(((char*)*__spptr) - sizeof(hal_ctx_t));

    /* initialize all registers to zero */
#if ( POSIX_THREADS_DEBUG != 0 )
    context->r4     = 4;
    context->r5     = 5;
    context->r6     = 6;
    context->r7     = 7;
    context->r8     = 8;
    context->r9     = 9;
    context->r10    = 10;
    context->r11    = 11;
    context->pc     = 15;
    context->r0     = 0;
    context->r1     = 1;
    context->r2     = 2;
    context->r3     = 3;
    context->r12    = 12;
    context->lr     = 14;
    context->ra     = 15;
    context->psr    = 0;
#else
    memset(context, 0, sizeof(*context));
#endif

    /* set parameter */
    context->r0 = (unsigned)_arg;

    /* set entry point (without bit0 set for interrupt return) */
    context->ra = (unsigned)_entry & ~1;

    /* From exception Return to Thread Mode; */
    context->pc = 0xfffffff9;

    /* set the stack pointer */
//    context->sp = (unsigned)(context);
    *__spptr = context;

    /* return address */
    context->lr = (unsigned)_exit | 1;

    /* set thumb bit so thumb bit gets set when context is loaded via interrupt return */
    context->psr    = 0x01000000;
    context->align0 = 0x01000000;

    return;
}

void cortex_continue_from_asr_return(void *sp, void *(*routine)(void))
{
    routine();
    cortex_load_ctx(sp);
}

void cortex_load_asr_ctx(void **__sp, void (*_asr_handler)(void*))
{
    hal_ctx_t *context = (hal_ctx_t *)((char *)*__sp - sizeof(hal_ctx_t));

    /* set parameter 0 and 1 */
    context->r0 = (uint32_t)*__sp;
    context->r1 = (uint32_t)_asr_handler;

    /* set entry point (without bit0 set for interrupt return) */
    context->ra = (uint32_t)cortex_continue_from_asr_return & ~1;

    /* From exception Return to Thread Mode; */
    context->pc = 0xfffffff9;

    /* correct the stack pointer */
    *__sp = context;

    /* return address */
    context->lr = (uint32_t)cortex_continue_from_asr_return | 1;

    /* set thumb bit so thumb bit gets set when context is loaded via interrupt return */
    context->psr    = 0x01000000;
    context->align0 = 0x01000000;
}

void pal_architecture_posix_interrupts_enable(void)
{
#ifdef __CPU_ARMV6M__
    NVIC->ISER[0] = _pal_enabled_posix_interrupts_bitmask;
#else
    for (int i = 0; i <= _pal_cortex_nvic_intlinesnum; i++)
    {
        NVIC->ISER[i] = _pal_enabled_posix_interrupts_bitmask_array[i];
    }
#endif
    pal_architecture_timer_interrupt_enable();
}

void pal_architecture_posix_interrupts_disable(void)
{
#ifdef __CPU_ARMV6M__
    NVIC->ICER[0] = _posix_interrupts_bitmask;
#else
    for (int i = 0; i <= _pal_cortex_nvic_intlinesnum; i++)
    {
        NVIC->ICER[i] = _posix_interrupts_bitmask_array[i];
    }
#endif
    pal_architecture_timer_interrupt_disable();
}

#endif /* __POSIX_KERNEL__ */
