/* This file is part of rflpc. Copyright 2010-2011 Michael Hauspie                        
 *									 
 * rflpc is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by	 
 * the Free Software Foundation, either version 3 of the License, or	 
 * (at your option) any later version.					 
 * 									 
 * rflpc is distributed in the hope that it will be useful,		 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of	 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the	 
 * GNU General Public License for more details.				 
 * 									 
 * You should have received a copy of the GNU General Public License	 
 * along with rflpc.  If not, see <http://www.gnu.org/licenses/>.	 
 */

/*
  Author: Michael Hauspie <michael.hauspie@univ-lille1.fr>
  Contributor: Francois Serman <francois.serman@inria.fr>
*/

#ifdef RFLPC_CONFIG_ENABLE_UART

#include <stdint.h>
#include <stdlib.h>
#include "../clock.h"
#include "../interrupt.h"
#include "../pinconf.h"
#include "../debug.h"

#include "uart.h"

struct uart_pin_conf
{
  /* (p. 108 and p. 299) */
  struct {
      uint8_t tx_pin;
      uint8_t rx_pin;
      unsigned char pin_function:2;
  }gpio;  

  LPC_UART_TypeDef *base_address;
};

typedef struct uart_pin_conf uart_pin_conf_t;

static uart_pin_conf_t _rflpc_uart_config[4] = 
  {
    { 
      { RFLPC_UART0_TXD_PIN, RFLPC_UART0_RXD_PIN, RFLPC_UART0_PIN_FUNCTION },
      (LPC_UART_TypeDef*)LPC_UART0,
    },
    { 
      { 0, 0, 0 },      
      NULL,
    },
    { 
      { RFLPC_UART2_TXD_PIN, RFLPC_UART2_RXD_PIN, RFLPC_UART2_PIN_FUNCTION },
      LPC_UART2,
    },
    { 
      { RFLPC_UART3_TXD_PIN, RFLPC_UART3_RXD_PIN, RFLPC_UART3_PIN_FUNCTION },
      LPC_UART3,
    }
      
  };


#define BASE_ADDR(n) (_rflpc_uart_config[(n)].base_address)

void rflpc_uart_putchar(rflpc_uart_t uart_num, char c)
{
  while (!(BASE_ADDR(uart_num)->LSR & (0x1UL << 5))); /* why << 5 here and not in available? */
  BASE_ADDR(uart_num)->THR = c & 0xFF;
}

int rflpc_uart_byte_available(rflpc_uart_t uart_num)
{
  return BASE_ADDR(uart_num)->LSR & 0x1UL;
}

char rflpc_uart_getchar(rflpc_uart_t uart_num)
{
  while (!rflpc_uart_byte_available(uart_num));
  return BASE_ADDR(uart_num)->RBR & 0xFF;
}


/* For now, we suppose that the CCLK is 96 Mhz. Thus,
   we set PCLK to 12 Mhz by setting it to CCLK/8.
   Then, (p. 315) we have to set 
   DIVADDVAL = 5
   MULVAL = 8
   DLM = 0
   DLL = 4
   UART rate will then be set to 115384
*/

int _rflpc_uart_init(uart_pin_conf_t *uart)
{
   
  /* Set line control values (p. 306) */
  /* 8 data bits, no parity, 1 stopbit */
  /* 3 is for 8 data bits, 7 is to enable DLL and DLM registers (for selecting baud rate) */
  uart->base_address->LCR = (3 | (1 << 7));

  /* Set DLL=4, DLM=0 (p. 315) */
  uart->base_address->DLL = 4;
  uart->base_address->DLM = 0;

  /* Set DIVADDVAL = 5 and MULVAL = 8 (p. 315) */
  uart->base_address->FDR = 5 | (8 << 4);

  /* Enable Fifo and clear TX & RX (p. 305) */
  uart->base_address->FCR = 7;

  /* Set pin mode for the UART to UART0 (TXD0, RXD0)  */
  /* CHANGEME */
  rflpc_pin_set(uart->gpio.rx_pin, uart->gpio.pin_function, 0, 0); /** @todo function value bad for UART3 */
  rflpc_pin_set(uart->gpio.tx_pin, uart->gpio.pin_function, 0, 0);

  /* Reset the DLAB bit in U0LCR to enable access to transmit and receive registers (p. 301) */
  /* CHECKME */
  uart->base_address->LCR &= ~(0x1UL << 7);

  return 0;
}

int rflpc_uart_init(rflpc_uart_t uart_num)
{

  /* Set the peripheral clock to 12 Mhz */
  /* First, check if system clock is 96 Mhz */
  if (rflpc_clock_get_system_clock() != 96000000)
    return -1;


  switch (uart_num)
  {
    /* Set UART CLOCK to 12 Mhz */
    /* 12 Mhz because of the frequency at 96 Mhz */
    case RFLPC_UART0:
      /* Enable UART (user manual, p. 63) */
      LPC_SC->PCONP |= (1UL << 3);
      /* Set UART clock Bits 6 and 7 are for UART3, 0x3 value is for CCLK/8 */
      LPC_SC->PCLKSEL0 |= (RFLPC_CCLK_8 << 6);
      break;
    case RFLPC_UART2:
      /* Enable UART (user manual, p. 63) */
      LPC_SC->PCONP |= (1UL << 24);
      /* Bits 16 and 17 are for UART3, 0x3 value is for CCLK/8 */
      LPC_SC->PCLKSEL1 |= (RFLPC_CCLK_8 << 16);
      break;
    case RFLPC_UART3:
      /* Enable UART (user manual, p. 63) */
      LPC_SC->PCONP |= (1UL << 25);
      /* Bits 18 and 19 are for UART3, 0x3 value is for CCLK/8 */
      LPC_SC->PCLKSEL1 |= (RFLPC_CCLK_8 << 18);
      break;
    default:
      return -1;
  }

  return _rflpc_uart_init(&_rflpc_uart_config[uart_num]);
}



void rflpc_uart_set_rx_callback(rflpc_uart_t uart_num, rflpc_irq_handler_t  callback)
{
    RFLPC_ASSERT(uart_num != 1);
    /* set the uart0 interrupt handler */
    rflpc_irq_set_handler(UART0_IRQn + uart_num, callback);
    /* enable the interrupt vector */
    rflpc_irq_enable(UART0_IRQn + uart_num);
    /* enable the uart0 irq generation (user manual p. 302) */
    BASE_ADDR(uart_num)->IER |= 1;
}

#endif /* ENABLE_UART */
