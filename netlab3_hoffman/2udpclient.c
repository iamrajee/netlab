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

int portno = 8567;

void error(char *msg) {
    perror(msg);
    exit(0);
}

char* stringToBinary(char* s) {
    if(s == NULL) return 0; /* no input string */
    size_t len = strlen(s)-1;
    char *binary = malloc(len*8 + 1); // each char is one byte and + 1 at the end for null terminator
    binary[0] = '\0';
    for(size_t i = 0; i < len; ++i) {
        char ch = s[i];
        for(int j = 7; j >= 0; --j){
            if(ch & (1 << j)) {
                strcat(binary,"1");
            } else {
                strcat(binary,"0");
            }
        }
    }
    return binary;
}

int main(int argc, char **argv) {
    int sockfd, n;
    int serverlen;
    struct sockaddr_in serveraddr;

    //struct hostent *server;
    char hostname[] = "192.168.1.2";
    char buf[BUFSIZE];


    printf("===================Creating socket====================\n");
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) error("ERROR opening socket");

	printf("===================build the server's Internet address====================\n");
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(hostname);
    serveraddr.sin_port = htons(portno);

	printf("===================get a message from the user====================\n");
    bzero(buf, BUFSIZE);
    printf("Enter msg:=>");
    fgets(buf, BUFSIZE, stdin);
	//printf("%s\n",buf);
	int msglen = strlen(buf)-1;
	//printf("msglen => %d\n",msglen);
	/*for(int i=0; i< msglen; i++){
		printf("i%d:%c\n",i,buf[i]);
	}
	printf("buf:%s,%d\n",buf,(-48+buf));*/
	serverlen = sizeof(serveraddr);
	int i=0;
	int idx=0;
	int m1,m2,m3,m4;
	char *buf3=stringToBinary(buf);
	msglen = strlen(buf3);
	printf("msglen => %d\n",msglen);
	printf("buf3:%s\n",buf3);
	
	for(i=0; i< ceil(msglen/3); i++){
		printf("i:%d  	",i);
		char buf2[10];
		idx=3*i;
		if(i==ceil(msglen/3)-1){
			m1=(-48+buf3[idx]);
			m2=(-48+buf3[idx+1]);
			m3=(-48+buf3[idx+2]);
			m4=0;
			printf("last chunk have:%d  ",msglen-idx);
			if((msglen-idx)==1){ //m=2,r=3
				m1=(-48+buf3[idx]);
				m2=0;
				buf2[0]=(char)(48+m1^m2);				//p1=m1^m2^m4
				buf2[1]=(char)(48+m1);					//p2=m1^m3^m4
				buf2[2]=(char)(48+m1);					//m1
				buf2[3]=(char)(48+m2);					//p4=m2^m3^m4
				buf2[4]=(char)(48+m2);					//m2
				
				m3=0;
				m4=0;
				buf2[5]=(char)(48+m3);//all other
				buf2[6]=(char)(48+m4);
				//buf2[7]='0';
			}else if((msglen-idx)==2){//m=3,r=3
				m1=(-48+buf3[idx]);
				m2=(-48+buf3[idx+1]);
				m3=0;
				buf2[0]=(char)(48+m1^m2);				//p1=m1^m2^m4
				buf2[1]=(char)(48+m1^m3);				//p2=m1^m3^m4
				buf2[2]=(char)(48+m1);					//m1
				buf2[3]=(char)(48+m2^m3);				//p4=m2^m3^m4
				buf2[4]=(char)(48+m2);					//m2
				buf2[5]=(char)(48+m3);					//m3
				
				m4=0;
				buf2[6]=(char)(48+m4);//all other
				//buf2[7]='0';
			}else{
				m1=(-48+buf3[idx]);
				m2=(-48+buf3[idx+1]);
				m3=(-48+buf3[idx+2]);
				m4=0;
				buf2[0]=(char)(48+m1^m2^m4);				//p1=m1^m2^m4
				buf2[1]=(char)(48+m1^m3^m4);				//p2=m1^m3^m4
				buf2[2]=(char)(48+m1);					//m1
				buf2[3]=(char)(48+m2^m3^m4);				//p4=m2^m3^m4
				buf2[4]=(char)(48+m2);					//m2
				buf2[5]=(char)(48+m3);					//m3
				buf2[6]=(char)(48+m4);					//m4
				
				//buf2[7]='0';//all other
			}
		}else{
			m1=(-48+buf3[idx]);
			m2=(-48+buf3[idx+1]);
			m3=(-48+buf3[idx+2]);
			m4=1;
			buf2[0]=(char)(48+m1^m2^m4);				//p1=m1^m2^m4
			buf2[1]=(char)(48+m1^m3^m4);				//p2=m1^m3^m4
			buf2[2]=(char)(48+m1);					//m1
			buf2[3]=(char)(48+m2^m3^m4);				//p4=m2^m3^m4
			buf2[4]=(char)(48+m2);					//m2
			buf2[5]=(char)(48+m3);					//m3
			buf2[6]=(char)(48+m4);					//m4
			//buf2[7]='\\';
			//buf2[8]='0';
		}
		printf("buf2:%s, m1m2m3m4=%d%d%d%d\n",buf2,m1,m2,m3,m4);
		n = sendto(sockfd, buf2, strlen(buf2), 0, (struct sockaddr *) &serveraddr, serverlen);
    	if (n < 0) error("ERROR in sendto");
	}
	
	char buf4[1]="\n";
	n = sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *) &serveraddr, serverlen);
    if (n < 0) error("ERROR in sendto");
	
	printf("\n\n");
    return 0;
}
