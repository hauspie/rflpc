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
  Author: Michael Hauspie <michael.hauspie@univ-lille1.fr>
  Created: 2011-07-04
  Time-stamp: <2011-07-04 11:30:46 (hauspie)>

  all ethernet constants, register definition, bits etc..

*/
#ifndef __RFLPC_ETH_CONST_H__
#define __RFLPC_ETH_CONST_H__


#define RFLPC_ETH_PCENET_BIT (1 << 30)

#define RFLPC_ETH_PIN_TXD0      0
#define RFLPC_ETH_PIN_TXD1      1
#define RFLPC_ETH_PIN_TX_EN     4
#define RFLPC_ETH_PIN_CRS       8
#define RFLPC_ETH_PIN_RXD0      9
#define RFLPC_ETH_PIN_RXD1     10
#define RFLPC_ETH_PIN_RX_ER    14
#define RFLPC_ETH_PIN_REF_CLK  15
#define RFLPC_ETH_PIN_MDC      16
#define RFLPC_ETH_PIN_MDIO     17


/* MAC Configuration Register 1, bits definition */
#define RFLPC_ETH_MAC1_RECEIVE_ENABLE  (1 << 0)      
#define RFLPC_ETH_MAC1_PASS_ALL_FRAMES (1 << 1)  
#define RFLPC_ETH_MAC1_RX_FLOW_CONTROL (1 << 2)  
#define RFLPC_ETH_MAC1_TX_FLOW_CONTROL (1 << 3)  
#define RFLPC_ETH_MAC1_LOOPBACK        (1 << 4)  
#define RFLPC_ETH_MAC1_RESET_TX        (1 << 8)  
#define RFLPC_ETH_MAC1_RESET_MCS_TX    (1 << 9)
#define RFLPC_ETH_MAC1_RESET_RX        (1 << 10)
#define RFLPC_ETH_MAC1_RESET_MCS_RX    (1 << 11)
#define RFLPC_ETH_MAC1_SIM_RESET       (1 << 14)
#define RFLPC_ETH_MAC1_SOFT_RESET      (1 << 15)

/* MAC Configuration Register 2, bits definition */
#define RFLPC_ETH_MAC2_FULL_DUPLEX            (1 << 0)
#define RFLPC_ETH_MAC2_FRAME_LENGTH_CHK       (1 << 1)   
#define RFLPC_ETH_MAC2_HUGE_FRAME_ENABLE      (1 << 2)    
#define RFLPC_ETH_MAC2_DELAYED_CRC            (1 << 3)
#define RFLPC_ETH_MAC2_CRC_ENABLE             (1 << 4)         
#define RFLPC_ETH_MAC2_PAD_ENABLE             (1 << 5)         
#define RFLPC_ETH_MAC2_VLAN_PAD_ENABLE        (1 << 6)    
#define RFLPC_ETH_MAC2_AUTO_DETECT_PAD_ENABLE (1 << 7)   
#define RFLPC_ETH_MAC2_PURE_PREAMBLE_ENFORCE  (1 << 8)     
#define RFLPC_ETH_MAC2_LONG_PREAMBLE_ENFORCE  (1 << 9)     
#define RFLPC_ETH_MAC2_NO_BACKOFF             (1 << 12)
#define RFLPC_ETH_MAC2_BACK_PRESSURE          (1 << 13)
#define RFLPC_ETH_MAC2_EXCESS_DEFER           (1 << 14)     

/* RMII Support */
#define RFLPC_ETH_SUPP_10MBPS          (0)
#define RFLPC_ETH_SUPP_100MBPS         (1 << 8)


/* MAC Control register bits */
#define RFLPC_ETH_CMD_RX_ENABLE        (1 << 0)
#define RFLPC_ETH_CMD_TX_ENABLE        (1 << 1)
#define RFLPC_ETH_CMD_REG_RESET        (1 << 3)
#define RFLPC_ETH_CMD_TX_RESET         (1 << 4)
#define RFLPC_ETH_CMD_RX_RESET         (1 << 5)
#define RFLPC_ETH_CMD_PASS_RUNT_FRAMES (1 << 6)
#define RFLPC_ETH_CMD_PASS_RX_FILTER   (1 << 7)
#define RFLPC_ETH_CMD_TX_FLOW_CONTROL  (1 << 8)
#define RFLPC_ETH_CMD_RMII             (1 << 9)
#define RFLPC_ETH_CMD_FULL_DUPLEX      (1 << 10)

/* MIND Control register */
#define RFLPC_ETH_MIND_BUSY            (1)
#define RFLPC_ETH_MIND_SCANNING        (1 << 1)
#define RFLPC_ETH_MIND_NOT_VALID       (1 << 2)
#define RFLPC_ETH_MIND_MII_LINK_FAIL   (1 << 3)

/* MII control register bits */
#define RFLPC_ETH_MCFG_SCAN_INCREMENT    (1 << 0)
#define RFLPC_ETH_MCFG_SUPPRESS_PREAMBLE (1 << 1)
#define RFLPC_ETH_MCFG_RESET_MIIM        (1 << 15)

#define RFLPC_ETH_MAX_FRAME_LENGTH 1538 /*   1500 bytes for payload 
				   * +    8 bytes for preamble/sfd
				   * +   12 bytes for src and dst address 
				   * +    2 bytes for length 
				   * +    4 bytes for CRC 
				   * +   12 bytes for minimum interframe gap */
#define RFLPC_ETH_MAX_CLOCK 2500000 /* Maximum allowed clock frequency for MII, defined by IEEE 802.3, see p. 154 */

/** Address of the PHY device.
    Used to talk to PHY D83848J through RMII interface.
    Defined as already shifted to right position to use in MADR register (p.155)
*/

#define RFLPC_ETH_PHY_ADDR (0x01)

/* Address of the DP83848J PHY registers */
#define RFLPC_ETH_PHY_BMCR     (0x0)
#define RFLPC_ETH_PHY_BMSR     (0x1)
#define RFLPC_ETH_PHY_PHYIDR1  (0x2)
#define RFLPC_ETH_PHY_PHYIDR2  (0x3)
#define RFLPC_ETH_PHY_ANAR     (0x4)
#define RFLPC_ETH_PHY_ANLPAR   (0x5)
#define RFLPC_ETH_PHY_ANLPARNP (0x5) /* Not a bug, it IS the same addr (p. 36 of the DP83848J datasheet */
#define RFLPC_ETH_PHY_ANER     (0x6)
#define RFLPC_ETH_PHY_ANNPTR   (0x7)
#define RFLPC_ETH_PHY_PHYSTS   (0x10)
#define RFLPC_ETH_PHY_FCSCR    (0x14)
#define RFLPC_ETH_PHY_RECR     (0x15)
#define RFLPC_ETH_PHY_PCSR     (0x16)
#define RFLPC_ETH_PHY_RBR      (0x17)
#define RFLPC_ETH_PHY_LEDCR    (0x18)
#define RFLPC_ETH_PHY_PHYCR    (0x19)
#define RFLPC_ETH_PHY_10BTSCR  (0x1A)
#define RFLPC_ETH_PHY_CDCTRL1  (0x1B)
#define RFLPC_ETH_PHY_EDCR     (0x1D)

/* PHY register bits */
/* Basic Mode Control Register (BMSR) */
#define RFLPC_ETH_BMCR_RESET            (1 << 15)
#define RFLPC_ETH_BMCR_LOOPBACK         (1 << 14)
#define RFLPC_ETH_BMCR_SPEED_SELECT     (1 << 13)
#define RFLPC_ETH_BMCR_ENABLE_AUTO_NEG  (1 << 12)
#define RFLPC_ETH_BMCR_POWER_DOWN       (1 << 11)
#define RFLPC_ETH_BMCR_ISOLATE          (1 << 10)
#define RFLPC_ETH_BMCR_RESTART_AUTO_NEG (1 << 9)
#define RFLPC_ETH_BMCR_DUPLEX_MODE      (1 << 8)
#define RFLPC_ETH_BMCR_COLLISION_TEST   (1 << 7)

/* Basic Mode Status Register */
#define RFLPC_ETH_BMSR_100BASET4                (1 << 15)
#define RFLPC_ETH_BMSR_100BASETX_FULL           (1 << 14)
#define RFLPC_ETH_BMSR_100BASETX_HALF           (1 << 13)
#define RFLPC_ETH_BMSR_10BASET_FULL             (1 << 12)
#define RFLPC_ETH_BMSR_10BASET_HALF             (1 << 11)
#define RFLPC_ETH_BMSR_MF_PREAMBLE_SUPPRESSION  (1 << 6)
#define RFLPC_ETH_BMSR_AUTO_NEG_COMPLETE        (1 << 5)
#define RFLPC_ETH_BMSR_REMOTE_FAULT             (1 << 4)
#define RFLPC_ETH_BMSR_CAN_AUTO_NEG             (1 << 3)
#define RFLPC_ETH_BMSR_LINK_STATUS              (1 << 2)
#define RFLPC_ETH_BMSR_JABBER_DETECT            (1 << 1)
#define RFLPC_ETH_BMSR_EXT_REGISTER_CAPS        (1 << 0)


/* Auto negotiation advertisement register */
#define RFLPC_ETH_ANAR_ASM_DIR (1 << 11)
#define RFLPC_ETH_ANAR_PAUSE   (1 << 10)
#define RFLPC_ETH_ANAR_T4      (1 << 9)
#define RFLPC_ETH_ANAR_TX_FD   (1 << 8)
#define RFLPC_ETH_ANAR_TX      (1 << 7)
#define RFLPC_ETH_ANAR_10_FD   (1 << 6)
#define RFLPC_ETH_ANAR_10      (1 << 5)

/* PHY status register */
#define RFLPC_ETH_PHYSTS_MDI_X                     (1 << 14)
#define RFLPC_ETH_PHYSTS_RX_ERROR_LATCH            (1 << 13)
#define RFLPC_ETH_PHYSTS_POLARITY_STATUS           (1 << 12)
#define RFLPC_ETH_PHYSTS_FALSE_CARRIER_SENSE_LATCH (1 << 11)
#define RFLPC_ETH_PHYSTS_SIGNAL_DETECT             (1 << 10)
#define RFLPC_ETH_PHYSTS_DESCRAMBLER_LOCK          (1 << 9)
#define RFLPC_ETH_PHYSTS_PAGE_RECEIVED             (1 << 8)
#define RFLPC_ETH_PHYSTS_REMOTE_FAULT              (1 << 6)
#define RFLPC_ETH_PHYSTS_JABBER_DETECT             (1 << 5)
#define RFLPC_ETH_PHYSTS_AUTO_NEG_COMPLETE         (1 << 4)
#define RFLPC_ETH_PHYSTS_LOOPBACK_STATUS           (1 << 3)
#define RFLPC_ETH_PHYSTS_DUPLEX_STATUS             (1 << 2)
#define RFLPC_ETH_PHYSTS_SPEED_STATUS              (1 << 1)
#define RFLPC_ETH_PHYSTS_LINK_STATUS               (1 << 0)


/* Interrupt enable bits */
#define RFLPC_ETH_IRQ_EN_RX_OVERRUN  (1 << 0)
#define RFLPC_ETH_IRQ_EN_RX_ERROR    (1 << 1)
#define RFLPC_ETH_IRQ_EN_RX_FINISHED (1 << 2)
#define RFLPC_ETH_IRQ_EN_RX_DONE     (1 << 3)
#define RFLPC_ETH_IRQ_EN_TX_UNDERRUN (1 << 4)
#define RFLPC_ETH_IRQ_EN_TX_ERROR    (1 << 5)
#define RFLPC_ETH_IRQ_EN_TX_FINISHED (1 << 6)
#define RFLPC_ETH_IRQ_EN_TX_DONE     (1 << 7)
#define RFLPC_ETH_IRQ_EN_SOFT        (1 << 12)
#define RFLPC_ETH_IRQ_EN_WAKE_UP     (1 << 13)

#endif
