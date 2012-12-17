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
 * Time-stamp: <2012-12-17 15:42:47 (hauspie)>
 */
/** @file
 * Pulse Width Modulation driver
 */
#ifndef __RFLPC_PWM_H__
#define __RFLPC_PWM_H__

#ifdef RFLPC_CONFIG_ENABLE_PWM

#include <stdint.h>
#include "../pinconf.h"

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
 *
 * @return returns -1 on error and 0 if no problem occurs
 */
extern int rflpc_pwm_init(rflpc_pin_t pin);

/** 
 * Sets the period in micro seconds. 
 * This also starts and reset the PWM timer so that all PWM outputs restarts with the new period
 * @param period 
 */
extern void rflpc_pwm_set_period(uint32_t period);

/** Sets the pwm parameters in single edge mode.
 * This call makes the PWM outputs a pulse of a given width at the start of each period
 *
 * @param pin the PWM pin (same restriction as ::rflpc_pwm_init apply)
 * @param pulsewidth The width of the pulse in micro seconds
 */
extern void rflpc_pwm_single_edge(rflpc_pin_t pin, uint32_t pulsewidth);

/** 
 * Sets the pwm parameters to operate on double edge mode.
 * 
 * @param pin the PWM pin (same restrictions as in ::rflpc_pwm_init)
 * @param high_edge time at which the PWM ouput goes in high state (in microseconds)
 * @param low_edge time at which the PWM ouput goes in low state (in microseconds)
 */
extern void rflpc_pwm_double_edge(rflpc_pin_t pin, uint32_t high_edge, uint32_t low_edge);

/** 
 * Starts the PWM on the given pin (do not reset the timer, only enable PWM output)
 * 
 * @param pin 
 */
extern void rflpc_pwm_start(rflpc_pin_t pin);

/** 
 * Stops the PWM output on the given pin
 * 
 * @param pin 
 */
extern void rflpc_pwm_stop(rflpc_pin_t pin);

/** @} */
#endif /* ENABLE_PWM */

#endif
