#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>
#include<math.h>
#include<unistd.h>

#include<sys/types.h>          /* See NOTES */
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>

int main(int argc, char * argv[]){

    int sal,sr;
    int optval;

    char serverip[] = "10.12.12.12";

    sr = socket(AF_INET, SOCK_DGRAM,0);

    struct sockaddr_in sa;
    bzero((char *)&sa,sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = inet_addr(serverip);
    sa.sin_port = htons(8080);
	sal = sizeof(sa);
    // int n = sentto();
    close(sr);
    printf("hi\n");
    return 0;
}