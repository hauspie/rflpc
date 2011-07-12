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

#ifndef __RFLPC_LEDS_H__
#define __RFLPC_LEDS_H__

/* Leds control.
   See schematic p. 3 for pin number

   LEDS are connected to GPIO1
   LED1 P1.18
   LED2 P1.20
   LED3 P1.21
   LED4 P1.23
*/
#include "../LPC17xx.h"
#include "gpio.h"

#define LED1_PIN (18)
#define LED2_PIN (20)
#define LED3_PIN (21)
#define LED4_PIN (23)

#define LED1 (1 << 18)
#define LED2 (1 << 20)
#define LED3 (1 << 21)
#define LED4 (1 << 23)

/** Inits the GPIO port to use the leds. Sets the GPIO1 led pins to output
 * pins */
static inline void rflpc_led_init()
{
    /* Connect GPIO1 to physical pins */
    rflpc_gpio_use_pin(1, LED1_PIN);
    rflpc_gpio_use_pin(1, LED2_PIN);
    rflpc_gpio_use_pin(1, LED3_PIN);
    rflpc_gpio_use_pin(1, LED4_PIN);
    /* Connect LED pins */
    rflpc_gpio_set_pin_mode_output(1, LED1_PIN);
    rflpc_gpio_set_pin_mode_output(1, LED2_PIN);
    rflpc_gpio_set_pin_mode_output(1, LED3_PIN);
    rflpc_gpio_set_pin_mode_output(1, LED4_PIN);
}

/** Turns the given led(s) on */
static inline void rflpc_led_set(uint32_t l)
{
    rflpc_gpio_set_pins_from_mask(1, l);
}


/** Clears the given led(s) (turn them off) */
static inline void rflpc_led_clr(uint32_t l)
{
    rflpc_gpio_clr_pins_from_mask(1, l);
}


/** Turns the leds included in the mask*/
static inline void rflpc_led_val(uint32_t l)
{
    rflpc_gpio_set_val(1, l, ~(LED1 | LED2 | LED3 | LED4));
}

/** Turns the led to reflect a binary 4 digit number 
    LED4 is LSB, LED1 MSB
 */
static inline void rflpc_led_binary_value(uint8_t v)
{
    uint32_t leds = 0;
    if (v & 1)
	leds |= LED4;
    if (v & 2)
	leds |= LED3;
    if (v & 4)
	leds |= LED2;
    if (v & 8)
	leds |= LED1;
    rflpc_led_val(leds);
}

#endif
