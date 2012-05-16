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
  Created: 
  Time-stamp: <2011-09-12 17:14:03 (hauspie)>
*/
#include <stdint.h>
#include "config/config.h"

#include "drivers/leds.h"

#include "clock.h"
#include "interrupt.h"
#include "debug.h"


/* These symbols are defined by the linker script.
   They are used to find bounds of text, data and bss sections in flash/ram
*/
extern char _data_start;
extern char _data_end;
extern char _text_end;
extern char _bss_start;
extern char _bss_end;

/* reset routine */
void _start(void);
/* default handlers */
extern RFLPC_IRQ_HANDLER _default_exception_handler();
extern RFLPC_IRQ_HANDLER _default_peripheral_handler();


/** This array will contain the stack. It is forced to be in the .stack section
 * so that the linker script can locate it where we want */
unsigned char _stack[RFLPC_STACK_SIZE] __attribute__((section(".stack")));

/** This array is the rom interrupt vector. It is force to be located in the
 * .interrupt_vector section so that the linker script puts it at address 0 */

/* see section 34.3.3 p 745 of the user manual */

void* _rom_interrupts[RFLPC_IRQn_COUNT] __attribute__ ((section(".interrupt_vector"))) = {
    _stack + RFLPC_STACK_SIZE,   /* initial SP value */
    _start, /* address of the reset routine */
    _default_exception_handler,	/* Non Masquable Interrupt handler, */
    _default_exception_handler,	/* Hardware Fault handler, */
    _default_exception_handler,	/* Memory protection unit exception handler, */
    _default_exception_handler,	/* Bus fault handler, */
    _default_exception_handler,	/* Usage fault handler, */
    0,			/* Reserved */
    0,			/* Reserved */
    0,			/* Reserved */
    0,			/* Reserved */
    _default_exception_handler,	/* Supervisor mode call handler, */
    0,			/* Reserved */
    0,			/* Reserved */
    _default_exception_handler,	/* pend sv handler (c.f. p746 of user manual) */
    _default_exception_handler,	/* system timer tick handler (generated when the timer reaches 0) */
/* From	now, all interrupts are external IRQ for peripherals */
    _default_peripheral_handler,	/* Watchdog timer */
    _default_peripheral_handler,	/* Timer0 */
    _default_peripheral_handler,	/* Timer1 */
    _default_peripheral_handler,	/* Timer2 */
    _default_peripheral_handler,	/* Timer3 */
    _default_peripheral_handler,	/* UART0 */
    _default_peripheral_handler,	/* UART1 */
    _default_peripheral_handler,	/* UART2 */
    _default_peripheral_handler,	/* UART3 */
    _default_peripheral_handler,	/* PWM1 */
    _default_peripheral_handler,	/* I2C0 */
    _default_peripheral_handler,	/* I2C1 */
    _default_peripheral_handler,	/* I2C2 */
    _default_peripheral_handler,	/* SPI */
    _default_peripheral_handler,	/* SSP0 */
    _default_peripheral_handler,	/* SSP1 */
    _default_peripheral_handler,	/* PLL0 Lock */
    _default_peripheral_handler,	/* Real Time Clock */
    _default_peripheral_handler,	/* External Interrupt 0 */
    _default_peripheral_handler,	/* External Interrupt 1 */
    _default_peripheral_handler,	/* External Interrupt 2 */
    _default_peripheral_handler,	/* External Interrupt 3 */
    _default_peripheral_handler,	/* Analogic Digital Converter */
    _default_peripheral_handler,	/* Brown-Out detect */
    _default_peripheral_handler,	/* USB */
    _default_peripheral_handler,	/* CAN */
    _default_peripheral_handler,	/* General Purpose DMA */
    _default_peripheral_handler,	/* I2S */
    _default_peripheral_handler,	/* Ethernet */
    _default_peripheral_handler,	/* Repetitive Interrupt Timer */
    _default_peripheral_handler,	/* Motor Control PWM */
    _default_peripheral_handler,	/* Quadrature Encoder */
    _default_peripheral_handler,	/* PLL1 Lock */	
};


/* Set all bss to zero */
static void _zero_bss(void)
{
    char *dst = &_bss_start;
    while (dst < &_bss_end)
	*dst++ = 0;
}

/* Copy data section from flash to ram */
static void _copy_data_section(void)
{
    char *src = &_text_end;
    char *dst = &_data_start;

    while (dst < &_data_end)
	*dst++ = *src++;
}

void _init_stack()
{
    static int i;
    for (i = 0 ; i < RFLPC_STACK_SIZE/4 ; ++i)
    {
	((uint32_t *)_stack)[i] = 0xFADEBEEF;
    }
}


extern void main(void);

void _start(void)
{
    /* clear stack with magic */    
    _copy_data_section();
    _zero_bss();
    /* set the clock to 96 Mhz */
    rflpc_clock_init();
    /* Copy the rom interrupt vector to ram and relocate it */
    rflpc_irq_init();
    /* start main program */
    _init_stack();
    main();
}
