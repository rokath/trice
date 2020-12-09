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

#include <ctype.h>
#include <string.h>

#include <interrupts.h>

#include "pal.h"
#include "pal_arch_cfg.h"
#include "interrupts_cfg.h"

#ifndef __CPU_ARMV6M__

#include "cortex_arch_specials_cfg.h"

#ifndef PAL_ARCH_VECTOR_TABLE_ADDRESS
#define PAL_ARCH_VECTOR_TABLE_ADDRESS   0x20000000
#endif

#ifdef __ALTIUM_BUILD_MAJOR__
extern long _lc_ub_vector_table_copy[]; // Altium Designer
#else
extern long _lc_ub_vector_table[];      // TASKING Eclipse
#endif

static uint32_t _pal_cortex_interrupt_set[(PAL_ARCH_MAXINTRS + 31) / 32];
static uint32_t _pal_cortex_vector_table[PAL_ARCH_MAXINTRS + 16] __align(sizeof(uint32_t) * (PAL_ARCH_MAXINTRS + 16)) __at(PAL_ARCH_VECTOR_TABLE_ADDRESS);

static void set_bit(uint32_t *set, int bit);
static int is_bit_set(uint32_t *set, int bit);
static void set_bits(uint32_t *set, uint32_t s, uint32_t e);


#if (__POSIX_KERNEL__)
static unsigned int _pal_cortex_saved_stackpointer;
#if ( POSIX_THREADS_DEBUG != 0 )
static unsigned int _pal_cortex_saved_resume_address;
#endif

static void cortex_posix_or_standard(void)
{
    int intnum;

    __asm("mrs      %0, ipsr" : "=r"(intnum));

    intnum -= 16;
    if (is_bit_set(_posix_interrupts_bitmask_array, intnum))
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

#if (__POSIX_KERNEL__)

/* suppress 'interrupt function "" does not save register "" warnings */
#pragma warning 750

/* avoid stack pops before function call */
#pragma optimize O0

static void __interrupt() __frame() cortex_default_interrupt_handler(void)
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
    __asm("ldr  r0, [sp,#36]" : : : "r0");  // saved epsr to align0, offset without floating point block
    __asm("str  r0, [sp,#0]");
   	__asm("skip_epsr_from_offset36:");

    /* top part of context and pc */
    __asm("stmdb    sp!, {r4, r5, r6, r7, r8, r9, r10, r11}");

    /* store $sp */
    __asm("str      sp, [%0]": : "r"(&_pal_cortex_saved_stackpointer));

#if ( POSIX_THREADS_DEBUG != 0 )
    /* store $ra incase this interrupt is a posix interrupt */
    __asm("str      lr, [%0]" : : "r"(&_pal_cortex_saved_resume_address));
#endif

    /* $sp to interrupt stack */
    __asm("ldr  sp, [%0]": : "r"(&_posix_interrupt_stack_top));

    cortex_posix_or_standard();

    /* load $sp */
    __asm("ldr      sp, [%0]": : "r"(&_pal_cortex_saved_stackpointer));

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

#pragma endoptimize

/* undo suppress 'interrupt function "" does not save register "" warnings */
#pragma warning restore

#else
static void __interrupt() cortex_default_interrupt_handler(void)
{
    uint32_t vectactive, number;
    interrupt_entry_t   *entry, *next;

    /*
     * get current pending interrupt form Interrupt Control State Register
     * bits [8-0], subtract 16 for int number
     */
    vectactive      = (SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk) >> SCB_ICSR_VECTACTIVE_Pos;
    if (vectactive < 16)
    {
        assert(0);
        /* TODO: 'handling' of internal interrupts */
        //handle_internal_interrupt(vectactive);
        //interrupt_acknowledge(vectactive - 16);
    }
    else
    {
        number = vectactive - 16;
        entry = interrupt_hash_table[number % INTERRUPTS_HASH_SIZE];
        while (entry != NULL)
        {
            next = entry->next;
            if (entry->number == number)
            {
                entry->handler(number, entry->context);
            }
            entry = next;
        }
        interrupt_acknowledge(vectactive - 16);
    }

    return;
}
#endif


bool pal_architecture_set_native_handler(uint32_t number, void *handler)
{
    unsigned int *tbl;

    tbl = (unsigned int *)SCB->VTOR;
    *(tbl + number + 16) = (unsigned int)handler | 1;

    return true;
}


static void set_bit(uint32_t *set, int bit)
{
    if (bit >= 0 && bit < PAL_ARCH_MAXINTRS)
    {
        set[bit/32] |= (1 << (bit % 32));
    }
}

static int is_bit_set(uint32_t *set, int bit)
{
    if (bit >= 0 && bit < PAL_ARCH_MAXINTRS)
    {
        return set[bit/32] & (1 << (bit % 32));
    }
    return 0;
}

static void set_bits(uint32_t *set, uint32_t s, uint32_t e)
{
    int i;

    if (s > e)
    {
        i = e;
        e = s;
    }
    else
    {
        i = s;
    }

    for (; i <= e; i++)
    {
        set_bit(set, i);
    }
}


/*
 * mask,    '\0' terminated string
 * set,     array of uint32_t's enough bits to contain PAL_ARCH_MAXINTRS bits
 */
static int interrupt_mask_to_set(const char *mask, uint32_t *set)
{
    int         s, e;
    const char  *p;

    memset(set, 0, ((PAL_ARCH_MAXINTRS + 31) / 32) * 4);

    /* scan/allow for digits, spaces and comma's */
    p = mask;
    while (*p != '\0')
    {
        if (isdigit(*p) || *p == ',' || *p == '-' || isspace(*p))
        {
            p++;
        }
        else
        {
            return -2;
        }
    }

    p = mask;
    while (p != NULL && *p != '\0')
    {
        if (*p == ',')
            p++;

        if (sscanf(p, " %d - %d ,", &s, &e) == 2)
        {
            set_bits(set, s, e);
            p = strchr(p, ',');
            continue;
        }

        if (sscanf(p, " %d ,", &s) == 1)
        {
            set_bit(set, s);
            p = strchr(p, ',');
            continue;
        }

        return -1;
    }

    return 0;
}

void cortex_install_interrupt_handlers(void)
{
#ifndef NDEBUG
    int err;

    err =
#endif
    interrupt_mask_to_set(CORTEX_ARCH_SPECIALS_INTERRUPT_MASK, _pal_cortex_interrupt_set);
    assert(!err);

    for (int i = 0; i < PAL_ARCH_MAXINTRS; i++)
    {
        if (is_bit_set(_pal_cortex_interrupt_set, i))
        {
            pal_architecture_set_native_handler(i, cortex_default_interrupt_handler);
        }
    }
}

void pal_architecture_restore_default_handler(uint32_t number)
{
    pal_architecture_set_native_handler(number, cortex_default_interrupt_handler);
}

void cortex_copy_and_switch_vector_table(void)
{
    uint32_t val, *table;

#ifdef __ALTIUM_BUILD_MAJOR__
    table = (uint32_t *)_lc_ub_vector_table_copy;
#else
    table = (uint32_t *)_lc_ub_vector_table;
#endif

    /* copy table */
    for (int i = 0; i < (PAL_ARCH_MAXINTRS + 16); i++)
    {
        _pal_cortex_vector_table[i] = table[i];
    }

    /* SCB->VTOR register:
     * - bit 29: base address: 0 -> Code area (0x00000000), 1 -> SRAM area (0x20000000)
     * - bit [28:0]: offset from base
     *
     * NOTE: this layout means the table address must be smaller than 0x40000000.
     */

    val = (uint32_t)_pal_cortex_vector_table;
    assert(val < 0x40000000);
    SCB->VTOR = val;
}
#endif /* ifndef __CPU_ARMV6M__ */

#if ( __POSIX_KERNEL__ != 0 )
void __interrupt(11) cortex_svc_load_ctx(int nbr)
{
    /* restore posix interrupts */
    posix_interrupts_enable();

    /* clear sv pending */
    SCB->ICSR |= SCB_ICSR_PENDSVCLR_Msk;

    /* load the stack pointer first */
    __asm("mov      sp, %0" : : "r"(_pal_load_ctx_from));

    /* top part of context */
#ifdef __CPU_ARMV6M__
    //__asm("ldmia    sp!, {r4, r5, r6, r7, r8, r9, r10, r11}");
    __asm("pop      {r4, r5, r6, r7}");
    __asm("pop      {r0}");
    __asm("mov      r8, r0");
    __asm("pop      {r0}");
    __asm("mov      r9, r0");
    __asm("pop      {r0}");
    __asm("mov      r10, r0");
    __asm("pop      {r0}");
    __asm("mov      r11, r0");
#else
    __asm("ldmia    sp!, {r4, r5, r6, r7, r8, r9, r10, r11}");
#endif

    /* let processor do the magic                       */
    __asm("ldr      r0, [sp,#4]");      // load 0xfffffff9
    __asm("add      sp, sp, #8");       // stack to r0

#if __FPU_VFP__
	/* restore upper floating point context (check for bit 5 in lr 0xffffffEx) */
	__asm("tst		r0, #16");
	__asm("bne		skip_restore_fp_upper2");
	__asm("vpop.32 {s16-s31}");
	__asm("skip_restore_fp_upper2:");
#endif

    __asm("bx       r0");               // processor magic
}
#endif

