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
  Time-stamp: <2012-03-21 09:38:35 (hauspie)>
*/
#include "nxp/LPC17xx.h" /* for IRQn enum */
#include "nxp/core_cm3.h"
#include "config.h"
#include "interrupt.h"

#ifndef RFLPC_CONFIG_ENABLE_PRINTF
#undef RFLPC_IRQ_DEBUG_ENABLE /* Can't debug without printf */
#endif

#ifdef RFLPC_IRQ_DEBUG_ENABLE
#include "tinylibc/printf.h"
#include "debug.h"
#endif

/* use a special section to put at start of ram (see link script) */
static volatile rflpc_irq_handler_t _ram_interrupts[RFLPC_IRQn_COUNT] __attribute__ ((section(".ram_int_vector")));
/* this array is defined in init.c and is the rom interrupt vector */
extern void* _rom_interrupts[RFLPC_IRQn_COUNT];

RFLPC_IRQ_HANDLER _default_exception_handler()
{
#ifdef RFLPC_IRQ_DEBUG_ENABLE
    printf("CFSR: %0x\r\n", SCB->CFSR);
    printf("HFSR: %0x\r\n", SCB->HFSR);
    printf("DFSR: %0x\r\n", SCB->DFSR);
    if (SCB->CFSR & (1UL << 7)) /* Memory Management Fault Address Register Valid */
       printf("Mem Managment Fault Adress: %p\r\n", SCB->MMFAR);
    if (SCB->CFSR & (1UL << 4)) /* MSTKERR */
       printf("Memory Fault caused by stacking for exception entry\r\n");
    if (SCB->CFSR & (1UL << (3))) /* MUNSTKERR */
       printf("Memory Fault caused by unstacking for return from exception\r\n");
    if (SCB->CFSR & (1UL << (1))) /* DACCVIOL */
       printf("Data Access Violation. PC value stacked point to the faulting instruction\r\n");
    if (SCB->CFSR & (1UL << (0))) /* IACCVIOL */
       printf("Instruction Access Violation. PC value stacked point to the faulting instruction\r\n");
    if (SCB->CFSR & (1UL << (8+7))) /* BFARVALID */
       printf("Bus fault with known address: %p\r\n", SCB->BFAR);
    if (SCB->CFSR & (1UL << (8+4))) /* STKERR */
       printf("Bus fault caused by stacking for exception entry\r\n");
    if (SCB->CFSR & (1UL << (8+3))) /* UNSTKERR */
       printf("Bus fault caused by unstacking for return from exception\r\n");
    if (SCB->CFSR & (1UL << (8+2))) /* IMPRECISERR */
       printf("Imprecise bus fault\r\n");
    if (SCB->CFSR & (1UL << (8+1))) /* PRECISERR */
       printf("Precise bus fault\r\n");
    if (SCB->CFSR & (1UL << (8+0))) /* IBUSERR */
       printf("Intruction bus error\r\n");

    if (SCB->CFSR & (1UL << (16+9))) /* DIVZERO */
       printf("Division by 0\r\n");
    if (SCB->CFSR & (1UL << (16+8))) /* UNALIGNED */
       printf("Unaligned access usage\r\n");
    if (SCB->CFSR & (1UL << (16+3))) /* NOCP */
       printf("Fault while accessing to a coprocessor\r\n");
    if (SCB->CFSR & (1UL << (16+2))) /* INVPC */
       printf("Invalid PC Load\r\n");
    if (SCB->CFSR & (1UL << (16+1))) /* INVSTATE */
       printf("The processor has attempted to execute an instruction that makes illegal use of the EPSR\r\n");
    if (SCB->CFSR & (1UL << (16+0))) /* UNDEFINSTR */
       printf("The processor has attempted to execute an undefined instruction\r\n");

    if (SCB->HFSR & (1UL << (30))) /* FORCED */
       printf("Forced hardware fault\r\n");
    if (SCB->HFSR & (1UL << (1))) /* FORCED */
       printf("Bus fault on vector table read\r\n");
    /* Stack frame when entering exception is:
     * R0-R3, R12 [0->4]
     * PC [5]
     * PSR [6]
     * LR [7]
     */
    printf("PC value at exception: %p\r\n", ((uint32_t*)__get_MSP())[5+4]); /* +4 is because GCC pushes r0,r4,r5 and lr on the stack */
    RFLPC_DUMP_STACK();
    /* stops the execution with a O--O <-> -OO- led pattern. */
    RFLPC_STOP(RFLPC_LED_1|RFLPC_LED_4, 2000000);
#endif
}

RFLPC_IRQ_HANDLER _default_peripheral_handler()
{
#ifdef RFLPC_IRQ_DEBUG_ENABLE
    /* stops the execution with a --OO <-> OO-- led pattern. */
    RFLPC_STOP(RFLPC_LED_1|RFLPC_LED_2, 2000000);
#endif
}

void rflpc_irq_init()
{
    /* copy the interrupt vector */
    int i;
    for (i = 0 ; i < RFLPC_IRQn_COUNT ; ++i)
    {
	_ram_interrupts[i] = (rflpc_irq_handler_t) _rom_interrupts[i];
    }

    /* Set the VTOR register to remap the interrupt vector */
    rflpc_irq_global_disable();
    SCB->VTOR = (uint32_t) _ram_interrupts;
    rflpc_irq_global_enable();
}

void rflpc_irq_set_handler(IRQn_Type irq, rflpc_irq_handler_t handler)
{
    /* +16 because of the system exception handler enum starting at -14 */
    _ram_interrupts[irq+16] = handler;
}
