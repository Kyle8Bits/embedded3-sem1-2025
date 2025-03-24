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
    AUX_MU_BAUD = 542;   //configure 57600 baud rate [system_clk_freq/(baud_rate*8) - 1]

    /* Note: refer to page 11 of ARM Peripherals guide for baudrate configuration 
    (system_clk_freq is 250MHz by default) */

    /* map UART1 to GPIO pins 32 and 33 */
    r = GPFSEL3;
    r &=  ~( (7 << 6)|(7 << 9) ); //clear bits 11-6 (FSEL33, FSEL32)
    r |= (0b010 << 6)|(0b010 << 9);   //set value 0b010 (select ALT5: TXD1/RXD1)
    GPFSEL3 = r;

	/* enable GPIO 32, 33 */
#ifdef RPI3 //RPI3
	GPPUD = 0;            //No pull up/down control
	//Toogle clock to flush GPIO setup
	r = 150; while(r--) { asm volatile("nop"); } //waiting 150 cycles
	GPPUDCLK1 = (1 << 0)|(1 << 1); //enable clock for GPIO 32, 33
	r = 150; while(r--) { asm volatile("nop"); } //waiting 150 cycles
	GPPUDCLK1 = 0;        // flush GPIO setup

#else //RPI4
	r = GPIO_PUP_PDN_CNTRL_REG2;
	r &= ~((3 << 0) | (3 << 2)); //No resistor is selected for GPIO 32, 33
	GPIO_PUP_PDN_CNTRL_REG2 = r;
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
