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

#ifndef __LPC1768_LEDS_H__
#define __LPC1768_LEDS_H__

/* Leds control.
   See schematic p. 3 for pin number

   LEDS are connected to GPIO1
   LED1 P1.18
   LED2 P1.20
   LED3 P1.21
   LED4 P1.23
*/
#include "LPC17xx.h"
#include "printf.h"

#define LED1 (1 << 18)
#define LED2 (1 << 20)
#define LED3 (1 << 21)
#define LED4 (1 << 23)


/** Inits the GPIO port to use the leds. Sets the GPIO1 led pins to output
 * pins */
static inline void lpc_init_leds()
{
    LPC_GPIO1->FIODIR |= (LED1 | LED2 | LED3 | LED4);
}

/** Turns the given led on */
static inline void lpc_set_led(uint32_t l)
{
    LPC_GPIO1->FIOMASK = ~(l); LPC_GPIO1->FIOSET = (l);
}


/** Clears the given led(s) (turn them off) */
static inline void lpc_clr_led(uint32_t l)
{
    LPC_GPIO1->FIOMASK = ~(l); LPC_GPIO1->FIOCLR = (l);
}


/** Turns the leds included in the mask*/
static inline void lpc_led_val(uint32_t l)
{
    LPC_GPIO1->FIOMASK = ~(LED1|LED2|LED3|LED4);
    LPC_GPIO1->FIOPIN = l;
}

#endif
