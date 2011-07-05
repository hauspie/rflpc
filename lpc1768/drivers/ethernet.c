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
  Author: Michael Hauspie <Michael.Hauspie@univ-lille1.fr>
  Created: Jun. 28 2011
  Time-stamp: <2011-07-05 01:07:28 (mickey)>
*/

#include "ethernet.h"
#include "gpio.h"
#include "leds.h"
#include "../LPC17xx.h"
#include "../clock.h"
#include "../printf.h"

#define PCENET_BIT (1 << 30)

#define ETH_PIN_TXD0      0
#define ETH_PIN_TXD1      1
#define ETH_PIN_TX_EN     4
#define ETH_PIN_CRS       8
#define ETH_PIN_RXD0      9
#define ETH_PIN_RXD1     10
#define ETH_PIN_RX_ER    14
#define ETH_PIN_REF_CLK  15
#define ETH_PIN_MDC      16
#define ETH_PIN_MDIO     17


/* MAC Configuration Register 1, bits definition */
#define MAC1_RECEIVE_ENABLE  (1 << 0)      
#define MAC1_PASS_ALL_FRAMES (1 << 1)  
#define MAC1_RX_FLOW_CONTROL (1 << 2)  
#define MAC1_TX_FLOW_CONTROL (1 << 3)  
#define MAC1_LOOPBACK        (1 << 4)  
#define MAC1_RESET_TX        (1 << 8)  
#define MAC1_RESET_MCS_TX    (1 << 9)
#define MAC1_RESET_RX        (1 << 10)
#define MAC1_RESET_MCS_RX    (1 << 11)
#define MAC1_SIM_RESET       (1 << 14)
#define MAC1_SOFT_RESET      (1 << 15)

/* MAC Configuration Register 2, bits definition */
#define MAC2_FULL_DUPLEX            (1 << 0)
#define MAC2_FRAME_LENGTH_CHK       (1 << 1)   
#define MAC2_HUGE_FRAME_ENABLE      (1 << 2)    
#define MAC2_DELAYED_CRC            (1 << 3)
#define MAC2_CRC_ENABLE             (1 << 4)         
#define MAC2_PAD_ENABLE             (1 << 5)         
#define MAC2_VLAN_PAD_ENABLE        (1 << 6)    
#define MAC2_AUTO_DETECT_PAD_ENABLE (1 << 7)   
#define MAC2_PURE_PREAMBLE_ENFORCE  (1 << 8)     
#define MAC2_LONG_PREAMBLE_ENFORCE  (1 << 9)     
#define MAC2_NO_BACKOFF             (1 << 12)
#define MAC2_BACK_PRESSURE          (1 << 13)
#define MAC2_EXCESS_DEFER           (1 << 14)     


/* MAC Control register bits */
#define CMD_RX_ENABLE        (1 << 0)
#define CMD_TX_ENABLE        (1 << 1)
#define CMD_REG_RESET        (1 << 3)
#define CMD_TX_RESET         (1 << 4)
#define CMD_RX_RESET         (1 << 5)
#define CMD_PASS_RUNT_FRAMES (1 << 6)
#define CMD_PASS_RX_FILTER   (1 << 7)
#define CMD_TX_FLOW_CONTROL  (1 << 8)
#define CMD_RMII             (1 << 9)
#define CMD_FULL_DUPLEX      (1 << 10)

/* MII control register bits */
#define MCFG_SCAN_INCREMENT    (1 << 0)
#define MCFG_SUPPRESS_PREAMBLE (1 << 1)
#define MCFG_RESET_MIIM        (1 << 15)

#define ETH_MAX_FRAME_LENGTH 1538 /*   1500 bytes for payload 
				   * +    8 bytes for preamble/sfd
				   * +   12 bytes for src and dst address 
				   * +    2 bytes for length 
				   * +    4 bytes for CRC 
				   * +   12 bytes for minimum interframe gap */
#define ETH_MAX_CLOCK 2500000 /* Maximum allowed clock frequency for MII, defined by IEEE 802.3, see p. 154 */

/** Address of the PHY device.
    Used to talk to PHY D83848J through RMII interface.
    Defined as already shifted to right position to use in MADR register (p.155)
*/

#define PHY_ADDR (0x01)

/* Address of the DP83848J PHY registers */
#define PHY_BMCR     (0x0)
#define PHY_BMSR     (0x1)
#define PHY_PHYIDR1  (0x2)
#define PHY_PHYIDR2  (0x3)
#define PHY_ANAR     (0x4)
#define PHY_ANLPAR   (0x5)
#define PHY_ANLPARNP (0x5) /* Not a bug, it IS the same addr (p. 36 of the DP83848J datasheet */
#define PHY_ANER     (0x6)
#define PHY_ANNPTR   (0x7)
#define PHY_PHYSTS   (0x10)
#define PHY_FCSCR    (0x14)
#define PHY_RECR     (0x15)
#define PHY_PCSR     (0x16)
#define PHY_RBR      (0x17)
#define PHY_LEDCR    (0x18)
#define PHY_PHYCR    (0x19)
#define PHY_10BTSCR  (0x1A)
#define PHY_CDCTRL1  (0x1B)
#define PHY_EDCR     (0x1D)

/* PHY register bits */
#define BMCR_RESET (1 << 15)

#define MBSR_LINK_STATUS (1 << 2)

#define ETH_DELAY do { int d = 100; for ( ; d != 0 ; --d); } while(0)

static const uint8_t clock_dividers[] = {4, 6, 8, 10, 14, 20, 28, 36, 40};

/* This function allows to write value to a PHY register through the RMII
 * interface 
 */
static void _write_to_phy_register_with_addr(uint8_t addr, uint8_t reg, uint16_t value)
{
    LPC_EMAC->MADR = (addr << 8)| reg;
    /* request a write cycle */
    LPC_EMAC->MWTD = value;

    /* Wait for write operation to end */
    while (LPC_EMAC->MIND & 1);
}

/* This function allows to read a value from a PHY register through the RMII
 * interface
 */

static uint16_t _read_from_phy_register_with_addr(uint8_t addr, uint8_t reg)
{
    /* Use procedure from p. 156 */
    /* Write register and peripheral address */
    LPC_EMAC->MADR = (addr << 8) | reg;
    /* Request Read cycle */
    LPC_EMAC->MCMD = 1;
    /* Wait for valid value in MRDD */
    while ((LPC_EMAC->MIND & 1) | (LPC_EMAC->MIND & (1 << 2)));
    LPC_EMAC->MCMD = 0;
    return LPC_EMAC->MRDD;
}

static void _eth_setup_pins()
{
    
    rflpc_pin_set(1, ETH_PIN_TXD0, 1, 0, 0);
    rflpc_pin_set(1, ETH_PIN_TXD1, 1, 0, 0);
    rflpc_pin_set(1, ETH_PIN_TX_EN, 1, 0, 0);
    rflpc_pin_set(1, ETH_PIN_CRS, 1, 0, 0);
    rflpc_pin_set(1, ETH_PIN_RXD0, 1, 0, 0);
    rflpc_pin_set(1, ETH_PIN_RXD1, 1, 0, 0);
    rflpc_pin_set(1, ETH_PIN_RX_ER, 1, 0, 0);
    rflpc_pin_set(1, ETH_PIN_REF_CLK, 1, 0, 0);
    rflpc_pin_set(1, ETH_PIN_MDC, 1, 0, 0);
    rflpc_pin_set(1, ETH_PIN_MDIO, 1, 0, 0);
}

#define _read_from_phy_register(reg) _read_from_phy_register_with_addr(PHY_ADDR, (reg))
#define _write_to_phy_register(reg, val) _write_to_phy_register_with_addr(PHY_ADDR, (reg), (val))

int rflpc_eth_init()
{
    int i;
    int dbg = 0;
    uint32_t divider;

    _eth_setup_pins(); 

    /* Power the ethernet device
       Set bit PCENET in PCONP register (p. 141 and 64) */
    LPC_SC->PCONP |= PCENET_BIT;


    /* Perform a full reset of the ethernet block.
       Procedure p. 207
    */
    /* Reset mac modules */
    LPC_EMAC->MAC1 = MAC1_SOFT_RESET;
    /* Reset datapaths */
    LPC_EMAC->Command = CMD_REG_RESET;
    /* wait for reset to perform */
    ETH_DELAY;
    /* Clear reset bit */
    LPC_EMAC->MAC1 = 0;

    /* Append CRC to the frame and PAD short frames (<64 bytes) */
    LPC_EMAC->MAC2 = MAC2_CRC_ENABLE | MAC2_PAD_ENABLE;
    /* Set max frame length */
    LPC_EMAC->MAXF = ETH_MAX_FRAME_LENGTH;

    /* Set MIIM clock multiplier to a value compatible with system clock
       (p. 154) 
       IEEE 802.3 defines the MII clock to be no faster than 2.5MHz
    */
    divider = rflpc_clock_get_system_clock() / ETH_MAX_CLOCK;
    for (i = 0 ; i < sizeof(clock_dividers); ++i)
    {
	if (clock_dividers[i] >= divider)
	    break;
    }
    ++i; /* after the incrementation, i hold the value to store in the MII
	  * configuration register (p. 154) */
    /* set the clock and reset MII to use new parameters */
    LPC_EMAC->MCFG = ((i & 0xF) << 2) | MCFG_RESET_MIIM;
    LPC_EMAC->MCFG &= ~(MCFG_RESET_MIIM);

    /* TODO set CLRT and IPGR registers */

    /* Enable RMII interface and allow reception of RUNT frames (less than 64 bytes)*/
    LPC_EMAC->Command = CMD_RMII | CMD_PASS_RUNT_FRAMES;
    
    /* Put the PHY in reset */
    _write_to_phy_register(PHY_BMCR, BMCR_RESET);

    /* Wait for PHY to finish reset */
    while (_read_from_phy_register(PHY_BMCR) & BMCR_RESET);

    return 1;
}

int rflpc_eth_link_state()
{
    return (_read_from_phy_register(PHY_BMSR) & MBSR_LINK_STATUS) == MBSR_LINK_STATUS;
}


#define PRINT_REG_VALUE(reg) printf(#reg"  \t%x\r\n", _read_from_phy_register(reg));

void rflpc_eth_print_infos()
{
    PRINT_REG_VALUE(PHY_BMCR);
    PRINT_REG_VALUE(PHY_BMSR);
    PRINT_REG_VALUE(PHY_PHYIDR1);
    PRINT_REG_VALUE(PHY_PHYIDR2);
    PRINT_REG_VALUE(PHY_ANAR);
    PRINT_REG_VALUE(PHY_ANLPAR);
    PRINT_REG_VALUE(PHY_ANLPARNP);
    PRINT_REG_VALUE(PHY_ANER);
    PRINT_REG_VALUE(PHY_ANNPTR);
    PRINT_REG_VALUE(PHY_PHYSTS);
    PRINT_REG_VALUE(PHY_FCSCR);
    PRINT_REG_VALUE(PHY_RECR);
    PRINT_REG_VALUE(PHY_PCSR);
    PRINT_REG_VALUE(PHY_RBR);
    PRINT_REG_VALUE(PHY_LEDCR);
    PRINT_REG_VALUE(PHY_PHYCR);
    PRINT_REG_VALUE(PHY_10BTSCR);
    PRINT_REG_VALUE(PHY_CDCTRL1);
    PRINT_REG_VALUE(PHY_EDCR);
}
