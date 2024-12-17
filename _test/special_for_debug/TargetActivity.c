#include "trice.h"

char* TargetActivity(void) {
	uint8_t buf[3] = {0xaa,0xbb,0xcc};
	TRice8B("%3x\n", buf, 3);
	TRice8B("%3x\n", buf, 3);
	return "feed3322  aa bb cc\nfeed3322  aa bb cc";
}
