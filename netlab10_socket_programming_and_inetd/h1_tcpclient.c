#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <math.h> 
#define BUFSIZE 1024


void error(char *msg) {
    perror(msg);
    exit(0);
}

typedef struct sendbuf{
    int n1;
    int n2;
    // int op;
} sendbuf;

int main(int argc, char **argv) {
    int sockfd, n;

    int optval;
    int parentfd;
    int childfd;

    int recvh3_data;
    sendbuf d;
    int h2portno;
    int op;
    printf("===========Creating socket===========\n");
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("ERROR opening socket");

	printf("==========get a message from the user==========\n");
    printf("Enter (number1,number2,operation{0:ADD,1:SUB,2:MUL,3:IDIV}):=>");
    // scanf("%d%d%d",&d.n1,&d.n2,&op);
    d.n1=3;
    d.n2=3;
    op=0;
    if(op>-1 && op<4){
        h2portno = 12500 + op;
    }else{
        printf("Enter valid option !!\n");
        return 0;
    }

    printf("=================h2 server address============\n");
    struct sockaddr_in h2serveraddr;
    char h2serverip[] = "192.168.1.3"; //h2 ip
    int h2serverlen;
    // int h2portno;
    bzero((char *) &h2serveraddr, sizeof(h2serveraddr));
    h2serveraddr.sin_family = AF_INET;
    h2serveraddr.sin_addr.s_addr = inet_addr(h2serverip);
    h2serveraddr.sin_port = htons(h2portno);
	h2serverlen = sizeof(h2serveraddr);

    // printf("===============h3 server address==============\n");
    // struct sockaddr_in h3serveraddr;
    // char h3serverip[] = "192.168.1.4"; //h3 ip
    // int h3serverlen;
    // int h3portno=8888;
    // bzero((char *) &h3serveraddr, sizeof(h3serveraddr));
    // h3serveraddr.sin_family = AF_INET;
    // h3serveraddr.sin_addr.s_addr = inet_addr(h3serverip);
    // h3serveraddr.sin_port = htons(h3portno);
	// h3serverlen = sizeof(h3serveraddr);    
	
    // **************************** SEND ****************************
    printf("=============Connecting to h2============\n");
    if (connect(sockfd, &h2serveraddr, sizeof(h2serveraddr)) < 0)  error("ERROR connecting!\n");
    printf("sending...");
    n = write(sockfd, &d, sizeof(d));
	// n = sendto(sockfd, &d, sizeof(d), 0, (struct sockaddr *) &h2serveraddr, h2serverlen);
    if (n < 0) error("ERROR in sendto");
    printf("done!\n");


    // **************************** RECIEVE ****************************
    close(sockfd);
    printf("==============Creating socket=============\n");
  	parentfd = socket(AF_INET, SOCK_STREAM, 0);
  	if (parentfd < 0) error("ERROR opening socket");

	printf("================setsockopt=================\n");
  	optval = 1;
  	setsockopt(parentfd, SOL_SOCKET, SO_REUSEADDR,(const void *)&optval , sizeof(int));

    printf("=================h3 udp server address===============\n");
    struct sockaddr_in h3serveraddr;
    char h3serverip[] = "192.168.1.4"; //h3 ip
    int h3serverlen;
    int h3portno=8888;
    bzero((char *) &h3serveraddr, sizeof(h3serveraddr));
    h3serveraddr.sin_family = AF_INET;
    // h3serveraddr.sin_addr.s_addr = inet_addr(h3serverip);
    h3serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    h3serveraddr.sin_port = htons(h3portno);
	// h3serverlen = sizeof(h3serveraddr);

  	printf("===================binding=========================\n");
  	if (bind(parentfd, (struct sockaddr *) &h3serveraddr,sizeof(h3serveraddr)) < 0) error("ERROR on binding");
	printf("f1\n");
	
    printf("===================listening========================\n");
    if (listen(parentfd, 5) < 0) error("ERROR on listen");

    h3serverlen = sizeof(h3serveraddr);


	while(1){

        // // **************************** SEND ****************************
        // printf("=============Connecting to h2============\n");
        // if (connect(sockfd, &h2serveraddr, sizeof(h2serveraddr)) < 0)  error("ERROR connecting!\n");
        // printf("sending...");
        // n = write(sockfd, &d, sizeof(d));
		// // n = sendto(sockfd, &d, sizeof(d), 0, (struct sockaddr *) &h2serveraddr, h2serverlen);
    	// if (n < 0) error("ERROR in sendto");
        // printf("done!\n");

        // // **************************** RECIEVE ****************************
        
        childfd = accept(parentfd, (struct sockaddr *)&h3serveraddr, &h3serverlen);
        if (childfd < 0) error("ERROR on accept");
        char buffer[200];
        n = read(childfd, buffer, sizeof(buffer));
        int *x = (int*) buffer;
        if (n < 0) error("ERROR reading from socket");
        printf("Recieved data is = %d\n", *x);

        break;
	}
    
    close(sockfd);
    return 0;
}