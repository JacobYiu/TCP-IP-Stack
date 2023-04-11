#include "imports.h"
#include "ethernet.h"
#include "arp.h"

struct eth_hdr* eth_hdr_init(char *buf)
{
    struct eth_hdr *hdr = (struct eth_hdr *) buf;

    hdr->ethertype = htons(hdr->ethertype);

    printf("Source MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
        hdr->smac[0], hdr->smac[1], hdr->smac[2],
        hdr->smac[3], hdr->smac[4], hdr->smac[5]);

    printf("Destination MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
        hdr->dmac[0], hdr->dmac[1], hdr->dmac[2],
        hdr->dmac[3], hdr->dmac[4], hdr->dmac[5]);

    return hdr;
}

