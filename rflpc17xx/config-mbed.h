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

/*
  Author: Michael Hauspie <michael.hauspie@univ-lille1.fr>
  Created: 2011-07-04
  Time-stamp: <2011-07-13 10:03:38 (hauspie)>

  This files provides the configuration of the lib for the mbed platform
*/

#ifndef __RFLPC_CONFIG_MBED_H__
#define __RFLPC_CONFIG_MBED_H__



/** Which GPIO port is used for leds? */
#define RFLPC_LED_PORT   1
/** On which pin is the LED1 ? */
#define RFLPC_LED_1_PIN 18
/** On which pin is the LED2 ? */
#define RFLPC_LED_2_PIN 20
/** On which pin is the LED3 ? */
#define RFLPC_LED_3_PIN 21
/** On which pin is the LED4 ? */
#define RFLPC_LED_4_PIN 23


#define RFLPC_PLATFORM_CONFIGURED

#endif
