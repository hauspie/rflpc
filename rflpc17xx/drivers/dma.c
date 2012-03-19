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
 * Author: Michael Hauspie <michael.hauspie@univ-lille1.fr>
 * Created: 2011-10-14
 * Time-stamp: <2012-03-19 16:05:32 (hauspie)>
 */
#include "../nxp/LPC17xx.h"
#include "dma.h"

#ifdef RFLPC_ENABLE_DMA

static LPC_GPDMACH_TypeDef *rflpc_dma_get_chan(rflpc_dma_channel_t channel)
{
   switch (channel)
   {
      case RFLPC_DMAC0: return LPC_GPDMACH0;
      case RFLPC_DMAC1: return LPC_GPDMACH1;
      case RFLPC_DMAC2: return LPC_GPDMACH2;
      case RFLPC_DMAC3: return LPC_GPDMACH3;
      case RFLPC_DMAC4: return LPC_GPDMACH4;
      case RFLPC_DMAC5: return LPC_GPDMACH5;
      case RFLPC_DMAC6: return LPC_GPDMACH6;
      case RFLPC_DMAC7: return LPC_GPDMACH7;
      default:
         break;
   }
   return (void*)0;
}

static volatile uint32_t *rflpc_dma_get_chan_control_register(rflpc_dma_channel_t channel)
{
   LPC_GPDMACH_TypeDef *dma = rflpc_dma_get_chan(channel);
   if (!dma)
      return (void*)0;
   return &(dma->DMACCControl);
}

static volatile uint32_t *rflpc_dma_get_chan_config_register(rflpc_dma_channel_t channel)
{
   LPC_GPDMACH_TypeDef *dma = rflpc_dma_get_chan(channel);
   if (!dma)
      return (void*)0;
   return &(dma->DMACCConfig);
}

static volatile uint32_t *rflpc_dma_get_chan_source(rflpc_dma_channel_t channel)
{
   LPC_GPDMACH_TypeDef *dma = rflpc_dma_get_chan(channel);
   if (!dma)
      return (void*)0;
   return &(dma->DMACCSrcAddr);
}

static volatile uint32_t *rflpc_dma_get_chan_destination(rflpc_dma_channel_t channel)
{
   LPC_GPDMACH_TypeDef *dma = rflpc_dma_get_chan(channel);
   if (!dma)
      return (void*)0;
   return &(dma->DMACCDestAddr);
}

static void rflpc_dma_set_channel_control(volatile uint32_t *control, uint16_t transfert_size, uint8_t burst_size)
{
   *control = (transfert_size & 0xFFF) | (1 << 26) | (1 << 27);
}

static void rflpc_dma_set_channel_config(volatile uint32_t *config, int enable)
{
   *config = (enable & 1);
}

static void rflpc_dma_set_channel_lli(rflpc_dma_channel_t channel, uint32_t lli)
{
   LPC_GPDMACH_TypeDef *dma = rflpc_dma_get_chan(channel);
   if (!dma)
      return;
   dma->DMACCLLI = lli;
}

void rflpc_dma_init(void)
{
   /* Power the DMA peripheral */
   LPC_SC->PCONP |= 1 << 29; /* p. 64 */

   /* Enable DMA controller and put in little-endian mode p. 599 */
   LPC_GPDMA->DMACConfig = 1;
}

int rflpc_dma_channel_ready(rflpc_dma_channel_t channel)
{
   return !(LPC_GPDMA->DMACEnbldChns & (1 << channel));
}

int rflpc_dma_start(rflpc_dma_channel_t channel, void *dest, const void *src, uint32_t size)
{
   if (!rflpc_dma_channel_ready(channel))
      return 0;

   volatile uint32_t *control = rflpc_dma_get_chan_control_register(channel);
   if (!control)
      return 0;
   volatile uint32_t *config = rflpc_dma_get_chan_config_register(channel);
   if (!config)
      return 0;
   volatile uint32_t *src_addr = rflpc_dma_get_chan_source(channel);
   volatile uint32_t *dst_addr = rflpc_dma_get_chan_destination(channel);

   if (!src_addr || !dst_addr)
      return 0;

   *src_addr = (uint32_t)src;
   *dst_addr = (uint32_t)dest;

   /* clear pending interrupt for this channel */
   LPC_GPDMA->DMACIntTCClear |=  (1 << channel);
   LPC_GPDMA->DMACIntErrClr |= (1 << channel);
   rflpc_dma_set_channel_lli(channel, 0);

   /* set transfert size */
   rflpc_dma_set_channel_control(control, size, 1);
   /* start transfert */
   rflpc_dma_set_channel_config(config, 1);
   return 1;
}

#endif /* ENABLE_DMA */
