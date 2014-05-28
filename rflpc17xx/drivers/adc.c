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
*/

#ifdef RFLPC_CONFIG_ENABLE_ADC

#include "adc.h"
#include "../tools.h"

/* Pin configuration value for ADC */
#define PINFUNC_ADC    0x03


/** Used to select which ADC port to use */
typedef enum rflpc_adc_channel_e {
   AD0_0 = 0, AD0_1, AD0_2, AD0_3, AD0_4, AD0_5, AD0_6, AD0_7, ADC_INVALID_CHANNEL
} rflpc_adc_channel_t;

/**
 * Returns the ADC channel corresponding to the specified pin.
 *
 * @param pin The desired pin for ADC.
 *
 * @return Corresponding ADC channel.
 */
static rflpc_adc_channel_t _pin_to_channel(rflpc_pin_t pin)
{
   switch (pin) {
      case RFLPC_PIN_P0_23:
         return AD0_0;
      case RFLPC_PIN_P0_24:
         return AD0_1;
      case RFLPC_PIN_P0_25:
         return AD0_2;
      case RFLPC_PIN_P0_26:
         return AD0_3;
      case RFLPC_PIN_P1_30:
         return AD0_4;
      case RFLPC_PIN_P1_31:
         return AD0_5;
      case RFLPC_PIN_P0_3:
         return AD0_6;
      case RFLPC_PIN_P0_2:
         return AD0_7;
   }
   return ADC_INVALID_CHANNEL;
}


struct {
   rflpc_irq_handler_t interrupt_handler;
   uint8_t enabled_channels;
} rflpc_adc_burst_control;

void rflpc_adc_init(rflpc_pin_t pin, rflpc_clock_divider_t clock_divider)
{
   /* Pin initialization: ADC, pull-up, no open drain */
   rflpc_pin_set(pin, PINFUNC_ADC, RFLPC_PIN_MODE_RESISTOR_PULL_UP, 0);

   /* Enable ADC support */
   RFLPC_SET_BIT (LPC_SC->PCONP, 12);

   /* Set APB clock down to 13Mhz */
   RFLPC_SET_BITS_VAL (LPC_SC->PCLKSEL0, 24, RFLPC_CCLK_8, 2);
   /* Set CLK divider to down clock if needed */
   RFLPC_SET_BITS_VAL (LPC_ADC->ADCR, 8, clock_divider, 8);

   /* Select channel to be sampled */
   RFLPC_SET_BIT (LPC_ADC->ADCR, _pin_to_channel(pin));


   /* Power up A/D Converter */
   RFLPC_SET_BIT(LPC_ADC->ADCR, 21);
}

void rflpc_adc_burst_init(rflpc_clock_divider_t clock_divider, rflpc_irq_handler_t handler)
{
   /* Enable ADC support */
   RFLPC_SET_BIT (LPC_SC->PCONP, 12);

   /* Set APB clock down to 13Mhz */
   RFLPC_SET_BITS_VAL (LPC_SC->PCLKSEL0, 24, RFLPC_CCLK_8, 2);

   /* Set CLK divider to down clock if needed */
   RFLPC_SET_BITS_VAL (LPC_ADC->ADCR, 8, clock_divider, 8);

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

void rflpc_adc_burst_enable(rflpc_pin_t pin)
{
   /* Initialize pin: ADC, Pull-up, no open drain */
   rflpc_pin_set(pin, PINFUNC_ADC, RFLPC_PIN_MODE_RESISTOR_PULL_UP, 0);

   /* Enable channel */
   RFLPC_SET_BIT (LPC_ADC->ADCR, _pin_to_channel(pin));

   /* Add channel to burst control struct internal mask */
   rflpc_adc_burst_control.enabled_channels |= _pin_to_channel(pin);
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
         RFLPC_SET_BIT (LPC_ADC->ADINTEN, channel);
   }

   /* Power up A/D converter */
   RFLPC_SET_BIT (LPC_ADC->ADCR, 21);

   /* Set burst mode */
   RFLPC_SET_BIT (LPC_ADC->ADCR, 16);
}

uint16_t rflpc_adc_read(rflpc_pin_t pin)
{
   /* Shift our ADDR pointer according to channel */
   const volatile uint32_t *addrp = &LPC_ADC->ADDR0 + _pin_to_channel(pin);

   uint32_t addr = 0;		/* Register value */
   uint16_t result = 0;		/* Returned result */

   do { /* Wait until conversion done */
   } while ((*addrp & 0x80000000) != 0x80000000);

   /* Read AD Data Register */
   addr = *addrp;

   /* Result is stored on 12 bits */
   result = (addr >> 4) & 0xFFF;

   /* Store OVERRUN flag at MSB */
   if (addr & 0x40000000)
      RFLPC_SET_BIT (result, 15);

   return result;
}

uint16_t rflpc_adc_read_global()
{
   uint32_t adgdr = 0;		/* Register value */
   uint16_t result = 0;		/* Returned value */

   do { /* Wait until conversion done */
   } while ((LPC_ADC->ADGDR & 0x80000000) != 0x80000000);

   /* Read AD Global Data Register */
   adgdr = LPC_ADC->ADGDR;

   /* Result is stored on 12 bits */
   result = (adgdr >> 4) & 0xFFF;

   /* Store (3 bits) channel from which the value has been sampled */
   result |= (adgdr >> 24) & 0x07;

   /* Store OVERRUN flag at MSB */
   if (adgdr & 0x40000000)
      RFLPC_SET_BIT (result, 15);

   return result;
}


#endif /* RFLPC_CONFIG_ENABLE_ADC */

