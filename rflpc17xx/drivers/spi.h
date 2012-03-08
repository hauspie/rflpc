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
/** @file
 * SPI device driver
 */
#ifndef __RFLPC_SPI_H__
#define __RFLPC_SPI_H__
/*
  Author: Michael Hauspie <michael.hauspie@univ-lille1.fr>
  Created:
  Time-stamp: <2011-07-13 14:24:40 (hauspie)>
*/

#include "../clock.h"

/** @addtogroup spi SPI
 * @ingroup drivers
 * @{
 */

/** Used to select which SPI port to use */
typedef enum
{
   RFLPC_SPI0, /**< Use the SPI port 0 */
   RFLPC_SPI1  /**< Use the SPI port 1 */
} rflpc_spi_t;


/**
 * Inits the SPI interface in master mode.
 *
 * @param port The port to configure
 * @param cpu_clock_divider The divider applied to CPU clock to get the SPI peripheral clock
 * @param clock_prescale Factor by which the prescaler divide the peripheral clock. Between 2 and 254. Bit 0 is always read as 0 (only even numbers)
 * @param serial_clock_rate Number of prescaler-outputs ber bit. This allows to set the SPI tranfert clock.
 * @param data_size_transfert the number of bits that are transfered in each frame (only values between 4 and 16bits are supported)
 *
 * @note The final clock used as SCK is then \f[\frac{CPU Clock}{CPUDivider \times ClockPrescale \times SerialClockRate}\f]
 * @see ::rflpc_clock_get_system_clock()
 **/
extern void rflpc_spi_init_master(rflpc_spi_t port, rflpc_clock_divider_t cpu_clock_divider, uint8_t clock_prescale, uint8_t serial_clock_rate, uint8_t data_size_transfert);

static inline LPC_SSP_TypeDef *rflpc_spi_get_base_addr(rflpc_spi_t port)
{
    return (port == RFLPC_SPI0) ? LPC_SSP0 : LPC_SSP1;
}

/**
 * Tests if transmition FIFO is empty
 *
 * @param port The SPI port to test
 * @return true if empty
 **/
static inline int rflpc_spi_tx_fifo_empty(rflpc_spi_t port)
{
   LPC_SSP_TypeDef *spi_base = rflpc_spi_get_base_addr(port);/* (port == RFLPC_SPI0) ? LPC_SSP0 : LPC_SSP1; */
   return spi_base->SR & 1;
}

/**
 * Tests if the transmition FIFO is full
 *
 * @param port The SPI port to test
 * @return true if full
 **/
static inline int rflpc_spi_tx_fifo_full(rflpc_spi_t port)
{
   LPC_SSP_TypeDef *spi_base = rflpc_spi_get_base_addr(port); /* (port == RFLPC_SPI0) ? LPC_SSP0 : LPC_SSP1;*/
   return !(spi_base->SR & 2);
}

/**
 * Sends data through the spi interface.
 * @param port The port to send to
 * @param data The data to send. If the port has been configured to send less than 16 bits in each frame, the data must be right justified
 * @warning The destination slave must have been enabled by putting its CS pin to a logical low. This is usually done with GPIO
 **/
static inline void rflpc_spi_write(rflpc_spi_t port, uint16_t data)
{
   LPC_SSP_TypeDef *spi_base = rflpc_spi_get_base_addr(port); /* (port == RFLPC_SPI0 ? LPC_SSP0) : LPC_SSP1;*/
   /* wait for FIFO to be not full */
   while (rflpc_spi_tx_fifo_full(port));
   spi_base->DR = data;
}


/** @} */

#endif


