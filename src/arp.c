#include "imports.h"
#include "ethernet.h"
#include "networkDev.h"
#include "arp.h"

//Holds table which contains arp_cache_entry structs
static struct arp_cache_entry arp_translation_table[ARP_TABLE_LEN]; 

//Inserts data into our translation table
//returns 0 on a successful insert and -1 on a failed insert
//Failed insert occurs when there is no more space left
static int arp_translation_table_insert(struct arp_hdr *arphdr, struct arp_ipv4 *arp_data)
{
    // printf("Inserting arp_cache\n");
    struct arp_cache_entry *entry;
    for(int i = 0; i < ARP_TABLE_LEN; i++)
    {
        entry = &arp_translation_table[i];

        if(entry->state == ARP_EMPTY)
        {
            entry->state = ARP_FILLED;

            entry->hardwareType = arphdr->hwtype;
            entry->sip = arp_data->sip;
            memcpy(entry->smac, arp_data->smac, sizeof(entry->smac)); 
            
            return 0;
        }
    }

    return -1;
}

//Update senders mac address in our translation table
//returns 0 on successful update and -1 on a failed update
//failed updated occurs when there is no matching hardware type and sender ip address
static int arp_translation_table_smac_update(struct arp_hdr *arphdr, struct arp_ipv4 *arp_data) 
{
    // printf("Updating arp table\n");
    struct arp_cache_entry *entry;
    for(int i = 0; i < ARP_TABLE_LEN; i++)
    {
        entry = &arp_translation_table[i];

        if(entry->state == ARP_EMPTY)
        {
            continue;
        }

        if((entry->hardwareType == arphdr->hwtype) && (entry->sip == arp_data->sip))
        {
            // printf("Found a matching arp_cache\n");
            memcpy(entry->smac, arp_data->smac, sizeof(entry->smac));
            return 1;
        }
    }

    // printf("Not found a matching arp_cache\n");
    return 0;
}

// static void print_arp_translation_table()
// {
//     // printf("Printing Table\n");
//     struct arp_cache_entry *entry;
//     for(int i = 0; i < ARP_TABLE_LEN; i++)
//     {
//         entry = &arp_translation_table[i];

//         if(entry -> state == ARP_EMPTY)
//         {
//             continue;
//         }

//         printf("Sender address is: %02x:%02x:%02x:%02x:%02x:%02x\n",
//         entry->smac[0], entry->smac[1], entry->smac[2],
//         entry->smac[3], entry->smac[4], entry->smac[5]);
//     }
// }

//Initialiaze our table to be 0
void arp_init()
{
    memset(arp_translation_table, 0, ARP_TABLE_LEN * sizeof(struct arp_cache_entry));
}

//Deal with incoming arp requests
void arp_incoming(struct networkDev *networkDev, struct eth_hdr *ethhdr)
{
    // printf("Received an ARP request from device %02x:%02x:%02x:%02x:%02x:%02x\n",
    //     ethhdr->smac[0], ethhdr->smac[1], ethhdr->smac[2],
    //     ethhdr->smac[3], ethhdr->smac[4], ethhdr->smac[5]);

    struct arp_hdr *arphdr;
    struct arp_ipv4 *arp_data;
    int merge_flag = 0;

    arphdr = (struct arp_hdr *) ethhdr -> payload;

    arphdr -> hwtype = ntohs(arphdr -> hwtype);     //Convert from network byte order to host byte order
    arphdr -> protype = ntohs(arphdr -> protype);
    arphdr -> opcode = ntohs(arphdr -> opcode);

    if(arphdr->hwtype != ARP_ETHERNET)
    {
        printf("Unsupported hardware type: %x\n", arphdr->hwtype);
    }

    if(arphdr->protype != ARP_IPv4)
    {
        printf("Unsupported protocol type: %x\n", arphdr->protype);
    }

    arp_data = (struct arp_ipv4 *) arphdr -> data;

    merge_flag = arp_translation_table_smac_update(arphdr, arp_data);

    if(arp_data -> dip != networkDev -> address)
    {
        printf("ARP is not for this device\n");

        printf("Supposed to send to: %02x:%02x:%02x:%02x:%02x:%02x\n",
        arp_data->dmac[0], arp_data->dmac[1], arp_data->dmac[2],
        arp_data->dmac[3], arp_data->dmac[4], arp_data->dmac[5]);
    }

    if(!merge_flag)//Meaning if no update happens
    {
        // printf("No updates need to be made\n");
        if(arp_translation_table_insert(arphdr, arp_data) != 0)
        {
            printf("No free space in the arp translation table\n");
        }
    } 

    // print_arp_translation_table();

    switch(arphdr -> opcode)
    {
        case ARP_REQUEST:
            arp_reply(networkDev, ethhdr, arphdr);
            break;
        default:
            printf("Opcode is currently not supported right now. Opcode is: \n");
    }

}

//Handle arp replies after receiving a "successful" arp request
void arp_reply(struct networkDev *networkDev, struct eth_hdr *ethhdr, struct arp_hdr *arphdr)
{
    // printf("Mapped ARP request of device %02x:%02x:%02x:%02x:%02x:%02x\n",
    //     ethhdr->smac[0], ethhdr->smac[1], ethhdr->smac[2],
    //     ethhdr->smac[3], ethhdr->smac[4], ethhdr->smac[5]);

    struct arp_ipv4 *arp_data;
    int length;

    arp_data = (struct arp_ipv4 *) arphdr -> data;

    memcpy(arp_data->dmac, arp_data->smac, sizeof(arp_data->dmac));
    arp_data->dip = arp_data->sip;

    memcpy(arp_data->smac, networkDev->hardwareAddr, sizeof(arp_data->smac));
    arp_data->sip = networkDev->address;

    arphdr->opcode = ARP_REPLY;

    arphdr->opcode = htons(arphdr->opcode);
    arphdr->protype = htons(arphdr->protype);
    arphdr->hwtype = htons(arphdr->hwtype);

    length = sizeof(struct arp_hdr) + sizeof(struct arp_ipv4);

    networkDev_transmit(networkDev, ethhdr, ETH_P_ARP, length, arp_data->dmac);
}