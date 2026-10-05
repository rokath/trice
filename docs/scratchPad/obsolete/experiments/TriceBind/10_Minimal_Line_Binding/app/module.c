#include "trice.h"
#include "module.c.triceIDs.h"

int main(void) {
	if (TriceInit() != 0) {
		return 1;
	}

	trice("msg:Hello trice bind world\n");

	TriceClose();
	return 0;
}
