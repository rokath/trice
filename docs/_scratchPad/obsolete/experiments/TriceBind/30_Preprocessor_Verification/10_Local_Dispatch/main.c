#include "trice_sim.h"

int main(void) {
	/* No sidecar is active: the existing ID is handled by INSERT. */
	trice(iD(1001u), "before sidecar");

/* The sidecar is intentionally included after trice_sim.h. */
#include "trice_module_c_K73A915E9C4021B8.h"

	/* Route selection is evaluated now, not when trice_sim.h was read. */
	trice("with sidecar");

/* A rare hybrid source can explicitly return to INSERT routing. */
#undef TRICE_BIND_FILE_KEY
	trice(iD(3003u), "after undef");

	return 0;
}
