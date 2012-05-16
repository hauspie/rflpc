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
 * Time-stamp: <2012-03-19 16:05:08 (hauspie)>
 */
/** @file
 * General Purpose DMA driver
 */
#ifndef __RFLPC_DMA_H__
#define __RFLPC_DMA_H__



#ifdef RFLPC_CONFIG_ENABLE_DMA

/** @addtogroup dma General Purpose DMA
 * @ingroup drivers
 * @{ */

/** Enum to identify DMA channels */
typedef enum
{
   RFLPC_DMAC0,
   RFLPC_DMAC1,
   RFLPC_DMAC2,
   RFLPC_DMAC3,
   RFLPC_DMAC4,
   RFLPC_DMAC5,
   RFLPC_DMAC6,
   RFLPC_DMAC7,
} rflpc_dma_channel_t;

/**
 * Inits the DMA Peripheral.
 * @note The DMA is enabled in little endian mode by default
 **/

extern void rflpc_dma_init(void);

/** Tells if a DMA channel is ready to be used
 * @return true if the DMA channel is ready (not already doing something)
 */
extern int rflpc_dma_channel_ready(rflpc_dma_channel_t channel);

/**
 * Starts a DMA copy.
 * The DMA channel must be ready before starting a DMA copy
 * @param channel The channel to use.
 * @param dest destination pointer
 * @param src source data
 * @param size number of bytes to transfert (limited to 0xFFF bytes)
 * @return true if the copy has been started, false otherwise
 *
 * @warning In the current implementation, size is limited by 0xFFF because only one DMA request is generated.
 * @todo Handle size > 0xFFF
 **/
extern int rflpc_dma_start(rflpc_dma_channel_t channel, void *dest, const void *src, uint32_t size);

/** @} */
#endif /* ENABLE_DMA */

#endif
