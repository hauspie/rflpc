#include <stdint.h>
#include "uart.h"
#include "registers.h"
#include "pll.h"
#include "pins.h"


/* For now, we suppose that the CCLK is 96 Mhz. Thus,
   we set PCLK to 12 Mhz by setting it to CCLK/8.
   Then, (p. 315) we have to set 
   DIVADDVAL = 5
   MULVAL = 8
   DLM = 0
   DLL = 4
   UART rate will then be set to 115384
*/

int lpc_uart0_init()
{
    /* Enable UART */
    LPC_PCONP |= (1UL << 3);
    
    /* set the peripheral clock to 12 Mhz */
    /* First, check if system clock is 96 Mhz */
    if (lpc_get_system_clock() != 96000000)
	return -1;

    /* set UART CLOCK to 12 Mhz */
    /* Bits 6 and 7 are for UART0, 0x3 value is for CCLK/8 */
    LPC_PCLKSEL0 |= (0x3 << 6);
    
    /* Set line control values (p. 306) */
    /* 8 data bits, no parity, 1 stopbit */
    /* 3 is for 8 data bits, 7 is to enable DLL and DLM registers (for selecting baud rate) */
    LPC_U0LCR = (3 | (1 << 7));

    /* Set DLL=4, DLM=0 (p. 315) */
    LPC_U0DLL = 4;
    LPC_U0DLM = 0;

    /* Set DIVADDVAL = 5 and MULVAL = 8 (p. 315) */
    LPC_U0FDR = 5 | (8 << 4);

    /* Enable UART0 Fifo and clear TX & RX (p. 305) */
    LPC_U0FCR = 7;

    /* Set pin mode to UART0 (TXD0, RXD0) (p. 108 and p. 299) */
    LPC_PINSEL0 = (LPC_PINSEL0 & ~(0xFUL << 4)) | (5 << 4);

    /* Reset the DLAB bit in U0LCR to enable access to transmit and receive
     * registers (p. 301) */
    LPC_U0LCR &= ~(0x1UL << 7);

    return 0;
}

