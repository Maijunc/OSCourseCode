#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <ctype.h>

#define MAXLINE 1024
#define LISTENQ 1024

void toggle(int conn_sock);
int open_listen_sock(int port);

int main(int argc, char **argv)
{
    int listen_sock, conn_sock, port, clientlen;
    struct sockaddr_in clientaddr;
    struct hostent *hp;
    char *haddrp;
    if(argc != 2) {
        fprintf(stderr, "usage:%s<port>\n", argv[0]);
        exit(1);
    }
    port = atoi(argv[1]);

    listen_sock = open_listen_sock(port);
    while(1)
    {
        clientlen = sizeof(clientaddr);
        conn_sock = accept(listen_sock, (struct sockaddr *)&clientaddr, (socklen_t *)&clientlen);

        hp = gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr, sizeof(clientaddr.sin_addr.s_addr), AF_INET);
        haddrp = inet_ntoa(clientaddr.sin_addr);
        printf("server connected to %s(%s)\n", hp->h_name, haddrp);

        toggle(conn_sock);
        close(conn_sock);
    }
    exit(0);
}

void toggle(int conn_sock)
{
    size_t n; int i;
    char buf[MAXLINE];

    while((n = recv(conn_sock, buf, MAXLINE, 0)) > 0)
    {
        printf("toggle server received %d bytes\n", n);

        for(i = 0; i < n; i++)
            if(isupper(buf[i]))
                buf[i] = tolower(buf[i]);
            else if(islower(buf[i]))
                buf[i] = toupper(buf[i]);

        send(conn_sock, buf, n, 0);
    }
}

int open_listen_sock(int port)
{
    int listen_sock, optval = 1;
    struct sockaddr_in serveraddr;

    /* Create a socket descriptor */
    if((listen_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        return -1;

    /* Eliminates "Address already in use" error from bind */
    if(setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval, sizeof(int)) < 0)
        return -1;

    /* listen_sock is an endpoint for all requests to port received from any IP address for this host */
    bzero((char *)&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons((unsigned short)port);
    if(bind(listen_sock, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
        return -1;

    /* convert the sock to a listening socket ready to accept connection requests */
    if(listen(listen_sock, LISTENQ) < 0)
        return -1;
    return listen_sock;
}
