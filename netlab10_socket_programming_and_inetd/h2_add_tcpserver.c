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

int portno = 	12500;

typedef struct sendbuf{
    int n1;
    int n2;
    // int op;
} sendbuf;

typedef struct h3datastruct{
    int n;
    char s[];
} h3datastruct;


int main(int argc, char **argv) {
    int parentfd;
    int childfd;
    int clientlen;
  	struct sockaddr_in serveraddr;  
	struct sockaddr_in clientaddr;
  	struct hostent *hostp;
  	char buf[BUFSIZE];
  	char *hostaddrp;
  	int optval;
  	int n; /* message byte size */
    sendbuf d;
    h3datastruct h3data;

	printf("==============Creating socket=============\n");
  	parentfd = socket(AF_INET, SOCK_STREAM, 0);
  	if (parentfd < 0) error("ERROR opening socket");

	printf("================setsockopt=================\n");
  	optval = 1;
  	setsockopt(parentfd, SOL_SOCKET, SO_REUSEADDR,(const void *)&optval , sizeof(int));

	printf("=========build the server's Internet address=========\n");
	bzero((char *) &serveraddr, sizeof(serveraddr));
  	serveraddr.sin_family = AF_INET;
  	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
  	serveraddr.sin_port = htons((unsigned short)portno);

    printf("=================h3 server address===============\n");
    struct sockaddr_in h3serveraddr;
    char h3serverip[] = "192.168.1.4"; //h3 ip
    int h3serverlen;
    int h3portno=9999;
    bzero((char *) &h3serveraddr, sizeof(h3serveraddr));
    h3serveraddr.sin_family = AF_INET;
    h3serveraddr.sin_addr.s_addr = inet_addr(h3serverip);
    h3serveraddr.sin_port = htons(h3portno);
	h3serverlen = sizeof(h3serveraddr);  

  	printf("===================binding=========================\n");
  	if (bind(parentfd, (struct sockaddr *) &serveraddr,sizeof(serveraddr)) < 0) error("ERROR on binding");
	printf("f1\n");
	
    printf("===================listening========================\n");
    if (listen(parentfd, 5) < 0) error("ERROR on listen");
    // printf("f2\n");
    
    clientlen = sizeof(clientaddr);
	int flag=0;
  	while (1){
        //**************************** RECIEVE ****************************
		// printf("f3\n");
        //=======wait for connection requ.
        childfd = accept(parentfd, (struct sockaddr *) &clientaddr, &clientlen);
        // printf("f4\n");
        if (childfd < 0) error("ERROR on accept");

        //==========read data

        bzero(buf, BUFSIZE);
        // n = recvfrom(parentfd, &d, sizeof(d), 0,(struct sockaddr *) &clientaddr, &clientlen);
        n = read(childfd, &d, sizeof(d));
        if (n < 0) error("ERROR re&dm socket");
        printf("server received %d bytes: %d %d\n", n, d.n1,d.n2);

        //**************************** SEND ****************************
        //=======find sender host
        // hostp = gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr, sizeof(clientaddr.sin_addr.s_addr), AF_INET);
        // if (hostp == NULL) error("ERROR on gethostbyaddr");
        hostaddrp = inet_ntoa(clientaddr.sin_addr);
        if (hostaddrp == NULL) error("ERROR on inet_ntoa\n");
        printf("server established connection with (%s)\n", hostaddrp);

        //============= Send data to h3 ========
        // close(childfd);
        int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        h3data.n = d.n1 + d.n2;
        // h3data.s = hostaddrp;
        strcpy(h3data.s, hostaddrp);
        printf("==============Connecting to h3===========\n");
        // if (connect(sockfd, &h3serveraddr, sizeof(h3serveraddr)) < 0)  error("ERROR connecting!\n");
        // n = write(sockfd, &h3data, sizeof(h3data));
        n = sendto(sockfd, &h3data, sizeof(h3data), 0, (struct sockaddr *) &h3serveraddr, h3serverlen);
        if (n < 0) error("ERROR writing to socket");

        strcpy(buf, inet_ntoa(clientaddr.sin_addr));   /* client IP */
        n = sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *)&h3serveraddr, h3serverlen);

        // close(sockfd);
        // break;
  	}
    close(childfd);
    // close(sockfd);
}
