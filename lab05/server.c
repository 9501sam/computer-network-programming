#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#define MAXLINE  64

void str_echo(int sockfd)
{
    int n;
    char buf[MAXLINE];
again:
    while ( (n = read(sockfd, buf, MAXLINE)))
        write(sockfd, buf, n);
    if (n < 0 && errno == EINTR)
        goto again;
    else if (n < 0)
        printf("str_echo: read error");
}

void sig_chld(int signo)
{
    pid_t pid;
    int stat;
    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0)
        printf("child %d terminated\n", pid);// printf() isn't suitable for use here
    return;
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr,"usage %s port\n", argv[0]);
        exit(0);
    }
    int portno = atoi(argv[1]);
    int listenfd, connfd;
    pid_t childpid;
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(portno);
    bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
    listen(listenfd, 5);
    while (1) {
        if (signal(SIGCHLD, sig_chld) == SIG_ERR) {
            fprintf(stderr, "can't catch SIGCHLD \n");
            exit(0);
        }
        clilen = sizeof(cliaddr);
        if ((connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &clilen)) < 0) {
            if (errno == EINTR) {
                continue; // back to for()
            } else {
                fprintf(stderr, "accept error\n");
                exit(0);
            }
        }

        if ((childpid = fork()) == 0) { // child
            close(listenfd); // close listening socket
            while (1) {
                char buf[MAXLINE];
                time_t rawtime;
                struct tm *timeinfo;
                time(&rawtime);
                timeinfo = localtime(&rawtime);

                sprintf(buf, "%s", asctime(timeinfo));
                write(connfd, buf, strlen(buf));
                sleep(1);
            }
            close(connfd);
            exit(0);
        }
        close(connfd); // parent closes connected socket
    }
    return 0;
}
