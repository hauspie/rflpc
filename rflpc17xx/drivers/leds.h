/* This file is part of rflpc.                        
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
#include "../config.h"
#include "gpio.h"


#define RFLPC_LED_1 (1 << RFLPC_LED_1_PIN)
#define RFLPC_LED_2 (1 << RFLPC_LED_2_PIN)
#define RFLPC_LED_3 (1 << RFLPC_LED_3_PIN)
#define RFLPC_LED_4 (1 << RFLPC_LED_4_PIN)

/** Inits the GPIO port to use the leds. Sets the GPIO1 led pins to output
 * pins */
static inline void rflpc_led_init()
{
    /* Connect GPIO1 to physical pins */
    rflpc_gpio_use_pin(RFLPC_LED_PORT, RFLPC_LED_1_PIN);
    rflpc_gpio_use_pin(RFLPC_LED_PORT, RFLPC_LED_2_PIN);
    rflpc_gpio_use_pin(RFLPC_LED_PORT, RFLPC_LED_3_PIN);
    rflpc_gpio_use_pin(RFLPC_LED_PORT, RFLPC_LED_4_PIN);
    /* Connect LED pins */
    rflpc_gpio_set_pin_mode_output(RFLPC_LED_PORT, RFLPC_LED_1_PIN);
    rflpc_gpio_set_pin_mode_output(RFLPC_LED_PORT, RFLPC_LED_2_PIN);
    rflpc_gpio_set_pin_mode_output(RFLPC_LED_PORT, RFLPC_LED_3_PIN);
    rflpc_gpio_set_pin_mode_output(RFLPC_LED_PORT, RFLPC_LED_4_PIN);
}

/** Turns the given led(s) on */
static inline void rflpc_led_set(uint32_t l)
{
    rflpc_gpio_set_pins_from_mask(RFLPC_LED_PORT, l);
}


/** Clears the given led(s) (turn them off) */
static inline void rflpc_led_clr(uint32_t l)
{
    rflpc_gpio_clr_pins_from_mask(RFLPC_LED_PORT, l);
}


/** Turns the leds included in the mask*/
static inline void rflpc_led_val(uint32_t l)
{
    rflpc_gpio_set_val(RFLPC_LED_PORT, l, ~(RFLPC_LED_1 | RFLPC_LED_2 | RFLPC_LED_3 | RFLPC_LED_4));
}

/** Turns the led to reflect a binary 4 digit number 
    RFLPC_LED_4 is LSB, RFLPC_LED_1 MSB
 */
static inline void rflpc_led_binary_value(uint8_t v)
{
    uint32_t leds = 0;
    if (v & 1)
	leds |= RFLPC_LED_4;
    if (v & 2)
	leds |= RFLPC_LED_3;
    if (v & 4)
	leds |= RFLPC_LED_2;
    if (v & 8)
	leds |= RFLPC_LED_1;
    rflpc_led_val(leds);
}

#endif
