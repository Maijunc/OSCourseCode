#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SERVER_PORT 8000
#define LENGTH_OF_LISTEN_QUEUE 20
#define BUFFER_SIZE 1024
#define FILE_NAME_MAX_SIZE 512 //文件的最大容量

int main()
{
    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr)); //将结构体清零
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(SERVER_PORT); //设置监听端口

    // 创建socket，若成功，返回socket描述符
    int server_socket_fd = socket(PF_INET, SOCK_STREAM, 0);
    if(server_socket_fd < 0)
    {
        perror("Create Socket Failed:");
        exit(1);
    }
    int opt = 1;
    // 允许地址重用
    setsockopt(server_socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // 绑定socket和socket地址结构
    if((bind(server_socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1))
    {
        perror("Server Bind Failed:");
        exit(1);
    }

    // socket监听
    if((listen(server_socket_fd, LENGTH_OF_LISTEN_QUEUE)) == -1)
    {
        perror("Server Listen Failed:");
        exit(1);
    }

    while(1)
    {
        // 定义客户端的socket地址结构
        struct sockaddr_in client_addr;
        socklen_t client_addr_length = sizeof(client_addr);

        // 接收连接请求，返回一个新的socket描述符，这个新socket用于同连接的客户端通信
        // accept函数会把连接到的客户端信息写到client_addr中
        int new_server_socket_fd = accept(server_socket_fd, (struct sockaddr*)&client_addr, &client_addr_length);
        if(new_server_socket_fd < 0)
        {
            perror("Server Accept Failed:");
            break;
        }

        // 获取Client端传来的文件名
        // recv函数接收数据到缓冲区buffer中
        char buffer[BUFFER_SIZE];
        bzero(buffer, BUFFER_SIZE);
        if(recv(new_server_socket_fd, buffer, BUFFER_SIZE, 0) < 0)
        {
            perror("Server Recieve Data Failed:");
            break;
        }

        // 然后从buffer拷贝到file_name中
        char file_name[FILE_NAME_MAX_SIZE + 1];
        bzero(file_name, sizeof(file_name));
        strncpy(file_name, buffer, strlen(buffer) > FILE_NAME_MAX_SIZE ? FILE_NAME_MAX_SIZE : strlen(buffer));
        printf("%s\n", file_name);

        // 打开文件并读取文件数据
        FILE *fp = fopen(file_name, "r");
        if(fp == NULL)
            printf("File:%s Not Found\n", file_name);
        else
        {
            bzero(buffer, BUFFER_SIZE);
            int len = 0;
            // 每读取一段数据，便将其发送给客户端，循环直到文件读完为止
            while((len = fread(buffer, sizeof(char), BUFFER_SIZE, fp)) > 0)
            {
                if(send(new_server_socket_fd, buffer, len, 0) < 0)
                {
                    printf("Send File:%s Failed.\n", file_name);
                    break;
                }
                bzero(buffer, BUFFER_SIZE);
            }

            //关闭文件
            fclose(fp);
            printf("File:%s Transfer Successful!\n", file_name);
        }
        // 关闭与客户端的连接
        close(new_server_socket_fd);
    }
    //关闭监听用的socket
    close(server_socket_fd);
    return 0;
}
