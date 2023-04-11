#include "imports.h"
#include "utilities.h"
#include "ethernet.h"
#include "networkDev.h"
#include "arp.h"
#include "tuntap_ifr.h"

#define BUFLENGTH 100

void handle_frame(struct networkDev *networkDev, struct eth_hdr *ethhdr)
{
    switch(ethhdr -> ethertype)
    {
        case ETH_P_ARP:
            printf("-------------------------------------ARP packet detected-------------------------------------\n");
            arp_incoming(networkDev, ethhdr);
            break;
        case ETH_P_IP:
            printf("IPv4 packet found!\n");
            break;
        default:
            printf("-------------------------------------Unsupported etherType %x-------------------------------------\n", ethhdr->ethertype);
            break;
    }
}

int main()
{
    char buf[BUFLENGTH];
    char *dev = calloc(10,1);
    
    struct networkDev networkDev;

    CLEAR(buf);
    tun_init(dev);
    
    networkDev_init(&networkDev, "10.0.0.4", "00:0c:29:6d:50:26");

    arp_init();

    while(1)
    {
        if(tun_read(buf, BUFLENGTH) < 0)
        {
            print_error("ERROR: Something wrong reading from tunnel");
        }

        // print_hexdump(buf, BUFLENGTH);

        struct eth_hdr *ethhdr = eth_hdr_init(buf);

        handle_frame(&networkDev, ethhdr);
    }
}