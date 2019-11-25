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

//int portno = 	8567;
int portno = 	9567;

int main(int argc, char **argv) {
  	int sockfd;
  	int clientlen;
  	struct sockaddr_in serveraddr;  
	struct sockaddr_in clientaddr;
  	struct hostent *hostp;
  	char buf[BUFSIZE];
  	char *hostaddrp;
  	int optval;
  	int n; /* message byte size */

	printf("=====================Creating socket======================\n");
  	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  	if (sockfd < 0) error("ERROR opening socket");

	printf("=======================setsockopt=======================\n");
  	optval = 1;
  	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR,(const void *)&optval , sizeof(int));

	printf("===================build the server's Internet address====================\n");
	bzero((char *) &serveraddr, sizeof(serveraddr));
  	serveraddr.sin_family = AF_INET;
  	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
  	serveraddr.sin_port = htons((unsigned short)portno);

  	printf("=======================binding==========================\n");
  	if (bind(sockfd, (struct sockaddr *) &serveraddr,sizeof(serveraddr)) < 0) error("ERROR on binding");
	printf("f1\n");
	clientlen = sizeof(clientaddr);
	printf("f2\n");
	int m1,m2,m3,m4,p1,p2,p4,p1_,p2_,p4_,f1,f2,f4;
	int flag=0;
  	while (1){
		//printf("f3\n");
    	bzero(buf, BUFSIZE);
    	n = recvfrom(sockfd, buf, BUFSIZE, 0,(struct sockaddr *) &clientaddr, &clientlen);
		if (n < 0) error("ERROR in recvfrom");
		printf("=> %s", buf);
		p1=(-48+buf[0]);
		p2=(-48+buf[1]);
		m1=(-48+buf[2]);
		p4=(-48+buf[3]);
		m2=(-48+buf[4]);
		m3=(-48+buf[5]);
		m4=(-48+buf[6]);
		

		p1_=m1^m2^m4;				//p1=m1^m2^m4
		p2_=m1^m3^m4;				//p2=m1^m3^m4
		p4_=m2^m3^m4;
		
		f1=0;
		f2=0;
		f4=0;
		if(p1!=p1_) f1=1;
		if(p2!=p2_) f2=1;
		if(p4!=p4_) f4=1;

		if(f1+f2+f4==3){
			if(m4==0) m4=1;
			else m4=0;
		}else if(f1+f2==2){
			if(m1==0) m1=1;
			else m1=0;
		}else if(f1+f4==2){
			if(m2==0) m2=1;
			else m2=0;
		}else if(f2+f4==2){	//f2+f4==2
			if(m3==0) m3=1;
			else m3=0;
		}
		if(m4==0){
			printf(", m1m2m3m4 = %d%d%d%d\n",m1,m2,m3,m4);
			//printf("\n");
			break;
			//flag=1;
		}else{
			printf(", m1m2m3m4 = %d%d%d%d\n",m1,m2,m3,m4);
		}
		/*else{
			if(flag!=1){
				print("%d%d%d%d",m1,m2,m3,m4);
			}else{
				flag=0;
			}
			
		}*/
			
  	}

}
