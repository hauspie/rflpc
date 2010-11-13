/*
  Linker script for lpc1768 mbed platform (http://www.mbed.org).

  Author: Michael Hauspie <Michael.Hauspie@univ-lille1.fr>
  Created: Nov. 12 2010
  Time-stamp: <2010-11-13 01:55:53 (mickey)>
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
  *(.interrupt_vector)
  *(.text) 
}
 /* Stack must be located in ram*/
 .stack : { *(.stack) } > ram
}
