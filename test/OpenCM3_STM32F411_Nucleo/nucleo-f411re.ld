/* Use the LibOpenCM3-provided defaults for the linker details.
 */
MEMORY
{
	rom (rx)  : ORIGIN = 0x08000000, LENGTH = 512K
	ram (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
}

INCLUDE cortex-m-generic.ld

