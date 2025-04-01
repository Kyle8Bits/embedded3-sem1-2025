// -----------------------------------main.c -------------------------------------

#include "../uart/uart1.h"

void main(){
    // intitialize UART
    uart_init();
    
    // say hello
    uart_puts("Hello World!\n");

    // echo everything back 
    while(1) {
    	//read each char
    	char c = uart_getc();

    	//send back twice
    	uart_sendc(c);
    	uart_sendc(c);
    }
}
