#ifndef NETWORKDEV_H
#define NETWORKDEV_H
#include "imports.h"

struct eth_hdr;

struct networkDev
{
    uint32_t address;               //IPv4
    unsigned char hardwareAddr[6];  //MAC
};

void networkDev_init(struct networkDev *device, char *address, char *hardwareAddr);
void networkDev_transmit(struct networkDev *device, struct eth_hdr *ethhdr, uint16_t etherType, int length, unsigned char *destination);

#endif