#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#define ADDRESS "127.0.1.1"

int
main()
{
    struct hostent *host;
    struct in_addr addr;

    addr.s_addr = inet_addr(ADDRESS);

    host = gethostbyaddr((const char *)&addr.s_addr,
            sizeof(addr.s_addr), AF_INET);
    if (host == NULL) {
        herror("gethostbyaddr");
        return 1;
    }
    printf("%s\n", host->h_name);

    return 0;
}
