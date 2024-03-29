#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFSIZE 1024
void error(char *msg) {
  	perror(msg);
  	exit(1);
}

typedef struct h3datastruct{
    int n;
    char s[];
} h3datastruct;

int main(int argc, char **argv) {
  	int sockfd;
  	int clientlen;
  	h3datastruct h3data;

  	struct hostent *hostp;
  	char buf[BUFSIZE];
  	char *hostaddrp;
  	int optval;
  	int n; /* message byte size */
    int val;

	printf("=============Creating socket==============\n");
  	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  	if (sockfd < 0) error("ERROR opening socket");

	printf("================setsockopt================\n");
  	optval = 1;
  	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR,(const void *)&optval , sizeof(int));

	printf("=========== Server's  address=============\n");
	//=======================h3===============
	// struct sockaddr_in h3_serveraddr;  //own
	// int h3_portno = 14503;
	// bzero((char *) &h3_serveraddr, sizeof(h3_serveraddr));
  	// h3_serveraddr.sin_family = AF_INET;
  	// h3_serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
  	// h3_serveraddr.sin_port = htons((unsigned short)h3_portno);

	//==================== h2 ==============
    char h2ip[] = "192.168.1.3";
    int h2_portno = 9999;
	struct sockaddr_in h2_serveraddr;	//to send
    bzero((char *) &h2_serveraddr, sizeof(h2_serveraddr));
    h2_serveraddr.sin_family = AF_INET;
    // h2_serveraddr.sin_addr.s_addr = inet_addr(h2ip);
	h2_serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    h2_serveraddr.sin_port = htons(h2_portno);
	// int h2_serverlen = sizeof(h2_serveraddr);

  	printf("==============binding====================\n");
  	if (bind(sockfd, (struct sockaddr *) &h2_serveraddr,sizeof(h2_serveraddr)) < 0) error("ERROR on binding");
	bzero(buf, BUFSIZE);
	int h2_serverlen = sizeof(h2_serveraddr);
	printf("f1\n");
	//?
	// printf("=============listening====================\n");
    // if (listen(sockfd, 5) < 0) error("ERROR on listen");
    
	printf("f2\n");
  	while(1){
		// printf("f3\n");
		printf("receiving...\n");
		// //=======wait for connection requ.
        // sockfd = accept(sockfd, (struct sockaddr *) &clientaddr, &clientlen);
        // printf("f3\n");
        // if (sockfd < 0) error("ERROR on accept");

		//==================receive============
        
    	n = recvfrom(sockfd, &h3data, sizeof(h3data), 0,(struct sockaddr *) &h2_serveraddr, &h2_serverlen);
		// n = read(sockfd, &h3data, sizeof(h3data));
		printf("f4\n");
        if (n < 0) error("ERROR in recvfrom");

		n = recvfrom(sockfd, buf, BUFSIZE, 0,(struct sockaddr *) &h2_serveraddr, &h2_serverlen);

        printf("done!\n");
		printf("Recieved n:%d, ip:%s \n", h3data.n, buf);

		//==================send============
		close(sockfd);
		//================== to send addr (e.g h1)==============
    	char * h1ip = "192.168.1.2"; //will be overwritten later by recievced addr
		h1ip = h3data.s;
    	int h1_portno = 8888;
		struct sockaddr_in h1_clientaddr;	//to send
    	bzero((char *) &h1_clientaddr, sizeof(h1_clientaddr));
    	h1_clientaddr.sin_family = AF_INET;
    	h1_clientaddr.sin_addr.s_addr = inet_addr(buf);
    	h1_clientaddr.sin_port = htons(h1_portno);
		int h1_clientlen = sizeof(h1_clientaddr);

		int tcpsockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (tcpsockfd < 0) error("ERROR opening tcp socket");

        if (connect(tcpsockfd, &h1_clientaddr, sizeof(h1_clientaddr)) < 0)  error("ERROR connecting");
        printf("sending...");
        // n = sendto(sockfd, (h3data.n)*(h3data.n), sizeof(int), 0, (struct sockaddr *) &h1_clientaddr, h1_clientlen);     
		int r = (h3data.n)*(h3data.n);
		printf("r:%d\n",r);
		n = send(tcpsockfd, &r, sizeof(r),0);
        if (n < 0) error("ERROR in sendto");
        printf("done!\n");

		// break;
  	}

	close(sockfd);
    return 0;
}
