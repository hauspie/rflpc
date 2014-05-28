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

#include <stdint.h> /* uint8_t */

/** @addtogroup i2c I2C
 * Provides functions for I2C bus manipulation.
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

/** Defines the return status of the I2C driver. */
typedef enum {
   RFLPC_I2C_SUCCESSFUL_TRANSMISSION = 0,
   RFLPC_I2C_SIZE_EQUAL_TO_ZERO,
   RFLPC_I2C_SLAVE_IS_BUSY,
   RFLPC_I2C_SLAVE_DID_NOT_ACKNOWLEDGED_DATA,
   RFLPC_I2C_MASTER_DID_NOT_ACKNOWLEDGED_DATA,
   RFLPC_I2C_STOP_CONDITION_RECEIVED,
   RFLPC_I2C_SLAVE_TRANSMITTED_DATA,
   RFLPC_I2C_UNKNOWN_ERROR,
   RFLPC_I2C_NOT_YET_IMPLEMENTED
} rflpc_i2c_status_t;

/** 
 * Inits a given I2C port on a given mode.
 *
 * @param port The I2C port to initialize (cf. ::rflpc_i2c_port_t)
 * @param mode The I2C mode to use (cf. ::rflpc_i2c_mode_t)
 * @param addr If Slave mode is requested, this parameter is the I2C address of 
 * @return If initialization was successful, returns 0. Else, returns -1.
 * @note The port is configured to operate at 100kHz (standard mode).
 * @note At this time, ONLY MASTER MODE is allowed and functionnal.
 */
extern int rflpc_i2c_init(rflpc_i2c_port_t port, rflpc_i2c_mode_t mode, uint8_t addr);

/** Send bytes on the I2C bus.
 * @param [in] port The I2C port to write on.
 * @param [in] addr The I2C address of the slave to which the message will be sent.
 *                  This parameter has no effect if Slave mode is set.
 * @param [in] data A pointer to the data buffer to be transmitted.
 * @param [in] nbytes The number of bytes to be transmitted.
 * @param [in] stop If equal to zero, the driver will send a RESTART condition
 *                  instead of a STOP condition at the end of transmission.
 * @return Returns a 16-bits value. The 8 MSBs store the final status of I2C
 *         action. The 8 LSBs store the effective number of bytes transmitted.
 */
extern uint16_t rflpc_i2c_write(rflpc_i2c_port_t port, uint8_t addr, 
				uint8_t *data, uint8_t nbytes, uint8_t stop);

/** Reads data from the I2C bus.
 * @param [in] port The I2C port to read on.
 * @param [in] addr The I2C address of the slave from which a message will be read.
 * @param [in] data A pointer to a data buffer where the message will be stored.
 * @param [in] nbytes The expected number of bytes to be received.
 * @param [in] stop If equal to zero, the driver will send a RESTART condition
 *                  instead of a STOP condition at the end of transmission.
 * @return Returns a 16-bits value. The 8 MSBs store the final status of I2C
 *         action. The 8 LSBs store the effective number of bytes transmitted.
 *
 * @note In Slave mode, in case of receiving a Read command, this driver will
 *       automatically switch to ::rflpc_i2c_write to transmit data. Data to be
 *       transmitted can be set using the ::rflpc_i2c_set_slave_transmitter_config
 *       function.
 *       If this Slave to Master transmission is successful, the 8 MSBs of the
 *       returned value will store the RFLPC_I2C_SLAVE_TRANSMITTED_DATA specific
 *       status, and the 8 LSBs of the returned value will store the number of
 *       bytes transmitted, matching the number of bytes to transmit as set with
 *       ::rflpc_i2c_set_slave_transmitter_config. 
 *       If this Slave to Master transmission fails, the returned value will be
 *       the returned value of the internal call of ::rflpc_i2c_write, which is
 *       similar as an external call. See documentation of ::rflpc_i2c_status_t for
 *       more informations.
 */
extern uint16_t rflpc_i2c_read(rflpc_i2c_port_t port, uint8_t addr, 
			       uint8_t *data, uint8_t nbytes, uint8_t stop);

/** Set data's location and size to be transmitted as slave.
 * @param [in] data A pointer to the data to be transmitted.
 * @param [in] size Size in bytes of the data to be transmitted.
 *
 * @note Size must be greater than zero. If not, this function has no effect.
 */
extern void rflpc_i2c_set_slave_transmitter_config(uint8_t *data, uint8_t size);

/** @} */

#endif /* ENABLE_I2C */

#endif
