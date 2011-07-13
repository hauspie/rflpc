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
  Time-stamp: <2011-07-13 14:25:39 (hauspie)>
*/
#ifndef __RFLPC_SYS_TICK_TIMER_H__
#define __RFLPC_SYS_TICK_TIMER_H__

/* This driver allows the use of the system tick timer.
   
   This timer is intended to generate a precise 10ms timer, however it can be
   used to count different times. But, the tick count is only on 24bits so, 
   at 96Mhz, the maximum wait time is 2^24 ticks (around 170ms)

   Its counter is decremented at each clock tick, and when the counter reaches
   0, an interrupt can be generated. The time between interrupts thus depends
   on the counter value and the clock speed.

   This driver uses the cpu clock as the reference clock for the timer. 

   user manual p. 504
*/

#include "../interrupt.h"
#include "../LPC17xx.h"

/** initializes the system timer */
extern void rflpc_sys_timer_init();

/** starts the system timer */
static inline void rflpc_sys_timer_start()
{
    SysTick->CTRL |= 1;
}

/** stops the system timer */
static inline void rflpc_sys_timer_stop()
{
    SysTick->CTRL &= ~1UL;
}

/** sets the callback for the timer interrupt */
static inline void rflpc_sys_timer_set_callback(rflpc_irq_handler_t c)
{
    /* Enable systick interrupt */
    SysTick->CTRL |= (1 << 1);
    
    rflpc_irq_set_handler(SysTick_IRQn, c);
}

/** initializes the timer with a given period (in µs) 
    This automatically starts the timer even if it was stopped.
 */
extern void rflpc_sys_timer_set_period(uint32_t micros_time);

/** initializes the timer with a given tick count 
    This automatically starts the timer even if it was stopped.
 */
extern void rflpc_sys_timer_set_tick_period(uint32_t ticks);

#endif
