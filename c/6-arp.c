// wont work
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>

#define ARP_REQUEST 1
#define ARP_REPLY   2
#define RARP_REQUEST 3
#define RARP_REPLY   4

struct arphdr {
    unsigned short  ar_hrd;         // format of hardware address
    unsigned short  ar_pro;         // format of protocol address
    unsigned char   ar_hln;         // length of hardware address
    unsigned char   ar_pln;         // length of protocol address
    unsigned short  ar_op;          // ARP/RARP operation
    unsigned char   ar_sha[6];      // sender hardware address
    unsigned char   ar_sip[4];      // sender IP address
    unsigned char   ar_tha[6];      // target hardware address
    unsigned char   ar_tip[4];      // target IP address
};

void send_arp_request(const char* ifname, const char* target_ip) {
    struct sockaddr_in target_addr;
    int sockfd, i;
    struct ifreq ifr;
    unsigned char buffer[1024];

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_PACKET, htons(ETH_P_ARP))) < 0) {
        perror("socket");
        return;
    }

    // Get interface index
    strcpy(ifr.ifr_name, ifname);
    if (ioctl(sockfd, SIOCGIFINDEX, &ifr) < 0) {
        perror("ioctl");
        close(sockfd);
        return;
    }

    // Set target IP address
    memset(&target_addr, 0, sizeof(target_addr));
    target_addr.sin_family = AF_INET;
    target_addr.sin_addr.s_addr = inet_addr(target_ip);

    // Prepare ARP packet
    struct arphdr *arp = (struct arphdr *)buffer;
    arp->ar_hrd = htons(ARPHRD_ETHER);
    arp->ar_pro = htons(ETH_P_IP);
    arp->ar_hln = 6;
    arp->ar_pln = 4;
    arp->ar_op = htons(ARP_REQUEST);

    // Set source MAC address
    unsigned char mac[6];
    if (ioctl(sockfd, SIOCGIFHWADDR, &ifr) < 0) {
        perror("ioctl");
        close(sockfd);
        return;
    }
    memcpy(arp->ar_sha, ifr.ifr_hwaddr.sa_data, 6);

    // Set source IP address
    struct sockaddr_in *sin = (struct sockaddr_in *)&ifr.ifr_addr;
    memcpy(arp->ar_sip, &sin->sin_addr, 4);

    // Set target MAC address (all zeros for ARP request)
    memset(arp->ar_tha, 0, 6);

    // Set target IP address
    memcpy(arp->ar_tip, &target_addr.sin_addr, 4);

    // Send ARP request
    struct sockaddr to;
    memset(&to, 0, sizeof(to));
    to.sa_family = AF_PACKET;
    strcpy(to.sa_data, ifr.ifr_name);
    if (sendto(sockfd, buffer, sizeof(struct arphdr), 0, &to, sizeof(to)) < 0) {
        perror("sendto");
        close(sockfd);
        return;
    }

    printf("ARP request sent\n");

    close(sockfd);
}

void send_rarp_request(const char* ifname, const char* target_mac) {
    // RARP is obsolete and not widely supported, so we won't implement it here
    printf("RARP is obsolete and not widely supported.\n");
}

int main() {
    const char* ifname = "eth0"; // Change to your interface name
    const char* target_ip = "192.168.1.1"; // Change to your target IP address

    send_arp_request(ifname, target_ip);

    return 0;
}
