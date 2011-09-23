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
  Time-stamp: <2011-09-23 16:40:00 (hauspie)>
*/
#include <rflpc17xx/memcpy.h>
#include <rflpc17xx/drivers/timer.h>
#include <rflpc17xx/drivers/uart.h>

#define ARRAY_SIZE 512

uint8_t src_buffer[ARRAY_SIZE];
uint8_t dst_buffer[ARRAY_SIZE];


int test(uint8_t *src, uint8_t *dst, uint32_t size)
{
    uint32_t i, before, after;
    uint8_t val = 0;
    
    for (i = 0 ; i < size ; ++i)
	src[i] = val++;

    before = rflpc_timer_get_counter(RFLPC_TIMER0);
    memcpy(dst, src, size);
    after = rflpc_timer_get_counter(RFLPC_TIMER0);

    for (i = 0 ; i < size ; ++i)
    {
	if (dst[i] != src[i])
	{
	    printf("Failed test for %d bytes at %d value is %d instead of %d\r\n", size, i, dst_buffer[i], val);
	    return -1;
	}
    }
    return after - before;
}


void fill_with_magic()
{
    uint32_t *ptr;
    int i;
    
    ptr = (uint32_t*)src_buffer;
    for (i = 0 ; i < ARRAY_SIZE / 4 ; ++i)
	*ptr++ = 0xCAFEDECA;
    ptr = (uint32_t*)dst_buffer;
    for (i = 0 ; i < ARRAY_SIZE / 4 ; ++i)
	*ptr++ = 0xFADEBEEF;
}

void dump()
{
    uint32_t *ptr;
    int i;
    ptr = (uint32_t*)src_buffer;
    for (i = 0 ; i < ARRAY_SIZE / 4 ; ++i)
    {
	if (*ptr != 0xCAFEDECA)
	    printf("s: %d %x\r\n", i, *ptr);
	ptr++;
    }
    ptr = (uint32_t*)dst_buffer;
    for (i = 0 ; i < ARRAY_SIZE / 4 ; ++i)
    {
	if (*ptr != 0xFADEBEEF)
	    printf("d: %d %x\r\n", i, *ptr);
	ptr++;
    }
 }

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

    rflpc_uart0_init();
    init_timer();

    fill_with_magic();

    test(src_buffer, dst_buffer, 100);


    dump();



    while (1);

    return 0;
}
