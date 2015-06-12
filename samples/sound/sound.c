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
#include <rflpc17xx/rflpc17xx.h>
#include "sound.h"

void sound_init_pin(rflpc_pin_t pin)
{
   rflpc_gpio_set_pin_mode_output(pin, 0);
}


void sound_play(rflpc_pin_t pin, uint32_t frequency_by_1000, int timems)
{
   uint32_t period_length_us = (uint32_t) 1000000000 / frequency_by_1000;
   uint32_t nb_period = 1000000 / period_length_us;
   uint32_t i;
   for (i = 0 ; i < nb_period ; ++i)
   {
      rflpc_gpio_set_pin(pin);
      RFLPC_DELAY_MICROSECS(period_length_us >> 2);
      rflpc_gpio_clr_pin(pin);
      RFLPC_DELAY_MICROSECS(period_length_us >> 2);
   }
}


void sound_play_note(rflpc_pin_t pin, int note, int timems)
{
}


