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

#include <stdbool.h>
#include <pal_arch_cortex_variant_cmsis.h>

bool pal_architecture_interrupt_configure(uint32_t number, bool edge, bool high)
{
    /* External interrupt/event controller (EXTI)   */
    /* Nested vectored interrupt controller (NVIC)  */

    /*
     * The cortex has interrupts enables in nvic and separate external interrupt enables via exti.
     * NVIC has numbers for ext int 0, 1, 2, 3, 4 and two group interrupts for 5-9 and 10-15.
     * EXTI has has configuration registers for 19 external interrupts.
     *
     * At the moment the interrupt number for this routine is the nuber of the interrupt in the EXTI.
     * Although the EXTI supports rising and falling edge at the same time this routine only sets one
     * and clear the other.
     *
     * Setting high or edge also enables the interrupt in the EXTI,
     * for now setting low no edge disable the interrupt.
     *
     * The software interrupts (events) are currently not supported.
     */
    switch (number)
    {
        case 0:         /* EXTI Line 0 Interrupt */
        case 1:         /* EXTI Line 1 Interrupt */
        case 2:         /* EXTI Line 2 Interrupt */
        case 3:         /* EXTI Line 3 Interrupt */
        case 4:         /* EXTI Line 4 Interrupt */
        case 5:         /* EXTI Line 5 Interrupt */
        case 6:         /* EXTI Line 6 Interrupt */
        case 7:         /* EXTI Line 7 Interrupt */
        case 8:         /* EXTI Line 8 Interrupt */
        case 9:         /* EXTI Line 9 Interrupt */
        case 10:        /* EXTI Line 10 Interrupt */
        case 11:        /* EXTI Line 11 Interrupt */
        case 12:        /* EXTI Line 12 Interrupt */
        case 13:        /* EXTI Line 13 Interrupt */
        case 14:        /* EXTI Line 14 Interrupt */
        case 15:        /* EXTI Line 15 Interrupt */
        case 16:        /* EXTI line 16 (is connected to the PVD output) */
        case 17:        /* EXTI line 17 (is connected to the RTC Alarm event) */
        case 18:        /* EXTI line 18 (is connected to the USB Wakeup event) */
        case 19:        /* EXTI line 19 (is connected to the Ethernet Wakeup event */
            if (edge)
            {
                if (high)
                {
                    EXTI->RTSR |= (1 << number);     // set rising edge bit
                    EXTI->FTSR &= ~(1 << number);    // clear falling edge bit
                }
                else
                {
                    EXTI->RTSR &= ~(1 << number);    // clear rising edge bit
                    EXTI->FTSR |= (1 << number);     // set falling edge bit
                }
                EXTI->IMR |= (1 << number);          // enable int
            }
            else
            {
                if (high)
                {
                    EXTI->IMR |= (1 << number);      // enable int
                }
                else
                {
                    EXTI->IMR &= ~(1 << number);     // disable int
                }
            }
            break;
        default:
            return false;
    }

    return true;
}

