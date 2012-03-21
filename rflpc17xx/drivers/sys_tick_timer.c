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
  Time-stamp: <2012-03-21 09:08:17 (hauspie)>
*/
#ifdef RFLPC_ENABLE_SYS_TICK_TIMER

#include "sys_tick_timer.h"
#include "../clock.h"
#include "../tinylibc/printf.h"

void rflpc_sys_timer_init()
{
    /* Enable the timer */
    SysTick->CTRL |= 1;

    /* Set the timer to use CPU clock */
    SysTick->CTRL |= 1 << 2;
}

void rflpc_sys_timer_set_period(uint32_t micros_time)
{
    uint32_t value;


    /*
      Each clock tick decrement the counter by one.
      Thus, the value must be computed so that it holds
      the number of clock ticks in mstime milliseconds
     */

    value = rflpc_clock_get_system_clock() / 1000000; /* number of clock ticks per microseconds */
    value *= micros_time; /* number of microseconds to wait */
    value -= 1; /* the interrupt is generated when counter is equal to 0, so to
		   count X ticks, we count from X-1 to 0
		*/
    rflpc_sys_timer_set_tick_period(value);
}


void rflpc_sys_timer_set_tick_period(uint32_t ticks)
{
    /* Turn the timer off */
    SysTick->CTRL &= ~1UL;

    /* Set the current timer value and the reload value */
    SysTick->VAL = ticks;
    SysTick->LOAD = ticks;

    /* And back on */
    SysTick->CTRL |= 1;
    
}
#endif /* ENABLE_SYS_TICK_TIMER */
