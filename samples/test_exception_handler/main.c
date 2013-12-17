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
  Time-stamp: <2013-12-17 17:04:55 (hauspie)>
*/
#include <rflpc17xx/rflpc17xx.h>

RFLPC_IRQ_HANDLER svc_handler()  __attribute__( ( naked ) );
RFLPC_IRQ_HANDLER svc_handler()
{
    uint32_t *sp;
    RFLPC_ARM_GET_REGISTER(sp,(uint32_t)sp);
    printf("SVC Handler\r\n");
    printf("SVC instruction address %p\r\n", sp[6]-2);
    printf("SVC call param: 0x%x\r\n", *((uint16_t*)(sp[6]-2)) & 0xff);
    RFLPC_DUMP_STACK();
}


void func(void)
{
    __ASM volatile ("svc 0x32\n");
    __ASM volatile ("nop\n");
    __ASM volatile ("nop\n");
    __ASM volatile ("nop\n");
    __ASM volatile ("nop\n");
    __ASM volatile ("nop\n");
    __ASM volatile ("nop\n");
    __ASM volatile ("nop\n");
    __ASM volatile ("nop\n");
    __ASM volatile ("nop\n");
    __ASM volatile ("nop\n");
    __ASM volatile ("nop\n");
    *((int*)0) = 0;
    __ASM volatile ("nop\n");
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
