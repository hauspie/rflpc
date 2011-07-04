/* This file is part of rfBareMbed.                        
 *									 
 * rfBareMbed is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by	 
 * the Free Software Foundation, either version 3 of the License, or	 
 * (at your option) any later version.					 
 * 									 
 * rfBareMbed is distributed in the hope that it will be useful,		 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of	 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the	 
 * GNU General Public License for more details.				 
 * 									 
 * You should have received a copy of the GNU General Public License	 
 * along with rfBareMbed.  If not, see <http://www.gnu.org/licenses/>.	 
 */

/*
  Author: Michael Hauspie <michael.hauspie@univ-lille1.fr>
  Created: 2011-07-04
  Time-stamp: <2011-07-04 17:04:16 (mickey)>

  GPIO driver for LPC.
*/
#ifndef __RFLPC_GPIO_H__
#define __RFLPC_GPIO_H__

#include <stdint.h>
#include "../LPC17xx.h"
#include "../tools.h"

/** @brief Configures the multi-purpose pins of the CM3 so that it uses a given GPIO pin 
    @param gpio the gpio port number
    @param pin the pin to activate
 */
static inline void rflpc_gpio_use_pin(int gpio, int pin)
{
    volatile uint32_t *pinsel_register;
    switch (gpio)
    {
	case 0: pinsel_register = (pin < 16) ? &(LPC_PINCON->PINSEL0) : &(LPC_PINCON->PINSEL1); 
	    break;
	case 1: pinsel_register = (pin < 16) ? &(LPC_PINCON->PINSEL2) : &(LPC_PINCON->PINSEL3); 
	    break;
	case 2: pinsel_register = (pin < 14) ? &(LPC_PINCON->PINSEL4) : ((uint32_t*)0);
	    break;
	case 3: if (pin < 25 || pin >26) 
		return;
	    pinsel_register = &(LPC_PINCON->PINSEL7);
	    break;
	case 4: 
	    if (pin < 28 || pin > 29)
		return;
	    pinsel_register = &(LPC_PINCON->PINSEL9);
	    break;
	default: return;
    }
    if (!pinsel_register)
	return;
    *pinsel_register &= ~(1 << ((pin % 16)<<1));
}


#define RFLPC_GPIO_BASE(gpio_number) TOKEN_PASTE(LPC_GPIO, gpio_number)

/** Sets the pin to input mode */
#define rflpc_gpio_set_pin_mode_input(gpio, pin) do {	\
	RFLPC_GPIO_BASE(gpio)->FIODIR &= ~(1 << (pin));	\
    } while(0)

#define rflpc_gpio_set_pin_mode_output(gpio, pin) do {	\
	RFLPC_GPIO_BASE(gpio)->FIODIR |= (1 << (pin));	\
    } while(0)

#define rflpc_gpio_set_pin(gpio, pin) do {			\
	RFLPC_GPIO_BASE(gpio)->FIOMASK &= (1 << (pin));		\
	RFLPC_GPIO_BASE(gpio)->FIOSET |= (1 << (pin));		\
    } while(0)

#define rflpc_gpio_clr_pin(gpio, pin) do {			\
	RFLPC_GPIO_BASE(gpio)->FIOMASK &= (1 << (pin));		\
	RFLPC_GPIO_BASE(gpio)->FIOCLR |= (1 << (pin));		\
    } while(0)

#define rflpc_gpio_set_pins_from_mask(gpio, mask) do {		\
	RFLPC_GPIO_BASE(gpio)->FIOMASK = ~(mask);		\
	RFLPC_GPIO_BASE(gpio)->FIOSET |= (mask);		\
    } while (0)

#define rflpc_gpio_clr_pins_from_mask(gpio, mask) do {		\
	RFLPC_GPIO_BASE(gpio)->FIOMASK = ~(mask);		\
	RFLPC_GPIO_BASE(gpio)->FIOCLR |= (mask);		\
    } while (0)

#define rflpc_gpio_set_pin_val(gpio, pin, val) do {			\
	if (val) rflpc_gpio_set_pin(gpio, pin); else rflpc_gpio_clr_pin(gpio, pin); \
    } while (0)

#define rflpc_gpio_set_val(gpio, val, mask) do {	\
	RFLPC_GPIO_BASE(gpio)->FIOMASK = (mask);	\
	RFLPC_GPIO_BASE(gpio)->FIOPIN = (val);		\
    } while (0)
#define rflpc_gpio_get_pin(gpio, pin) (RFLPC_GPIO_BASE(gpio)->FIOMASK = ~= (1 << (pin)), (RFLPC_GPIO_BASE(gpio)->FIOPIN >> (pin)) & 1)
#define rflpc_gpio_get_val(gpio) (RFLPC_GPIO_BASE(gpio)->FIOMASK = 0, RFLPC_GPIO_BASE(gpio)->FIOPIN)

#endif
