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
  Created:
  Time-stamp: <2012-03-21 09:40:41 (hauspie)>
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
    int i, count;
    uint8_t *address = (uint8_t *)anAddress;

    printf("Simple RAM to Flash Test started\r\n");

    for(i = 0; i < DATA_SIZE; i++)
      data[i] = i % 256;

/*    rflpc_iap_prepare_sectors_for_writing(aStartSector, anEndSector);
    rflpc_iap_erase_sectors(aStartSector, anEndSector);*/

    rflpc_iap_prepare_sectors_for_writing(aStartSector, anEndSector);
    rflpc_iap_copy_ram_to_flash(anAddress, data, DATA_SIZE);

    for(i = 0; i < DATA_SIZE; i++) {

	printf("Checking 0x%x (%d)\r\n", i, address[i]);

	if( address[i] != data[i]) {
            printf("Test Failed at %d\r\n", (i -SECTOR_START_ADDRESS));
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
      printf("Test Failed : %d\r\n", ret);
      return -1;
    }

    for(i = (int)anAddress;  i < count; i++)
      printf("%02x ", *((char *)i));

    printf("\r\nRAM to Flash Test done\r\n");
    return 0;
}

char testBuffer[4096];

int test_writing_from_ram(void *anAddress) {
    int i;

    /*Write the buffer down to flash*/
    int ret = rflpc_iap_prepare_sectors_for_writing(getSectorFromAddress(anAddress),
				       getSectorFromAddress(anAddress));
    ret = rflpc_iap_erase_sectors(getSectorFromAddress(anAddress),
				       getSectorFromAddress(anAddress));

    ret = rflpc_iap_prepare_sectors_for_writing(getSectorFromAddress(anAddress),
				       getSectorFromAddress(anAddress));
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
      printf("Test Failed : %d\r\n", ret);
      return -1;
    }

    destination += sizeof(buffer);

    for(j = 0; j < 128; j++) {

	    if(!(j % 16))
		    printf("\r\n");
	    printf("%02x ", ((uint8_t*)anAddress)[j]);
/*      sequenceBuffer[j] = ((uint8_t*)anAddress)[j];*/
    }
    printf("\r\n");


    /*printf("From Sequence buffer \r\n");
    for(j = 0; j < 128; j++) {

	    if(!(j % 16))
		    printf("\r\n");
	    printf("%02x ", sequenceBuffer[j]);
    }
    printf("\r\n");*/
  }

  return 0;
}

int main() {

    uint8_t address;
    int i;

    rflpc_clock_init();
    rflpc_uart_init(RFLPC_UART0);

    /*char *start_address = (char *)(&_text_end + (&_data_end - &_data_start));*/
    test_simple_copy((void *)0x8000, 8, 8);

/*    test_writing(0x00006000);
    test_writing(0x00006005);
    test_writing(0x00006010);*/

    address = 0x8000;
    srand(12);
    /*Fill the buffer*/
    for(i = 0; i < 4096; i++) {
    	testBuffer[i] = (char)(rand() & 0xFF);
    }

    test_writing_from_ram(address);


    printf("---------------------------------------------------------------\r\n");

///    test_writing_from_ram(0x8000);

    testSequence((void *)0x8000);


    while (1);
    return 0;
}
