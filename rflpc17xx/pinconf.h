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
  Time-stamp: <2011-07-04 11:30:46 (hauspie)>

  Provides macro to set pin function and modes
*/
/** @file
 * Multi-purpose pins configuration made easy.
 */
#ifndef __RFLPC_PINCONF_H__
#define __RFLPC_PINCONF_H__

/** @defgroup pinconf Pin configuration
 * @ingroup system
 * @{
 */

/** Pin mode.
 * Affects the behavior only if the pin is used as input
 */
typedef enum
{
   RFLPC_PIN_MODE_RESISTOR_PULL_UP,     /**< The pin has pull-up resistor enabled.      */
   RFLPC_PIN_MODE_REPEATER,             /**< The pin has repeater mode enabled.         */
   RFLPC_PIN_MODE_RESISTOR_NONE,        /**< The pin has neither pull-up nor pull-down. */
   RFLPC_PIN_MODE_RESISTOR_PULL_DOWN,   /**< The pin has pull-down resistor enabled.    */
} rfPinMode;

/**
 * Sets the operational mode for a given pin on a given port.
 * @note See the LPC17xx user manual to get a description of functions as the functions are dependent on the pin
 *
 * @param port The port of the pin (0..4)
 * @param pin The pin (0..31)
 * @param function How the pin is used (0..3).
 * @param mode The pin mode (see ::rfPinMode)
 * @param opendrain Control the open drain mode for pins used as output. 0 for normal mode (no open drain), 1 for open drain.
 **/
static inline void rflpc_pin_set(int port, int pin, int function, rfPinMode mode, int opendrain)
{
    int register_idx = 2*port;
    if (pin >= 16)
    {
	pin -= 16;
	register_idx++;
    }

    /* Configure function */
    *(&(LPC_PINCON->PINSEL0) + register_idx) &= ~(3 << (pin * 2));
    *(&(LPC_PINCON->PINSEL0) + register_idx) |= ((function & 0x3) << (pin * 2));

    /* Configure mode */
    *(&(LPC_PINCON->PINMODE0) + register_idx) &= ~(3 << (pin * 2));
    *(&(LPC_PINCON->PINMODE0) + register_idx) |= ((mode & 0x3) << (pin * 2));

    /* Configure opendrain */
    if (opendrain)
	*(&(LPC_PINCON->PINMODE_OD0) + port) |= (1 << pin);
    else
	*(&(LPC_PINCON->PINMODE_OD0) + port) &= ~(1 << pin);
}
/** @} */
#endif
