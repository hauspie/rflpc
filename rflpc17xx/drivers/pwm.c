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
 * Time-stamp: <2012-12-17 15:49:45 (hauspie)>
 */
#ifdef RFLPC_CONFIG_ENABLE_PWM

#include "pwm.h"
#include "timer.h"
#include "../nxp/LPC17xx.h"


/* Some macros to clarify the switch cases in the driver functions */
#define PWM1_PINS RFLPC_PIN_P1_18: \
                  case RFLPC_PIN_P2_0

#define PWM2_PINS RFLPC_PIN_P1_20: \
                  case RFLPC_PIN_P2_1:  \
                  case RFLPC_PIN_P3_25

#define PWM3_PINS RFLPC_PIN_P1_21: \
                  case RFLPC_PIN_P2_2:  \
                  case RFLPC_PIN_P3_26

#define PWM4_PINS RFLPC_PIN_P1_23: \
                  case RFLPC_PIN_P2_3

#define PWM5_PINS RFLPC_PIN_P1_24: \
                  case RFLPC_PIN_P2_4

#define PWM6_PINS RFLPC_PIN_P1_26: \
                  case RFLPC_PIN_P2_5




int rflpc_pwm_init(rflpc_pin_t pin)
{
    /* Enable PWM1 peripheral */
    RFLPC_SET_BIT(LPC_SC->PCONP, 6); /* p. 63 */

    /* Clock the PWM to System Clock / 8 */
    RFLPC_SET_BITS_VAL(LPC_SC->PCLKSEL0, 12, RFLPC_CCLK_8, 2);

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
    
    /* Reset on match. The PWM will constantly output its duty cycle */
    RFLPC_SET_BIT(LPC_PWM1->MCR, 1);
    return 0;
}

void rflpc_pwm_set_period(uint32_t period)
{
    /* Stop, set new period, reset and restart. */
    rflpc_timer_stop(RFLPC_TIMER_PWM);
    LPC_PWM1->MR0 = period;
    rflpc_timer_reset(RFLPC_TIMER_PWM);
    rflpc_timer_start(RFLPC_TIMER_PWM);
}

void rflpc_pwm_start(rflpc_pin_t pin)
{
    /* Enable the PWM output for selected pin (p. 519) */
    switch (pin)
    {
	case PWM1_PINS:
	    RFLPC_SET_BIT(LPC_PWM1->PCR, 9);
	    break;
	case PWM2_PINS:
	    RFLPC_SET_BIT(LPC_PWM1->PCR, 10);
	    break;
	case PWM3_PINS:
	    RFLPC_SET_BIT(LPC_PWM1->PCR, 11);
	    break;
	case PWM4_PINS:
	    RFLPC_SET_BIT(LPC_PWM1->PCR, 12);
	    break;
	case PWM5_PINS:
	    RFLPC_SET_BIT(LPC_PWM1->PCR, 13);
	    break;
	case PWM6_PINS:
	    RFLPC_SET_BIT(LPC_PWM1->PCR, 14);
	    break;
    }
}

void rflpc_pwm_stop(rflpc_pin_t pin)
{
    /* Disable the PWM output for selected pin (p. 519) */
    switch (pin)
    {
	case PWM1_PINS:
	    RFLPC_CLR_BIT(LPC_PWM1->PCR, 9);
	    break;
	case PWM2_PINS:
	    RFLPC_CLR_BIT(LPC_PWM1->PCR, 10);
	    break;
	case PWM3_PINS:
	    RFLPC_CLR_BIT(LPC_PWM1->PCR, 11);
	    break;
	case PWM4_PINS:
	    RFLPC_CLR_BIT(LPC_PWM1->PCR, 12);
	    break;
	case PWM5_PINS:
	    RFLPC_CLR_BIT(LPC_PWM1->PCR, 13);
	    break;
	case PWM6_PINS:
	    RFLPC_CLR_BIT(LPC_PWM1->PCR, 14);
	    break;
    }
}

void rflpc_pwm_single_edge(rflpc_pin_t pin, uint32_t pulsewidth)
{
    switch (pin)
    {
	case PWM1_PINS:
	    /* PWM1 can not be a double edge register and thus is always a
	     * single edge, hence not selecting mode here */
	    LPC_PWM1->MR1 = pulsewidth;
	    break;
	case PWM2_PINS:
	    /* Select singled edge */
	    RFLPC_CLR_BIT(LPC_PWM1->PCR, 2);
	    LPC_PWM1->MR2 = pulsewidth;
	    break;
	case PWM3_PINS:
	    /* Select singled edge */
	    RFLPC_CLR_BIT(LPC_PWM1->PCR, 3);
	    LPC_PWM1->MR3 = pulsewidth;
	    break;
	case PWM4_PINS:
	    /* Select singled edge */
	    RFLPC_CLR_BIT(LPC_PWM1->PCR, 4);
	    LPC_PWM1->MR4 = pulsewidth;
	    break;
	case PWM5_PINS:
	    /* Select singled edge */
	    RFLPC_CLR_BIT(LPC_PWM1->PCR, 5);
	    LPC_PWM1->MR5 = pulsewidth;
	    break;
	case PWM6_PINS:
	    /* Select singled edge */
	    RFLPC_CLR_BIT(LPC_PWM1->PCR, 6);
	    LPC_PWM1->MR6 = pulsewidth;
	    break;
	default:
	    break;
    }
}

void rflpc_pwm_double_edge(rflpc_pin_t pin, uint32_t high_edge, uint32_t low_edge)
{
    switch (pin)
    {
	case PWM1_PINS:
	    /* PWM1 can not be a double edge register and thus is always a
	     * single edge, hence not seting anything here */
	    break;
	case PWM2_PINS:
	    /* Select double edge */
	    RFLPC_SET_BIT(LPC_PWM1->PCR, 2);
	    LPC_PWM1->MR1 = high_edge;
	    LPC_PWM1->MR2 = low_edge;
	    break;
	case PWM3_PINS:
	    /* Select double edge */
	    RFLPC_SET_BIT(LPC_PWM1->PCR, 3);
	    LPC_PWM1->MR2 = high_edge;
	    LPC_PWM1->MR3 = low_edge;
	    break;
	case PWM4_PINS:
	    /* Select double edge */
	    RFLPC_SET_BIT(LPC_PWM1->PCR, 4);
	    LPC_PWM1->MR3 = high_edge;
	    LPC_PWM1->MR4 = low_edge;
	    break;
	case PWM5_PINS:
	    /* Select double edge */
	    RFLPC_SET_BIT(LPC_PWM1->PCR, 5);
	    LPC_PWM1->MR4 = high_edge;
	    LPC_PWM1->MR5 = low_edge;
	    break;
	case PWM6_PINS:
	    /* Select double edge */
	    RFLPC_SET_BIT(LPC_PWM1->PCR, 6);
	    LPC_PWM1->MR5 = high_edge;
	    LPC_PWM1->MR6 = low_edge;
	    break;
	default: 
	    break;
    }
}


#endif
