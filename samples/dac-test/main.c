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
/*
  Author: Christophe Bacara <christophe.bacara@etudiant.univ-lille1.fr>
*/

#include <rflpc17xx/rflpc17xx.h>

void sleep_us(uint32_t us)
{
   uint32_t delay, counter;

   for (delay = (16 * us), counter = 0; counter < delay; ++counter)
      asm("");  
}

int main()
{
   /* Plug-in a pair of earphones ! */
   uint8_t flag = 0;
   rflpc_dac_init(RFLPC_CCLK_8);


   while (1) {
      if (flag) {
         rflpc_dac_write(0);
         flag = 0;
      }
      else {
         rflpc_dac_write(0x3ff);
         flag = 1;
      }

      sleep_us(2272); /* Some 440 Hz */
   }

   while (1);

   return 0;
}
