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
  Time-stamp: <2012-03-08 16:11:20 (hauspie)>
*/
#include <rflpc17xx/rflpc17xx.h>
#include "scroller.h"


#define SPI_PORT RFLPC_SPI1

void wait(int micros)
{
   uint32_t start = rflpc_timer_get_counter(RFLPC_TIMER0);
   while ((rflpc_timer_get_counter(RFLPC_TIMER0) - start) < micros);
   return;
}

void led_matrix_display_buffer(uint8_t *buffer)
{
   int i;
   for (i = 0 ; i < 64 ; ++i)
      rflpc_spi_write(SPI_PORT, buffer[i]);
   /* wait for transfer to finish */
   while (!rflpc_spi_tx_fifo_empty(SPI_PORT));
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
   int c = 0;

   while (needed_divider / serial_clock_rate_divider > 254)
   {
      serial_clock_rate_divider++;
   }
   needed_divider /= serial_clock_rate_divider;
   printf("Computed clock: %d (%d %d) \r\n", spi_peripheral_clock / (needed_divider * serial_clock_rate_divider), needed_divider, serial_clock_rate_divider);

   rflpc_spi_init_master(SPI_PORT, RFLPC_CCLK_8, needed_divider, serial_clock_rate_divider, 8);

   
   for (i = 0 ; i < 64 ; ++i)
       pic[i] = 7 << 5;
   led_matrix_display_buffer(pic);
   wait(500000);

   for (i = 0 ; i < 64 ; ++i)
       pic[i] = 7 << 2;
   led_matrix_display_buffer(pic);
   wait(500000);

   for (i = 0 ; i < 64 ; ++i)
       pic[i] = 3;
   led_matrix_display_buffer(pic);
   wait(500000);
   


   c = 0;
   while (1)
   {
       
       for (i = 0 ; i < 64 ; ++i)
	   pic[i] = 0;
       display_text(pic, "Hello World!", c++, 0xFF);
       if (c == 96)
	   c = -8;
	   
       led_matrix_display_buffer(pic);
       wait(200000);
   }
}

int main()
{
   rflpc_uart_init(RFLPC_UART0);
   rflpc_timer_enable(RFLPC_TIMER0);
   rflpc_timer_set_clock(RFLPC_TIMER0,RFLPC_CCLK/8);
   rflpc_timer_set_pre_scale_register(RFLPC_TIMER0, rflpc_clock_get_system_clock()/8000000); /* microsecond timer */
   rflpc_timer_start(RFLPC_TIMER0);

   printf("SPI test sample build on %s %s\r\n", __DATE__, __TIME__);
   printf("Waiting 1 second\r\n");
   wait(1000000);
   printf("OK\r\n");

   test_spi();
   while(1);
   return 0;
}
