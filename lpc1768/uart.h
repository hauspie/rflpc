#ifndef __LPC1768_UART_H__
#define __LPC1768_UART_H__

#include "registers.h"

#define LPC_UART0 3
#define LPC_UART1 4
#define LPC_UART2 24
#define LPC_UART3 25

static inline uart_init(int uart)
{
    /* Enable UART */
    LPC_PCONP |= (1UL << uart);
    
}

#endif
