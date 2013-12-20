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
  Author: Gregory Guche <gregory.guche@lifl.fr>
  Created: 2012-03-28
*/



#ifdef RFLPC_CONFIG_ENABLE_IAP

#include "iap.h"
#include "nxp/LPC17xx.h"
#include "clock.h"
#include "tinylibc/memcpy.h"

/* IAP buffers */
#ifndef IAP_SMALL_SECTOR_BUFFER_SIZE
#define IAP_SMALL_SECTOR_BUFFER_SIZE  4096
#define IAP_LARGE_SECTOR_BUFFER_SIZE 32768
#endif

char iap_sector_buffer[IAP_SMALL_SECTOR_BUFFER_SIZE];


/* IAP commands */
#define IAP_PREPARE_SECTORS_FOR_WRITING 50
#define IAP_COPY_RAM_TO_FLASH           51
#define IAP_ERASE_SECTORS               52
#define IAP_READ_SERIAL_COMMAND         58

/* IAP Function */
#define IAP_FUNCTION_ADDR               0x1FFF1FF1

typedef void (*iap_function_t)(unsigned long[], unsigned long[]);
#define DECLARE_IAP_FUNCTION iap_function_t iap = (iap_function_t) IAP_FUNCTION_ADDR

/* IAP return values.*/
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

int rflpc_iap_erase_sectors(int start_sector, int end_sector) {
    unsigned long command[4] = {IAP_ERASE_SECTORS, 0L, 0L, 0L};
    DECLARE_IAP_FUNCTION;

    command[1] = (unsigned long)start_sector;
    command[2] = (unsigned long)end_sector;
    /* Clock in KHz.*/
    command[3] = (unsigned long)(rflpc_clock_get_system_clock() / 1000);

    iap(command, command);

    return (command[0] == IAP_CMD_SUCCESS) ? 0 : -1;
}

int rflpc_iap_copy_ram_to_flash(const void *destination, const void *source, int length) {
    unsigned long command[5] = { IAP_COPY_RAM_TO_FLASH, 0L, 0L, 0L, 0L};
    DECLARE_IAP_FUNCTION;
  
    command[1] = (unsigned long)destination;
    command[2] = (unsigned long)source;
    command[3] = (unsigned long)length;
    /* Clock in KHz.*/
    command[4] = (unsigned long)(rflpc_clock_get_system_clock() / 1000);

    iap(command, command);


    return (command[0] == IAP_CMD_SUCCESS) ? 0 : -1;
}

/* IAP memory map */
int rflpc_iap_get_sector_from_address(const void *address) {

    if(address< (const void *)0x00010000)
	return ((uint32_t)address) >> 12/*/ 4096*/;

    return 16 + ((uint32_t)(address - (const void *)0x00010000)>>15); /*/32768*/
}

void *rflpc_iap_get_address_from_sector(int aSector) {
    if(aSector < 16)
	return (void *)(aSector << 12);

    return (void *)(0x00010000 + ((aSector - 16) << 15));
}

/*#define DEBUG*/
#ifdef DEBUG
#include "tinylibc/printf.h"
/* To check if trying to write to initial code */
extern uint8_t _text_end;
extern uint8_t _text_start;
#endif

int rflpc_iap_write_buffer_to_sector(const void *buffer, int sector_number) {

    uint8_t *destination = (uint8_t *) rflpc_iap_get_address_from_sector(sector_number);
    int ret;

#ifdef DEBUG
    /* Check if trying to write in code! */
    if (destination >= &_text_start && destination < &_text_end)
    {
        printf("Warning, trying to write a buffer in sector %d (%p): this will overwrite code!\r\n", sector_number, destination);
    }
    
#endif

    ret = rflpc_iap_prepare_sectors_for_writing(sector_number, sector_number);
    if(ret != 0) {
	return ret;
    }

    ret = rflpc_iap_erase_sectors(sector_number, sector_number);
    if(ret != 0)
	return ret;

    ret = rflpc_iap_prepare_sectors_for_writing(sector_number, sector_number);
    if(ret != 0)
	return ret;

    
    if (sector_number < 16)
	ret = rflpc_iap_copy_ram_to_flash(destination, buffer, IAP_SMALL_SECTOR_BUFFER_SIZE);
    else
	ret = rflpc_iap_copy_ram_to_flash(destination, buffer, IAP_LARGE_SECTOR_BUFFER_SIZE);

    return ret;
}

int rflpc_iap_write_to_sector(const void *destination, const void *buffer, int length) {
    int offset;

    int sector = rflpc_iap_get_sector_from_address(destination);
    void *startSectorAddress = rflpc_iap_get_address_from_sector(sector);

    /* Not over multiple sectors. */
    if( sector != rflpc_iap_get_sector_from_address(((uint8_t*)destination) + length - 1))
	return -1;

    /* This function does not work for 32k sectors */
    if (sector >= 16)
    {
#ifdef DEBUG
	printf("Trying to use write_to_sector on a 32k sector\r\n");
#endif
	return -1;
    }
  
    /* Retrieve the sector content into the buffer*/
    memcpy(iap_sector_buffer, startSectorAddress, IAP_SMALL_SECTOR_BUFFER_SIZE);


    offset = (int)(destination - rflpc_iap_get_address_from_sector(sector));

    /* In the buffer, at offset, copy the data*/
    memcpy(iap_sector_buffer + offset, buffer, length);
  
    /* Burn down the sector buffer to the flash*/
    return rflpc_iap_write_buffer_to_sector(iap_sector_buffer, sector);
}

int rflpc_iap_write_buffer(const void *destination, const void *buffer, int length) {

    int sector = rflpc_iap_get_sector_from_address(destination);
    int len, ret;

  
    if(sector != rflpc_iap_get_sector_from_address(((uint8_t*)destination) + length)) {
	/* Copy from destination till the end of the current sector. */
	len = rflpc_iap_get_address_from_sector(sector + 1) - destination;
	ret = rflpc_iap_write_to_sector(destination, buffer, len);
	if(ret != 0)
	    return ret;

	length      -= len;
	destination += len;
	buffer      += len;
	sector++;

	/* Copy full sectors */
	while(sector != rflpc_iap_get_sector_from_address(destination + length) && (length>0)) {
	    len = (int)(rflpc_iap_get_address_from_sector(sector + 1) - rflpc_iap_get_address_from_sector(sector) - 1);
	  
	    ret = rflpc_iap_write_to_sector(destination, buffer, len);

	    if(ret != 0)
		return ret;
	  
	    destination += len;
	    buffer      += len;
	    length      -= len;
	    sector      = rflpc_iap_get_sector_from_address(destination);
	}
    }
    /* Copy remaining data */
    return (length > 0) ? rflpc_iap_write_to_sector(destination, buffer, length) : 0;
}

/* erase once 32k, write multiple times. */
int rflpc_iap_transfert_4ks_to_32k(const void *destination32k, const void *source4ks, int length) {
    int sector = rflpc_iap_get_sector_from_address(destination32k);  
    int offset = 0;

    /* erase 32ks */
    if(rflpc_iap_prepare_sectors_for_writing(sector, sector) != 0) {
	return -1;
    }

    if(rflpc_iap_erase_sectors(sector, sector) != 0) {
	return -2;
    }

    /* write down 4ks */
    while(length>IAP_SMALL_SECTOR_BUFFER_SIZE) {

	memcpy(iap_sector_buffer, source4ks + offset, IAP_SMALL_SECTOR_BUFFER_SIZE);

	if(rflpc_iap_prepare_sectors_for_writing(sector, sector) != 0) {
	    return -3;
	}

	if(rflpc_iap_copy_ram_to_flash(destination32k + offset, iap_sector_buffer, IAP_SMALL_SECTOR_BUFFER_SIZE) != 0) {
	    return -4;
	}
	
	length -= IAP_SMALL_SECTOR_BUFFER_SIZE;
	offset += IAP_SMALL_SECTOR_BUFFER_SIZE;
    }
    
    if(length > 0) {
	
	memcpy(iap_sector_buffer, source4ks + offset, length);
	
	if(rflpc_iap_prepare_sectors_for_writing(sector, sector) != 0) {
	    return -5;
	}
	
	if(rflpc_iap_copy_ram_to_flash(destination32k + offset, iap_sector_buffer, IAP_SMALL_SECTOR_BUFFER_SIZE) != 0) {
	    return -6;
	}
    }
    
    return 0;
}

#endif /* ENABLE_IAP */
