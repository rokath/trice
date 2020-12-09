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

#include <stdbool.h>
#include <pal.h>
#include "pal_cfg.h"

#if ( __TIMERS__ != 0 )

#include <stdlib.h>
#include <pal.h>

typedef struct{
    void                *context;
    pal_timer_handler_t handler;
    int64_t             tick_count;
    int64_t             reload; // to tick_count
} pal_timer_handler_entry_t;

static pal_timer_handler_entry_t   _pal_timer_handlers[PAL_MAXTIMERS];
static uint32_t                    _pal_timers_registrations=0;


void pal_timers_common_handler (void)
{
    uint32_t i,reg=0;
    pal_timer_handler_entry_t  * entry;

    for (   i = 0, entry = &_pal_timer_handlers[0];
            reg < _pal_timers_registrations && i < PAL_MAXTIMERS ;
            i++, entry++)
    {
        if ( entry->handler )
        {
            reg++;
            if  (--entry->tick_count <= 0)
            {
                (*entry->handler)(entry->context);
                entry->tick_count = entry->reload;
            }
        }
    }
    pal_architecture_timer_interrupt_acknowledge();

    return;
}

void* pal_timer_register_handler (void* context,
                                 int64_t frequency,
                                 pal_timer_handler_t handler)
{
    uint32_t i;
    pal_timer_handler_entry_t  * entry;

    if ( _pal_timers_registrations >= PAL_MAXTIMERS ){
        return NULL;
    }

    pal_timer_interrupt_disable();

    for (i=0, entry = &_pal_timer_handlers[0] ;
              i< PAL_MAXTIMERS && entry->handler != NULL;
              i++, entry++);

    if (i==PAL_MAXTIMERS)
        return NULL;

    _pal_timers_registrations++;
    entry->handler    = handler;
    entry->context    = context;
    entry->reload     = frequency;
    entry->tick_count = frequency;

    pal_timer_interrupt_enable();

    return (void*)&_pal_timer_handlers[i];
}

bool pal_timer_deregister_handler (void* id)
{
    pal_timer_handler_entry_t* timer = (pal_timer_handler_entry_t*)id;

    pal_timer_interrupt_disable();

    if( timer !=NULL && _pal_timers_registrations )
    {
            if ( timer->handler != NULL )
            {
                timer->handler      = NULL;
                _pal_timers_registrations--;
                return true;
            }
    }

    pal_timer_interrupt_enable();

    return false;
}

#endif
