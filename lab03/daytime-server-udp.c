#include <stdio.h> 
#include <sys/types.h> 
#include <netinet/in.h>
#include <sys/socket.h> 
#include <string.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <time.h>
#define PORT 5678

int
main(int argc,char **argv)
{
    char buf[2000];
    int sockfd,len,a;
    struct sockaddr_in servaddr,cliaddr;
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        printf("Error creating socket\n"); 
        exit(0);
    }
    printf("UDP Server Socket Created Successfully.\n"); 
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT); 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Error binding socket."); 
        exit(0);
    }
    printf("UDP Server Socket Binded.\n"); 
    len = sizeof(cliaddr);
    do {
        strcpy(buf," "); 

        if((a = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&cliaddr, &len)) < 0) {
            perror("ERROR"); 
            exit(0);
        }
        printf("From Client : %s",buf); 
        // get time
        time_t my_time;
        struct tm * timeinfo;
        time (&my_time);
        timeinfo = localtime (&my_time);
        sprintf(buf, "%d-%d-%d %d:%d:%d\n",
                timeinfo->tm_year+1900,
                timeinfo->tm_mon+1,
                timeinfo->tm_mday,
                timeinfo->tm_hour,
                timeinfo->tm_min,
                timeinfo->tm_sec);

        if((sendto(sockfd, buf, a, 0, (struct sockaddr *)&cliaddr, len)) < 0) {
            perror("NOTHING SENT"); 
            exit(0);
        }
        printf("Server : \n");
    } while(1); 
    close(sockfd); 
    return 0;
}
