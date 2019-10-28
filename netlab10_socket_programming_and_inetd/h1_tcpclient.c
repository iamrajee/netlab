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
    int op;
} sendbuf;

int main(int argc, char **argv) {
    int sockfd, n;
    int recvh3_data;
    sendbuf d;
    int h2portno;

    printf("===================Creating socket====================\n");
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("ERROR opening socket");

	printf("===================get a message from the user====================\n");
    printf("Enter (number1,number2,operation{0:ADD,1:SUB,2:MUL,3:IDIV}):=>");
    scanf("%d%d%d",&d.n1,&d.n2,&d.op);
    if(d.op>-1 && d.op<4){
        h2portno = 12500 + d.op;
    }else{
        printf("Enter valid option !!\n");
        return 0;
    }

    printf("========================h2 server address====================\n");
    struct sockaddr_in h2serveraddr;
    char h2serverip[] = "192.168.1.3"; //h2 ip
    int h2serverlen;
    // int h2portno;
    bzero((char *) &h2serveraddr, sizeof(h2serveraddr));
    h2serveraddr.sin_family = AF_INET;
    h2serveraddr.sin_addr.s_addr = inet_addr(h2serverip);
    h2serveraddr.sin_port = htons(h2portno);
	h2serverlen = sizeof(h2serveraddr);

    printf("========================h3 server address====================\n");
    struct sockaddr_in h3serveraddr;
    char h3serverip[] = "192.168.1.3"; //h2 ip
    int h3serverlen;
    int h3portno;
    bzero((char *) &h3serveraddr, sizeof(h3serveraddr));
    h3serveraddr.sin_family = AF_INET;
    h3serveraddr.sin_addr.s_addr = inet_addr(h3serverip);
    h3serveraddr.sin_port = htons(h3portno);
	h3serverlen = sizeof(h3serveraddr);    
	
    

	while(1){
        printf("========================Connecting to h2====================\n");
        if (connect(sockfd, &h2serveraddr, sizeof(h2serveraddr)) < 0)  error("ERROR connecting!\n");
        
        printf("sending...");
        n = write(sockfd, &d, sizeof(d));
		// n = sendto(sockfd, &d, sizeof(d), 0, (struct sockaddr *) &h2serveraddr, h2serverlen);
    	if (n < 0) error("ERROR in sendto");
        printf("done!\n");

        printf("========================Connecting to h3====================\n");
        if (connect(sockfd, &h3serveraddr, sizeof(h3serveraddr)) < 0)  error("ERROR connecting!\n");

        n = read(sockfd, recvh3_data, sizeof(recvh3_data));
        if (n < 0) error("ERROR reading from socket");
        printf("Recieved data is = %d", recvh3_data);

        break;
	}
    
    close(sockfd);
    return 0;
}
