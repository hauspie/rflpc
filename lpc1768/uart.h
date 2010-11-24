#ifndef __LPC1768_UART_H__
#define __LPC1768_UART_H__

#include "leds.h"


/** UART0 Line Control Register */
#define LPC_U0LCR (*((uint32_t*) 0x4000C00C))
/** UART0 Line Status Register */
#define LPC_U0LSR (*((uint32_t*) 0x4000C014))

/** UART0 Divisor Latch LSB Register */
#define LPC_U0DLL (*((uint32_t*) 0x4000C000))
/** UART0 Divisor Latch MSB Register */
#define LPC_U0DLM (*((uint32_t*) 0x4000C004))
/** UART0 Fractional Divider Register */
#define LPC_U0FDR (*((uint32_t*) 0x4000C028))
/** UART0 FIFO Control Register */
#define LPC_U0FCR (*((uint32_t*) 0x4000C008))

/** UART0 Transmit Hold Register */
#define LPC_U0THR (*((uint32_t*) 0x4000C000))


/* Inits the UART, using 115200 baud, 8 bits data, no parity and 1 stop bit */

/* returns 0 if successful, -1 otherwise */

extern int lpc_uart0_init();

/** Sends a byte to the uart0 */
static inline void lpc_uart0_putchar(char c)
{
    /* Wait for THR to be empty before sending byte (p. 307)*/
    
    while (!(LPC_U0LSR & (0x1UL << 5)));
    LPC_U0THR = c & 0xFF;
}

#endif
