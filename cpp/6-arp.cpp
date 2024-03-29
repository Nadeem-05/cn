//dont use this. wont work

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if_arp.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>

void send_arp_request(const char* ifname, const char* target_ip) {
    int sockfd = socket(AF_INET, SOCK_PACKET, htons(ETH_P_ARP));
    if (sockfd < 0) {
        perror("Socket creation failed");
        return;
    }

    struct ifreq ifr;
    strcpy(ifr.ifr_name, ifname);
    if (ioctl(sockfd, SIOCGIFHWADDR, &ifr) < 0) {
        perror("ioctl");
        close(sockfd);
        return;
    }

    struct arpreq arp;
    memset(&arp, 0, sizeof(arp));
    struct sockaddr_in* sin = (struct sockaddr_in*)&arp.arp_pa;
    sin->sin_family = AF_INET;
    sin->sin_addr.s_addr = inet_addr(target_ip);
    sin = (struct sockaddr_in*)&arp.arp_ha;
    sin->sin_family = ARPHRD_ETHER;
    memcpy(arp.arp_ha.sa_data, ifr.ifr_hwaddr.sa_data, 6);
    arp.arp_hln = 6;
    arp.arp_pln = 4;
    arp.arp_op = ARPOP_REQUEST;

    if (ioctl(sockfd, SIOCSARP, &arp) < 0) {
        perror("ioctl");
        close(sockfd);
        return;
    }

    std::cout << "ARP request sent" << std::endl;

    close(sockfd);
}

void send_rarp_request(const char* ifname, const char* target_mac) {
    // RARP is obsolete and not widely supported, so we won't implement it here
    std::cout << "RARP is obsolete and not widely supported." << std::endl;
}

int main() {
    const char* ifname = "eth0"; // Change to your interface name
    const char* target_ip = "192.168.1.1"; // Change to your target IP address

    send_arp_request(ifname, target_ip);

    return 0;
}
