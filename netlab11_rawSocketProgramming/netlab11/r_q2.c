#include<stdio.h>
#include<malloc.h>
#include<string.h>
#include<signal.h>
#include<stdbool.h>
#include<sys/socket.h>
#include<sys/types.h>
#include <errno.h>
#include<string.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include<linux/if_packet.h>
#include<netinet/in.h>		 
#include<netinet/if_ether.h>    // for ethernet header
#include<netinet/ip.h>		// for ip header
#include<netinet/udp.h>		// for udp header
#include<netinet/tcp.h>
#include<arpa/inet.h>           // to avoid warning at inet_ntoa

void BindToInterface(int raw , char *device , int protocol) { 
    struct sockaddr_ll sll;
    struct ifreq ifr; 
    bzero(&sll , sizeof(sll));
    bzero(&ifr , sizeof(ifr)); 
    strncpy((char *)ifr.ifr_name ,device , IFNAMSIZ); 
    //copy device name to ifr 
    if((ioctl(raw , SIOCGIFINDEX , &ifr)) == -1)
    { 
        perror("Unable to find interface index");
        exit(-1); 
    }
    sll.sll_family = AF_PACKET; 
    sll.sll_ifindex = ifr.ifr_ifindex; 
    sll.sll_protocol = htons(ETH_P_IP); 
    if((bind(raw , (struct sockaddr *)&sll , sizeof(sll))) ==-1)
    {
        perror("bind: ");
        exit(-1);
    }
    return;
} 

int main(){

	int sock_r;
	sock_r=socket(AF_PACKET,SOCK_RAW,htons(ETH_P_IP));
	if(sock_r<0)
	{
		printf("error in socket\n");
		return -1;
	}
	char * opt;
	opt = "eth1"; 
	BindToInterface(sock_r,"eth1", htons(ETH_P_IP));
	FILE *log_txt;
	log_txt=fopen("log.txt","w");
	while(1){
		unsigned char *buffer = (unsigned char *) malloc(65536); //to receive data
		memset(buffer,0,65536);
		struct sockaddr saddr;
		int saddr_len = sizeof (saddr);
		 
		//Receive a network packet and copy in to buffer
		int buflen =recvfrom(sock_r,buffer,65536,0,&saddr,(socklen_t *)&saddr_len);
		if(buflen<0){
			printf("error in reading recvfrom function\n");
			return -1;
		}
		fflush(stdout);
		struct ethhdr *eth = (struct ethhdr *)(buffer);
		printf("\nEthernet Header\n");
		printf("\t|-Source Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n",eth->h_source[0],eth->h_source[1],eth->h_source[2],eth->h_source[3],eth->h_source[4],eth->h_source[5]);
		printf("\t|-Destination Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n",eth->h_dest[0],eth->h_dest[1],eth->h_dest[2],eth->h_dest[3],eth->h_dest[4],eth->h_dest[5]);
		printf("\t|-Protocol : %d\n",eth->h_proto);

		
		struct sockaddr_in source, dest;
		unsigned short iphdrlen;
		struct iphdr *ip = (struct iphdr*)(buffer + sizeof(struct ethhdr));
		memset(&source, 0, sizeof(source));
		source.sin_addr.s_addr = ip->saddr;
		memset(&dest, 0, sizeof(dest));
		dest.sin_addr.s_addr = ip->daddr;
	
		
		printf("\nIP Header\n");
		printf("\t|-Version : %d\n",(unsigned int)ip->version);

		printf("\t|-Protocol : %d\n",(unsigned int)ip->protocol);

		

		printf("\t|-Source IP : %s\n", inet_ntoa(source.sin_addr));

		printf("\t|-Destination IP : %s\n",inet_ntoa(dest.sin_addr));
		buflen = -1;
	}


	close(sock_r);// use signals to close socket 08-00-27-C9-61-5A
	return 0;
}
