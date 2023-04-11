#include "imports.h"
#include "utilities.h"
#include "tuntap_ifr.h"

//Private-------------------------
static int tun_fd;

static int set_ifr(char *device)
{
    return command_run("ip link set dev %s up", device);
}

static int set_ifr_ip(char *device)
{
    return command_run("ip addr add 192.168.75.60 dev %s", device);
}

// static int set_ifr_mac(char *device)
// {
//     return command_run("ip link set dev %s address 00:11:22:33:44:55", device);
// }

static int set_ifr_route(char *device, char *cidrSF) //CIDRSF specifies the number of significiant bits in the network prefix of an IP address
{
    return command_run("ip route add dev %s %s", device, cidrSF);
}

static int tun_alloc(char * device)
{
    struct ifreq ifr; //interface request. Used as a way to access the network device through ioctl 
    int fd, err;      //file name and error 

    fd =  open("/dev/net/tap", O_RDWR); //Open all the available tap devices in the network

    if(fd < 0)
    {
        print_error("Cannot open TUN TAP device");
        exit(1);
    }

    CLEAR(ifr); //clear our interface

    /* Flags: IFF_TUN   - TUN device (no Ethernet headers)
     *        IFF_TAP   - TAP device
     *
     *        IFF_NO_PI - Do not provide packet information
    */
    // We set to TAP and not TUN because we are working on layer 2 of the OSI model
    ifr.ifr_flags = IFF_TAP | IFF_NO_PI; //Set flags
    // if(*device)
    // {
    //     strncpy(ifr.ifr_name, device, IFNAMSIZ); //IFNAMSIZ is the size of our ifr_name which was initialized in <net/if.h> header
    // }   
    
    //ioctl is a system call in unix to manipulate the behaviour of devices in kernel space, such as setting their configurations, reading and writing data to them, or getting info about them
    //TUNSETIFF is ioctl set name and options of our ifr 
    //Set devices in kernel space with contents in fd
    //Basically writing to ifr. Reading from fd
    //The kernel will not assign tap as the new tap device
    //The kernel will dynamically assign a tap device with a unique tap name such as tap0
    err = ioctl(fd, TUNSETIFF, (void*) &ifr);
    if(err < 0)
    {
        print_error("Error: Could not reach ioctl tun: %s\n", strerror(errno));
        close(fd);
        return err;
    }

    strncpy(device, ifr.ifr_name, IFNAMSIZ); //IFNAMSIZ is the size of our ifr_name which was initialized in <net/if.h> header
    return fd;
}

//Public------------------------------------------
int tun_read(char *buf, int bytesToRead)
{
    return read(tun_fd, buf, bytesToRead); // reading data from fd and writing to buffer
}

int tun_write(char *buf, int bytesToRead)
{
    return write(tun_fd, buf, bytesToRead); //buffer into tun_fd
}

void tun_init(char *device)
{
    tun_fd = tun_alloc(device);
    
    
    if(set_ifr(device) != 0)
    {
        print_error("ERROR: Setting up interface failed\n");
        return;
    }

    if(set_ifr_ip(device) != 0)
    {
        print_error("ERROR: Setting up interface IP address failed\n");
        return;
    }

    // if(set_ifr_mac(device) != 0)
    // {
    //     print_error("ERROR: Setting up interface MAC address failed\n");
    // }

    system("ifconfig tap0");

    if(set_ifr_route(device, "10.0.0.0/24") != 0)
    {
        print_error("ERROR: Setting up interface route failed\n");
        return;
    }  

}

