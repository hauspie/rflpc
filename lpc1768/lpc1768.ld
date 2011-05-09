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

/*
  Linker script for lpc1768 mbed platform (http://www.mbed.org).

  Author: Michael Hauspie <Michael.Hauspie@univ-lille1.fr>
  Created: Nov. 12 2010
  Time-stamp: <2011-01-23 10:06:36 (mickey)>
*/

/* output format of the file.
   default, big endian, little endian
   Arm cortex M3 is little endian so use that as default
*/
OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")
/* Architecture, obviously arm */
OUTPUT_ARCH(arm)
ENTRY(_start)

/* Memory map, give names and properties to the mbed different memories */
MEMORY
{
 /* Flash: starting at 0x00000000 512kB available. 
    rx stands for read only executable.
    Writing to flash must use algorithms included in boot rom so read 
    only from ld point of view
 */
  flash(rx) : ORIGIN = 0, LENGTH = 512K
/* on chip sram: starting at 0x10000000, 32kB available */
  ram(rwx): ORIGIN = 0x10000000, LENGTH = 32K
/* outside chip sram (accessible via AHB matrix: starting at 0x2007C000, 32kB available */
  out_ram(rwx): ORIGIN = 0x2007C00, LENGTH = 32K
}


SECTIONS
{	
 /* .text sections starts at address 0 */
 . = 0x0;
 /* output .text section is the concatenation of all .text section of all object files */
 /* for this to work fine, the start.o made from the assembly of start.s must
 appear first in the linker command line */
  .text : { 
  /* The interrupt vector will be remapped in RAM so that it can be easily modified
     and the one in flash will be copied at startup before performing the actual remap
     by modifying the VTOR register (p. 75, user manual)
  */
  _interrupt_start = .;
  *(.interrupt_vector) ;
  _interrupt_end = .;
  *(.text) ;
  /* Constant data is also put in flash */
  *(.rodata) ;
  } 
  /* this declares a global constant which contains the address of the end of section text.
     This will be used by the startup code to copy data from flash to ram */
  _text_end = .;

  /* bss has to be located in ram */
  .bss (NOLOAD): { 
  /* stores the start address of the bss so that the startup code can 0 it */
  _bss_start = .;
 /* This will enforce the relocated interrupt vector to be located at the start of the RAM */
   KEEP(*(.ram_int_vector));
   KEEP(*(.bss)) ;
  } > ram AT>flash
  /* stores the end address of the bss so that the startup code can 0 it */
   _bss_end = .;


 /* Data section is put in flash and will be copied in ram by startup script */

 /* the > ram makes all the symbol referencing section .data to be relocated in
    ram (i.e. addresses used in code are in ram) the AT>flash makes the section
    to be located in flash.  Thus, the startup script will just have to copy
    the data section from flash to ram */

 /* this declares a global constant _data_start which contains the address of
 the section in ram */
 .data : { 
  _data_start = .; 
  *(.data) ;
} > ram AT>flash
 _data_end = .; 
 /* declares a global constant _data_end which contains the address of the end of the section in ram */


 /* Stack must be located in ram*/
 .stack : { *(.stack) } > ram
}
