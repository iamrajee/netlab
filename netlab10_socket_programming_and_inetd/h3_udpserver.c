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



int main(int argc, char **argv) {
  	int sockfd;
  	int clientlen;
  	struct sockaddr_in h3_serveraddr;  
    struct sockaddr_in h1_serveraddr;  
	struct sockaddr_in h2_clientaddr;
    int h3_portno = 9567;
  	struct hostent *hostp;
  	char buf[BUFSIZE];
  	char *hostaddrp;
  	int optval;
  	int n; /* message byte size */
    int val;

	printf("=====================Creating socket======================\n");
  	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  	if (sockfd < 0) error("ERROR opening socket");

	printf("=======================setsockopt=======================\n");
  	optval = 1;
  	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR,(const void *)&optval , sizeof(int));

	printf("===================build the server's Internet address====================\n");
	bzero((char *) &h3_serveraddr, sizeof(h3_serveraddr));
  	h3_serveraddr.sin_family = AF_INET;
  	h3_serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
  	h3_serveraddr.sin_port = htons((unsigned short)h3_portno);

    char h1_hostname[] = "192.168.1.2";
    int h1_portno = 8550;
    bzero((char *) &h1_serveraddr, sizeof(h1_serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(h1_hostname);
    serveraddr.sin_port = htons(h1_portno);
	serverlen = sizeof(h1_serveraddr);

  	printf("=======================binding==========================\n");
  	if (bind(sockfd, (struct sockaddr *) &h3_serveraddr,sizeof(h3_serveraddr)) < 0) error("ERROR on binding");
	printf("f1\n");


	h2_clientlen = sizeof(h2_clientaddr);
  	while (1){
		
        printf("receiving...");
    	n = recvfrom(sockfd, val, sizeof(int), 0,(struct sockaddr *) &h2_clientaddr, &h2_clientlen);
		printf("f3\n");
        if (n < 0) error("ERROR in recvfrom");
        printf("done!\n");

        if (connect(sockfd, &h1_serveraddr, sizeof(h1_serveraddr)) < 0)  error("ERROR connecting");
        printf("sending...");
        n = sendto(sockfd, val*val, sizeof(val*val), 0, (struct sockaddr *) &h1_serveraddr, h1_serverlen);        
        if (n < 0) error("ERROR in sendto");
        printf("done!\n");

  	}

}
