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
 * Time-stamp: <2012-12-14 11:45:56 (hauspie)>
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


/** Identifies PWM outputs */
typedef enum
{
    RFLPC_PWM_1,
    RFLPC_PWM_2,
    RFLPC_PWM_3,
    RFLPC_PWM_4,
    RFLPC_PWM_5,
    RFLPC_PWM_6,
} rflpc_pwm_output_t;

/** 
 * Inits the PWM peripheral and set the selected pin to work as PWM
 * 
 * @param output the PWM output to enable
 * @param port the port to connect the PWM output to
 * @param pin  the pin to connect the PWM output to
 */
void rflpc_pwm_init(rflpc_pwm_output_t output, int port, int pin);

/** @} */
#endif /* ENABLE_PWM */

#endif
