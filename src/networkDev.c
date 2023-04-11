#include "imports.h"
#include "utilities.h"
#include "networkDev.h"
#include "ethernet.h"
#include "tuntap_ifr.h"


//Used to intialize the network device
void networkDev_init(struct networkDev *device, char *address, char *hardwareAddr)
{
    CLEAR(*device);

    if(inet_pton(AF_INET, address, &device->address) != 1) //Convert IPv4 address to binary representation. Saved in 3rd argument
    {
        print_error("ERROR: Failed to parse address\n");
        exit(-1);
    }

    sscanf(hardwareAddr, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", 
       (unsigned char *)&device->hardwareAddr[0],
       (unsigned char *)&device->hardwareAddr[1],
       (unsigned char *)&device->hardwareAddr[2],
       (unsigned char *)&device->hardwareAddr[3],
       (unsigned char *)&device->hardwareAddr[4],
       (unsigned char *)&device->hardwareAddr[5]);


}

//Used to transmit from the device
void networkDev_transmit(struct networkDev *device, struct eth_hdr *ethhdr, uint16_t etherType, int length, unsigned char *destination)
{
    ethhdr -> ethertype = htons(etherType);        //convert from host byte order to network byte order

    memcpy(ethhdr->smac, device->hardwareAddr, 6); //device-> hardwareAddr to hdr->smac  
    memcpy(ethhdr->dmac, destination, 6);          //destination to hdr->dmac

    length += sizeof(struct eth_hdr);

    // printf("-------------------------------------End of ARP Packet-------------------------------------\n");

    tun_write((char *)ethhdr, length); //Write to tunnel
}
