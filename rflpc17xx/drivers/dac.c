/* This file is part of rflpc. Copyright 2010-2014 Michael Hauspie
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
  Author: Christophe Bacara <christophe.bacara@etudiant.univ-lille1.fr
*/

#ifdef RFLPC_CONFIG_ENABLE_DAC

#include "dac.h"
#include "../nxp/LPC17xx.h"
#include "../pinconf.h"
#include "../tools.h"

#define PINFUNC_DAC 0x02

void rflpc_dac_init(rflpc_pin_t pin, uint8_t clock_divider)
{
  rflpc_pin_set(MBED_DIP18, PINFUNC_DAC, RFLPC_PIN_MODE_RESISTOR_PULL_UP, 0);
  RFLPC_SET_BITS_VAL (LPC_SC->PCLKSEL0, 22, clock_divider, 2);
}

#endif /* RFLPC_CONFIG_ENABLE_DAC */
