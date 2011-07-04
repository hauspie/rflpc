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
  Time-stamp: <2011-07-04 11:30:46 (hauspie)>

  GPIO driver for LPC.
*/
#ifndef __RFLPC_GPIO_H__
#define __RFLPC_GPIO_H__

#define rflpc_gpio_set_pin_mode(gpio, pin, mode)
#define rflpc_gpio_set_pin(gpio, pin)
#define rflpc_gpio_clr_pin(gpio, pin)
#define rflpc_gpio_set_pin_val(gpio, pin, val)
#define rflpc_gpio_set_val(gpio, val, mask)
#define rflpc_gpio_get_pin(gpio, pin)
#define rflpc_gpio_get_val(gpio)

#endif
