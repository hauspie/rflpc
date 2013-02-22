#ifndef __DHCP_PACKET_H__
#define __DHCP_PACKET_H__

#define DHCPDISCOVER 1     
#define ODCPOFFER    2     
#define DHCPREQUEST  3    
#define DHCPDECLINE  4     
#define DHCPACK      5     
#define DHCPNAK      6     
#define DHCPRELEASE  7     
#define DHCPINFORM   8     


struct dhcp_message {
  /* Each line corresponds to the RFC 2131 spec page 8 */
  unsigned char op, htype, hlen, hops;
  unsigned char xid[4];
  unsigned char secs[2], flags[2];
  unsigned char ciaddr[4];
  unsigned char yiaddr[4];
  unsigned char siaddr[4];
  unsigned char giaddr[4];
  unsigned char chaddr[16];
  unsigned char sname[64];
  unsigned char file[128];
  unsigned char options[64];
};

#endif /* __DHCP_PACKET_H__ */
