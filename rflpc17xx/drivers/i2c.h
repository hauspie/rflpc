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
  Author: Michael Hauspie <michael.hauspie@univ-lille1.fr>  
*/

#ifdef RFLPC_CONFIG_ENABLE_I2C

#include <stddef.h> /* size_t */
#include <stdint.h> /* uint8_t */

/** @addtogroup i2c I2C
 * @ingroup drivers
 * @{
 */

/** These constants selects which port the driver functions manipulate. */
typedef enum
{
    RFLPC_I2C_PORT0,
    RFLPC_I2C_PORT1,
    RFLPC_I2C_PORT2,
} rflpc_i2c_port_t;

/** Defines the operating mode of the I2C bus. */
typedef enum
{
    RFLPC_I2C_MODE_MASTER,
    RFLPC_I2C_MODE_SLAVE,
} rflpc_i2c_mode_t;

/** Inits a given I2C port on a given mode.
 * @param port the port to initialize (cf ::rflpc_i2c_port_t)
 * @param mode the mode to use (::rflpc_i2c_mode_t)
 * @param addr if slave mode is requested, the addr of the device. If the least significant bit is set, the device will answer to the general call address (0)
 * @return 0 if successful -1 if error (mainly if the board is not running at 96MHz as the SCL calc is hard coded yet...)
 * @note The port is configured to operate at 100kHz (standard mode). 
 */
int rflpc_i2c_init(rflpc_i2c_port_t port, rflpc_i2c_mode_t mode, uint8_t addr);

/** Writes data buffer to i2c.
 * @param [in] port the port to use
 * @param [in] addr the i2c address to write to (no effect if set up in slave mode)
 * @param [in] byte the byte to send
 * @return -1 if error
 * TODO: update documentation
 */
int rflpc_i2c_write(rflpc_i2c_port_t port, uint8_t addr, 
		    uint8_t *data, uint8_t nbytes, uint8_t stop);

/** Reads data buffer from i2c port.
 * @param [in] port the port to use
 * @param [in] addr the i2c address to read from (no effect in slave mode)
 * @param [out] byte pointer to a byte where to store the read data
 * @return -1 if error
 * TODO: update documentation
 */
int rflpc_i2c_read(rflpc_i2c_port_t port, uint8_t addr, uint8_t *data, uint8_t nbytes);

int rflpc_i2c_get_status(rflpc_i2c_port_t port);
/** @} */


#endif /* ENABLE_I2C */
#endif
