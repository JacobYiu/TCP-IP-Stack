#ifndef ARP_H
#define ARP_H

#include "imports.h"

#define ARP_IPv4        0x0800
#define ARP_ETHERNET    0x0001
#define ARP_REQUEST     0x0001
#define ARP_REPLY       0x0002


#define ARP_TABLE_LEN   32
#define ARP_EMPTY       0
#define ARP_WAITING     1
#define ARP_FILLED      2

struct arp_hdr
{
    u_int16_t hwtype;       //Link Layer which is the ethernet in our case (0x0001)
    u_int16_t protype;      //protocol type is IPv4 in our case. ARP requests are resolved via IPv4
    unsigned char hwsize;   //hardware size of our address. In this case it is 6 bytes for MAC address
    unsigned char prosize;  //Protocol size of our address. In this case it is 4 bytes for IP address
    u_int16_t opcode;       //(1)ARP request, (2)ARP reply, (3)RARP request, (4)RARP reply
                            //ARP is used to map an IP address to MAC address. RARP maps a MAC address to an IP address
                            //But we will only use ARP req and ARP reply
    unsigned char data[];   //data held. In our case it is the arp_ipv4 struct
}__attribute__((packed));

struct arp_ipv4
{
    unsigned char smac[6];  //Senders MAC address
    u_int32_t sip;          //Senders IP address
    unsigned char dmac[6];  //Receivers MAC address
    u_int32_t dip;          //Receivers IP address
}__attribute__((packed));

struct arp_cache_entry
{
    uint16_t hardwareType;
    uint32_t sip;
    unsigned char smac[6];
    unsigned int state;
};

void arp_init();
void arp_incoming(struct networkDev *networkDev, struct eth_hdr *ethhdr);
void arp_reply(struct networkDev *networkDevm, struct eth_hdr *ethhdr, struct arp_hdr *arphdr);

#endif