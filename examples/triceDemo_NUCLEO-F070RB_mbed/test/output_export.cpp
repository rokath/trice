#include <mbed.h>
#include <output_export.h>

Serial pc(USBTX, USBRX);

#ifdef __GNUC__
void output_start(unsigned int baudrate __attribute__((unused)))
#else
void output_start(unsigned int baudrate)
#endif
{
    pc.baud(115200);
}

void output_char(int c)
{
    pc.putc(c);
}

void output_flush(void)
{
    ;
}

void output_complete(void)
{
   ;
}