#include "uart1.h"

/**
 * Set baud rate and characteristics and map to GPIO
 */
void uart_init()
{
    unsigned int r;

    /* initialize UART */
    AUX_ENABLE |= 1;     //enable mini UART (UART1) 
    AUX_MU_CNTL = 0;	 //stop transmitter and receiver
    AUX_MU_LCR  = 3;     //8-bit mode (also enable bit 1 to be used for RPI3)
    AUX_MU_MCR  = 0;	 //clear RTS (request to send)
    AUX_MU_IER  = 0;	 //disable interrupts
    AUX_MU_IIR  = 0xc6;  //enable and clear FIFOs
    AUX_MU_BAUD = 270;   //configure 115200 baud rate [system_clk_freq/(baud_rate*8) - 1]

    /* Note: refer to page 11 of ARM Peripherals guide for baudrate configuration 
    (system_clk_freq is 250MHz by default) */

    /* map UART1 to GPIO pins 14 and 15 */
    r = GPFSEL1;
    r &=  ~( (7 << 12)|(7 << 15) ); //clear bits 17-12 (FSEL15, FSEL14)
    r |= (0b010 << 12)|(0b010 << 15);   //set value 0b010 (select ALT5: TXD1/RXD1)
    GPFSEL1 = r;

	/* enable GPIO 14, 15 */
#ifdef RPI3 //RPI3
	GPPUD = 0;            //No pull up/down control
	//Toogle clock to flush GPIO setup
	r = 150; while(r--) { asm volatile("nop"); } //waiting 150 cycles
	GPPUDCLK0 = (1 << 14)|(1 << 15); //enable clock for GPIO 14, 15
	r = 150; while(r--) { asm volatile("nop"); } //waiting 150 cycles
	GPPUDCLK0 = 0;        // flush GPIO setup

#else //RPI4
	r = GPIO_PUP_PDN_CNTRL_REG0;
	r &= ~((3 << 28) | (3 << 30)); //No resistor is selected for GPIO 14, 15
	GPIO_PUP_PDN_CNTRL_REG0 = r;
#endif

    AUX_MU_CNTL = 3;      //enable transmitter and receiver (Tx, Rx)
}

/**
 * Send a character
 */
void uart_sendc(char c) {
    // wait until transmitter is empty
    do {
    	asm volatile("nop");
    } while ( !(AUX_MU_LSR & 0x20) );

    // write the character to the buffer 
    AUX_MU_IO = c;
}

/**
 * Receive a character
 */
char uart_getc() {
    char c;

    // wait until data is ready (one symbol)
    do {
    	asm volatile("nop");
    } while ( !(AUX_MU_LSR & 0x01) );

    // read it and return
    c = (unsigned char)(AUX_MU_IO);

    // convert carriage return to newline character
    return (c == '\r' ? '\n' : c);
}

/**
 * Display a string
 */
void uart_puts(char *s) {
    while (*s) {
        // convert newline to carriage return + newline
        if (*s == '\n')
            uart_sendc('\r');
        uart_sendc(*s++);
    }
}
