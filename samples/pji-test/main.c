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

#include "c12832.h"

int main()
{
  lcd_init();
  rflpc_printf_set_putchar(&lcd_putchar);

  printf("Hello World\n");

  lcd_refresh();

  while (1);

  return 0;
}
