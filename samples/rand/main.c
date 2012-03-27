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
  Time-stamp: <2012-03-21 09:40:41 (hauspie)>
*/
#include <rflpc17xx/rflpc17xx.h>


void configure_timer(void)
{
   rflpc_timer_enable(RFLPC_TIMER0);
   rflpc_timer_set_clock(RFLPC_TIMER0,RFLPC_CCLK_8);
   rflpc_timer_set_pre_scale_register(RFLPC_TIMER0, rflpc_clock_get_system_clock()/8000000); /* microsecond timer */
   rflpc_timer_start(RFLPC_TIMER0);
}

void wait(int micros)
{
   uint32_t start = rflpc_timer_get_counter(RFLPC_TIMER0);
   while ((rflpc_timer_get_counter(RFLPC_TIMER0) - start) < micros);
   return;
}


void test_rand()
{
    int i;
    printf("Testing non-reentrant rand with seed 10\r\n");
    srand(10);
    for (i = 0 ; i < 10 ; ++i)
    {	
	printf("%d\r\n", rand());
	wait(1000000);
    }
}

void test_rand_r()
{
    unsigned int seed = 10;
    int i;
    
    printf("Testing reentrant rand_r with seed 10\r\n");
    for (i = 0 ; i < 10 ; ++i)
    {	
	printf("%d\r\n", rand_r(&seed));
	wait(1000000);
    }
    
}

int main()
{    
    rflpc_uart_init(RFLPC_UART0);
    configure_timer();
    printf("Hello World\r\n");
    test_rand();
    test_rand_r();
    while (1);
    return 0;
}
