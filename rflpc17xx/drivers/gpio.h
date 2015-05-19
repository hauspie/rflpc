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
  Time-stamp: <2015-05-19 10:42:32 (hauspie)>

  GPIO driver for LPC.
*/
#ifndef __RFLPC_GPIO_H__
#define __RFLPC_GPIO_H__

#include <stdint.h>
#include "../nxp/LPC17xx.h"
#include "../tools.h"
#include "../pinconf.h"

/** @addtogroup gpio General Purpose Input Output (GPIO)
 * @ingroup drivers
 * @{
 */

/** @brief Configures the multi-purpose pins of the CM3 so that it uses a given GPIO pin
    @param pin the pin to activate
    @param mode the mode to set for the pin (pull-up, pull-down,...) see ::rflpc_pin_mode_t
 */
extern void rflpc_gpio_use_pin(rflpc_pin_t pin, rflpc_pin_mode_t mode);

/** Sets a pin to input mode 
 * @note This automatically calls ::rflpc_gpio_use_pin
*/
extern void rflpc_gpio_set_pin_mode_input(rflpc_pin_t pin, rflpc_pin_mode_t mode);

/** Sets a pin to output mode 
* Uses a pin as output pin and sets its value. 
* @note This automatically calls ::rflpc_gpio_use_pin with a pull-up configuration
* 
* @param pin pin to use
* @param val value to set on pin
*/
extern void rflpc_gpio_set_pin_mode_output(rflpc_pin_t pin, uint8_t val);

/** Set a pin (put a logical 1 on it) */
extern void rflpc_gpio_set_pin(rflpc_pin_t pin);

/** Clear a pin (put a logical 0 on it) */
extern void rflpc_gpio_clr_pin(rflpc_pin_t pin);

/** Set multiple pins from a mask */
extern void rflpc_gpio_set_pins_from_mask(uint8_t gpio, uint32_t mask);

/** Clean multiple pins from a mask */
extern void rflpc_gpio_clr_pins_from_mask(uint8_t gpio, uint32_t mask);

/** Sets the value of a pin (0 or 1) */
extern void rflpc_gpio_set_pin_val(rflpc_pin_t pin, int val);

/** Sets the value of multiple pins of a given port from a mask and a value */
extern void rflpc_gpio_set_val(uint8_t gpio, uint32_t val, uint32_t mask);

/** Gets the value of a pin */
extern int rflpc_gpio_get_pin(rflpc_pin_t pin);

/** Gets the value of the entire port */
extern uint32_t rflpc_gpio_get_val(uint8_t gpio);


/** @} */

#endif
