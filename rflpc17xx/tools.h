/* This file is part of rflpc.                        
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
  Time-stamp: <2011-07-04 15:17:22 (mickey)>

  This file contains some macro tools used in rflpc

*/
#ifndef __RFLPC_TOOLS_H__
#define __RFLPC_TOOLS_H__

#define TOKEN_PASTE2(x, y) x ## y

/** This macro allows concatenation of tokens even in recursive macro */
#define TOKEN_PASTE(x, y) TOKEN_PASTE2(x, y)

#endif
