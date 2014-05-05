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

#include <rflpc17xx/drivers/i2c.h>

#include "c12832.h"

#define THERMOMETER_I2C_SLAVE_ADDRESS 0x90

int main()
{
  uint8_t r[2] = { 0 };

  rflpc_led_init();
  /* rflpc_led_set(RFLPC_LED_4); */

  lcd_init();
  rflpc_printf_set_putchar(&lcd_putchar);

  rflpc_i2c_init(RFLPC_I2C_PORT2, RFLPC_I2C_MODE_MASTER, 0);
  rflpc_i2c_write(RFLPC_I2C_PORT2, THERMOMETER_I2C_SLAVE_ADDRESS, 0, 1, 0);
  rflpc_i2c_read(RFLPC_I2C_PORT2, THERMOMETER_I2C_SLAVE_ADDRESS, &r, 2);

  lcd_reset();
  printf("%x %x\n", r[0], r[1]);
  lcd_refresh();

  while (1);

  return 0;
}
