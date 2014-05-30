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

#define SLAVE_MBED_ADDR 0x40

void sleep(uint32_t ms)
{
  uint32_t delay, counter;

  for (delay = (16000 * ms), counter = 0; counter < delay; ++counter)
    asm("");  
}

#define MAKE_MASTER

int main()
{
  uint8_t sla = SLAVE_MBED_ADDR;
  uint8_t something = 1;
  uint16_t tmp;
  uint8_t status;
  uint8_t realnbytes;

  rflpc_led_init();

#ifdef MAKE_MASTER
  /* Initialize I2C driver */
  rflpc_i2c_init(RFLPC_I2C_PORT2, RFLPC_I2C_MODE_MASTER, 0);

  while (1) {
    /* Read from slave */
    tmp = rflpc_i2c_read(RFLPC_I2C_PORT2, sla, &something, 1, 1);

    if (tmp == 1) {
      rflpc_led_binary_value(something);
      sleep(500);
    }
  }
#endif

#ifdef MAKE_SLAVE
  /* Initialize I2C driver */
  rflpc_i2c_init(RFLPC_I2C_PORT2, RFLPC_I2C_MODE_SLAVE, sla);
  /* Set up slave data transmit buffer */
  rflpc_i2c_set_slave_transmitter_config(&something, 1);

  while (1) {
    /* Read on bus, wait for own slave call */
    tmp = rflpc_i2c_read(RFLPC_I2C_PORT2, sla, &something, 1, 1);
    status = (tmp & 0xFF00) >> 8;
    realnbytes = tmp & 0xFF;

    /* If success, dump the counter on leds and increment it */
    if (status == RFLPC_I2C_SLAVE_TRANSMITTED_DATA) {
      rflpc_led_binary_value(something);
      something++;
      if (something > 15)
	something = 0;
    }
  }
#endif

  return 0;
}
