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
    int op;
} sendbuf;

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

	printf("=====================Creating socket======================\n");
  	parentfd = socket(AF_INET, SOCK_STREAM, 0);
  	if (parentfd < 0) error("ERROR opening socket");

	printf("=======================setsockopt=======================\n");
  	optval = 1;
  	setsockopt(parentfd, SOL_SOCKET, SO_REUSEADDR,(const void *)&optval , sizeof(int));

	printf("===================build the server's Internet address====================\n");
	bzero((char *) &serveraddr, sizeof(serveraddr));
  	serveraddr.sin_family = AF_INET;
  	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
  	serveraddr.sin_port = htons((unsigned short)portno);

  	printf("=======================binding==========================\n");
  	if (bind(parentfd, (struct sockaddr *) &serveraddr,sizeof(serveraddr)) < 0) error("ERROR on binding");
	printf("f1\n");
	
    printf("=======================listening==========================\n");
    if (listen(parentfd, 5) < 0) error("ERROR on listen");
    printf("f2\n");

    clientlen = sizeof(clientaddr);
	int m1,m2,m3,m4,p1,p2,p4,p1_,p2_,p4_,f1,f2,f4;
	int flag=0;
  	while (1){
		printf("f3\n");
    	
        //wait for connection requ.
        childfd = accept(parentfd, (struct sockaddr *) &clientaddr, &clientlen);
        printf("f4\n");
        if (childfd < 0) error("ERROR on accept");

        //find sender host
        hostp = gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr, sizeof(clientaddr.sin_addr.s_addr), AF_INET);
        if (hostp == NULL) error("ERROR on gethostbyaddr");
        hostaddrp = inet_ntoa(clientaddr.sin_addr);
        if (hostaddrp == NULL) error("ERROR on inet_ntoa\n");
        printf("server established connection with %s (%s)\n", hostp->h_name, hostaddrp);

        //read data
        bzero(buf, BUFSIZE);
        n = recvfrom(parentfd, &d, sizeof(d), 0,(struct sockaddr *) &clientaddr, &clientlen);
        // n = read(childfd, buf, BUFSIZE);
        // n = read(childfd, &d, BUFSIZE);
        if (n < 0) error("ERROR re&dm socket");
        printf("server received %d bytes: %d %d %d", n, d.n1,d.n2,d.op);

        //echo back the data
        // n = write(childfd, buf, strlen(buf));
        // if (n < 0) error("ERROR writing to socket");

        close(childfd);
			
  	}

}
