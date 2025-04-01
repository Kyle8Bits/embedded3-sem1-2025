// -----------------------------------main.c -------------------------------------

#ifdef UART1
#include "../uart/uart1.h"
#define MESSAGE "HELLO WORLD, THIS IS UART1\n"
#elif defined(UART0)
#include "../uart/uart0.h"
#define MESSAGE "HELLO WORLD, THIS IS UART0\n"
#else
#define MESSAGE "HELLO WORLD!"
#endif

void main()
{
    // intitialize UART
    uart_init();

    // say hello
    uart_puts(MESSAGE);

    // echo everything back
    while (1)
    {
        // read each char
        char c = uart_getc();

        // send back twice
        uart_sendc(c);
        uart_sendc(c);
    }
}
