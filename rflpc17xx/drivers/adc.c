/* This file is part of rflpc. Copyright 2010-2014 Michael Hauspie
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
  Author: Christophe Bacara <christophe.bacara@etudiant.univ-lille1.fr
  Created: 2014-01-27

  ADC driver for LPC.
*/

#ifdef RFLPC_CONFIG_ENABLE_ADC

#include "adc.h"
#include "../tools.h"

/* Pin configuration value for ADC */
#define PINFUNC_ADC    0x03

struct {
  rflpc_irq_handler_t interrupt_handler;
  uint8_t enabled_channels;
} rflpc_adc_burst_control;

void rflpc_adc_init(rflpc_pin_t pin, rflpc_adc_channel_t channel, uint8_t clock_divider)
{
  /* Pin initialization: ADC, pull-up, no open drain */
  rflpc_pin_set(pin, PINFUNC_ADC, RFLPC_PIN_MODE_RESISTOR_PULL_UP, 0);

  /* Enable ADC support */
  RFLPC_SET_BIT (LPC_SC->PCONP, 12);

  /* Set APB clock down to 13Mhz */
  RFLPC_SET_BITS_VAL (LPC_SC->PCLKSEL0, 24, RFLPC_CCLK_8, 2);
  /* Set CLK divider to down clock if needed */
  RFLPC_SET_BITS_VAL (ADC_BASE->ADCR, 8, clock_divider, 8);

  /* Select channel to be sampled */
  RFLPC_SET_BIT (ADC_BASE->ADCR, channel);

  /* Power up A/D Converter */
  RFLPC_SET_BIT(ADC_BASE->ADCR, 21);
}

void rflpc_adc_burst_init(uint8_t clock_divider, rflpc_irq_handler_t handler)
{
  /* Enable ADC support */
  RFLPC_SET_BIT (LPC_SC->PCONP, 12);

  /* Set APB clock down to 13Mhz */
  RFLPC_SET_BITS_VAL (LPC_SC->PCLKSEL0, 24, RFLPC_CCLK_8, 2);

  /* Set CLK divider to down clock if needed */
  RFLPC_SET_BITS_VAL (ADC_BASE->ADCR, 8, clock_divider, 8);

  /* Enable interrupts if handler has been specified */
  if (handler != NULL) {
    /* Set handler */
    rflpc_adc_burst_control.interrupt_handler = handler;
    rflpc_irq_set_handler(ADC_IRQn, rflpc_adc_burst_control.interrupt_handler);
    /* Enable ADC IRQ */
    rflpc_irq_enable(ADC_IRQn);
  }

  rflpc_adc_burst_control.enabled_channels = 0;
}

void rflpc_adc_burst_enable_channel(rflpc_pin_t pin, rflpc_adc_channel_t channel)
{
  /* Initialize pin: ADC, Pull-up, no open drain */
  rflpc_pin_set(pin, PINFUNC_ADC, RFLPC_PIN_MODE_RESISTOR_PULL_UP, 0);

  /* Enable channel */
  RFLPC_SET_BIT (ADC_BASE->ADCR, channel);

  /* Add channel to burst control struct internal mask */
  rflpc_adc_burst_control.enabled_channels |= channel;
}

void rflpc_adc_burst_start()
{
  uint8_t mask = 0x80;
  int8_t channel = 7;

  /* Enable interrupt on highest channel if handler has been specified
     at initialization time. */
  if (rflpc_adc_burst_control.interrupt_handler != NULL) {
    /* First, we need to find the highest ADC channel used */
    while ((rflpc_adc_burst_control.enabled_channels & mask) != mask
	   && channel >= 0) {
      /* Try next bit */
      mask = mask >> 2;
      channel -= 1;
    }

    /* If at least one channel is enabled, enable interrupt on the one 
       previously found */
    if (channel >= 0)
      RFLPC_SET_BIT (ADC_BASE->ADINTEN, channel);
  }

  /* Power up A/D converter */
  RFLPC_SET_BIT (ADC_BASE->ADCR, 21);

  /* Set burst mode */
  RFLPC_SET_BIT (ADC_BASE->ADCR, 16);
}

#endif /* RFLPC_CONFIG_ENABLE_ADC */
