#include <iostream>
#include<stdio.h>
#include <unistd.h>
#include<malloc.h>
#include<string.h>
#include<signal.h>
#include<stdbool.h>
#include<sys/socket.h>
#include<sys/types.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include<linux/if.h>
#include<linux/if_packet.h>
#include<netinet/in.h>		 
#include<netinet/if_ether.h>      // for ethernet header
#include<netinet/ip.h>		      // for ip header
#include<netinet/udp.h>		      // for udp header
#include<netinet/tcp.h>
#include<arpa/inet.h>             // to avoid warning at inet_ntoa

#define BUFF_MAX_SIZE 65536

using namespace std;

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
	int raw_socket;
	int n;
	struct sockaddr saddr;
	int saddr_len = sizeof(saddr);
	unsigned char *buff = (unsigned char *) malloc(BUFF_MAX_SIZE);


	raw_socket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_IP));
	if (raw_socket < 0) error("Failed to create socket\n");

	sockaddr_ll sll;
    ifreq ifr; 
    bzero(&sll , sizeof(sll));
    bzero(&ifr , sizeof(ifr));  
    strncpy((char *)ifr.ifr_name ,"eth1" , IFNAMSIZ);
    
    if((ioctl(raw_socket , SIOCGIFINDEX , &ifr)) == -1) error("Unable to find interface index");
    sll.sll_family = AF_PACKET; 
    sll.sll_ifindex = ifr.ifr_ifindex; 
    sll.sll_protocol = htons(ETH_P_IP);
    if((bind(raw_socket, (struct sockaddr *)&sll , sizeof(sll))) == -1) error("bind: ");
	
	int t = 0;        
	while (true) {
		
		n = recvfrom(raw_socket, buff, BUFF_MAX_SIZE, 0, NULL, NULL);
		if (n < 0) error("Failed to get packets\n");

		ethhdr *eth = (ethhdr *) buff;
		iphdr *ip = (struct iphdr*)(buff + sizeof(struct ethhdr));
		
		sockaddr_in source;
    	sockaddr_in dest;
		memset(&source, 0, sizeof(source));
		memset(&dest, 0, sizeof(dest));
		source.sin_addr.s_addr = ip->saddr;
		dest.sin_addr.s_addr = ip->daddr;

		/* ============ print ================  */
		string temp(inet_ntoa(source.sin_addr));
		printf("t : %d\n", ++t);
		printf("\nEthernet Header\n");
		printf("\t|-Source Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n",eth->h_source[0], eth->h_source[1], eth->h_source[2], eth->h_source[3], eth->h_source[4], eth->h_source[5]);
		printf("\t|-Destination Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n",eth->h_dest[0], eth->h_dest[1], eth->h_dest[2], eth->h_dest[3], eth->h_dest[4], eth->h_dest[5]);
		printf("\t|-Protocol : %d\n",eth->h_proto);
		printf("\t|-Source IP : %s\n", inet_ntoa(source.sin_addr));
		printf("\t|-Destination IP : %s\n",inet_ntoa(dest.sin_addr));
		
	}

	close(raw_socket);

	return 0;
}