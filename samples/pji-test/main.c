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
#include <rflpc17xx/tinylibc/printf.h>

#include <rflpc17xx/drivers/dac.h>

#include "c12832.h"

#define DAC_BASE      ((LPC_DAC_TypeDef *) LPC_DAC_BASE)
#define PINFUNC_DAC   0x02
#define __DELAY(d)      \
  { unsigned delay_i; for (delay_i = 0; delay_i < d; ++delay_i)	asm(""); }

int main()
{
  uint16_t i, step;
  uint8_t flag;

  lcd_init();
  rflpc_printf_set_putchar(&lcd_putchar);
  printf("Hello World\n");
  lcd_refresh();

  rflpc_dac_init(MBED_DIP18, RFLPC_CCLK_8);

  step = 0x3FF / 10;
  flag = 0;
  i = 0;
  flag = 0;

  while (1) {
    /* __DELAY (320); */

    /* if (flag) { */
    /*   rflpc_dac_write(0); */
    /* } */
    /* else { */
    /*   rflpc_dac_write(0x3FF); */
    /* } */

    /* flag = flag ^ 0xFF; */

    /* RFLPC_SET_BITS_VAL (DAC_BASE->DACR, 6, (i * step), 10); */
    rflpc_dac_write(i * step);

    /* __DELAY(320); */
    
    if (flag) {
      if (--i == 0)
    	flag = 0;
    }
    else {
      if (++i == 10)
    	flag = 1;
    }

    /* if (flag) { */
    /*   rflpc_dac_write(0); */
    /*   flag = 0; */
    /* } */
    /* else { */
    /*   rflpc_dac_write(0x3FF); */
    /*   flag = 1; */
    /* } */

    __DELAY (360000);
  }


  while (1);

  return 0;
}
