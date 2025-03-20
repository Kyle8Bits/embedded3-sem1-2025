// -----------------------------------uart.c -------------------------------------
#include "uart.h"
#include "gpio.h"
/**
 * Set baud rate and characteristics (115200 8N1) and map to GPIO
 */
void uart_init()
{
    unsigned int r;
    /* initialize UART */
    AUX_ENABLE |= 1;   // enable mini UART (UART1)
    AUX_MU_CNTL = 0;   // stop transmitter and receiver
    AUX_MU_LCR = 3;    // 8-bit mode (also enable bit 1 to be used for RBP3)
    AUX_MU_MCR = 0;    // clear RTS (request to send)
    AUX_MU_IER = 0;    // disable interrupts
    AUX_MU_IIR = 0xc6; // enable and clear FIFOs
    AUX_MU_BAUD = 270; // configure 115200 baud [system_clk_freq/(baud_rate*8) - 1]
    /* map UART1 to GPIO pins 14 and 15 */
    r = GPFSEL1;
    r &= ~((7 << 12) | (7 << 15)); // clear bits 17-12 (FSEL15, FSEL14)
    r |= (2 << 12) | (2 << 15);    // set value 2 (select ALT5: TXD1/RXD1)
    GPFSEL1 = r;
    /* enable GPIO 14, 15 */
    GPPUD = 0; // No pull up/down control
    r = 150;
    while (r--)
    {
        asm volatile("nop");
    } // waiting 150 cycles
    GPPUDCLK0 = (1 << 14) | (1 << 15); // enable clock for GPIO 14, 15
    r = 150;
    while (r--)
    {
        asm volatile("nop");
    } // waiting 150 cycles
    GPPUDCLK0 = 0;   // flush GPIO setup
    AUX_MU_CNTL = 3; // enable transmitter and receiver (Tx, Rx)
}

/**
 * Send a character
 */
void uart_sendc(char c)
{
    // wait until transmitter is empty
    do
    {
        asm volatile("nop");
    } while (!(AUX_MU_LSR & 0x20));
    // write the character to the buffer
    AUX_MU_IO = c;
}
/**
 * Receive a character
 */
char uart_getc()
{
    char c;
    // wait until data is ready (one symbol)
    do
    {
        asm volatile("nop");
    } while (!(AUX_MU_LSR & 0x01));
    // read it and return
    c = (unsigned char)(AUX_MU_IO);
    // convert carriage return to newline character
    return (c == '\r' ? '\n' : c);
}
/**
 * Display a string
 */
void uart_puts(char *s)
{
    while (*s)
    {
        // convert newline to carriage return + newline
        if (*s == '\n')
            uart_sendc('\r');
        uart_sendc(*s++);
    }
}