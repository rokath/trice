#include "time.h"
#include <libopencm3/cm3/systick.h>
/* monotonically increasing number of milliseconds from reset
 */
static volatile uint32_t system_millis;

/* Magic: This is called when systick fires.
 * GCC insists there is a decalaration before
 * a definition. (LibOpenCM3 doesn't do this?)
 */
void sys_tick_handler(void);
void sys_tick_handler(void)
{
	system_millis++;
}

/* "sleep" for delay milliseconds */
void msleep(uint32_t delay)
{
	uint32_t wake = system_millis + delay;
	while (wake > system_millis);
}

uint32_t wallclock_ms(void)
{
	return system_millis;
}
