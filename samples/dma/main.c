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
  Time-stamp: <2012-03-08 16:09:45 (hauspie)>
*/
#include <rflpc17xx/rflpc17xx.h>

#define BUFFER_SIZE 128
#define BUFFER_BYTES (BUFFER_SIZE*sizeof(uint32_t))
static uint32_t source_buffer[BUFFER_SIZE];
static uint32_t destination_buffer[BUFFER_SIZE];

void copy_and_check(rflpc_dma_channel_t channel, void *dest, const void *src, uint32_t size)
{
   uint32_t i;

   printf("Copy from %p to %p of %d bytes using channel %d\r\n", src, dest, size, channel);
   if (rflpc_dma_channel_ready(channel))
   {
      if (!rflpc_dma_start(channel, dest, src, size))
      {
         printf("Dma start failed\r\n");
      }
      printf("--- %x\r\n", size);
      while (!rflpc_dma_channel_ready(channel)) /* wait for transfert to finish */
      {
         RFLPC_DELAY(1000000);
         /*printf("%x\r\n", LPC_GPDMA->DMACEnbldChns);
         printf("%x\r\n", LPC_GPDMACH0->DMACCControl);*/
      }
      printf("DMA copy finished\r\n");
      for (i = 0 ; i < size ; ++i)
      {
         if (((uint8_t*)src)[i] != ((uint8_t*)dest)[i])
         {
            printf("Assertion failed: expected 0x%02x got 0x%02x at %d\r\n", ((uint8_t*)src)[i], ((uint8_t*)dest)[i], i);
            break;
         }
      }
   }
   else
   {
      printf("Channel %d not ready\r\n", channel);
   }
}

int main()
{
   int i;
   rflpc_uart_init(RFLPC_UART0);
   rflpc_dma_init();
   printf("%x\r\n", LPC_GPDMA->DMACEnbldChns);

   for (i = 0 ; i < BUFFER_SIZE ; ++i)
   {
      source_buffer[i] = i;
      destination_buffer[i] = 0xCAFEDECA;
   }

   copy_and_check(RFLPC_DMAC0, destination_buffer, source_buffer, BUFFER_BYTES);
   copy_and_check(RFLPC_DMAC0, ((uint8_t*)destination_buffer)+1, ((uint8_t*)source_buffer), BUFFER_BYTES-1);

   while (1){}
   return 0;
}
