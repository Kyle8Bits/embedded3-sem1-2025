#include "../header/uart0.h"

/**
 * Set baud rate and characteristics and map to GPIO
 */
void uart_init()
{
    unsigned int r;

	/* Turn off UART0 */
	UART0_CR = 0x0;

	/* Setup GPIO pins 14 and 15 */

	/* Set GPIO14 and GPIO15 to be pl011 TX/RX which is ALT0	*/
	r = GPFSEL3;
	r &=  ~((7 << 6) | (7 << 21)); //clear bits 17-12 (FSEL32, FSEL37)
	r |= (0b111 << 6)|(0b110 << 21);   //Set value 0b100 (select ALT0: TXD0/RXD0)
	GPFSEL1 = r;
	

	/* enable GPIO 32, 37 */
#ifdef RPI3 //RPI3
	GPPUD = 0;            //No pull up/down control
	//Toogle clock to flush GPIO setup
	r = 150; while(r--) { asm volatile("nop"); } //waiting 150 cycles
	GPPUDCLK1 = (1 << 0)|(1 << 5); //enable clock for GPIO 32, 37
	r = 150; while(r--) { asm volatile("nop"); } //waiting 150 cycles
	GPPUDCLK1 = 0;        // flush GPIO setup

#else //RPI4
	r = GPIO_PUP_PDN_CNTRL_REG0;
	r &= ~((3 << 28) | (3 << 30)); //No resistor is selected for GPIO 14, 15
	GPIO_PUP_PDN_CNTRL_REG0 = r;
#endif

	/* Mask all interrupts. */
	UART0_IMSC = 0;

	/* Clear pending interrupts. */
	UART0_ICR = 0x7FF;

	/* Set integer & fractional part of Baud rate
	Divider = UART_CLOCK/(16 * Baud)            
	Default UART_CLOCK = 48MHz (old firmware it was 3MHz); 
	Integer part register UART0_IBRD  = integer part of Divider 
	Fraction part register UART0_FBRD = (Fractional part * 64) + 0.5 */

	//115200 baud
	UART0_IBRD = 156;       
	UART0_FBRD = 16;

	/* Set up the Line Control Register */
	/* Enable FIFO */
	/* Set length to 8 bit */
	/* Defaults for other bit are No parity, 1 stop bit */
	UART0_LCRH = UART0_LCRH_FEN | UART0_LCRH_WLEN_8BIT;

	/* Enable UART0, receive, and transmit */
	UART0_CR = 0x301;     // enable Tx, Rx, FIFO
}



/**
 * Send a character
 */
void uart_sendc(char c) {

    /* Check Flags Register */
	/* And wait until transmitter is not full */
	do {
		asm volatile("nop");
	} while (UART0_FR & UART0_FR_TXFF);

	/* Write our data byte out to the data register */
	UART0_DR = c ;
}

/**
 * Receive a character
 */
char uart_getc() {
    char c = 0;

    /* Check Flags Register */
    /* Wait until Receiver is not empty
     * (at least one byte data in receive fifo)*/
	do {
		asm volatile("nop");
    } while ( UART0_FR & UART0_FR_RXFE );

    /* read it and return */
    c = (unsigned char) (UART0_DR);

    /* convert carriage return to newline */
    return (c == '\r' ? '\n' : c);
}


/**
 * Display a string
 */
void uart_puts(char *s) {
    while (*s) {
        /* convert newline to carriage return + newline */
        if (*s == '\n')
            uart_sendc('\r');
        uart_sendc(*s++);
    }
}
