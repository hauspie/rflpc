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

/** @file
 * ADC peripheral driver
 */

#ifndef __RFLPC_ADC_H__
#define __RFLPC_ADC_H__

/*
  Author: Christophe Bacara <christophe.bacara@etudiant.univ-lille1.fr
*/

#ifdef RFLPC_CONFIG_ENABLE_ADC

#include <rflpc17xx/rflpc17xx.h>
#include <stdint.h>

/** @addtogroup adc ADC
 * @ingroup drivers
 * @{
 */

/** Used to select which ADC port to use */
typedef enum rflpc_adc_channel_e {
  AD0_0 = 0, AD0_1, AD0_2, AD0_3, AD0_4, AD0_5, AD0_6, AD0_7
} rflpc_adc_channel_t;

/**
 * Returns the ADC channel corresponding to the specified pin.
 *
 * @param pin The desired pin for ADC.
 *
 * @return Corresponding ADC channel.
 */
static inline rflpc_adc_channel_t rflpc_adc_get_pin_channel(rflpc_pin_t pin)
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
}

/**
 * Initializes the Analog/Digital Converter on given pin.
 *
 * @param pin The desired pin to sample.
 * @param channel The ADC channel corresponding to desired pin.
 * @param clock_divider The divider applied to APB peripheral clock.
 *
 * @note This function actually starts the driver.
 * @note APB peripheral clock is set by default at 13MHz.
 */
extern void rflpc_adc_init(rflpc_pin_t pin,
			   rflpc_adc_channel_t channel,
			   uint8_t clock_divider);

/**
 * Initializes the Analog/Digital converter for Burst Mode. 
 *
 * @param clock_divider The divider applied to APB peripheral clock.
 * @param handler Interruption handler
 *
 * @note This function DOES NOT actually starts the driver 
 *       (see rflpc_adc_burst_start()).
 */
extern void rflpc_adc_burst_init(uint8_t clock_divider, rflpc_irq_handler_t handler);

/**
 * Enable an ADC channel to be sampled in burst mode.
 *
 * @param pin The desired pin to sample.
 * @param channel The ADC channel corresponding to desired pin.
 */
extern void rflpc_adc_burst_enable_channel(rflpc_pin_t pin,
					   rflpc_adc_channel_t channel);

/**
 * Start ADC burst mode.
 *
 * @note This function actually starts the driver with burst mode enabled.
 *       Please check you have previously enabled all the adc channels you want
 *       to be sampled.
 */
extern void rflpc_adc_burst_start();

/**
 * Start an ADC conversion on specified channel.
 *
 * @param channel The channel to be sampled.
 *
 * @note This only starts sampling, it does not wait until conversion is done.
 */
static inline void rflpc_adc_sample_channel(rflpc_adc_channel_t channel)
{
  LPC_ADC_TypeDef *adc_base = (LPC_ADC_TypeDef *)LPC_ADC_BASE;
  RFLPC_SET_BITS_VAL (adc_base->ADCR, 24, 0x01, 3);
}

/**
 * Read the last converted value of specified channel using dedicated data register.
 *
 * @param channel The ADC channel to read.
 *
 * @return The value corresponding to specified channel's last conversion.
 *
 * @note Returned value is stored on 12 bits. Bit 15 stores the OVERRUN flag.
 * @note This function waits for a conversion to be done.
 * @note You should use ONLY ONE method to read converted values on ADC channels,
 *       as OVERRUN flag may be out of synchronization between registers.
 */
static inline uint16_t rflpc_adc_read_channel(rflpc_adc_channel_t channel)
{
  LPC_ADC_TypeDef *adc_base = (LPC_ADC_TypeDef *)LPC_ADC_BASE;

  /* Shift our ADDR pointer according to channel */
  const volatile uint32_t *addrp = &adc_base->ADDR0 + channel;

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

/**
 * Read the last converted value of any ADC channel, using global data register.
 *
 * @return The last converted value by any channel.
 *
 * @note Returned value is stored on 12 bits. Bits 12 to 14 store the channel
 *       from which the value has been converted. Bit 15 stores the OVERRUN flag.
 * @note This function waits for a conversion to be done.
 * @note You should use ONLY ONE method to read converted values on ADC channels,
 *       as OVERRUN flag may be out of synchronization between registers.
 */
static inline uint16_t rflpc_adc_read_global()
{
  LPC_ADC_TypeDef *adc_base = (LPC_ADC_TypeDef *)LPC_ADC_BASE;
  uint32_t adgdr = 0;		/* Register value */
  uint16_t result = 0;		/* Returned value */

  do { /* Wait until conversion done */
  } while ((adc_base->ADGDR & 0x80000000) != 0x80000000);

  /* Read AD Global Data Register */
  adgdr = adc_base->ADGDR;

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

/** @} */

#endif /* __RFLPC_ADC_H__ */
