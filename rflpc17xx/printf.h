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

#ifndef __RFLPC_PRINTF_H__
#define __RFLPC_PRINTF_H__
/*
  Author: Michael Hauspie <michael.hauspie@univ-lille1.fr>
  Created:
  Time-stamp: <2011-07-13 14:20:45 (hauspie)>
*/
/** @file
 * An implementation of formated output
 */

/** @addtogroup libc
 * @{ */

/**
 * @brief Ouputs a single character.
 * Used by ::rflpc_printf() to output its formating
 *
 * @note This function is NOT provided by the library.
 * It is meant to be implemented by each specific application
 * so that it can choose how and where to ouput messages
 *
 * @param [in] c character to print
 * @return the printed character
 **/

extern int putchar(int c);

/** Formated output function.
 * This function is similar to stdio's printf.
    However, it only handles:
    - %%d
    - %%x/X (%0?X can also be used, where ? is a digit [1-9])
    - %%p
    - %%c
    - %%s

    @note If another format char is used, int is assumed to skip
    argument, but nothing is printed unless LPC_VERBOSE_PRINTF
    is defined when compiling lib. If ATOMIC_PRINTF is defined,
    interrupts are disabled when entering printf and enabled before exiting

    @warning It uses the ::putchar function to output all characters. You have to define
    this function in your code so that the link works

   @param [in] format The format string. Prefer to use constant string
   instead of user supplied string to be sure that some escape characters are not put for wrong purpose.
*/
extern int rflpc_printf(const char *format,...);

/** Maccro to be able to use printf.
 * @note The main reason why the lib defines ::rflpc_printf instead of printf is to avoid
 * many weird problems when gcc tries to use its builtin function where it should not.
 */
#define printf rflpc_printf

/** @} */

#endif
