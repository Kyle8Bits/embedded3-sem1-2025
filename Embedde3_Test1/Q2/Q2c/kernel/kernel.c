// -----------------------------------main.c -------------------------------------
#ifdef UART1
#include "uart1.c"
#define MSF "From UART1 \n"
#else
#include "../peripheral/uart0.c"
#define MSF "From UART0 \n"
#endif

#include "../peripheral/gpio.c"

void main(){
    // intitialize UART
    uart_init();
    
    // say hello
    uart_puts(MSF);
    led_blink();


    // echo everything back 
    while(1) {
    	//read each char
    	char c = uart_getc();
    	//send back twice
    	uart_sendc(c);
    	uart_sendc(c);
    }
}
