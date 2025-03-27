#include "uart3.h"

/**
 * Set baud rate and characteristics and map to GPIO
 */
void uart_init()
{
    unsigned int r;

	/* Turn off UART3 */
	UART3_CR = 0x0;

	/* Setup GPIO pins 4 and 5 */

	/* Set GPIO4 and GPIO5 to be pl011 TX/RX which is ALT4	*/
	r = GPFSEL0;
	r &=  ~((7 << 12) | (7 << 15)); //clear bits 17-12 (FSEL5, FSEL4)
	r |= (0b011 << 12)|(0b011 << 15);   //Set value 0b011 (select ALT4: TXD3/RXD3)
	GPFSEL0 = r;

	/* enable GPIO 4, 5 */
#ifdef RPI3 //RPI3
	#warning "UART3 is not available on RPI3"
	GPPUD = 0;            //No pull up/down control
	//Toogle clock to flush GPIO setup
	r = 150; while(r--) { asm volatile("nop"); } //waiting 150 cycles
	GPPUDCLK0 = (1 << 4)|(1 << 5); //enable clock for GPIO 4, 5
	r = 150; while(r--) { asm volatile("nop"); } //waiting 150 cycles
	GPPUDCLK0 = 0;        // flush GPIO setup

#else //RPI4
	r = GPIO_PUP_PDN_CNTRL_REG0;
	r &= ~((3 << 8) | (3 << 10)); //No resistor is selected for GPIO 4, 5
	GPIO_PUP_PDN_CNTRL_REG0 = r;
#endif

	/* Mask all interrupts. */
	UART3_IMSC = 0;

	/* Clear pending interrupts. */
	UART3_ICR = 0x7FF;

	/* Set integer & fractional part of Baud rate
	Divider = UART_CLOCK/(16 * Baud)            
	Default UART_CLOCK = 48MHz (old firmware it was 3MHz); 
	Integer part register UART3_IBRD  = integer part of Divider 
	Fraction part register UART3_FBRD = (Fractional part * 64) + 0.5 */

	//115200 baud
	UART3_IBRD = 26;       
	UART3_FBRD = 3;

	/* Set up the Line Control Register */
	/* Enable FIFO */
	/* Set length to 8 bit */
	/* Defaults for other bit are No parity, 1 stop bit */
	UART3_LCRH = UART3_LCRH_FEN | UART3_LCRH_WLEN_8BIT;

	/* Enable UART3, receive, and transmit */
	UART3_CR = 0x301;     // enable Tx, Rx, FIFO
}



/**
 * Send a character
 */
void uart_sendc(char c) {

    /* Check Flags Register */
	/* And wait until transmitter is not full */
	do {
		asm volatile("nop");
	} while (UART3_FR & UART3_FR_TXFF);

	/* Write our data byte out to the data register */
	UART3_DR = c ;
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
    } while ( UART3_FR & UART3_FR_RXFE );

    /* read it and return */
    c = (unsigned char) (UART3_DR);

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
