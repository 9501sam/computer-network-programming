#include <stdio.h> //printf
#include <string.h> //strlen
#include <sys/socket.h> //socket
#include <arpa/inet.h> //inet_addr
#include <unistd.h>
#include <stdlib.h>

#define STDIN 0

int main(int argc , char *argv[]){
    int sock;
    struct sockaddr_in server;
    char message[256] , server_reply[256];
    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1) {
        printf("Could not create socket");
    }
    puts("Socket created");
    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[2]));
    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0){
        perror("connect failed. Error");
        return 1;
    }
    puts("Connected\n");
    //keep communicating with server
    for(;;){
        puts("Enter message: ");
        memset(message, 0, 256);
        memset(server_reply, 0, 256);
        struct timeval tv;
        fd_set readfds;

        tv.tv_sec = 60;
        tv.tv_usec = 0;

        FD_ZERO(&readfds);
        FD_SET(STDIN, &readfds);

        select(STDIN + 1, &readfds, NULL, NULL, &tv);

        //Send some data
        if (FD_ISSET(STDIN, &readfds)) {
            fgets(message, 256,stdin);
            if(send(sock , message , strlen(message) , 0) < 0) {
                puts("Send failed");
                return 1;
            }
        } else { // timeout
            sprintf(message, "idle message");
            if(send(sock , message , strlen(message) , 0) < 0) {
                puts("Send failed");
                return 1;
            }
            printf("Timeout\n");
        }
        //Receive a reply from the server
        if( recv(sock , server_reply , 256 , 0) < 0) {
            puts("recv failed");
            break;
        }
        printf("Server Reply: %s\n", server_reply);
        server_reply[0]='\0';
    }
    close(sock);
    return 0;
}
