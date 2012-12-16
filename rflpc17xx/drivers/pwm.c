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
 * Author: Michael Hauspie <michael.hauspie@univ-lille1.fr>
 * Created: 2012-12-14
 * Time-stamp: <2012-12-16 18:42:56 (mickey)>
 */
#ifdef RFLPC_CONFIG_ENABLE_PWM

#include "pwm.h"
#include "timer.h"
#include "../nxp/LPC17xx.h"

int rflpc_pwm_init(rflpc_pin_t pin)
{
    /* Enable PWM1 peripheral */
    LPC_SC->PCONP |= 1 << 6; /* p. 63 */

    /* Clock the PWM to System Clock / 8 */
    LPC_SC->PCLKSEL0 &= ~(0x3 << 12);
    LPC_SC->PCLKSEL0 |= ((RFLPC_CCLK_8 & 0x3)<< 12);

    /* Check if the pin to use and set function accordingly, or return with error */
    
    switch(pin)
    {
	case RFLPC_PIN_P1_18: /* PWM1 */
	case RFLPC_PIN_P1_20: /* PWM2 */
	case RFLPC_PIN_P1_21: /* PWM3 */
	case RFLPC_PIN_P1_23: /* PWM4 */
	case RFLPC_PIN_P1_24: /* PWM5 */
	case RFLPC_PIN_P1_26: /* PWM6 */
	    /* p. 109, 110 */
	    /* Select PWM function of selected pin */
	    rflpc_pin_set(pin, 2, 0, 0);
	    break;
	case RFLPC_PIN_P2_0: /* PWM1 */
	case RFLPC_PIN_P2_1: /* PWM2 */
	case RFLPC_PIN_P2_2: /* PWM3 */
	case RFLPC_PIN_P2_3: /* PWM4 */
	case RFLPC_PIN_P2_4: /* PWM5 */
	case RFLPC_PIN_P2_5: /* PWM6 */
	    /* p. 109, 110 */
	    /* Select PWM function of selected pin */
	    rflpc_pin_set(pin, 1, 0, 0);
	    break;
	case RFLPC_PIN_P3_25: /* PWM2 */
	case RFLPC_PIN_P3_26: /* PWM3 */
	    /* p. 111 */
	    /* Select PWM function of selected pin */
	    rflpc_pin_set(pin, 3, 0, 0);
	    break;
	default:
	    return -1;
    }

    /* Set timer parameters */
    /* Pre scale value is set to system clock / 8000000 to have TC counts in microsecond */
    LPC_PWM1->PR = rflpc_clock_get_system_clock() / 8000000;
    return 0;
}

void rflpc_pwm_set_period(uint32_t period)
{
    /* Set period using MR0 */
    LPC_PWM1->MR0 = period;
}

void rflpc_pwm_start(rflpc_pin_t pin)
{
    rflpc_timer_reset(RFLPC_TIMER_PWM);
    rflpc_timer_start(RFLPC_TIMER_PWM);

    switch (pin)
    {
	/* PWM1 */
	case RFLPC_PIN_P1_18:
	case RFLPC_PIN_P2_0:
	    
	    break;
}

void rflpc_pwm_stop(rflpc_pin_t pin)
{
}

void rflpc_pwm_single_edge(rflpc_pint_t pin, uint32_t period, uint32_t pulsewidth)
{
    
    switch (pin)
    {
	/* PWM1 */
	case RFLPC_PIN_P1_18:
	case RFLPC_PIN_P2_0:
	    
	    break;
    }
}


#endif
