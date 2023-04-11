#ifndef TUNTAP_IFR_H
#define TUNTAP_IFR_H

int tun_read(char *buf, int length);
int tun_write(char *buf, int length);
void tun_init(char *device);

#endif