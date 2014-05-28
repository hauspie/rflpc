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
   /* test_adc_simple(); */
  /* test_adc_burst(); */
  test_adc_burst_with_interruptions();

   return 0;
}
void test_adc_simple()
{
   rflpc_adc_init(ADC_TEST_PIN1, RFLPC_CCLK_8);

   while (1) {
      rflpc_adc_sample(ADC_TEST_PIN1);
      printf("%d\r\n", rflpc_adc_read(ADC_TEST_PIN1));

      /* Do something (binary value on leds, lcd dump ?) */
   }
}


void test_adc_burst()
{
   /* Initialize burst mode WITHOUT interruptions generation */
   rflpc_adc_burst_init(RFLPC_CCLK, NULL);

   /* Enable the two tests channels to be sampled in burst mode */
   rflpc_adc_burst_enable(ADC_TEST_PIN1);
   rflpc_adc_burst_enable(ADC_TEST_PIN2);

   /* Start burst conversions */
   rflpc_adc_burst_start();

   while (1) {
      uint16_t result1, result2;
      result1 = rflpc_adc_read(ADC_TEST_PIN1);
      result2 = rflpc_adc_read(ADC_TEST_PIN2);

      /* Do something (binary value on leds, lcd dump ?) */
      printf("%d %d\r\n", result1, result2);
   }
}


RFLPC_IRQ_HANDLER adc_burst_interrupt_handler(void)
{
   uint16_t result1 = 0, result2 = 0;

   result1 = rflpc_adc_read(ADC_TEST_PIN1);
   result2 = rflpc_adc_read(ADC_TEST_PIN2);

   printf("%d %d\r\n", result1, result2);
}

void test_adc_burst_with_interruptions()
{
   /* Initialize burst mode with interruptions generation */
   rflpc_adc_burst_init(RFLPC_CCLK, adc_burst_interrupt_handler);

   /* Enable the two tests channels to be sampled in burst mode */
   rflpc_adc_burst_enable(ADC_TEST_PIN1);
   rflpc_adc_burst_enable(ADC_TEST_PIN2);

   /* Start burst conversions */
   rflpc_adc_burst_start();

   while (1) {
      /* Wait for interruptions */
      asm("wfi");
   }
}
