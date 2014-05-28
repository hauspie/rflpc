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
 * Analog-To-Digital Converter driver
 * @ingroup drivers
 * @{
 */

/**
 * Initializes the Analog/Digital Converter on given pin.
 *
 * @param pin The desired pin to sample.
 * @param clock_divider The divider applied to APB peripheral clock.
 *
 * @note This function actually starts the driver.
 * @note APB peripheral clock is set by default at 13MHz, because the maximum sample clock is 13Mhz anyway.
 */
extern void rflpc_adc_init(rflpc_pin_t pin,
			   rflpc_clock_divider_t clock_divider);

/**
 * Initializes the Analog/Digital converter for Burst Mode. 
 *
 * @param clock_divider The divider applied to APB peripheral clock.
 * @param handler Interruption handler
 *
 * @note This function DOES NOT actually starts the driver 
 *       (see rflpc_adc_burst_start()).
 */
extern void rflpc_adc_burst_init(rflpc_clock_divider_t clock_divider, rflpc_irq_handler_t handler);

/**
 * Enable an ADC pin to be sampled in burst mode.
 *
 * @param pin The desired pin to sample.
 */
extern void rflpc_adc_burst_enable(rflpc_pin_t pin);

/**
 * Start ADC burst mode.
 *
 * @note This function actually starts the driver with burst mode enabled.
 *       Please check you have previously enabled all the adc pins you want
 *       to be sampled.
 */
extern void rflpc_adc_burst_start();

/**
 * Start an ADC conversion on specified pin.
 *
 * @param pin the pin to be sampled.
 *
 * @note This only starts sampling, it does not wait until conversion is done.
 */
static inline void rflpc_adc_sample(rflpc_pin_t pin)
{
   RFLPC_SET_BITS_VAL (LPC_ADC->ADCR, 24, 0x01, 3);
}

/**
 * Read the last converted value of specified pin using dedicated data register.
 *
 * @param pin The ADC pin to read.
 *
 * @return The value corresponding to specified pin's last conversion.
 *
 * @note Returned value is stored on 12 bits. Bit 15 stores the OVERRUN flag.
 * @note This function waits for a conversion to be done.
 * @note You should use ONLY ONE method to read converted values on ADC pins,
 *       as OVERRUN flag may be out of synchronization between registers.
 */
uint16_t rflpc_adc_read(rflpc_pin_t pin);

/**
 * Read the last converted value of any ADC pin, using global data register.
 *
 * @return The last converted value by any pin.
 *
 * @note Returned value is stored on 12 bits. Bits 12 to 14 store the pin
 *       from which the value has been converted. Bit 15 stores the OVERRUN flag.
 * @note This function waits for a conversion to be done.
 * @note You should use ONLY ONE method to read converted values on ADC pins,
 *       as OVERRUN flag may be out of synchronization between registers.
 */
uint16_t rflpc_adc_read_global();


#endif /* RFLPC_CONFIG_ENABLE_ADC */

/** @} */

#endif /* __RFLPC_ADC_H__ */
