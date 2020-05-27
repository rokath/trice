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
**  Utility functions for threading support.
**
** ------------------------------------------------------------
*/

#if __POSIX_KERNEL__ != 0
#include <pthread.h>
#endif

extern void devsync_cleanup_mutexlock ( void * arg )
{
#if __POSIX_KERNEL__ != 0
    pthread_mutex_unlock ( (pthread_mutex_t*) arg );
#endif
}
