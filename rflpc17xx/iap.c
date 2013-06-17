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
  Created: 2012-03-28
*/



#ifdef RFLPC_CONFIG_ENABLE_IAP

#include "iap.h"
#include "nxp/LPC17xx.h"
#include "clock.h"

#define IAP_PREPARE_SECTORS_FOR_WRITING 50
#define IAP_COPY_RAM_TO_FLASH           51
#define IAP_READ_SERIAL_COMMAND         58

#define IAP_FUNCTION_ADDR               0x1FFF1FF1

typedef void (*iap_function_t)(unsigned long[], unsigned long[]);

#define DECLARE_IAP_FUNCTION iap_function_t iap = (iap_function_t) IAP_FUNCTION_ADDR

enum
{
    IAP_CMD_SUCCESS,
    IAP_CMD_INVALID_COMMAND,
    IAP_SRC_ADDR_ERROR,
    IAP_DST_ADDR_ERROR,
    IAP_SRC_ADDR_NOT_MAPPED,
    IAP_DST_ADDR_NOT_MAPPED,
    IAP_COUNT_ERROR,
    IAP_INVALID_SECTOR,
    IAP_SECTOR_NOT_BLANK,
    IAP_SECTOR_NOT_PREPARED_FOR_WRITE_OPERATION,
    IAP_COMPARE_ERROR,
    IAP_BUSY,
};

int rflpc_iap_get_serial_number(unsigned long result[4])
{
    unsigned long command[5] = {IAP_READ_SERIAL_COMMAND, 0, 0, 0, 0};
    DECLARE_IAP_FUNCTION;
    
    iap(command, command);
    
    if (command[0] != IAP_CMD_SUCCESS)
	return -1;
    result[0] = command[1];
    result[1] = command[2];
    result[2] = command[3];
    result[3] = command[4];
    return 0;
}

int rflpc_iap_prepare_sectors_for_writing(int start_sector, int end_sector) {
    unsigned long command[3] = {IAP_PREPARE_SECTORS_FOR_WRITING, 0L, 0L};
    DECLARE_IAP_FUNCTION;

    command[1] = (unsigned long)start_sector;
    command[2] = (unsigned long)end_sector;

    iap(command, command);

    return (command[0] == IAP_CMD_SUCCESS) ? 0 : -1;
}

int rflpc_iap_copy_ram_to_flash(void *destination, const void *source, int length) {
    unsigned long command[5] = { IAP_COPY_RAM_TO_FLASH, 0L, 0L, 0L, 0L};
    DECLARE_IAP_FUNCTION;
  
    command[1] = (unsigned long)destination;
    command[2] = (unsigned long)source;
    command[3] = (unsigned long)length;
    // Clock in KHz.
    command[4] = (unsigned long)(rflpc_clock_get_system_clock() / 1000);

    iap(command, command);

    return (command[0] == IAP_CMD_SUCCESS) ? 0 : -1;
}

#endif /* ENABLE_IAP */
