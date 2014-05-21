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
  Author: Christophe Bacara <christophe.bacara@etudiant.univ-lille1.fr>
*/

#include <rflpc17xx/rflpc17xx.h>
#include <rflpc17xx/tinylibc/printf.h>

#include <rflpc17xx/drivers/i2c.h>

#include "c12832.h"
#include "lm75b.h"

#define __DELAY(d)      \
  { unsigned delay_i; for (delay_i = 0; delay_i < d; ++delay_i)	asm(""); }

void sleep(uint32_t ms)
{
  uint32_t delay, counter;

  for (delay = (16000 * ms), counter = 0; counter < delay; ++counter)
    asm("");  
}

#define SLAVE

int main()
{
  uint8_t sla = 0x40;
  uint8_t something = 13;

  rflpc_led_init();
  lcd_init();
  rflpc_printf_set_putchar(&lcd_putchar);

#ifdef MASTER
  rflpc_i2c_init(RFLPC_I2C_PORT2, RFLPC_I2C_MODE_MASTER, 0);
  
  /* while (rflpc_i2c_write(RFLPC_I2C_PORT2, sla, &something, 1, 1)) { */
  /*   rflpc_led_set(RFLPC_LED_1); */
  /*   sleep(500); */
  /*   rflpc_led_clr(RFLPC_LED_1); */
  /*   sleep(500); */
  /* } */

  rflpc_i2c_write(RFLPC_I2C_PORT2, sla, &something, 1, 1);
#endif

#ifdef SLAVE
  rflpc_i2c_init(RFLPC_I2C_PORT2, RFLPC_I2C_MODE_SLAVE, sla);

  /* while (rflpc_i2c_read(RFLPC_I2C_PORT2, 0, &something, 1, 0)) { */
  /*   rflpc_led_set(RFLPC_LED_1); */
  /*   sleep(500); */
  /*   rflpc_led_clr(RFLPC_LED_1); */
  /*   sleep(500); */
  /* } */
  rflpc_i2c_read(RFLPC_I2C_PORT2, 0, &something, 1, 0);
#endif

  lcd_refresh();
  while (1);
  return 0;
}

/* #define ADDR 0x98 */

/* int main() */
/* { */
/*   uint8_t buffer[10] = { 0 }; */
/*   uint8_t modreg_addr = 0x07; */
/*   uint8_t outreg_addr = 0x00; */
/*   uint8_t tiltreg_addr = 0x03; */
/*   uint8_t samreg_addr = 0x04; */

/*   rflpc_led_init(); */

/*   lcd_init(); */
/*   rflpc_printf_set_putchar(&lcd_putchar); */

/*   rflpc_i2c_init(RFLPC_I2C_PORT2, RFLPC_I2C_MODE_MASTER, 0); */

/*   buffer[0] = 0x07; */
/*   buffer[1] = 0x19; /\* Active Mode *\/ */
/*   buffer[2] = 0x00; */
/*   buffer[3] = 0x00; */

/*   /\* while (rflpc_i2c_write(RFLPC_I2C_PORT2, ADDR, buffer, 2, 1) != 0) { *\/ */
/*   /\*   rflpc_led_set(RFLPC_LED_1); *\/ */
/*   /\*   sleep(500); *\/ */
/*   /\*   rflpc_led_clr(RFLPC_LED_1); *\/ */
/*   /\*   sleep(500); *\/ */
/*   /\* } *\/ */

/*   if (rflpc_i2c_write(RFLPC_I2C_PORT2, ADDR, buffer, 2, 1) != 0) { */
/*       /\* || rflpc_i2c_write(RFLPC_I2C_PORT2, ADDR, &buffer[2], 2, 1)) { *\/ */
/*     rflpc_led_set(RFLPC_LED_1); */
/*     printf("Error at init"); */
/*     while (1); */
/*   } */
/*   else { */
/*     buffer[0] = 0; */
/*     buffer[1] = 0; */
/*     buffer[2] = 0; */
/*     buffer[3] = 0; */
/*   } */
    
/*   while (1) { */
/*     if (rflpc_i2c_write(RFLPC_I2C_PORT2, ADDR, &outreg_addr, 1, 0) == 0 */
/*   	&& rflpc_i2c_read(RFLPC_I2C_PORT2, ADDR, buffer, 3, 1) == 0) { */
/*       buffer[0] = buffer[0] & 0x3F; */
/*       buffer[1] = buffer[1] & 0x3F; */
/*       buffer[2] = buffer[2] & 0x3F; */

/*       if (buffer[0] & 0x40 || buffer[1] & 0x40 || buffer[2] & 0x40) */
/* 	rflpc_led_set(RFLPC_LED_2); */

/*       if (buffer[0] & 0x20) */
/* 	printf("-%i ", buffer[0] & 0x1F); */
/*       else */
/* 	printf("%i ", buffer[0] & 0x1F); */

/*       if (buffer[1] & 0x20) */
/* 	printf("-%i ", buffer[1] & 0x1F); */
/*       else */
/* 	printf("%i ", buffer[1] & 0x1F); */

/*       if (buffer[2] & 0x20) */
/* 	printf("-%i  ", buffer[2] & 0x1F); */
/*       else */
/* 	printf("%i  ", buffer[2] & 0x1F); */

/*       if (rflpc_i2c_write(RFLPC_I2C_PORT2, ADDR, &tiltreg_addr, 1, 0) == 0 */
/* 	&& rflpc_i2c_write(RFLPC_I2C_PORT2, ADDR, &buffer[3], 2, 1) == 0) { */
/*         printf("%i %i    ", buffer[3], buffer[4]); */

/* 	switch (buffer[3] & 0x03) { */
/* 	case 0x00: */
/* 	  rflpc_led_set(RFLPC_LED_1 | RFLPC_LED_4); */
/* 	  break; */
/* 	case 0x01: */
/* 	  rflpc_led_clr(RFLPC_LED_4); */
/* 	  rflpc_led_set(RFLPC_LED_1); */
/* 	  break; */
/* 	case 0x10: */
/* 	  rflpc_led_clr(RFLPC_LED_1); */
/* 	  rflpc_led_set(RFLPC_LED_4); */
/* 	  break; */
/* 	} */
/*       } */
/*       else { */
/*         rflpc_led_set(RFLPC_LED_3); */
/*         break; */
/*       } */

/*       printf("\r"); */
/*       lcd_refresh(); */
/*     } */
/*     else { */
/*       rflpc_led_set(RFLPC_LED_3); */
/*       break; */
/*     } */

/*     __DELAY (320000); */
/*   } */

/*   while (1); */

/*   return 0; */

/* } */

/* int main() */
/* { */
/*   int16_t temp = 0; */

/*   rflpc_led_init(); */
  
/*   lcd_init(); */
/*   rflpc_printf_set_putchar(&lcd_putchar); */

/*   libboard_lm75b_init(); */

/*   while (1) { */
/*     printf("Temp= %i C  \r", libboard_lm75b_get_temp()); */
/*     lcd_refresh(); */
/*     sleep(1000); */
/*   } */

/*   return 0; */
/* } */

