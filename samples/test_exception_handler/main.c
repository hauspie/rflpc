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
  Time-stamp: <2013-12-17 17:34:45 (hauspie)>
*/
#include <rflpc17xx/rflpc17xx.h>

/* The naked attribute is needed so that the stack frame is clean
   when entering the handler. This is the only way to get
   the address of the svc instruction responsible for the exception 
   in a consitent way
*/
RFLPC_IRQ_HANDLER svc_handler()  __attribute__( ( naked ) );
RFLPC_IRQ_HANDLER svc_handler()
{
    uint32_t *sp;
    /* Get the stack pointer first to get the real stack frame */
    RFLPC_ARM_GET_REGISTER(sp,(uint32_t)sp);
    /* As the function is naked, we have to save registers ourselves. Push all
     * as we do not really know which registers will be used by the function */
    __ASM volatile ("push {r0-r12,lr}"); 

    printf("SVC Handler\r\n");
    /* sp[6] holds the return address of the svc call, SVC is a two bytes
     * intruction so sp[6]-2 is the address of the svc instruction
     */
    printf("SVC instruction address %p\r\n", sp[6]-2);
    printf("PC instruction address %p\r\n", sp[6]);
    printf("SVC call param: 0x%x\r\n", *((uint16_t*)(sp[6]-2)) & 0xff);
    RFLPC_DUMP_STACK();
    __ASM volatile ("pop {r0-r12,pc}"); /* this generates a return by poping the old lr value into PC */
}


void func(void)
{
    printf("Making a svc call\r\n");
    __ASM volatile ("svc 0x32\n");
    printf("Returned from svc\r\n");
    printf("Will generate a fault now\r\n");
    *((int*)0) = 0;
    __ASM volatile ("nop\n");
    __ASM volatile ("nop\n");
    __ASM volatile ("nop\n");
    __ASM volatile ("nop\n");
    __ASM volatile ("nop\n");
    __ASM volatile ("nop\n");
    __ASM volatile ("nop\n");
    
}

int main()
{
    rflpc_uart_init(RFLPC_UART0);
    rflpc_led_init();
    rflpc_irq_set_handler(SVCall_IRQn, svc_handler);
    printf("Main addr: %p\r\n", main);
    func();

    while (1);
    return 0;
}
