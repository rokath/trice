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
**
**  Macro definition to handle thread safety when posix
**  threads are selected.
**
** ------------------------------------------------------------
*/

#ifndef __UTIL_THREADS_H
#define __UTIL_THREADS_H

#if defined (__POSIX_KERNEL__)

#include <pthread.h>

#include "posix_threads_cfg.h"

extern void devsync_cleanup_mutexlock ( void * arg );   // defined in devsync.c

#define CANCELLATION_POINT()    pthread_testcancel()

#if ( POSIX_THREADS_CANCEL_SUPPORT != 0 )

#define POSIX_DEV_MUTEX_PREPARE(buf)                                         \
                struct pthread_cleanup_buffer __mutex##buf##__;              \
                int    __canceltype##buf##__;

// Variant taking an address
#define POSIX_DEV_MUTEX_LOCK(mutexptr,buf)                                                          \
        pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED,&__canceltype##buf##__);                      \
        __pthread_cleanup_push( &__mutex##buf##__, devsync_cleanup_mutexlock, (void*)mutexptr );    \
        pthread_mutex_lock ( mutexptr );

#define POSIX_DEV_MUTEX_UNLOCK(mutexptr,buf)                                        \
        __pthread_cleanup_pop(&__mutex##buf##__, 1 );                               \
        pthread_setcanceltype   ( __canceltype##buf##__ , NULL ) ;

#else
// no cancel support
#define POSIX_DEV_MUTEX_PREPARE(mutexbuf)

// Variant taking an address
#define POSIX_DEV_MUTEX_LOCK(mutexptr,buf)                                       \
        pthread_mutex_lock ( mutexptr );

#define POSIX_DEV_MUTEX_UNLOCK(mutexptr,buf)                                      \
        pthread_mutex_unlock ( mutexptr );

// Variant taking actual mutex, deprecated
#define DEV_MUTEX_LOCK(mutex,buf)                                                 \
        pthread_mutex_lock ( &(mutex) );
#define DEV_MUTEX_UNLOCK(mutex,buf)                                               \
        pthread_mutex_unlock( &(mutex) ) ;

#endif /* POSIX_THREADS_CANCEL_SUPPORT */

#else /* no threads */

#define CANCELLATION_POINT()                    do {} while (0)
#define DEV_MUTEX_PREPARE(buf  )                do {} while (0)
#define POSIX_DEV_MUTEX_LOCK(mutexptr,buf)      do {} while (0)
#define POSIX_DEV_MUTEX_UNLOCK(mutexptr,buf)    do {} while (0)
#define DEV_MUTEX_LOCK(mutex,buf)               do {} while (0)
#define DEV_MUTEX_UNLOCK(mutex,buf)             do {} while (0)

#endif /* __POSIX_KERNEL__ */

#endif /* __UTIL_THREADS_H */

