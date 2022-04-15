#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
// #include <linux/socket.h>
#include <linux/socket.h>
#include <linux/in.h>
#include <linux/if.h>
#include <linux/ioctl.h>
#include <linux/sockios.h>
// #include <sys/ioctl.h>
// #include <linux/in.h>
// #include <net/if.h>

char mac[32] = {0};
/**
	@berf Get the MAC address of hi3516 device
*/
int get_mac_addr(void)
{
    struct ifreq ifreq;
    int sock = 0;
    

    sock = socket(AF_INET,SOCK_STREAM,0);
    if(sock < 0) {
        perror("error sock");
        return 2;
    }

    strcpy(ifreq.ifr_name,"ap0");
    if(ioctl(sock,SIOCGIFHWADDR,&ifreq) < 0) {
        perror("error ioctl");
        return 3;
    }

    int i = 0;
    for(i = 0; i < 6; i++){
        sprintf(mac+3*i, "%02X:", (unsigned char)ifreq.ifr_hwaddr.sa_data[i]);
    }
    mac[strlen(mac) - 1] = 0;
    printf("MAC: %s\n", mac);
               
    return 0;
}