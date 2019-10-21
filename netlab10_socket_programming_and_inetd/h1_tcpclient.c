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
    int serverlen;
    struct sockaddr_in serveraddr;

    char hostname[] = "192.168.1.3";
    int portno;

    sendbuf d;

    printf("===================Creating socket====================\n");
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("ERROR opening socket");

	printf("===================get a message from the user====================\n");
    // bzero(buf, BUFSIZE);
    printf("Enter (number1,number2,operation{0:ADD,1:SUB,2:MUL,3:IDIV}):=>");
    scanf("%d%d%d",&d.n1,&d.n2,&d.op);
    if(d.op>-1 && d.op<4){
        portno = 12500 + d.op;
    }else{
        printf("Enter valid option !!\n");
        return 0;
    }

    printf("===================build the server's Internet address====================\n");
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(hostname);
    serveraddr.sin_port = htons(portno);
	serverlen = sizeof(serveraddr);
	

    if (connect(sockfd, &serveraddr, sizeof(serveraddr)) < 0)  error("ERROR connecting");

	while(1){
        printf("sending...");
		char buf2[10];
		n = sendto(sockfd, &d, sizeof(d), 0, (struct sockaddr *) &serveraddr, serverlen);
    	if (n < 0) error("ERROR in sendto");
        printf("done!\n");
        break;
	}
    
    close(sockfd);
    return 0;
}
