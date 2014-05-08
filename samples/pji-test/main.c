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

#include <rflpc17xx/drivers/adc.h>

#include "c12832.h"

void test_adc_simple();
void test_adc_burst();
void test_adc_burst_with_interruptions();

int main()
{
  lcd_init();
  rflpc_printf_set_putchar(&lcd_putchar);

  test_adc_simple();
  /* test_adc_burst(); */
  /* test_adc_burst_with_interruptions(); */

  return 0;
}

#define ADC_TEST_PIN1    MBED_DIP19
#define ADC_TEST_PIN2    MBED_DIP20

void test_adc_simple()
{
  rflpc_adc_channel_t channel;
  uint16_t result;

  channel = rflpc_adc_get_pin_channel(ADC_TEST_PIN1);
  rflpc_adc_init(ADC_TEST_PIN1, channel, RFLPC_CCLK);

  while (1) {
    rflpc_adc_sample_channel(channel);
    result = rflpc_adc_read_channel(channel);
    printf("Valeur: %i   \r", result);
    lcd_refresh();
  }
}

void test_adc_burst()
{
  rflpc_adc_channel_t channel1, channel2;
  uint16_t result1 = 0, result2 = 0;

  rflpc_adc_burst_init(RFLPC_CCLK, NULL);

  channel1 = rflpc_adc_get_pin_channel(ADC_TEST_PIN1);
  channel2 = rflpc_adc_get_pin_channel(ADC_TEST_PIN2);

  rflpc_adc_burst_enable_channel(ADC_TEST_PIN1, channel1);
  rflpc_adc_burst_enable_channel(ADC_TEST_PIN2, channel2);

  rflpc_adc_burst_start();

  while (1) {
    result1 = rflpc_adc_read_channel(channel1);
    result2 = rflpc_adc_read_channel(channel2);

    printf("%i   %i   \r", result1, result2);

    lcd_refresh();
  }
}

static rflpc_adc_channel_t test_channel1, test_channel2;
void adc_burst_interrupt_handler()
{
  uint32_t res;

  res = (rflpc_adc_read_channel(test_channel1) << 16);
  res |= rflpc_adc_read_channel(test_channel2);

  printf("%i    %i   \r", res >> 16, res & 0xFFFF);
  lcd_refresh();
}
void test_adc_burst_with_interruptions()
{
  uint16_t result1 = 0, result2 = 0;

  rflpc_adc_burst_init(RFLPC_CCLK, &adc_burst_interrupt_handler);

  test_channel1 = rflpc_adc_get_pin_channel(ADC_TEST_PIN1);
  test_channel2 = rflpc_adc_get_pin_channel(ADC_TEST_PIN2);

  rflpc_adc_burst_enable_channel(ADC_TEST_PIN1, test_channel1);
  rflpc_adc_burst_enable_channel(ADC_TEST_PIN2, test_channel2);

  rflpc_adc_burst_start();

  while (1) {
    asm("wfi");
  }
}
