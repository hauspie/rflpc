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
  Author: Michael Hauspie <michael.hauspie@univ-lille1.fr>
  Created:
  Time-stamp: <2011-09-23 11:14:13 (hauspie)>
*/
#include <rflpc17xx/rflpc17xx.h>



void wait(int micros)
{
   uint32_t start = rflpc_timer_get_counter(RFLPC_TIMER0);
   while ((rflpc_timer_get_counter(RFLPC_TIMER0) - start) < micros);
   return;
}

void led_matrix_display_buffer(uint8_t *buffer)
{
   int i;
   /* Assert chip select */
   rflpc_gpio_clr_pin(0,6);
   wait(500);
   for (i = 0 ; i < 64 ; ++i)
      rflpc_spi_write(RFLPC_SPI1, buffer[i]);
   /* wait for transfer to finish */
   while (!rflpc_spi_tx_fifo_empty(RFLPC_SPI1));
   /* wait 0.5 more ms */
   wait(500);
   rflpc_gpio_set_pin(0,6);
}



/* Putchar has to be defined in order to printf to work */
int putchar(int c)
{
   rflpc_uart0_putchar(c);
   return c;
}

/* Function to test SPI device.
 * This sample used a sparkfun LED matrix connected to SPI1
 * http://www.sparkfun.com/products/760
 */
void test_spi()
{
   uint8_t pic[64];
   /* The led matrix spi can be clock at maximum 125Khz, calc the needed dividers */
   int spi_peripheral_clock = rflpc_clock_get_system_clock() / 8;
   int needed_divider = spi_peripheral_clock / 125000;
   int serial_clock_rate_divider = 1;
   int i = 1;
   int r = 0, g = 0, b = 0;

   while (needed_divider / serial_clock_rate_divider > 254)
   {
      serial_clock_rate_divider++;
   }
   needed_divider /= serial_clock_rate_divider;
   printf("Computed clock: %d (%d %d) \r\n", spi_peripheral_clock / (needed_divider * serial_clock_rate_divider), needed_divider, serial_clock_rate_divider);

   rflpc_spi_init_master(RFLPC_SPI1, RFLPC_CCLK_8, needed_divider, serial_clock_rate_divider, 8);

   /* Configure gpio for chip select 0.6 is the DIP8 on the MBED */
   rflpc_gpio_use_pin(0, 6);
   rflpc_gpio_set_pin_mode_output(0,6);
   rflpc_gpio_set_pin(0,6); /* chip selection is made on low logical level so put high to disable yet */



   while (1)
   {
      if (b <= 7)
         b++;
      else if (g <= 7)
         g++;
      else if (r <= 7)
         r++;
      else{ r = 0 ; b = 0 ; g = 0;}
      for (i = 0 ; i < 64 ; ++i)
      {
         pic[i] = r << 6 | g << 3 | b;
      }
      led_matrix_display_buffer(pic);
      wait(5000);
   }
}

int main()
{
   rflpc_uart0_init();
   rflpc_timer_enable(RFLPC_TIMER0);
   rflpc_timer_set_clock(RFLPC_TIMER0,RFLPC_CCLK/8);
   rflpc_timer_set_pre_scale_register(RFLPC_TIMER0, rflpc_clock_get_system_clock()/8000000); /* microsecond timer */
   rflpc_timer_start(RFLPC_TIMER0);

   printf("Waiting 1 second\r\n");
   wait(1000000);
   printf("OK\r\n");

   test_spi();
   while(1);
   return 0;
}
