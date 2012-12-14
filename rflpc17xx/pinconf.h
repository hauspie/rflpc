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
  Time-stamp: <2012-12-14 17:09:27 (hauspie)>

  Provides macro to set pin function and modes
*/
/** @file
 * Multi-purpose pins configuration made easy.
 */
#ifndef __RFLPC_PINCONF_H__
#define __RFLPC_PINCONF_H__

#include <stdint.h>

/** @defgroup pinconf Pin configuration
 * @ingroup system
 * @{
 */


/** Represents a pin.
    The 3 MSB represents the port
    The 5 LSB bits reprensent the pin

    i.e. To use pin 2.3, set the value to (2 << 5 | 3)
*/
typedef uint8_t rflpc_pin_t;

/** Can be used to create a rflpc_pin_t from a port index and a pin index */
#define RFLPC_PIN_MAKE(port, pin) ((port) << 5 | (pin))

#define RFLPC_PIN_GET_PORT(pin) (((pin) >> 5) & 0x7)
#define RFLPC_PIN_GET_PIN(pin) ((pin) & 0x1F)

/** Shortcuts macro for all pins */
enum
{
    /* Port 0 */
    RFLPC_PIN_P0_0,
    RFLPC_PIN_P0_1,
    RFLPC_PIN_P0_2,
    RFLPC_PIN_P0_3,
    RFLPC_PIN_P0_4,
    RFLPC_PIN_P0_5,
    RFLPC_PIN_P0_6,
    RFLPC_PIN_P0_7,
    RFLPC_PIN_P0_8,
    RFLPC_PIN_P0_9,
    RFLPC_PIN_P0_10,
    RFLPC_PIN_P0_11,
    RFLPC_PIN_P0_12,
    RFLPC_PIN_P0_13,
    RFLPC_PIN_P0_14,
    RFLPC_PIN_P0_15,
    RFLPC_PIN_P0_16,
    RFLPC_PIN_P0_17,
    RFLPC_PIN_P0_18,
    RFLPC_PIN_P0_19,
    RFLPC_PIN_P0_20,
    RFLPC_PIN_P0_21,
    RFLPC_PIN_P0_22,
    RFLPC_PIN_P0_23,
    RFLPC_PIN_P0_24,
    RFLPC_PIN_P0_25,
    RFLPC_PIN_P0_26,
    RFLPC_PIN_P0_27,
    RFLPC_PIN_P0_28,
    RFLPC_PIN_P0_29,
    RFLPC_PIN_P0_30,
    RFLPC_PIN_P0_31,


    /* Port 1 */
    RFLPC_PIN_P1_0 = RFLPC_PIN_MAKE(1,0),
    RFLPC_PIN_P1_1,
    RFLPC_PIN_P1_2,
    RFLPC_PIN_P1_3,
    RFLPC_PIN_P1_4,
    RFLPC_PIN_P1_5,
    RFLPC_PIN_P1_6,
    RFLPC_PIN_P1_7,
    RFLPC_PIN_P1_8,
    RFLPC_PIN_P1_9,
    RFLPC_PIN_P1_10,
    RFLPC_PIN_P1_11,
    RFLPC_PIN_P1_12,
    RFLPC_PIN_P1_13,
    RFLPC_PIN_P1_14,
    RFLPC_PIN_P1_15,
    RFLPC_PIN_P1_16,
    RFLPC_PIN_P1_17,
    RFLPC_PIN_P1_18,
    RFLPC_PIN_P1_19,
    RFLPC_PIN_P1_20,
    RFLPC_PIN_P1_21,
    RFLPC_PIN_P1_22,
    RFLPC_PIN_P1_23,
    RFLPC_PIN_P1_24,
    RFLPC_PIN_P1_25,
    RFLPC_PIN_P1_26,
    RFLPC_PIN_P1_27,
    RFLPC_PIN_P1_28,
    RFLPC_PIN_P1_29,
    RFLPC_PIN_P1_30,
    RFLPC_PIN_P1_31,

    /* Port 2 */
    RFLPC_PIN_P2_0 = RFLPC_PIN_MAKE(2,0),
    RFLPC_PIN_P2_1,
    RFLPC_PIN_P2_2,
    RFLPC_PIN_P2_3,
    RFLPC_PIN_P2_4,
    RFLPC_PIN_P2_5,
    RFLPC_PIN_P2_6,
    RFLPC_PIN_P2_7,
    RFLPC_PIN_P2_8,
    RFLPC_PIN_P2_9,
    RFLPC_PIN_P2_10,
    RFLPC_PIN_P2_11,
    RFLPC_PIN_P2_12,
    RFLPC_PIN_P2_13,

    /* Port 3 */
    RFLPC_PIN_P3_25 = RFLPC_PIN_MAKE(3,25),
    RFLPC_PIN_P3_26,

    /* Port 4 */
    RFLPC_PIN_P4_28 = RFLPC_PIN_MAKE(4,28),
    RFLPC_PIN_P4_29,
};



/** Pin mode.
 * Affects the behavior only if the pin is used as input
 */
typedef enum
{
   RFLPC_PIN_MODE_RESISTOR_PULL_UP,     /**< The pin has pull-up resistor enabled.      */
   RFLPC_PIN_MODE_REPEATER,             /**< The pin has repeater mode enabled.         */
   RFLPC_PIN_MODE_RESISTOR_NONE,        /**< The pin has neither pull-up nor pull-down. */
   RFLPC_PIN_MODE_RESISTOR_PULL_DOWN,   /**< The pin has pull-down resistor enabled.    */
} rflpc_pin_mode_t;

/**
 * Sets the operational mode for a given pin on a given port.
 * @note See the LPC17xx user manual to get a description of functions as the functions are dependent on the pin
 *
 * @param pin The pin
 * @param function How the pin is used (0..3).
 * @param mode The pin mode (see ::rflpc_pin_mode_t)
 * @param opendrain Control the open drain mode for pins used as output. 0 for normal mode (no open drain), 1 for open drain.
 **/
static inline void rflpc_pin_set(rflpc_pin_t p, int function, rflpc_pin_mode_t mode, int opendrain)
{
    int register_idx = 2*RFLPC_PIN_GET_PORT(p);
    int pin = RFLPC_PIN_GET_PIN(p);

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
	*(&(LPC_PINCON->PINMODE_OD0) + RFLPC_PIN_GET_PORT(p)) |= (1 << pin);
    else
	*(&(LPC_PINCON->PINMODE_OD0) + RFLPC_PIN_GET_PORT(p)) &= ~(1 << pin);
}
/** @} */
#endif
