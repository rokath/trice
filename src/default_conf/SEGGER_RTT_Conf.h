// SPDX-License-Identifier: MIT
// Trice-specific RTT defaults. SEGGER_RTT_ConfDefaults.h supplies everything else.
// Search project and generated headers first, then src, then src/default_conf.
// A project's SEGGER_RTT_Conf.h replaces this fallback; it is never merged here.

#ifndef SEGGER_RTT_CONF_H
#define SEGGER_RTT_CONF_H

// Trice uses channel 0 only. Keep these values consistent with triceConfig.h.
#ifndef SEGGER_RTT_MAX_NUM_UP_BUFFERS
#define SEGGER_RTT_MAX_NUM_UP_BUFFERS 1
#endif
#ifndef SEGGER_RTT_MAX_NUM_DOWN_BUFFERS
#define SEGGER_RTT_MAX_NUM_DOWN_BUFFERS 1
#endif

// This distribution ships the C implementation, not SEGGER's optional assembly.
#ifndef RTT_USE_ASM
#define RTT_USE_ASM 0
#endif

// Preserve the memory barrier for supported ARM cores with the C implementation.
// Older ARM cores have no DMB instruction and must not assemble this macro.
#if (defined(__GNUC__) || defined(__clang__)) &&                         \
    (defined(__ARM_ARCH_6M__) || defined(__ARM_ARCH_7M__) ||             \
	 defined(__ARM_ARCH_7EM__) || defined(__ARM_ARCH_8M_BASE__) ||       \
	 defined(__ARM_ARCH_8M_MAIN__) || defined(__ARM_ARCH_8_1M_MAIN__) || \
	 defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) ||             \
	 defined(__ARM_ARCH_8A__) || defined(__ARM_ARCH_8R__))
#ifndef RTT__DMB
#define RTT__DMB() __asm volatile("dmb sy" ::: "memory")
#endif
#elif defined(__ICCARM__)
#if (defined(__ARM7EM__) && (__CORE__ == __ARM7EM__)) ||                   \
    (defined(__ARM8M_BASELINE__) && (__CORE__ == __ARM8M_BASELINE__)) ||   \
    (defined(__ARM8M_MAINLINE__) && (__CORE__ == __ARM8M_MAINLINE__)) ||   \
    (defined(__ARM8EM_MAINLINE__) && (__CORE__ == __ARM8EM_MAINLINE__)) || \
    (defined(__ARM7A__) && (__CORE__ == __ARM7A__)) ||                     \
    (defined(__ARM7R__) && (__CORE__ == __ARM7R__)) ||                     \
    (defined(__ARM8A__) && (__CORE__ == __ARM8A__)) ||                     \
    (defined(__ARM8R__) && (__CORE__ == __ARM8R__))
#ifndef RTT__DMB
#if (__VER__ < 6300000)
#define RTT__DMB() asm("DMB")
#else
#define RTT__DMB() asm volatile("DMB")
#endif
#endif
#endif
#endif

// SEGGER's Windows defaults assume embOS Simulation. Ordinary native builds
// have no OS_SIM functions. Concurrent host users must supply project locks;
// defining SEGGER_RTT_LOCK_EMBOS explicitly keeps SEGGER's embOS defaults.
#if defined(WIN32) && !defined(SEGGER_RTT_LOCK_EMBOS)
#ifndef SEGGER_RTT_LOCK
#define SEGGER_RTT_LOCK()
#endif
#ifndef SEGGER_RTT_UNLOCK
#define SEGGER_RTT_UNLOCK()
#endif
#endif

#endif
