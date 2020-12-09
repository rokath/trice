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

#ifndef _PAL_ARCH_POSIX_H
#define _PAL_ARCH_POSIX_H

#include <stddef.h>          /* offsetof */
#include "posix_threads_cfg.h"

#define _hal_STACKDOWNWARDS            1

extern struct thread_t * volatile _posix_current_thread;
#if ( POSIX_THREADS_DEBUG != 0 )
extern unsigned int _pal_last_pc_load_ctx;
extern unsigned int _pal_last_psr_load_ctx;
extern unsigned int _pal_last_load_ctx;
extern unsigned int _pal_last_load_intr;
#endif

extern unsigned int _pal_load_ctx_from;

typedef struct _hal_ctx_t
{
    /* saved by context code */
    uint32_t    r4;
    uint32_t    r5;
    uint32_t    r6;
    uint32_t    r7;
    uint32_t    r8;
    uint32_t    r9;
    uint32_t    r10;
    uint32_t    r11;
    uint32_t    align0;
    uint32_t    pc;     /* pc is special, if it starts with 0xf... then lower part was saved by context code
                         * need to check if in interrupt context or thread context,
                         * if interrupt then load pc with pc and processor will handle the rest
                         * otherwise we load the rest
                         */
    /* save by interrupt or context code */
    uint32_t    r0;
    uint32_t    r1;
    uint32_t    r2;
    uint32_t    r3;
    uint32_t    r12;
    uint32_t    lr;     // LR (R14)
    uint32_t    ra;
    uint32_t    psr;
} hal_ctx_t;

typedef struct _hal_ctx_fphi_s {
	uint32_t	s16;
	uint32_t	s17;
	uint32_t	s18;
	uint32_t	s19;
	uint32_t	s20;
	uint32_t	s21;
	uint32_t	s22;
	uint32_t	s23;
	uint32_t	s24;
	uint32_t	s25;
	uint32_t	s26;
	uint32_t	s27;
	uint32_t	s28;
	uint32_t	s29;
	uint32_t	s30;
	uint32_t	s31;
} hal_ctx_fphi_t;

typedef struct _hal_ctx_fplo_s {
	uint32_t	s0;
	uint32_t	s1;
	uint32_t	s2;
	uint32_t	s3;
	uint32_t	s4;
	uint32_t	s5;
	uint32_t	s6;
	uint32_t	s7;
	uint32_t	s8;
	uint32_t	s9;
	uint32_t	s10;
	uint32_t	s11;
	uint32_t	s12;
	uint32_t	s13;
	uint32_t	s14;
	uint32_t	s15;
	uint64_t	fpscr;
} hal_ctx_fplo_t;

typedef struct _hal_fpctx_t
{
    /* saved by context code */
    uint32_t    r4;
    uint32_t    r5;
    uint32_t    r6;
    uint32_t    r7;
    uint32_t    r8;
    uint32_t    r9;
    uint32_t    r10;
    uint32_t    r11;
    uint32_t    align0;
    uint32_t    pc;     /* pc is special, if it starts with 0xf... then lower part was saved by context code
                         * need to check if in interrupt context or thread context,
                         * if interrupt then load pc with pc and processor will handle the rest
                         * otherwise we load the rest
                         */
    uint32_t	s31;
    uint32_t	s16;
    /* save by interrupt or context code */
    uint32_t    r0;
    uint32_t    r1;
    uint32_t    r2;
    uint32_t    r3;
    uint32_t    r12;
    uint32_t    lr;     // LR (R14)
    uint32_t    ra;
    uint32_t    psr;
    uint32_t	s15;
    uint32_t	s0;
    uint32_t	fpscrhi;
    uint32_t	fpscrlo;
} hal_fpctx_t;

typedef struct _hal_ctx_bottom {
    uint32_t    r0;
    uint32_t    r1;
    uint32_t    r2;
    uint32_t    r3;
    uint32_t    r12;
    uint32_t    lr;
    uint32_t    ra;
    uint32_t    psr;
} hal_ctx_bottom_t;

extern uint32_t                     _posix_resume_address;
extern uint32_t                     _posix_user_stack;
extern volatile hal_ctx_bottom_t    *_pal_ctx_bottom;
extern uint32_t                     _posix_irq_sp;
extern void                         *_posix_interrupt_stack_top;

extern void pal_architecture_posix_interrupts_enable(void);
extern void pal_architecture_posix_interrupts_disable(void);
extern void cortex_init_ctx(void* (*)(void*),void*, void (*)(void*),void **);
#define _hal_init_ctx       cortex_init_ctx

static inline void _hal_jump_main(void)
{
    __asm(".extern main");
    __asm("bl main");
}


/* NO interprocedural register optimization */
static inline void cortex_load_ctx(void *_spptr)
{
    _pal_load_ctx_from = (unsigned int)_spptr;
    __asm("svc  1");
}
#define _hal_load_ctx       cortex_load_ctx

#if __CPU_ARMV6M__
static inline void cortex_save_ctx(void)
{
    /* TODO: make sure stack alignment is 8 and handled in load_ctx */

    /* bottom part of context   */
    /* reserve space for psr and ra ( 8-4 compensate for 4 bytes that are already on the stack)     */
    //__asm("sub      sp, sp, #8");
    __asm("sub      sp, sp, #4");

    /* save r0, r1, r2, r3, r12 and lr  */
    __asm("push     {lr}");
    __asm("mov      lr, r12");
    __asm("push     {lr}");
    __asm("push     {r3}");
    __asm("push     {r2}");
    __asm("push     {r1}");
    __asm("push     {r0}");
    __asm("mov      r0, sp");       /* r0 has stack */
    __asm("mov      r1, lr");       /* r1 has lr */
    __asm("ldr      r1, [r0,#20]");
    /*
     * save lr also on ra position with bit0 cleared in case we get loaded via interrupt handler return
     */
    //__asm("bfc      r1, #0, #1");
    __asm("bics     r2, r2, r2");
    __asm("adds     r2, r2, #1");   /* r2 is 1  */
    __asm("bics     r1, r1, r2");
    __asm("str      r1, [r0,#24]");
    /*
     * save psr with T bit set (this T bit make Thumb mode when loaded via interrupt return)
     * TODO: see if interrupted multiple store/restores can be continued
     */
    __asm("mrs      r3, apsr");
    __asm("lsls     r2, r2, #24");  /* r2 bit 24 T-bit set */
    __asm("orrs     r3, r3, r2");
    __asm("str      r3, [r0,#28]");
    /*
     * modified apsr also in align0
     */
    __asm("sub      sp, sp, #4");
    __asm("push     {r3}");
    /*
     * set pc to 0xfffffff9 to return to thread mode form interrupt
     */
    __asm("mov      r0, sp");       /* r0 has stack */
    __asm("bics     r2, r2, r2");   /* r2 is 0 */
    __asm("adds     r2, r2, #6");   /* r2 i s6 */
    __asm("mvns     r3, r2");       /* r3 = 0xfffffff9 */
    __asm("str      r3, [r0,#4]");

    /* top part of context */
//    __asm("stmdb    sp!, {r4, r5, r6, r7, r8, r9, r10, r11}");
    __asm("mov      r0, r11");
    __asm("push     {r0}");
    __asm("mov      r0, r10");
    __asm("push     {r0}");
    __asm("mov      r0, r9");
    __asm("push     {r0}");
    __asm("mov      r0, r8");
    __asm("push     {r0}");
    __asm("push     {r4, r5, r6, r7}");

    /* store sp in _posix_current_thread->sp ( first field )    */
    __asm("mov      r0, sp");
    __asm("str      r0, [%0,#0]": : "r"(_posix_current_thread) : "r0");

#if ( POSIX_THREADS_DEBUG != 0 )
    /* store $ra in _posix_resume_address                       */
    __asm("mov      r3, lr": : : "r3");
    __asm("str      r3, [%0,#0]" : : "r"(&_posix_resume_address) : "r3");
#endif
}
#else
static inline void cortex_save_ctx(void)
{
    /* TODO: make sure stack alignment is 8 and handled in load_ctx */

    /* bottom part of context   */
    /* reserve space for psr and ra ( 8-4 compensate for 4 bytes that are already on the stack)     */
	/* for FPU these 8 bytes are for FPSCR */

    //__asm("sub      sp, sp, #8");
    __asm("sub      sp, sp, #4");

#if __FPU_VFP__
	/* should/could check FPCA bit in CONTROL register for now save all */
	__asm("vpush.32 {s0-s15}");
	/* make room psr and ra */
	__asm("sub      sp, sp, #8");
#endif

    /* save r0, r1, r2, r3, r12 and lr  */
    __asm("stmdb    sp!, {r0, r1, r2, r3, r12, lr}");

#if __FPU_VFP__
    /* save FPSCR */
    __asm("vmrs     r0, FPSCR");
    __asm("str      r0, [sp,#96]");
#endif

    /*
     * save lr also on ra position with bit0 cleared (we get loaded via interrupt handler return)
     */
    __asm("bfc      lr, #0, #1");
    __asm("str      lr, [sp,#24]");

    /*
     * save psr with T bit set (this T bit make Thumb mode when loaded via interrupt return)
     * TODO: see if interrupted multiple store/restores can be continued
     */
    __asm("mrs      r0, apsr");
    __asm("mov      r1, #1");
    __asm("orr      r0, r0, r1, lsl #24");
    __asm("str      r0, [sp,#28]");

#if __FPU_VFP__
	/* should/could check FPCA bit in CONTROL register for now save all */
	__asm("vpush.32 {s16-s31}");
#endif

	/*
     * Modified apsr also in align0
     */
    __asm("sub      sp, sp, #8");
    __asm("str      r0, [sp]");

    /*
     * set pc to 0xfffffff9 e9 to return to thread mode form interrupt
     */
#if __FPU_VFP__
    __asm("mvn      r0, #22");
    __asm("str      r0, [sp,#4]");
#else
    __asm("mvn      r0, #6");
    __asm("str      r0, [sp,#4]");
#endif

    /* top part of context */
    __asm("stmdb    sp!, {r4, r5, r6, r7, r8, r9, r10, r11}");

    /* store sp in _posix_current_thread->sp ( first field )    */
    __asm("str      sp, [%0]": : "r"(_posix_current_thread));

#if ( POSIX_THREADS_DEBUG != 0 )
    /* store $ra in _posix_resume_address                       */
    __asm("str      lr, [%0]" : : "r"(&_posix_resume_address));
#endif
}
#endif

#define _hal_save_ctx           cortex_save_ctx

extern void  cortex_load_asr_ctx(void **__sp, void (*_asr_handler)(void*));
#define _hal_load_asr_ctx        cortex_load_asr_ctx

extern void  cortex_switch_ctx( void **, void * );
#define _hal_switch_ctx          cortex_switch_ctx


#if __FPU_VFP__
/* with lazy stacking of floating point registers, when context switch is taken make sure floating point registers are flushed before loading new context */
static inline void _hal_flush_ctx(void)
{
	__asm("mrs			r0, control": : : "r0");
	__asm("tst			r0, #4");
	__asm("beq			skip_fp_flush");
	__asm("vmov.f32		s0, #0");
	__asm("skip_fp_flush:");

	return;
}
#else
#define _hal_flush_ctx()
#endif

#endif /* ndef _PAL_ARCH_POSIX_H */
