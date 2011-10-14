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
 * Time-stamp: <2011-10-14 14:18:07 (hauspie)>
 */
#include "../nxp/LPC17xx.h"
#include "dma.h"

void rflpc_dma_init(void)
{
   /* Power the DMA peripheral */
   LPC_SC->PCONP |= 1 << 29; /* p. 64 */

   /* Enable DMA controller and put in little-endian mode p. 599 */
   LPC_GPDMA->DMACConfig = 1;
}

