#include "trice.h"

                                    // Needed safety space is TRICE_SINGLE_MAX_SIZE = 104
                                    // TRICE_HALF_BUFFER_SIZE == 256 - TRICE_DATA_OFFSET == 64 = 192 bytes
char* TargetActivity (void ) {      // -4 bytes TRICE_DATA_OFFSET = 188 bytes space
    TRice( iD(12000), "Hello " );   // -8 bytes = 180 bytes space
    TRice( iD(12001), "World!\n" ); // -8 bytes = 172 bytes space
    TRice64( iD(12002), "msg:Twelve 64-bit values: %d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12); // -104 bytes = 68 bytes space
    // so the next Trice does not fit
    trice( iD(12003), "Hello again\n");   // -4 bytes -> no fit! 
	return " 842,150_450 Hello World!\n 842,150_450 Twelve 64-bit values: -1,-2,-3,-4,-5,-6,-7,-8,-9,-10,-11,-12";
}