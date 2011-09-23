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
  Time-stamp: <2011-09-23 11:12:02 (hauspie)>
*/
#include <rflpc17xx/memcpy.h>
#include <rflpc17xx/drivers/timer.h>
#include <rflpc17xx/drivers/uart.h>

#define ARRAY_SIZE 256
#define TEST_SIZE (ARRAY_SIZE - 3)

uint8_t src_buffer[ARRAY_SIZE];
uint8_t dst_buffer[ARRAY_SIZE];


void init_timer()
{
    rflpc_timer_set_clock(RFLPC_TIMER0, RFLPC_CCLK);
    rflpc_timer_set_pre_scale_counter(RFLPC_TIMER0, 10);
    rflpc_timer_enable(RFLPC_TIMER0);
    rflpc_timer_start(RFLPC_TIMER0);
}

int putchar(int c)
{
    rflpc_uart0_putchar(c);
    return c;
}

int main()
{
    int i;
    uint32_t before;
    uint32_t after;

    rflpc_uart0_init();
    init_timer();


    for (i = 0 ; i < TEST_SIZE ; ++i)
	src_buffer[i] = i;

    printf("%p %p\r\n", src_buffer, dst_buffer);

    /* test aligned copy */
    before = rflpc_timer_get_counter(RFLPC_TIMER0);
    memcpy(dst_buffer, src_buffer, TEST_SIZE);
    after = rflpc_timer_get_counter(RFLPC_TIMER0);

    printf("aligned copy time: %d\r\n", after - before);

    for (i = 0 ; i < TEST_SIZE ; ++i)
    {
	if (dst_buffer[i] != i)
	{
	    printf("Bad copy starting at %d\r\n", i);
	    break;
		
	}
	dst_buffer[i] = 0; /* prepare for next test */
    }
    
    
    /* test unaligned copy */
    before = rflpc_timer_get_counter(RFLPC_TIMER0);
    memcpy(dst_buffer + 1, src_buffer + 1, TEST_SIZE-1);
    after = rflpc_timer_get_counter(RFLPC_TIMER0);
    printf("unaligned copy time: %d\r\n", after - before);

    for (i = 1 ; i < TEST_SIZE ; ++i)
    {
	if (dst_buffer[i] != i)
	{
	    printf("Bad copy starting at %d\r\n", i);
	    break;
		
	}
	dst_buffer[i] = 0; /* prepare for next test */
    }


    while (1);

    return 0;
}
