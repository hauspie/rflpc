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
 * I2C Device driver
 */

#ifndef __RFLPC_I2C_H__
#define __RFLPC_I2C_H__

/*
  Author: Christophe Bacara <christophe.bacara@etudiant.univ-lille1.fr>  
*/

#ifdef RFLPC_CONFIG_ENABLE_I2C

#include <stddef.h> /* size_t */
#include <stdint.h> /* uint8_t */

/** @addtogroup i2c I2C
 * @ingroup drivers
 * @{
 */

/** These constants selects which port the driver functions manipulate. */
typedef enum { 
  RFLPC_I2C_PORT0, 
  RFLPC_I2C_PORT1, 
  RFLPC_I2C_PORT2
} rflpc_i2c_port_t;

/** Defines the operating mode of the I2C bus. */
typedef enum { 
  RFLPC_I2C_MODE_MASTER, 
  RFLPC_I2C_MODE_SLAVE
} rflpc_i2c_mode_t;

/** Inits a given I2C port on a given mode.
 * @param port The I2C port to initialize (cf. ::rflpc_i2c_port_t)
 * @param mode The I2C mode to use (cf. ::rflpc_i2c_mode_t)
 * @param addr If Slave mode is requested, this parameter is the I2C address of 
 *             the initialized port. If the least significant bit is set, the
 *             device will answer to the general call address (0).
 * @return If initialization was successful, returns 0. Else, returns -1.
 * @note The port is configured to operate at 100kHz (standard mode).
 * @note At this time, ONLY MASTER MODE is allowed and functionnal.
 */
int rflpc_i2c_init(rflpc_i2c_port_t port, rflpc_i2c_mode_t mode, uint8_t addr);

/** Send bytes from data in Write mode on the I2C driver.
 * @param [in] port The I2C port to write on.
 * @param [in] addr The I2C address of the slave to which the message will be sent.
 *                  This parameter has no effect if Slave mode is set.
 * @param [in] data A pointer to the data buffer to be transmitted.
 * @param [in] nbytes The number of bytes to be transmitted.
 * @param [in] stop If equal to zero, the driver will send a RESTART condition
 *                  instead of a STOP condition at the end of transmission.
 * @return There is three different return values for this function :
 *         - If the message has been successfully transmitted, returns 0.
 *         - If any known error has occured, returns the last state of I2C port.
 *         - If any unknown error has occured, returns -1.
 */
int rflpc_i2c_write(rflpc_i2c_port_t port, uint8_t addr, 
		    uint8_t *data, uint8_t nbytes, uint8_t stop);

/** Reads data buffer from i2c port.
 * @param [in] port The I2C port to read on.
 * @param [in] addr The I2C address of the slave from which a message will be read.
 * @param [in] data A pointer to a data buffer when the message will be stored.
 * @param [in] nbytes The expected number of bytes to be received.
 * @param [in] stop If equal to zero, the driver will send a RESTART condition
 *                  instead of a STOP condition at the end of transmission.
 * @return There is three different return values for this function :
 *         - If the message has been successfully transmitted, returns 0.
 *         - If any known error has occured, returns the last state of I2C port.
 *         - If any unknown error has occured, returns -1.
 */
int rflpc_i2c_read(rflpc_i2c_port_t port, uint8_t addr, 
		   uint8_t *data, uint8_t nbytes, uint8_t stop);

/** DEVELOPMENT PURPOSE ONLY: Send back the status of specified i2c port.
 */
int rflpc_i2c_get_status(rflpc_i2c_port_t port);

/** @} */

#endif /* ENABLE_I2C */
#endif
