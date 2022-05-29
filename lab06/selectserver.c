#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/select.h>

#define PORT "512345" // �ڭ̥��b listen �� port

// ���o sockaddr�AIPv4 �� IPv6�G
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(void)
{
    fd_set master; // master file descriptor �M��
    fd_set read_fds; // �� select() �Ϊ��Ȯ� file descriptor �M��
    int fdmax; // �̤j�� file descriptor �ƥ�

    int listener; // listening socket descriptor
    int newfd; // �s������ accept() socket descriptor
    struct sockaddr_storage remoteaddr; // client address
    socklen_t addrlen;

    char buf[256]; // �x�s client ��ƪ��w�İ�
    int nbytes;

    char remoteIP[INET6_ADDRSTRLEN];

    int yes=1; // �ѩ��U�� setsockopt() �]�w SO_REUSEADDR
    int i, j, rv;

    struct addrinfo hints, *ai, *p;

    FD_ZERO(&master); // �M�� master �P temp sets
    FD_ZERO(&read_fds);

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((rv = getaddrinfo(NULL, PORT, &hints, &ai)) != 0) {
        fprintf(stderr, "selectserver: %s\n", gai_strerror(rv));
        exit(1);
    }

    for(p = ai; p != NULL; p = p->ai_next) {
        listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (listener < 0) {
            continue;
        }

        setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

        if (bind(listener, p->ai_addr, p->ai_addrlen) < 0) {
            close(listener);
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "selectserver: failed to bind\n");
        exit(2);
    }
    freeaddrinfo(ai); // all done with this

    // listen
    if (listen(listener, 10) == -1) {
        perror("listen");
        exit(3);
    }

    // �N listener �s�W�� master set
    FD_SET(listener, &master);

    // ����l�̤ܳj�� file descriptor
    fdmax = listener; // �즹����A�N�O���F

    // �D�n�j��
    for( ; ; ) {
        read_fds = master; // �ƻs master

        if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1) {
            perror("select");
            exit(4);
        }

        // �b�{�s���s�u���M��ݭnŪ�������
        for(i = 0; i <= fdmax; i++) {
            if (FD_ISSET(i, &read_fds)) { // �ڭ̧��@�ӡI�I
                if (i == listener) {
                    // handle new connections
                    addrlen = sizeof remoteaddr;
                    newfd = accept(listener,
                            (struct sockaddr *)&remoteaddr,
                            &addrlen);

                    if (newfd == -1) {
                        perror("accept");
                    } else {
                        FD_SET(newfd, &master); // �s�W�� master set
                        if (newfd > fdmax) { // ����l�̤ܳj�� fd
                            fdmax = newfd;
                        }
                        printf("selectserver: new connection from %s on "
                                "socket %d\n",
                                inet_ntop(remoteaddr.ss_family,
                                    get_in_addr((struct sockaddr*)&remoteaddr),
                                    remoteIP, INET6_ADDRSTRLEN),
                                newfd);
                    }

                } else {
                    // �B�z�Ӧ� client �����
                    if ((nbytes = recv(i, buf, sizeof buf, 0)) <= 0) {
                        // got error or connection closed by client
                        if (nbytes == 0) {
                            // �����s�u
                            printf("selectserver: socket %d hung up\n", i);
                        } else {
                            perror("recv");
                        }
                        close(i); // bye!
                        FD_CLR(i, &master); // �q master set ������

                    } else {
                        // �ڭ̱q client ����@�Ǹ��
                        for(j = 0; j <= fdmax; j++) {
                            // �e���j�a�I
                            if (FD_ISSET(j, &master)) {
                                // ���ΰe�� listener ��ڭ̦ۤv
                                if (j != listener && j != i) {
                                    if (send(j, buf, nbytes, 0) == -1) {
                                        perror("send");
                                    }
                                }
                            }
                        }
                    }
                } // END handle data from client
            } // END got new incoming connection
        } // END looping through file descriptors
    } // END for( ; ; )--and you thought it would never end!

    return 0;
    }
