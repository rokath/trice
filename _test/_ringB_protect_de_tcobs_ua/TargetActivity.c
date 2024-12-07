#include "trice.h"

// Needed safety space is 4*((TRICE_DATA_OFFSET>>2) + (TRICE_SINGLE_MAX_SIZE>>1)-1) = 208
// Needed safety space is TRICE_DATA_OFFSET + (2*TRICE_SINGLE_MAX_SIZE)-4) = 208
// TRICE_DEFERRED_BUFFER_SIZE == 324
char* TargetActivity(void) {                                                                                                       // -4 bytes TRICE_DATA_OFFSET = 320 bytes space
	TRice("Hello ");                                                                                                               // -8 bytes = 312 bytes space
	TRice("World!\n");                                                                                                             // -8 bytes = 304 bytes space
	TRice64("msg:Twelve 64-bit values: %d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12); // -104 bytes = 200 bytes space
	// so the next Trice does not fit
	trice("Hello again\n"); // -4 bytes -> no fit!
	return "feed3322 Hello World!\nfeed3322 Twelve 64-bit values: -1,-2,-3,-4,-5,-6,-7,-8,-9,-10,-11,-12";
}
