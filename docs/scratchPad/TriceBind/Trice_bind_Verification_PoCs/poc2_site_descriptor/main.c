#include "trice_sim.h"
#include "trice_module_c_K73A915E9C4021B8.h"

int main(void)
{
#line 100 "module.c"
    TRICE("Hello");
#line 101 "module.c"
    TRICE(id(0), "world");
#line 102 "module.c"
    TRICE(Id(0), "stamp16");
#line 103 "module.c"
    TRICE(ID(0), "stamp32");
#line 104 "module.c"
    trice("value=%d", 7);
#line 105 "module.c"
    TRICE(ID(0), "value=%d", 8);
#line 200 "main.c"
    return 0;
}
