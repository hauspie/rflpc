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

#include <rflpc17xx/drivers/adc.h>

#define ADC_TEST_PIN1    MBED_DIP19
#define ADC_TEST_PIN2    MBED_DIP20

/* Simple ADC conversion on one channel */
void test_adc_simple();
/* Hardware-controlled conversions on multiples channels */
void test_adc_burst();

/* Interrupt handler */
void adc_burst_interrupt_handler();
/* Hardware-controlled conversions on multiples channels with interruptions */
void test_adc_burst_with_interruptions();

int main()
{
   rflpc_uart_init(RFLPC_UART0);
   test_adc_simple();
   /* test_adc_burst(); */
   /* test_adc_burst_with_interruptions(); */

   return 0;
}
void test_adc_simple()
{
   rflpc_adc_channel_t channel;

   channel = rflpc_adc_get_pin_channel(ADC_TEST_PIN1);
   rflpc_adc_init(ADC_TEST_PIN1, channel, RFLPC_CCLK);

   while (1) {
      rflpc_adc_sample_channel(channel);
      if (rflpc_adc_read_channel(channel) == 0)
         printf("0 on ADC\r\n");

      /* Do something (binary value on leds, lcd dump ?) */
   }
}


void test_adc_burst()
{
   rflpc_adc_channel_t channel1, channel2;

   /* Initialize burst mode WITHOUT interruptions generation */
   rflpc_adc_burst_init(RFLPC_CCLK, NULL);

   channel1 = rflpc_adc_get_pin_channel(ADC_TEST_PIN1);
   channel2 = rflpc_adc_get_pin_channel(ADC_TEST_PIN2);

   /* Enable the two tests channels to be sampled in burst mode */
   rflpc_adc_burst_enable_channel(ADC_TEST_PIN1, channel1);
   rflpc_adc_burst_enable_channel(ADC_TEST_PIN2, channel2);

   /* Start burst conversions */
   rflpc_adc_burst_start();

   while (1) {
      uint16_t result1, result2;
      result1 = rflpc_adc_read_channel(channel1);
      result2 = rflpc_adc_read_channel(channel2);

      /* Do something (binary value on leds, lcd dump ?) */
      if (result1 > result2)
         printf("ADC1 > ADC2\r\n");
   }
}


static rflpc_adc_channel_t test_channel1, test_channel2;

RFLPC_IRQ_HANDLER adc_burst_interrupt_handler(void)
{
   uint16_t result1 = 0, result2 = 0;

   result1 = rflpc_adc_read_channel(test_channel1);
   result2 = rflpc_adc_read_channel(test_channel2);

   /* Do something (binary value on leds, lcd dump ?) */
   if (result1 > result2)
      printf("ADC1 > ADC2\r\n");
}

void test_adc_burst_with_interruptions()
{
   /* Initialize burst mode with interruptions generation */
   rflpc_adc_burst_init(RFLPC_CCLK, adc_burst_interrupt_handler);

   test_channel1 = rflpc_adc_get_pin_channel(ADC_TEST_PIN1);
   test_channel2 = rflpc_adc_get_pin_channel(ADC_TEST_PIN2);

   /* Enable the two tests channels to be sampled in burst mode */
   rflpc_adc_burst_enable_channel(ADC_TEST_PIN1, test_channel1);
   rflpc_adc_burst_enable_channel(ADC_TEST_PIN2, test_channel2);

   /* Start burst conversions */
   rflpc_adc_burst_start();

   while (1) {
      /* Wait for interruptions */
      asm("wfi");
   }
}
