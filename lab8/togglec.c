#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>

#define MAXLINE 1024

int open_client_sock(char *hostname, int port);

int main(int argc, char **argv)
{
    int client_sock, port;
    char *host, buf[MAXLINE];

    if(argc != 3) {
        fprintf(stderr, "usage:%s<host><port>\n",argv[0]);
        exit(1);
    }
    host = argv[1];
    port = atoi(argv[2]); //转数字

    client_sock = open_client_sock(host, port); //与服务器建立连接

    while(fgets(buf, MAXLINE, stdin) != NULL) {
        send(client_sock, buf, strlen(buf), 0);
        recv(client_sock, buf, MAXLINE, 0);
        fputs(buf, stdout);
    }
    close(client_sock);
    exit(0);
}

int open_client_sock(char *hostname, int port)
{
    int client_sock;
    struct hostent *hp;
    struct sockaddr_in serveraddr;

    // AF_INET 表示基于Internet通信
    // SOCK_STREAM 表示这个套接字是Internet连接的一个端点，采用TCP协议（如果采用UDP，则参数type的值为SOCK_DGRAM)
    if((client_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        return -1;  /* 如果出错，返回-1 */

    /* 填写服务器的IP地址和端口 */
    if((hp = gethostbyname(hostname)) == NULL)
        return -2;  /* 如果出错，返回-2 */
    bzero((char *)&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    bcopy((char *)hp->h_addr_list[0], (char *)&serveraddr.sin_addr.s_addr, hp->h_length);
    serveraddr.sin_port = htons(port);
    
    /* 与服务器建立连接 */ /* 我猜是握手 */
    if(connect(client_sock, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
        return -1;
    return client_sock;
}
