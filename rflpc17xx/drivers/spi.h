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
  Time-stamp: <2012-03-21 09:28:14 (hauspie)>
*/

#ifdef RFLPC_CONFIG_ENABLE_SPI

#include "../clock.h"
#include "../interrupt.h"

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

/** Used to select the SPI operating mode */
typedef enum
{
    RFLPC_SPI_MASTER, /**< Use the SPI as master */
    RFLPC_SPI_SLAVE,  /**< Use the SPI as slave */
} rflpc_spi_mode_t;

/**
 * Inits the SPI interface in master mode.
 *
 * @param port The port to configure
 * @param mode The mode to use
 * @param cpu_clock_divider The divider applied to CPU clock to get the SPI peripheral clock
 * @param data_size_transfert the number of bits that are transfered in each frame (only values between 4 and 16bits are supported)
 * @param clock_prescale Factor by which the prescaler divide the peripheral clock. Between 2 and 254. Bit 0 is always read as 0 (only even numbers). Used only for master mode
 * @param serial_clock_rate Number of prescaler-outputs ber bit. This allows to set the SPI tranfert clock. Used only for master mode. 
 *
 * @note The final clock used as SCK is then \f[\frac{CPU Clock}{CPUDivider \times ClockPrescale \times SerialClockRate}\f]
 *       In slave mode, the clock_prescale and serial_clock_rate parameters are not used. The clock sent by the master must not exceed 1/12 of the frequency used to clock the SPI peripheral
 * @see ::rflpc_clock_get_system_clock()
 **/
extern void rflpc_spi_init(rflpc_spi_t port, rflpc_spi_mode_t mode, rflpc_clock_divider_t cpu_clock_divider, uint8_t data_size_transfert, uint8_t clock_prescale, uint8_t serial_clock_rate);

/**
 * Returns the base address of the SPI control block depending of the desired port
 * @param port the port to get
 * @return pointer to a LPC_SSP_TypeDef structure that can be used to configure the desired SPI.
 * @note Used by the library internals. You should not need to use this function.
 */
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
   LPC_SSP_TypeDef *spi_base = rflpc_spi_get_base_addr(port);
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
   LPC_SSP_TypeDef *spi_base = rflpc_spi_get_base_addr(port);
   return !(spi_base->SR & (1UL << 1));
}

/**
 * Tests if the reception FIFO is empty
 *
 * @param port The SPI port to test
 * @return true if empty
 **/
static inline int rflpc_spi_rx_fifo_empty(rflpc_spi_t port)
{
   LPC_SSP_TypeDef *spi_base = rflpc_spi_get_base_addr(port);
   return !(spi_base->SR & (1UL << 2));
}

/**
 * Sends data through the spi interface.
 * @param port The port to send to
 * @param data The data to send. If the port has been configured to send less than 16 bits in each frame, the data must be right justified
 * @note If the port is configured master, the slave select pin will be activated as long as there is data in the FIFO.
 *       If the port is configured slave, the data will be transmitted only when the slave select is activated by master
 **/
static inline void rflpc_spi_write(rflpc_spi_t port, uint16_t data)
{
   LPC_SSP_TypeDef *spi_base = rflpc_spi_get_base_addr(port);   
   while (rflpc_spi_tx_fifo_full(port));
   spi_base->DR = data;
}

/** Receive data through the spi interface
 * @param port the port to receive from
 * @return the value read on the spi bus
 * @warning this function waits for a data to be available!
 */
static inline uint16_t rflpc_spi_read(rflpc_spi_t port)
{
    LPC_SSP_TypeDef *spi_base = rflpc_spi_get_base_addr(port);    
    while (rflpc_spi_rx_fifo_empty(port));
    return spi_base->DR;
}

/**
 * Sets the interrupt handler for SPI reception.
 * The interrupt will be generated when the RX FIFO is half-full
 * @param port the port to use
 * @param callback the callback to set
 */
extern void rflpc_spi_set_rx_callback(rflpc_spi_t port, rflpc_irq_handler_t callback);


/** @} */

#endif /* ENABLE_SPI */


#endif
