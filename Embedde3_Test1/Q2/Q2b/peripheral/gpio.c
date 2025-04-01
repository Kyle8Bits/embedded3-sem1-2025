#include "../header/gpio.h"
#include "../header/uart0.h"

void delay(){
    //Have some delay
    unsigned int count = 1000000;
    while (count--) { asm volatile("nop"); } //waiting 1000000 cycles
}

void led_blink() {
    uart_puts("Running LED blinking program....\n");

    //Configure GPIO Pin 3 as an output (set bit 9 in GPFSEL0)
    GPFSEL0 |= 1 << 9;
    
    while (1){
        //Turn the LED at GPIO Pin 3 ON (set bit 3 in GPSET0)
        GPSET0 |= 1 << 3;

        //Have some delay
        delay();

        //Turn the LED at GPIO Pin 3 OFF (set bit 3 in GPCLR0)
        GPCLR0 |= 1 << 3;

        //Have some delay
        delay();
    }
}