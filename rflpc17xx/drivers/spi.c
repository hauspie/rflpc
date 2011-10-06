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
 Time-stamp: <2011-10-06 14:18:07 (hauspie)>
*/

#include "spi.h"
#include "../nxp/LPC17xx.h"
#include "../pinconf.h"
#include "../printf.h"

#define PINFUNC_SPI 2

#define SPI0_PORT  0
#define SCK0_PIN  15
#define MISO0_PIN 17
#define MOSI0_PIN 18

#define SPI1_PORT  0
#define SCK1_PIN   7
#define MISO1_PIN  8
#define MOSI1_PIN  9

void rflpc_spi_init_master(rflpc_spi_t port, rflpc_clock_divider_t cpu_clock_divider, uint8_t clock_prescale, uint8_t serial_clock_rate, uint8_t data_size_transfert)
{
   LPC_SSP_TypeDef *spi_base = 0;
   /* Power and clock the SPI device */
   if (port == RFLPC_SPI0)
   {
      LPC_SC->PCONP |= (1UL << 21);
      LPC_SC->PCLKSEL1 &= ~(0x3 << 10);
      LPC_SC->PCLKSEL1 |= ((cpu_clock_divider & 0x3)<< 10);

      rflpc_pin_set(SPI0_PORT, SCK0_PIN, PINFUNC_SPI, RFLPC_PIN_MODE_RESISTOR_PULL_UP, 0);
      rflpc_pin_set(SPI0_PORT, MISO0_PIN, PINFUNC_SPI, RFLPC_PIN_MODE_RESISTOR_PULL_UP, 0);
      rflpc_pin_set(SPI0_PORT, MOSI0_PIN, PINFUNC_SPI, RFLPC_PIN_MODE_RESISTOR_PULL_UP, 0);
      spi_base = LPC_SSP0;
   }
   else if (port == RFLPC_SPI1)
   {
      LPC_SC->PCONP |= (1UL << 10);
      LPC_SC->PCLKSEL0 &= ~(0x3 << 20);
      LPC_SC->PCLKSEL0 |= ((cpu_clock_divider & 0x3)<< 20);

      rflpc_pin_set(SPI1_PORT, SCK1_PIN, PINFUNC_SPI, RFLPC_PIN_MODE_RESISTOR_PULL_UP, 0);
      rflpc_pin_set(SPI1_PORT, MISO1_PIN, PINFUNC_SPI, RFLPC_PIN_MODE_RESISTOR_PULL_UP, 0);
      rflpc_pin_set(SPI1_PORT, MOSI1_PIN, PINFUNC_SPI, RFLPC_PIN_MODE_RESISTOR_PULL_UP, 0);
      spi_base = LPC_SSP1;
   }

   /* user manual p. 422. Set the data transfert size and the serial clock rate */
   spi_base->CR0 = ((data_size_transfert - 1) & 0xF) | (serial_clock_rate - 1) << 8;
   printf("%x\r\n", spi_base->CR0);
   spi_base->CPSR = clock_prescale;
   /* Master mode, no loop back, enable ssp controler */
   spi_base->CR1 = (1UL << 1);
}

void rflpc_spi_send(rflpc_spi_t port, uint16_t data)
{
   LPC_SSP_TypeDef *spi_base = port == RFLPC_SPI0 ? LPC_SSP0 : LPC_SSP1;
   /* wait for FIFO to be not full */
   while (rflpc_spi_tx_fifo_full(port));
   spi_base->DR = data;
}
