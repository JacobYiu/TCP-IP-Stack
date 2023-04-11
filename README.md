# Jacob TCP/IP Stack

## Building Instructions

### Ensure that the tap device is initialized and created.
```bash
sudo mknod /dev/net/tap c 11 201 && sudo chmod 0666 /dev/net/tap
```

### Emulation
While currently in the root folder, run the command: 
```bash
make
```

Change directory to the bin folder and run the command:
```bash
sudo ./arp
```

On a separate terminal, run the command: 
```bash
sudo arping -I tap0 10.0.0.4
```

### Debugging
For debugging purposes, I had run wireshark. 
I had to ensure:
- The tap0 device was sending a broadcast with MAC address ff:ff:ff:ff:ff
- The tap0 device received a reply back with an appropriate MAC address


### Results
After running all the commands you should get a ARP reply as such: 

```bash
ARPING 10.0.0.4
42 bytes from 00:0c:29:6d:50:26 (10.0.0.4): index=0 time=88.998 usec
42 bytes from 00:0c:29:6d:50:26 (10.0.0.4): index=1 time=80.562 usec
42 bytes from 00:0c:29:6d:50:26 (10.0.0.4): index=2 time=78.478 usec
42 bytes from 00:0c:29:6d:50:26 (10.0.0.4): index=3 time=92.948 usec
42 bytes from 00:0c:29:6d:50:26 (10.0.0.4): index=4 time=72.797 usec
42 bytes from 00:0c:29:6d:50:26 (10.0.0.4): index=5 time=79.489 usec
42 bytes from 00:0c:29:6d:50:26 (10.0.0.4): index=6 time=84.717 usec
```

