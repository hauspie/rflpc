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
  Time-stamp: <2013-03-01 13:47:47 (hauspie)>
*/
#ifdef RFLPC_CONFIG_ENABLE_TIMERS

#include "timer.h"

static LPC_TIM_TypeDef *rflpc_timer_base(rflpc_timer_t timer)
{
    switch (timer)
    {
	case RFLPC_TIMER0: return LPC_TIM0;
	case RFLPC_TIMER1: return LPC_TIM1;
	case RFLPC_TIMER2: return LPC_TIM2;
	case RFLPC_TIMER3: return LPC_TIM3;
	    /* For all register up to CR1, the PWM timer memory mapping is
	     * exactly the same as a timer one. Thus, the timer functions can
	     * be used also for PWM timer configuration */
	case RFLPC_TIMER_PWM: return (LPC_TIM_TypeDef*) LPC_PWM1;
	default: break;
    }
    return (LPC_TIM_TypeDef*)0;
}


void rflpc_timer_enable(rflpc_timer_t timer)
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
	default:
	    break;
    }
}

void rflpc_timer_disable(rflpc_timer_t timer)
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
	default:
	    break;
    }
}

void rflpc_timer_set_clock(rflpc_timer_t timer, rflpc_clock_divider_t divider)
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
	case RFLPC_TIMER_PWM:
	    break;
    }
}

void rflpc_timer_set_callback(rflpc_timer_t timer, rflpc_irq_handler_t callback)
{
    /* Set the interrupt vector entry */
    rflpc_irq_set_handler(TIMER0_IRQn + timer, callback);
    /* enable the interrupt vector */
    rflpc_irq_enable(TIMER0_IRQn + timer);
}


void rflpc_timer_start(rflpc_timer_t timer)
{
    /* Start the timer, see p. 494 */
    rflpc_timer_base(timer)->TCR |= 1;
}

int rflpc_timer_running(rflpc_timer_t timer)
{
    return rflpc_timer_base(timer)->TCR & 1;
}

void rflpc_timer_stop(rflpc_timer_t timer)
{
    /* Stop the timer, see p. 494 */
    rflpc_timer_base(timer)->TCR &= ~1UL;
}

void rflpc_timer_reset(rflpc_timer_t timer)
{
    /* To reset the timer, set bit 1 of TCR to 1, then reset to 0 */
    rflpc_timer_base(timer)->TCR |= 2UL;
    rflpc_timer_base(timer)->TCR &= ~2UL;
}

uint32_t rflpc_timer_get_counter(rflpc_timer_t timer)
{
    return rflpc_timer_base(timer)->TC;
}

uint32_t rflpc_timer_get_pre_scale_counter(rflpc_timer_t timer)
{
    return rflpc_timer_base(timer)->PC;
}

uint32_t rflpc_timer_get_pre_scale_register(rflpc_timer_t timer)
{
    return rflpc_timer_base(timer)->PR;
}


void rflpc_timer_set_counter(rflpc_timer_t timer, uint32_t value)
{
    rflpc_timer_base(timer)->TC = value;
}

void rflpc_timer_set_pre_scale_counter(rflpc_timer_t timer, uint32_t value)
{
    rflpc_timer_base(timer)->PC = value;
}

void rflpc_timer_set_pre_scale_register(rflpc_timer_t timer, uint32_t value)
{
    rflpc_timer_base(timer)->PR = value;
}

void rflpc_timer_set_match_value(rflpc_timer_t timer, rflpc_timer_match_t match_register, uint32_t match_value)
{
    ((uint32_t *)&(rflpc_timer_base(timer)->MR0))[match_register] = match_value;
}

void rflpc_timer_set_irq_on_match(rflpc_timer_t timer, rflpc_timer_match_t match_register, uint32_t options)
{
    rflpc_timer_base(timer)->MCR &= ~(7UL << (match_register*3)); /* clear old options */
    rflpc_timer_base(timer)->MCR |= (options << (match_register*3)); /* set new */
}


void rflpc_timer_reset_irq(rflpc_timer_t timer, rflpc_timer_match_t match_register)
{
    rflpc_timer_base(timer)->IR |= (1 << match_register);
}

int rflpc_timer_test_irq(rflpc_timer_t timer, rflpc_timer_match_t match_register)
{
    return rflpc_timer_base(timer)->IR & (1 << match_register);
}

/** @} */

#endif /* ENABLE_TIMERS */
