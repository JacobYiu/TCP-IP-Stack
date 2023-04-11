#ifndef ETHERNET_H
#define ETHERNET_H
#include <sys/types.h>
#include "imports.h"
#include "networkDev.h"

struct eth_hdr
{
    unsigned char dmac[6];  //destination MAC
    unsigned char smac[6];  //source MAC
    u_int16_t ethertype;    //Either the length of a payload or the type of payload. If ethertype >= 1536 then contains type of payload(ARP, IPv4) else contains length of payload 
    unsigned char payload[];//Pointer to the start of the payload. Minimum bytes is 48 bytes. If less than 48, pad until it reaches 48 bytes
}__attribute__((packed));   //No padding should be added in between fields 


//Initialize the ethernet header
struct eth_hdr* eth_hdr_init(char *buf);

//Does something based upon the value of the etherType field of the ethernet header
// void handle_frame(struct networkDev *networkDev, struct eth_hdr *ethhdr);
#endif