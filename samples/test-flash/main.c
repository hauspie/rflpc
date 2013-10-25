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
  Created:
  Time-stamp: <2013-10-25 10:10:39 (hauspie)>
*/
#include <rflpc17xx/rflpc17xx.h>

#define DATA_SIZE            256
#define SECTOR_START_INDEX   20
#define SECTOR_END_INDEX     20
#define SECTOR_START_ADDRESS 0x00030000

extern char _text_end;
extern char _data_start;
extern char _data_end;

static char data[DATA_SIZE];

static char buffer[] = { 0xCA, 0xFE, 0xBA, 0xBE, 0xDE, 0xAD,0xBE,0xEF};

int test_simple_copy(void *anAddress, int aStartSector, int anEndSector) {
    int i;
    uint8_t *address = (uint8_t *)anAddress;

    printf("------------------------%s------------------------\r\n", __FUNCTION__);

    for(i = 0; i < DATA_SIZE; i++)
	data[i] = i % 256;

    rflpc_iap_prepare_sectors_for_writing(aStartSector, anEndSector);
    rflpc_iap_erase_sectors(aStartSector, anEndSector);

    rflpc_iap_prepare_sectors_for_writing(aStartSector, anEndSector);
    rflpc_iap_copy_ram_to_flash(anAddress, data, DATA_SIZE);

    for(i = 0; i < DATA_SIZE; i++) {

	printf("Checking 0x%x (%d)\r\n", i, address[i]);

	if( address[i] != data[i]) {
            printf("%s Failed at %d\r\n", __FUNCTION__, i);
            return -1;
        }
    }

    printf("Simple RAM to Flash Test done successfully\r\n\r\n");
    return 0;
}

int test_writing(void *anAddress) {
    int i, count, ret;
    printf("RAM to Flash Test started\r\n");

    count = (int)anAddress + sizeof(buffer);
    for(i = (int)anAddress;  i < count; i++)
	printf("%02x ", *((char *)i));

    printf("\r\n");
    

    ret = rflpc_iap_write_to_sector(anAddress, buffer, sizeof(buffer));

    if(ret != 0) {
	printf("%s Failed : %d\r\n", __FUNCTION__, ret);
	return -1;
    }

    for(i = (int)anAddress;  i < count; i++)
	printf("%02x ", *((char *)i));

    printf("\r\nRAM to Flash Test done\r\n");
    return 0;
}

char testBuffer[4096];

int test_writing_from_ram(void *anAddress) {
    int i, sector, ret;
    printf("------------------------%s------------------------\r\n", __FUNCTION__);

    
    /*Fill the buffer with random*/
    srand(10);
    for(i = 0; i < 4096; i++)
	testBuffer[i] =rand() & 0xFF;


    /*Write the buffer down to flash*/
    sector = getSectorFromAddress(anAddress);
    ret = rflpc_iap_prepare_sectors_for_writing(sector, sector);
    ret = rflpc_iap_erase_sectors(sector, sector);

    ret = rflpc_iap_prepare_sectors_for_writing(sector, sector);
    ret = rflpc_iap_copy_ram_to_flash(anAddress, testBuffer, 4096);

    if(ret != 0) {
	printf("%s Failed : %d\r\n", __FUNCTION__, ret);
	return -1;
    }

    /* Display & check*/
    for(i = 0; i < 4096; i++) {

	if(!(i % 16))
	    printf("\r\n");
	printf("%02x ", ((uint8_t*)anAddress)[i]);

    	if(testBuffer[i] != ((uint8_t*)anAddress)[i]) {
	    printf("%s failed at %d (%x vs %x)\r\n", __FUNCTION__, i, 
		   testBuffer[i], 	((uint8_t*)anAddress)[i]);
	    return -2;
	}
    }

    printf("\r\nRAM to Flash Test done\r\n");
    return 0;
}

char sequenceBuffer[128];


int testSequence(void *anAddress) {
    int i, j, ret;
    uint8_t *destination = anAddress;

    printf("------------------------%s------------------------\r\n", __FUNCTION__);

    for(i = 0; i < 2; i++) {

	ret = rflpc_iap_write_to_sector(destination, buffer, sizeof(buffer));

	if(ret != 0) {
	    printf("%s Failed : %d\r\n", __FUNCTION__, ret);
	    return -1;
	}

	destination += sizeof(buffer);

	for(j = 0; j < 128; j++) {

	    if(!(j % 16))
		printf("\r\n");
	    printf("%02x ", ((uint8_t*)anAddress)[j]);
	}
	printf("\r\n");
    }

    return 0;
}

const char object []= {
    127,69,76,70,1,1,1,0,0,0,0,0,0,0,0,0,1,0,40,0,1,0,0,0,0,0,0,0,0,0,0,0,-128,1,0,0,0,0,0,5,52,0,0,0,0,0,40,0,11,0,8,0,-128,-76,0,-81,64,-14,0,3,-64,-14,0,3,27,104,24,70,-67,70,-128,-68,112,71,0,-65,-128,-75,0,-81,64,-14,0,3,-64,-14,0,3,27,104,64,-14,0,0,-64,-14,0,0,25,70,-1,-9,-2,-1,-1,-9,-2,-1,3,70,1,51,24,70,-128,-67,-128,-76,0,-81,64,-14,0,3,-64,-14,0,3,64,-14,0,2,-64,-14,0,2,26,96,64,-14,0,3,-64,-14,0,3,64,-14,0,2,-64,-14,0,2,90,96,64,-14,0,3,-64,-14,0,3,24,70,-67,70,-128,-68,112,71,42,0,0,0,43,0,0,0,79,75,61,62,37,100,13,10,0,0,0,0,0,71,67,67,58,32,40,76,105,110,97,114,111,32,71,67,67,32,52,46,55,45,50,48,49,51,46,48,49,41,32,52,46,55,46,51,32,50,48,49,51,48,49,48,50,32,40,112,114,101,114,101,108,101,97,115,101,41,0,65,50,0,0,0,97,101,97,98,105,0,1,40,0,0,0,5,67,111,114,116,101,120,45,77,51,0,6,10,7,77,9,2,18,4,20,1,21,1,23,3,24,1,25,1,26,1,30,6,34,1,0,46,115,121,109,116,97,98,0,46,115,116,114,116,97,98,0,46,115,104,115,116,114,116,97,98,0,46,114,101,108,46,116,101,120,116,0,46,100,97,116,97,0,46,98,115,115,0,46,114,111,100,97,116,97,0,46,99,111,109,109,101,110,116,0,46,65,82,77,46,97,116,116,114,105,98,117,116,101,115,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,31,0,0,0,1,0,0,0,6,0,0,0,0,0,0,0,52,0,0,0,120,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,27,0,0,0,9,0,0,0,0,0,0,0,0,0,0,0,-80,4,0,0,-112,0,0,0,9,0,0,0,1,0,0,0,4,0,0,0,8,0,0,0,37,0,0,0,1,0,0,0,3,0,0,0,0,0,0,0,-84,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,43,0,0,0,8,0,0,0,3,0,0,0,0,0,0,0,-84,0,0,0,8,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,48,0,0,0,1,0,0,0,2,0,0,0,0,0,0,0,-84,0,0,0,20,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,56,0,0,0,1,0,0,0,48,0,0,0,0,0,0,0,-64,0,0,0,59,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,65,0,0,0,3,0,0,112,0,0,0,0,0,0,0,0,-5,0,0,0,51,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,17,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,46,1,0,0,81,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,56,3,0,0,48,1,0,0,10,0,0,0,15,0,0,0,4,0,0,0,16,0,0,0,9,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,104,4,0,0,69,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,4,0,-15,-1,0,0,0,0,0,0,0,0,0,0,0,0,3,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,5,0,8,0,0,0,0,0,0,0,0,0,0,0,0,0,5,0,11,0,0,0,0,0,0,0,4,0,0,0,1,0,5,0,15,0,0,0,4,0,0,0,4,0,0,0,1,0,5,0,8,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,19,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,29,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,32,0,0,0,8,0,0,0,0,0,0,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,7,0,37,0,0,0,1,0,0,0,22,0,0,0,18,0,1,0,43,0,0,0,25,0,0,0,40,0,0,0,18,0,1,0,49,0,0,0,0,0,0,0,0,0,0,0,16,0,0,0,56,0,0,0,65,0,0,0,56,0,0,0,18,0,1,0,0,84,101,115,116,46,99,0,36,100,0,99,52,50,0,99,52,51,0,99,97,108,108,98,97,99,107,115,0,36,116,0,46,76,67,48,0,116,101,115,116,48,0,116,101,115,116,49,0,112,114,105,110,116,102,0,103,101,116,67,97,108,108,98,97,99,107,115,0,0,0,0,4,0,0,0,47,7,0,0,8,0,0,0,48,7,0,0,28,0,0,0,47,7,0,0,32,0,0,0,48,7,0,0,38,0,0,0,47,12,0,0,42,0,0,0,48,12,0,0,48,0,0,0,10,17,0,0,52,0,0,0,10,15,0,0,68,0,0,0,47,10,0,0,72,0,0,0,48,10,0,0,76,0,0,0,47,15,0,0,80,0,0,0,48,15,0,0,86,0,0,0,47,10,0,0,90,0,0,0,48,10,0,0,94,0,0,0,47,16,0,0,98,0,0,0,48,16,0,0,104,0,0,0,47,10,0,0,108,0,0,0,48,10,0,0
};
const int object_size = sizeof(object);

#define WRITING_BUFFER_SIZE 128

char writingBuffer[WRITING_BUFFER_SIZE];

int testBufferedWriting(void *anAddress) {
    int i, j, k;
    uint8_t *destination = (uint8_t *)anAddress;

    j = 0;
    k = 0;

    printf("------------------------%s------------------------\r\n", __FUNCTION__);
    printf("Object size = %d\r\n", object_size);
    for(i = 0; i < object_size; i++) {
	if(j < WRITING_BUFFER_SIZE) {
	    writingBuffer[j] = object[i];
	    j++;

	} else {
      
	    if(rflpc_iap_write_to_sector(destination + k, writingBuffer, WRITING_BUFFER_SIZE) != 0) {
		printf("%s Failed to write sector\r\n", __FUNCTION__);
		return -1;
	    }

	    writingBuffer[0] = object[i];
	    j = 1;
	    k += WRITING_BUFFER_SIZE;
	    printf("Progress %d\r\n", k);
	}
    }

    if(j > 0) {
	if(rflpc_iap_write_to_sector(destination + k, writingBuffer, j) != 0) {
	    printf("An error happened while flashing elf to storage\r\n");
	    return 1;
	}
    }

    printf("Checking...\r\n");
    for(i = 0; i < object_size; i++) {
	if(destination[i] != object[i]) {
	    printf("%s Failed at %d\r\n", __FUNCTION__, i);
	    return -2;
	}
    }

    printf("Done.\r\n");

    return 0;
}

char largeBuffer[6000];

int testLargeBuffer(void *anAddress) {
    int i;
    int size = sizeof(largeBuffer);
    char *address = (char *)anAddress;

    printf("------------------------%s------------------------\r\n", __FUNCTION__);

    printf("Size %d\r\n", size);
    printf("Generating...\r\n");
    for(i = 0; i < size; i++) {

	largeBuffer[i] = (char)(i % 256);
    }

    if(rflpc_iap_write_buffer(anAddress, largeBuffer, size) != 0) {
	printf("%s Failed at writing large buffer\r\n", __FUNCTION__);
	return -1;
    }

    printf("Checking...\r\n");
    for(i = 0; i < size; i++)
	if(address[i] != largeBuffer[i]){
	    printf("%s Failed at %d\r\n", __FUNCTION__, i);
	    return -2;
	}


    printf("Done.\r\n");
    return 0;
}

int testWritingInto32k() {
    int sector32k    = 16;
    char *address32k = (char *)getAddressFromSector(sector32k);
    int sector4k     = 9;
    char *address4k  = (char *)getAddressFromSector(sector4k);
    int length       = sizeof(largeBuffer);
    int i;

    printf("%s started..\r\n", __FUNCTION__);

    for(i = 0; i < length; i++)
	largeBuffer[i] = i % 256;

    if(rflpc_iap_write_buffer(address4k, largeBuffer, length) != 0) {
	printf("%s Failed at writing large buffer\r\n", __FUNCTION__);
	return -1;
    }

    if(rflpc_iap_transfert_4ks_to_32k(address32k, address4k, length) != 0) {
	printf("%s Failed at transferring %p to %p (%d bytes)\r\n", __FUNCTION__, address4k, address32k, length);
	return -2;
    }

    printf("Checking...\r\n");

    for(i = 0; i < length; i++) {
	if(address32k[i] != largeBuffer[i]) {
	    printf("%s Failed at %d\r\n", __FUNCTION__, i);
	    return -3;
	}
    }

    printf("%s done.\r\n", __FUNCTION__);
    return 0;
}

const char *ok = {0};

int main() {

    uint8_t *address;
    int      sector;

    rflpc_clock_init();
    rflpc_uart_init(RFLPC_UART0);

    /*char *start_address = (char *)(&_text_end + (&_data_end - &_data_start));*/


    // Sector is 8. @see NXP's user manual to set the sector accordingly with the address or use getSectorFromAddress.
    address = 0x9000/*(uint8_t*)(&_text_end + (&_data_end - &_data_start))*/;
    sector  = getSectorFromAddress(address);
    printf("Address is %p Sector is %d\r\n", address, sector);

/*    test_simple_copy(address, sector, sector);*/
/*
  printf("---------------------------------------------------------------\r\n");
  test_writing_from_ram(address);*/


/*    printf("---------------------------------------------------------------\r\n");
      testSequence(address);*/


/*    printf("---------------------------------------------------------------\r\n");
      testBufferedWriting(address);*/

/*    printf("---------------------------------------------------------------\r\n");
      testLargeBuffer(address);*/

    testWritingInto32k();

    while (1);
    return 0;
}
