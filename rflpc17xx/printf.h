/* This file is part of rfBareMbed.                        
 *									 
 * rfBareMbed is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by	 
 * the Free Software Foundation, either version 3 of the License, or	 
 * (at your option) any later version.					 
 * 									 
 * rfBareMbed is distributed in the hope that it will be useful,		 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of	 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the	 
 * GNU General Public License for more details.				 
 * 									 
 * You should have received a copy of the GNU General Public License	 
 * along with rfBareMbed.  If not, see <http://www.gnu.org/licenses/>.	 
 */

#ifndef __RFLPC_PRINTF_H__
#define __RFLPC_PRINTF_H__



/** This function is similar to stdio's printf.
    However, it only handles:
    - %d
    - %x
    - %X
    - %p
    - %c
    - %s

    If another format char is used, int is assumed to skip
    argument, but nothing is printed unless LPC_VERBOSE_PRINTF
    is defined when compiling liblpc1768.a

    It uses the putchar function to output all characters. You have to define
    this function in your code so that the link works

*/
extern int rflpc_printf(const char *format,...);

#ifdef printf
#undef printf
#endif
#define printf rflpc_printf

#endif
