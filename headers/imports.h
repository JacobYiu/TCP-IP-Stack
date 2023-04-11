#ifndef IMPORTS_H
#define IMPORTS_H

#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>

#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>       //Used to initialize the interface device
//Down below is what the header <net/if.h> is used for
// struct ifreq {
//     char ifr_name[IFNAMSIZ]; /* Interface name */
//     union {
//         struct sockaddr ifr_addr;
//         struct sockaddr ifr_dstaddr;
//         struct sockaddr ifr_broadaddr;
//         struct sockaddr ifr_netmask;
//         struct sockaddr ifr_hwaddr;
//         short           ifr_flags;
//         int             ifr_ifindex;
//         int             ifr_metric;
//         int             ifr_mtu;
//         struct ifmap    ifr_map;
//         char            ifr_slave[IFNAMSIZ];
//         char            ifr_newname[IFNAMSIZ];
//         char           *ifr_data;
//     } ifr_ifru;
// };

#include <linux/if_tun.h> //Used to initialize the flags and ioctl command
#include <fcntl.h>

#endif