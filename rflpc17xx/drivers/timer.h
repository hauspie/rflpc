/* This file is part of rflpc.                        
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
  Time-stamp: <2011-09-12 17:29:01 (hauspie)>
*/
#ifndef __RFLPC_TIMER_H__
#define __RFLPC_TIMER_H__

#include "nxp/LPC17xx.h"
#include "clock.h"
#include "tools.h"



/**
   Value to use for the timer value in functions
 */
typedef enum
{
    RFLPC_TIMER0 = 0,
    RFLPC_TIMER1 = 1,
    RFLPC_TIMER2 = 2,
    RFLPC_TIMER3 = 3
} rflpc_timer_t;


static inline LPC_TIM_TypeDef *rflpc_timer_base(rflpc_timer_t timer)
{
    switch (timer)
    {
	case RFLPC_TIMER0: return LPC_TIM0;
	case RFLPC_TIMER1: return LPC_TIM1;
	case RFLPC_TIMER2: return LPC_TIM2;
	case RFLPC_TIMER3: return LPC_TIM3;
	default: break;
    }
    return (LPC_TIM_TypeDef*)0;
}


/** 
 * @brief power and clock the given timer
 * 
 * @param timer_number 
 */
static inline void rflpc_timer_enable(rflpc_timer_t timer)
{
    /* p. 490 and p. 63 */
    switch (timer)
    {
	case RFLPC_TIMER0:
	case RFLPC_TIMER1:
	    LPC_SC->PCONP |= (1UL << (1 + timer));
	    break;
	case RFLPC_TIMER2:
	case RFLPC_TIMER3:
	    LPC_SC->PCONP |= (1UL << (20 + timer));
	    break;
    }
}

/** 
 * @brief disable the given timer
 * 
 * @param timer_number 
 */
static inline void rflpc_timer_disable(rflpc_timer_t timer)
{
    /* p. 490 and p. 63 */
    switch (timer)
    {
	case RFLPC_TIMER0:
	case RFLPC_TIMER1:
	    LPC_SC->PCONP &= ~(1UL << (1 + timer));
	    break;
	case RFLPC_TIMER2:
	case RFLPC_TIMER3:
	    LPC_SC->PCONP &= ~(1UL << (20 + timer));
	    break;
    }
}

/** 
 * @brief Sets the cpu clock divider for a given timer.
 * 
 * @param timer 
 * @param divider @sa ::rflpc_clock_divider_t
 */
static inline void rflpc_timer_set_clock(rflpc_timer_t timer, rflpc_clock_divider_t divider)
{
    /* p. 490 and p. 56 */
    switch (timer)
    {
	case RFLPC_TIMER0:
	case RFLPC_TIMER1:
	    LPC_SC->PCLKSEL0 &= ~( 3UL << ((timer+1)<<1));
	    LPC_SC->PCLKSEL0 |= ( divider << ((timer+1)<<1));
	    break;
	case RFLPC_TIMER2:
	case RFLPC_TIMER3:
	    LPC_SC->PCLKSEL1 &= ~( 3UL << ((timer+4)<<1));
	    LPC_SC->PCLKSEL1 |= ( divider << ((timer+4)<<1));
	    break;
    }
}

/** 
 * @brief Sets the timer interrupt callback. Also enables timer interrupt to be received.
 * 
 * @param timer 
 * @param callback 
 */
static inline void rflpc_timer_set_callback(rflpc_timer_t timer, rflpc_irq_handler_t callback)
{
    /* Set the interrupt vector entry */
    rflpc_irq_set_handler(TIMER0_IRQn + timer, callback);
    /* enable the interrupt vector */
    rflpc_irq_enable(TIMER0_IRQn + timer);
}


/** 
 * @brief starts the given timer. 
 *
 * This does not modify timer register value. It just starts to count
 * 
 * @param timer 
 */
static inline void rflpc_timer_start(rflpc_timer_t timer)
{
    /* Start the timer, see p. 494 */
    rflpc_timer_base(timer)->TCR |= 1;
}

/** 
 * Stops the timer.
 *
 * Does not modify timer registers value 
 * @param timer 
 */
static inline void rflpc_timer_stop(rflpc_timer_t timer)
{
    /* Stop the timer, see p. 494 */
    rflpc_timer_base(timer)->TCR &= ~1UL;
}

/** Resets the timer
    @param timer
*/
static inline void rflpc_timer_reset(rflpc_timer_t timer)
{
    /* To reset the timer, set bit 1 of TCR to 1, then reset to 0 */
    rflpc_timer_base(timer)->TCR |= 2UL;
    rflpc_timer_base(timer)->TCR &= ~2UL;
}

/** 
 * Returns the timer counter value. This value is incremented each time the
 * prescale counter matches the prescale register.
 * 
 * @param timer 
 * 
 * @return 
 */
static inline uint32_t rflpc_timer_get_counter(rflpc_timer_t timer)
{
    return rflpc_timer_base(timer)->TC;
}

/** 
 * Returns the prescale counter value. This value is incremented at each clock
 * cycle.
 * 
 * @param timer 
 * 
 * @return 
 */
static inline uint32_t rflpc_timer_get_pre_scale_counter(rflpc_timer_t timer)
{
    return rflpc_timer_base(timer)->PC;
}

/** 
 * Returns the value of the prescale register. This register contains the
 * number of clock cycle needed to increment the timer counter. That is, at
 * each clock cycle, the prescale counter is incrementer and when it reaches
 * the prescale register, the timer counter is incremented.
 * 
 * @param timer 
 * 
 * @return 
 */
static inline uint32_t rflpc_timer_get_pre_scale_register(rflpc_timer_t timer)
{
    return rflpc_timer_base(timer)->PR;
}


/** 
 * Sets the timer counter value. This value is incremented each time the
 * prescale counter matches the prescale register.
 * 
 * @param timer 
 * @param value
 * 
 */
static inline void rflpc_timer_set_counter(rflpc_timer_t timer, uint32_t value)
{
    rflpc_timer_base(timer)->TC = value;
}

/** 
 * Sets the prescale counter value. This value is incremented at each clock
 * cycle.
 * 
 * @param timer
 * @param value
 * 
 */
static inline void rflpc_timer_set_pre_scale_counter(rflpc_timer_t timer, uint32_t value)
{
    rflpc_timer_base(timer)->PC = value;
}

/** 
 * Sets the value of the prescale register. This register contains the
 * number of clock cycle needed to increment the timer counter. That is, at
 * each clock cycle, the prescale counter is incrementer and when it reaches
 * the prescale register, the timer counter is incremented.
 * 
 * @param timer 
 * @param value
 * 
 */
static inline void rflpc_timer_set_pre_scale_register(rflpc_timer_t timer, uint32_t value)
{
    rflpc_timer_base(timer)->PR = value;
}

#endif

