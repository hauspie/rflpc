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
 Created: 2011-10-06
 Time-stamp: <2012-12-17 14:55:05 (hauspie)>
*/
#ifdef RFLPC_CONFIG_ENABLE_SPI

#include "spi.h"
#include "../nxp/LPC17xx.h"
#include "../pinconf.h"
#include "../tools.h"

#define PINFUNC_SPI 2

#define SCK0_PIN  RFLPC_PIN_P0_15
#define SSEL0_PIN RFLPC_PIN_P0_16
#define MISO0_PIN RFLPC_PIN_P0_17
#define MOSI0_PIN RFLPC_PIN_P0_18


#define SSEL1_PIN  RFLPC_PIN_P0_6
#define SCK1_PIN   RFLPC_PIN_P0_7
#define MISO1_PIN  RFLPC_PIN_P0_8
#define MOSI1_PIN  RFLPC_PIN_P0_9

void rflpc_spi_init(rflpc_spi_t port, rflpc_spi_mode_t mode, rflpc_clock_divider_t cpu_clock_divider, uint8_t data_size_transfert, uint8_t clock_prescale, uint8_t serial_clock_rate)
{
   LPC_SSP_TypeDef *spi_base = rflpc_spi_get_base_addr(port);
   
   /* Power and clock the SPI device */
   if (port == RFLPC_SPI0)
   {
      RFLPC_SET_BIT(LPC_SC->PCONP, 21);
      RFLPC_SET_BITS_VAL(LPC_SC->PCLKSEL1, 10, cpu_clock_divider, 2);

      rflpc_pin_set(SCK0_PIN, PINFUNC_SPI, RFLPC_PIN_MODE_RESISTOR_PULL_UP, 0);
      rflpc_pin_set(MISO0_PIN, PINFUNC_SPI, RFLPC_PIN_MODE_RESISTOR_PULL_UP, 0);
      rflpc_pin_set(MOSI0_PIN, PINFUNC_SPI, RFLPC_PIN_MODE_RESISTOR_PULL_UP, 0);      
      rflpc_pin_set(SSEL0_PIN, PINFUNC_SPI, RFLPC_PIN_MODE_RESISTOR_PULL_UP, 0);
   }
   else if (port == RFLPC_SPI1)
   {
      RFLPC_SET_BIT(LPC_SC->PCONP, 10);
      RFLPC_SET_BITS_VAL(LPC_SC->PCLKSEL0, 20, cpu_clock_divider, 2);

      rflpc_pin_set(SCK1_PIN, PINFUNC_SPI, RFLPC_PIN_MODE_RESISTOR_PULL_UP, 0);
      rflpc_pin_set(MISO1_PIN, PINFUNC_SPI, RFLPC_PIN_MODE_RESISTOR_PULL_UP, 0);
      rflpc_pin_set(MOSI1_PIN, PINFUNC_SPI, RFLPC_PIN_MODE_RESISTOR_PULL_UP, 0);
      rflpc_pin_set(SSEL1_PIN, PINFUNC_SPI, RFLPC_PIN_MODE_RESISTOR_PULL_UP, 0);
   }

   /* user manual p. 422. Set the data transfert size */
   spi_base->CR0 = ((data_size_transfert - 1) & 0xF); /* | (serial_clock_rate - 1) << 8; */
   /* No loop back, enable ssp controler */
   spi_base->CR1 = (1UL << 1);
   if (mode == RFLPC_SPI_SLAVE)
       RFLPC_SET_BIT(spi_base->CR1, 2);
   else /* master mode */
   {
       /* Set clock parameters */
       spi_base->CPSR = clock_prescale;
       spi_base->CR0 |= (serial_clock_rate - 1) << 8;
   }
}

void rflpc_spi_set_rx_callback(rflpc_spi_t port, rflpc_irq_handler_t callback)
{
    LPC_SSP_TypeDef *spi_base = rflpc_spi_get_base_addr(port);
    /* set the SPI interrupt handler */
    rflpc_irq_set_handler(SSP0_IRQn + port, callback);
    /* enable the interrupt vector */
    rflpc_irq_enable(SSP0_IRQn + port);    
    spi_base->IMSC |= (1UL << 2); /* Enable interrupt generation on RX Fifo half full event */
}

#endif
