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
 * Time-stamp: <2012-12-14 16:55:26 (hauspie)>
 */
/** @file
 * Pulse Width Modulation driver
 */
#ifndef __RFLPC_PWM_H__
#define __RFLPC_PWM_H__



#ifdef RFLPC_CONFIG_ENABLE_PWM

/** @addtogroup pwm Pulse Width Modulation
 * @ingroup drivers
 * @{ */


/** 
 * Inits the PWM peripheral for use with selected pin
 * 
 * @note not all pins are available! And one PWM channel can only be used on one pin at a time.
 * Available pins are:
 * - P1_18 and P2_0 for PWM1
 * - P1_20, P2_1 and P3_25 for PWM2
 * - P1_21, P2_2 and P3_26 for PWM3
 * - P1_23 and P2_3 for PWM4
 * - P1_24 and P2_4 for PWM5
 * - P1_26 and P2_5 for PWM6
 */
void rflpc_pwm_init(rflpc_pin_t pin);

/** Sets the pwm parameters in single edge mode.
 * This call makes the PWM outputs a signal of a given micro seconds
 * period. The signal will be high for pulsewidth microsecond and low for the
 * remainder of the period
 * 
 * @param pin the PWM pin (same restriction as ::rflpc_pwm_init apply)
 * @param period The period of the pwm cycle in micro seconds
 * @param pulsewidth The width of the pulse in micro seconds
 */
void rflpc_pwm_single_edge(rflpc_pint_t pin, uint32_t period, uint32_t pulsewidth)

/** @} */
#endif /* ENABLE_PWM */

#endif
