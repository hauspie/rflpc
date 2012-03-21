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
/** @file
 * GPIO peripheral driver
 */
/*
  Author: Michael Hauspie <michael.hauspie@univ-lille1.fr>
  Created: 2011-07-04
  Time-stamp: <2012-03-21 09:50:28 (hauspie)>

  GPIO driver for LPC.
*/
#ifndef __RFLPC_GPIO_H__
#define __RFLPC_GPIO_H__

#ifdef RFLPC_CONFIG_ENABLE_GPIO

#include <stdint.h>
#include "../nxp/LPC17xx.h"
#include "../tools.h"
#include "../pinconf.h"

/** @addtogroup gpio General Purpose Input Output (GPIO)
 * @ingroup drivers
 * @{
 */

/** @brief Configures the multi-purpose pins of the CM3 so that it uses a given GPIO pin
 *  @todo Add more parameters to allow the configuration of pull-up pull-down and/or open drain mode
    @param gpio the gpio port number
    @param pin the pin to activate
 */
static inline void rflpc_gpio_use_pin(int gpio, int pin)
{
    /* Use GPIO function for pin */
    rflpc_pin_set(gpio, pin, 0, 0, 0);
}


/** Macro used to convert a digit in the corresponding pointer */
#define RFLPC_GPIO_BASE(gpio_number) TOKEN_PASTE(LPC_GPIO, gpio_number)

/** Sets a pin to input mode */
#define rflpc_gpio_set_pin_mode_input(gpio, pin) do {	\
	RFLPC_GPIO_BASE(gpio)->FIODIR &= ~(1 << (pin));	\
    } while(0)

/** Sets a pin to output mode */
#define rflpc_gpio_set_pin_mode_output(gpio, pin) do {	\
	RFLPC_GPIO_BASE(gpio)->FIODIR |= (1 << (pin));	\
    } while(0)

/** Set a pin (put a logical 1 on it) */
#define rflpc_gpio_set_pin(gpio, pin) do {			\
	RFLPC_GPIO_BASE(gpio)->FIOMASK &= (1 << (pin));		\
	RFLPC_GPIO_BASE(gpio)->FIOSET |= (1 << (pin));		\
    } while(0)

/** Clear a pin (put a logical 0 on it) */
#define rflpc_gpio_clr_pin(gpio, pin) do {			\
	RFLPC_GPIO_BASE(gpio)->FIOMASK &= (1 << (pin));		\
	RFLPC_GPIO_BASE(gpio)->FIOCLR |= (1 << (pin));		\
    } while(0)

/** Set multiple pins from a mask */
#define rflpc_gpio_set_pins_from_mask(gpio, mask) do {		\
	RFLPC_GPIO_BASE(gpio)->FIOMASK = ~(mask);		\
	RFLPC_GPIO_BASE(gpio)->FIOSET |= (mask);		\
    } while (0)

/** Clean multiple pins from a mask */
#define rflpc_gpio_clr_pins_from_mask(gpio, mask) do {		\
	RFLPC_GPIO_BASE(gpio)->FIOMASK = ~(mask);		\
	RFLPC_GPIO_BASE(gpio)->FIOCLR |= (mask);		\
    } while (0)

/** Sets the value of a pin (0 or 1) */
#define rflpc_gpio_set_pin_val(gpio, pin, val) do {			\
	if (val) rflpc_gpio_set_pin(gpio, pin); else rflpc_gpio_clr_pin(gpio, pin); \
    } while (0)

/** Sets the value of multiple pins of a given port from a mask and a value */
#define rflpc_gpio_set_val(gpio, val, mask) do {	\
	RFLPC_GPIO_BASE(gpio)->FIOMASK = (mask);	\
	RFLPC_GPIO_BASE(gpio)->FIOPIN = (val);		\
    } while (0)

/** Gets the value of a pin */
#define rflpc_gpio_get_pin(gpio, pin) (RFLPC_GPIO_BASE(gpio)->FIOMASK = ~= (1 << (pin)), (RFLPC_GPIO_BASE(gpio)->FIOPIN >> (pin)) & 1)
/** Gets the value of the entire port */
#define rflpc_gpio_get_val(gpio) (RFLPC_GPIO_BASE(gpio)->FIOMASK = 0, RFLPC_GPIO_BASE(gpio)->FIOPIN)

/** @} */
#endif /* ENABLE_GPIO */

#endif
