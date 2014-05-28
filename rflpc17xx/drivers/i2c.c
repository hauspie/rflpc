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

#ifdef RFLPC_CONFIG_ENABLE_I2C

#include <rflpc17xx/rflpc17xx.h>

#include "i2c.h"

/* Useful flag for writing CONSET/CONCLR registers.For more informations, 
 * please refer to the pages 441/442 of the user manual. */
#define RFLPC_I2C_FLAG_AA    0x04
#define RFLPC_I2C_FLAG_SI    0x08
#define RFLPC_I2C_FLAG_STO   0x10
#define RFLPC_I2C_FLAG_STA   0x20
#define RFLPC_I2C_FLAG_I2EN  0x40

/* Registers writing macros (I2DAT, I2CONSET, I2CONCLR) */
#define RFLPC_I2C_WRITE_DAT(i2c, byte)          \
   do { i2c->conf_addr->I2DAT = ((byte) & 0xFF); } while(0)
#define RFLPC_I2C_WRITE_CONSET(i2c, flag)               \
   do { i2c->conf_addr->I2CONSET = ((flag) & 0x7C); } while(0)
#define RFLPC_I2C_WRITE_CONCLR(i2c, flag)               \
   do { i2c->conf_addr->I2CONCLR = ((flag) & 0x6C); } while(0)

/* Registers reading macros (I2STAT, I2DAT) */ 
#define RFLPC_I2C_READ_STAT(i2c)          (i2c->conf_addr->I2STAT & 0xFF)
#define RFLPC_I2C_READ_DAT(i2c)           (i2c->conf_addr->I2DAT & 0xFF)

/* Macro for building I2C return instruction (status + transmitted nbytes) */
#define RFLPC_I2C_RETURN(status, nbytes)        \
   return (uint16_t)(((status) << 8) | nbytes)

/* Configuration data structure for I2C ports */
typedef struct {
   struct {
      /* TODO: Update the structure model */
      unsigned char sda_pin:5;
      unsigned char scl_pin:5;
      unsigned char pin_function:2;
   } gpio;      
   uint32_t volatile *pclksel;
   unsigned char pclk_bit:5;
   unsigned char pconp_bit:5;
   LPC_I2C_TypeDef *conf_addr;
} rflpc_i2c_config_t;

/* Configuration data structure for Slave Transmitter mode */
struct {
   uint8_t *data;
   uint8_t size;
} _slave_transmitter_config;


/** @name I2C Configuration */
/** @{ */
/** I2C0 GPIO Port. 
 * @warning This I2C port is not accessible on the MBED! (the pins are not connected to DIP pins)
 */
/** I2C0 SDA Pin */
#define RFLPC_I2C0_SDA_PIN	RFLPC_PIN_P0_27
/** I2C0 SCL Pin */
#define RFLPC_I2C0_SCL_PIN	RFLPC_PIN_P0_28
/** I2C0 Pin func */
#define RFLPC_I2C0_PIN_FUNC	1

/** I2C1 SDA Pin */
#define RFLPC_I2C1_SDA_PIN 	RFLPC_PIN_P0_0
/** I2C1 SCL Pin */
#define RFLPC_I2C1_SCL_PIN 	RFLPC_PIN_P0_1
/** I2C1 Pin func */
#define RFLPC_I2C1_PIN_FUNC 	3

/** I2C2 SDA Pin */
#define RFLPC_I2C2_SDA_PIN	RFLPC_PIN_P0_10
/** I2C2 SCL Pin */
#define RFLPC_I2C2_SCL_PIN	RFLPC_PIN_P0_11
/** I2C2 Pin func */
#define RFLPC_I2C2_PIN_FUNC	2
/** @} */


static const rflpc_i2c_config_t _config[3] = {
   { /* Configuration values for I2C0 */
      { RFLPC_I2C0_SDA_PIN, RFLPC_I2C0_SCL_PIN, RFLPC_I2C0_PIN_FUNC }, /* gpio */
      &LPC_SC->PCLKSEL0,   	                         /* PCLKSEL register */
      14, 			                                 /* pclk_bit */
      7,                                  			/* pconp_bit */ 
      LPC_I2C0,   		                                /* conf_addr */
   },    
   { /* Configuration values for I2C1 */
      { RFLPC_I2C1_SDA_PIN, RFLPC_I2C1_SCL_PIN, RFLPC_I2C1_PIN_FUNC }, /* gpio */
      &LPC_SC->PCLKSEL1,   	                         /* PCLKSEL register */
      6, 		                                             	 /* pclk_bit */
      19,			                                        /* pconp_bit */
      LPC_I2C1,   		                                /* conf_addr */
   },
   { /* Configuration values for I2C2 */
      { RFLPC_I2C2_SDA_PIN, RFLPC_I2C2_SCL_PIN, RFLPC_I2C2_PIN_FUNC }, /* gpio */
      &LPC_SC->PCLKSEL1,   	                         /* PCLKSEL register */
      20, 			                                 /* pclk_bit */
      26,			                                        /* pconp_bit */
      LPC_I2C2,   	                                     	/* conf_addr */
   },    
};

static rflpc_i2c_mode_t _mode;

int rflpc_i2c_init(rflpc_i2c_port_t port, rflpc_i2c_mode_t mode, uint8_t addr)
{
   const rflpc_i2c_config_t *i2c = &_config[port];

   if (rflpc_clock_get_system_clock() != 96000000)
      return -1;

   _mode = mode;

   /* Power the port */
   LPC_SC->PCONP |= (1UL << i2c->pconp_bit);

   /* Select the desired clock */
   /* To operate at 100kHz, we set the I2C peripheral clock to 12Mhz,
    * and then the sum I2CSCLL+I2CSCLH to 120 */

   /* Set peripheral clock to 12 Mhz (96/8) */
   *(i2c->pclksel) &= ~(RFLPC_CCLK_8 << i2c->pclk_bit);
   *(i2c->pclksel) |= (RFLPC_CCLK_8 << i2c->pclk_bit);
   /* 60 + 60 = 120 */
   i2c->conf_addr->I2SCLL = 60;
   i2c->conf_addr->I2SCLH = 60;
    
   /* Select appropriate pins. Documentation says that the pins should
      configured with neither pull-up nor pull-down and in opendrain */
   rflpc_pin_set(i2c->gpio.sda_pin, i2c->gpio.pin_function, RFLPC_PIN_MODE_RESISTOR_NONE, 1);
   rflpc_pin_set(i2c->gpio.scl_pin, i2c->gpio.pin_function, RFLPC_PIN_MODE_RESISTOR_NONE, 1);
    
   /*  Cleanup the full configuration register */
   i2c->conf_addr->I2CONCLR = 0xFFFFFFFF;

   /* Let's switch to wanted mode */
   if (_mode == RFLPC_I2C_MODE_MASTER) {
      i2c->conf_addr->I2CONSET = 0x40; /* p. 466, 450 */
   }
   else {
      i2c->conf_addr->I2CONSET = 0x44; /* p. 466, 454 */
      i2c->conf_addr->I2ADR0 = addr;
      i2c->conf_addr->I2MASK0 = 0;
   }

   return 0;
}

uint16_t rflpc_i2c_write(rflpc_i2c_port_t port, uint8_t addr,
			 uint8_t *data, uint8_t nbytes, uint8_t stop)
{
   const rflpc_i2c_config_t *i2c = &_config[port];
   uint8_t realnbytes = 0;                /* Effective number of bytes transmitted */
   uint8_t state;                             /* The state of the I2C automata */
  
   while ((state = RFLPC_I2C_READ_STAT (i2c))) {
      switch (state) {
         /*************************************************************************
          * Status 0xF8: The interrupt flag SI is not yet set. This occurs between
          *              other states and when the I2C block is not involved in a 
          *              transfer. For now, this only means that no transfer has 
          *              been started yet.
          */
         case 0xF8:
            RFLPC_I2C_WRITE_CONSET (i2c, RFLPC_I2C_FLAG_STA);        /* Set STA bit */
            break;

            /*************************************************************************
             *                   MASTER TRANSMITTER MODE STATES
             ************************************************************************/

            /*************************************************************************
             * Status 0x08: START condition has been transmitted. Slave address and W
             *              bit will now be transmitted.
             */
         case 0x08:
            RFLPC_I2C_WRITE_DAT (i2c, addr);                      /* Write to I2DAT */
            RFLPC_I2C_WRITE_CONCLR (i2c, RFLPC_I2C_FLAG_SI);       /* Clear SI flag */
            break;

            /*************************************************************************
             * Status 0x10: Repeated START condition has been transmitted. Slave 
             *       	      address and W bit will now be transmitted.
             */
         case 0x10:
            RFLPC_I2C_WRITE_DAT (i2c, addr);                      /* Write to I2DAT */
            RFLPC_I2C_WRITE_CONCLR (i2c, RFLPC_I2C_FLAG_SI);       /* Clear SI flag */
            break;

            /*************************************************************************
             * Status 0x18: Previous state was 0x08 or 0x10. Slave address and W bit
             *              has been transmitted, ACK has been received. The first
             *              data byte will be transmitted.
             */
         case 0x18:
            /* No byte to transmit ? Ok then. Send STOP and tell why. */
            if (nbytes == 0) {
               RFLPC_I2C_WRITE_CONSET (i2c, RFLPC_I2C_FLAG_STO);     /* Set STO flag */
               RFLPC_I2C_WRITE_CONCLR (i2c, RFLPC_I2C_FLAG_SI);       /* Set SI flag */
               RFLPC_I2C_RETURN (RFLPC_I2C_SIZE_EQUAL_TO_ZERO, 0);         /* RETURN */
            }

            RFLPC_I2C_WRITE_DAT (i2c, *data);                /* Write byte to I2DAT */
            nbytes -= 1;                                      /* Update bytes count */
            realnbytes += 1;                       /* Update transmitted bytes count */
            data += 1;                                       /* Update data pointer */
            RFLPC_I2C_WRITE_CONCLR (i2c,                   /* Clear STA and SI flag */
                                    (RFLPC_I2C_FLAG_STA | RFLPC_I2C_FLAG_SI));
            break;

            /*************************************************************************
             * Status 0x20: Slave address and W bit has been transmitted. NOT ACK has
             *              been received. A STOP condition will be transmitted.
             */
         case 0x20:
            RFLPC_I2C_WRITE_CONSET (i2c, RFLPC_I2C_FLAG_STO);       /* Set STO flag */
            RFLPC_I2C_WRITE_CONCLR (i2c, RFLPC_I2C_FLAG_SI);       /* Clear SI flag */
            RFLPC_I2C_RETURN (RFLPC_I2C_SLAVE_IS_BUSY, 0);                /* RETURN */

            /*************************************************************************
             * Status 0x28: Data has been transmitted. ACK has been received. If the
             *              transmitted data was the last data byte then transmit a
             *              STOP condition, otherwise transmit the next data byte.
             */
         case 0x28:
            if (nbytes == 0) {
               /* Last bytes just has been transmitted. Now, we need to know which 
                  condition to transmit next. */
               if (stop) {
                  RFLPC_I2C_WRITE_CONSET (i2c, RFLPC_I2C_FLAG_STO);   /* Set STO flag */
               }
               else {
                  RFLPC_I2C_WRITE_CONSET (i2c, RFLPC_I2C_FLAG_STA);   /* Set STA flag */
               }

               RFLPC_I2C_WRITE_CONCLR (i2c, RFLPC_I2C_FLAG_SI);     /* Clear SI flag */
               RFLPC_I2C_RETURN (RFLPC_I2C_SUCCESSFUL_TRANSMISSION,        /* RETURN */
                                 realnbytes); 
            }

            /* There is remaining bytes to be transmitted, then we need to send next
             * byte of data. */
            RFLPC_I2C_WRITE_DAT (i2c, *data);                /* Write byte to I2DAT */
            nbytes -= 1;                                      /* Update bytes count */
            realnbytes += 1;                      /* Update transmitted bytes count */
            data += 1;                                       /* Update data pointer */
            RFLPC_I2C_WRITE_CONCLR (i2c, RFLPC_I2C_FLAG_SI);       /* Clear SI flag */
            break;

            /*************************************************************************
             * Status 0x30: Data has been transmitted. NOT ACK has been received. A
             *    	      STOP condition will be transmitted.
             */
         case 0x30:
            RFLPC_I2C_WRITE_CONSET (i2c, RFLPC_I2C_FLAG_STO);       /* Set STO flag */
            RFLPC_I2C_WRITE_CONCLR (i2c, RFLPC_I2C_FLAG_SI);       /* Clear SI flag */
            RFLPC_I2C_RETURN (RFLPC_I2C_SLAVE_DID_NOT_ACKNOWLEDGED_DATA,  /* RETURN */
                              realnbytes);

            /*************************************************************************
             * Status 0x38: Arbitration has been lost during Slave address + W or
             *   	      data. The bus has been released and not addressed Slave 
             *              mode is entered. A new START condition will be transmitted
             *              when the bus is free again.
             */
         case 0x38:
            RFLPC_I2C_WRITE_CONSET (i2c, RFLPC_I2C_FLAG_STA);       /* Set STA flag */
            RFLPC_I2C_WRITE_CONCLR (i2c, RFLPC_I2C_FLAG_SI);       /* Clear SI flag */
            break;

            /*************************************************************************
             *                   SLAVE TRANSMITTER MODE STATES
             ************************************************************************/

            /*************************************************************************
             * State 0xA8: Own Slave Address + R has been received, ACK has been 
             *             returned. Data will be transmitted, ACK bit will be
             *             received.
             */
         case 0xA8:
            RFLPC_I2C_WRITE_DAT (i2c, *data);                /* Write byte to I2DAT */
            nbytes -= 1;                                      /* Update bytes count */
            realnbytes += 1;                      /* Update transmitted bytes count */

            /* If last byte has been transmitted, stop transmission at next iteration,
             * i.e. state 0xC8. */
            if (nbytes == 0) {
               RFLPC_I2C_WRITE_CONCLR (i2c, RFLPC_I2C_FLAG_AA);
            }
            else {
               RFLPC_I2C_WRITE_CONSET (i2c, RFLPC_I2C_FLAG_AA);
               data += 1;      
            }

            RFLPC_I2C_WRITE_CONCLR (i2c, RFLPC_I2C_FLAG_SI);       /* Clear SI flag */
            break;
      
            /*************************************************************************
             * State 0xB0: Arbitration lost in SLA+R/W as master; Own SLA+R has been
             *             received, ACK has been returned.
             *
             * IMPORTANT NOTE
             * (Description is from user manual, page 461, table 401)
             * Without tests with wanted arbitration lost, cannot go through this
             * state.
             */
         case 0xB0:
            RFLPC_I2C_RETURN (RFLPC_I2C_NOT_YET_IMPLEMENTED,              /* RETURN */
                              state);

            /*************************************************************************
             * State 0xB8: Data has been transmitted, ACK has been received. Data will
             *             be transmitted, ACK will be received.
             */
         case 0xB8:
            RFLPC_I2C_WRITE_DAT (i2c, *data);                /* Write byte to I2DAT */
            nbytes -= 1;                                      /* Update bytes count */
            realnbytes += 1;                      /* Update transmitted bytes count */
            RFLPC_I2C_WRITE_CONSET (i2c, RFLPC_I2C_FLAG_AA);         /* Set AA flag */
            RFLPC_I2C_WRITE_CONCLR (i2c, RFLPC_I2C_FLAG_SI);       /* Clear SI flag */
            break;

            /*************************************************************************
             * State 0xC0: Data has been transmitted, NOT ACK has been received. Not
             *             addressed Slave mode is entered.
             */
         case 0xC0:
            RFLPC_I2C_WRITE_CONSET (i2c, RFLPC_I2C_FLAG_AA);         /* Set AA flag */
            RFLPC_I2C_WRITE_CONCLR (i2c, RFLPC_I2C_FLAG_SI);       /* Clear SI flag */
            RFLPC_I2C_RETURN (RFLPC_I2C_SUCCESSFUL_TRANSMISSION,          /* RETURN */
                              realnbytes);

            /*************************************************************************
             * State 0xC8: The last data byte has been transmitted, ACK has been 
             *             received. Not addressed Slave mode is entered.
             */
         case 0xC8:
            RFLPC_I2C_WRITE_CONSET (i2c, RFLPC_I2C_FLAG_AA);         /* Set AA flag */
            RFLPC_I2C_WRITE_CONCLR (i2c, RFLPC_I2C_FLAG_SI);       /* Clear SI flag */
            RFLPC_I2C_RETURN (RFLPC_I2C_SUCCESSFUL_TRANSMISSION,          /* RETURN */
                              realnbytes);
      } /* switch (state) */

      /* Wait for SI flag to be set */
      do { } while ((i2c->conf_addr->I2CONSET & RFLPC_I2C_FLAG_SI) == 0);
   }

   RFLPC_I2C_RETURN (RFLPC_I2C_UNKNOWN_ERROR, realnbytes);
}

uint16_t rflpc_i2c_read(rflpc_i2c_port_t port, uint8_t addr, 
			uint8_t *data, uint8_t nbytes, uint8_t stop)
{
   const rflpc_i2c_config_t *i2c = &_config[port];
   uint8_t realnbytes = 0;               /* Effective number of bytes received */
   uint8_t state;                             /* The state of the I2C automata */
   uint16_t tmp;

   while ((state = RFLPC_I2C_READ_STAT (i2c))) {
      switch (state) {
         /*************************************************************************
          * Status 0xF8: The interrupt flag SI is not yet set. This occurs
          *              between other states and when the I2C block is not 
          *              involved in a transfer. For now, this only means that no
          *              transfer has been started yet.
          */
         case 0xF8:
            /* If Master mode, send a START condition now. Otherwise, just wait. */
            if (_mode == RFLPC_I2C_MODE_MASTER)
               RFLPC_I2C_WRITE_CONSET (i2c, RFLPC_I2C_FLAG_STA);      /* Set STA bit */
            break;

            /*************************************************************************
             *                     MASTER RECEIVER MODE STATES
             ************************************************************************/

            /*************************************************************************
             * Status 0x08: START condition has been transmitted. Slave address and
             *   	     R bit will now be transmitted.
             */
         case 0x08:
            RFLPC_I2C_WRITE_DAT (i2c, (addr | 1));                /* Write to I2DAT */
            RFLPC_I2C_WRITE_CONCLR (i2c, RFLPC_I2C_FLAG_SI);       /* Clear SI flag */
            break;

            /*************************************************************************
             * Status 0x10: Repeated START condition has been transmitted. Slave
             * 	     address and R bit will now be transmitted.
             */
         case 0x10:
            RFLPC_I2C_WRITE_DAT (i2c, (addr | 1));                /* Write to I2DAT */
            RFLPC_I2C_WRITE_CONCLR (i2c, RFLPC_I2C_FLAG_SI);       /* Clear SI flag */
            break;
      
            /*************************************************************************
             * Status 0x40: Previous state was 0x08 or 0x10. Slave address + R has
             *  	      been transmitted. ACK has been received. Data will be 
             *              received and ACK returned.
             */
         case 0x40:
            if (nbytes > 1)
               RFLPC_I2C_WRITE_CONSET (i2c, RFLPC_I2C_FLAG_AA);   /* Set the AA flag */

            RFLPC_I2C_WRITE_CONCLR (                      /* Clear STA and SI flags */
               i2c, RFLPC_I2C_FLAG_STA | RFLPC_I2C_FLAG_SI);
            break;

            /*************************************************************************
             * Status 0x48: Slave address + R has been transmitted. NOT ACK has been
             *  	      received. A STOP condition will be transmitted.
             *
             * TODO: Handle case with STOP followed by START. User manual P.458
             */
         case 0x48:
            RFLPC_I2C_WRITE_CONSET (i2c, RFLPC_I2C_FLAG_STO);   /* Set the STO flag */
            RFLPC_I2C_WRITE_CONCLR (i2c, RFLPC_I2C_FLAG_SI);   /* Clear the SI flag */
            RFLPC_I2C_RETURN (RFLPC_I2C_SLAVE_IS_BUSY, 0);                /* RETURN */

            /*************************************************************************
             * Status 0x50: Data has been received. ACK has been returned. Data will
             *	      be read from I2DAT. Additionnal data will be received. If 
             *              this will be last data byte then NOT ACK will be returned,
             *              otherwise, ACK will be returned.
             */
         case 0x50:
            *data = RFLPC_I2C_READ_DAT (i2c);                 /* Read received data */
            data += 1;                                       /* Update data pointer */
            nbytes -= 1;                                             /* Update size */
            realnbytes += 1;                         /* Update received bytes count */

            if (nbytes > 1) {
               /* There is more than one byte to be received. We need to return an
                * ACK after receiving the next byte. */
               RFLPC_I2C_WRITE_CONSET (i2c, RFLPC_I2C_FLAG_AA);       /* Set AA flag */
            }
            else { 
               /* The next byte we will receive is the last one we expect. We need to
                * return a NOT ACK after receiving it.*/
               RFLPC_I2C_WRITE_CONCLR (i2c, RFLPC_I2C_FLAG_AA);     /* Clear AA flag */
            }

            RFLPC_I2C_WRITE_CONCLR (i2c, RFLPC_I2C_FLAG_SI);   /* Clear the SI flag */
            break;

            /*************************************************************************
             * Status 0x58: Data has been received. NOT ACK has been returned. Data
             *   	     will be read from I2DAT. A STOP condition will be
             *              transmitted.
             */
         case 0x58:
            *data = RFLPC_I2C_READ_DAT (i2c);                 /* Read received data */
            realnbytes += 1;                         /* Update received bytes count */

            if (stop) {
               /* Send a STOP condition */
               RFLPC_I2C_WRITE_CONSET (i2c, RFLPC_I2C_FLAG_STO);     /* Set STO flag */
            }
            else {
               /* Send a repeated START condition */
               RFLPC_I2C_WRITE_CONSET (i2c, RFLPC_I2C_FLAG_STA);     /* Set STA flag */
            }

            RFLPC_I2C_WRITE_CONCLR (i2c, RFLPC_I2C_FLAG_SI);       /* Clear SI flag */
            RFLPC_I2C_RETURN (RFLPC_I2C_SUCCESSFUL_TRANSMISSION,          /* RETURN */
                              realnbytes);

            /*************************************************************************
             *                      SLAVE RECEIVER MODE STATUS
             ************************************************************************/

            /*************************************************************************
             * State 0x60: Own SLA+W has been received. ACK has been returned.
             */
         case 0x60:
            RFLPC_I2C_WRITE_CONSET (i2c, RFLPC_I2C_FLAG_AA);         /* Set AA flag */      
            RFLPC_I2C_WRITE_CONCLR (i2c, RFLPC_I2C_FLAG_SI);       /* Clear SI flag */
            break;

            /*************************************************************************
             * State 0x68: Arbitration lost in SLA+R/W as master; Own SLA+W has been
             *             received, ACK returned.
             *
             * IMPORTANT NOTE
             * (Description is from user manual, page 461, table 401)
             * Without tests with wanted arbitration lost, cannot go through this
             * state.
             */
         case 0x68:
            RFLPC_I2C_RETURN (RFLPC_I2C_NOT_YET_IMPLEMENTED,              /* RETURN */
                              state);

            /*************************************************************************
             * State 0x70: General Call address (0x00) has been received; ACK has been
             *             returned.
             *
             * IMPORTANT NOTE
             * This is part of the not (yet) implemented General Call functionnality.
             */
         case 0x70:
            RFLPC_I2C_RETURN (RFLPC_I2C_NOT_YET_IMPLEMENTED,              /* RETURN */
                              state);

            /*************************************************************************
             * State 0x78: Arbitration lost in SLA+R/W as master; Own SLA+W has been
             *             received, ACK returned.
             *
             * IMPORTANT NOTE
             * This is part of the not (yet) implemented General Call functionnality.
             *
             * IMPORTANT NOTE II
             * (Description is from user manual, page 461, table 401)
             * Without tests with wanted arbitration lost, cannot go through this
             * state.
             */
         case 0x78:
            RFLPC_I2C_RETURN (RFLPC_I2C_NOT_YET_IMPLEMENTED,              /* RETURN */
                              state);

            /*************************************************************************
             * State 0x80: Previously addressed with own Slave Address. Data has been
             *             received and ACK has been returned. Additionnal data will
             *             be read.
             */
         case 0x80:
            *data = RFLPC_I2C_READ_DAT (i2c);                 /* Read received data */
            nbytes -= 1;                                      /* Update bytes count */
            realnbytes += 1;                         /* Update received bytes count */

            /* If this was the last data byte to receive, send NOT ACK */
            if (nbytes == 0) {
               RFLPC_I2C_WRITE_CONCLR (i2c, RFLPC_I2C_FLAG_AA);     /* Clear AA flag */
            }
            else {
               RFLPC_I2C_WRITE_CONSET (i2c, RFLPC_I2C_FLAG_AA);       /* Set AA flag */
               data += 1;                                     /* Update data pointer */
            }

            RFLPC_I2C_WRITE_CONCLR (i2c, RFLPC_I2C_FLAG_SI);       /* Clear SI flag */
            break;

            /*************************************************************************
             * State 0x88: Previously addressed with own Slave Address. Data has been
             * received and NOT ACK has been returned. Received data will not be 
             * saved. Not addressed Slave mode is entered.
             */
         case 0x88:
            RFLPC_I2C_WRITE_CONSET (i2c, RFLPC_I2C_FLAG_AA);         /* Set AA flag */
            RFLPC_I2C_WRITE_CONCLR (i2c, RFLPC_I2C_FLAG_SI);       /* Clear SI flag */
            RFLPC_I2C_RETURN (RFLPC_I2C_SUCCESSFUL_TRANSMISSION,          /* RETURN */
                              realnbytes);

            /*************************************************************************
             * State 0x90: Previously addressed with General Call; Data byte has been
             *             received; ACK has been returned.
             *
             * IMPORTANT NOTE
             * This is part of the not (yet) implemented General Call functionnality.
             */
         case 0x90:
            RFLPC_I2C_RETURN (RFLPC_I2C_NOT_YET_IMPLEMENTED,              /* RETURN */
                              state);

            /*************************************************************************
             * State 0x98: Previously addressed with General Call; Data byte has been
             *             received; NOT ACK has been returned.
             *
             * IMPORTANT NOTE
             * This is part of the not (yet) implemented General Call functionnality.
             */
         case 0x98:
            RFLPC_I2C_RETURN (RFLPC_I2C_NOT_YET_IMPLEMENTED,              /* RETURN */
                              state);


            /*************************************************************************
             * State 0xA0: A STOP condition or repeated START has been received, while
             * still addressed as a Slave. Data will not be saved. Not addressed Slave
             * mode is entered.
             */
         case 0xA0:
            RFLPC_I2C_WRITE_CONSET (i2c, RFLPC_I2C_FLAG_AA);         /* Set AA flag */
            RFLPC_I2C_WRITE_CONCLR (i2c, RFLPC_I2C_FLAG_SI);       /* Clear SI flag */
            RFLPC_I2C_RETURN (RFLPC_I2C_STOP_CONDITION_RECEIVED,          /* RETURN */
                              realnbytes);

            /*************************************************************************
             * State 0xA8: Own Slave Address + R has been received, ACK has been 
             * returned. Data will be transmitted, ACK bit will be received.
             *
             * NOTE: This is the ONLY Slave Transmitter state which defined here. This
             *       is needed because as a Slave, the driver needs to "read" the I2C
             *       bus for a R bit before switching to Slave Transmitter Mode.
             *
             * NOTE: This is the moment when the driver switches from Slave Receiver
             *       mode to Slave Transmitter mode. Output data's buffer and his
             *       length MUST be set using the dedicated function to ensure a good
             *       transmission.
             */
         case 0xA8:
            tmp = rflpc_i2c_write(RFLPC_I2C_PORT2, 0,
                                  _slave_transmitter_config.data,
                                  _slave_transmitter_config.size,
                                  0);

            if (tmp == _slave_transmitter_config.size) {
               RFLPC_I2C_RETURN (RFLPC_I2C_SLAVE_TRANSMITTED_DATA,         /* RETURN */
                                 _slave_transmitter_config.size);
            }
            else {
               return tmp;
            }
      }

      /* Wait for SI flag to be set */
      do { } while ((i2c->conf_addr->I2CONSET & RFLPC_I2C_FLAG_SI) == 0);
   }

   return -1;
}

void rflpc_i2c_set_slave_transmitter_config(uint8_t *data, uint8_t size)
{
   if (size == 0)
      return;

   _slave_transmitter_config.data = data;
   _slave_transmitter_config.size = size;
}

#endif
