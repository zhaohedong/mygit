/*
#    Copyright By Schips, All Rights Reserved
#    https://gitee.com/schips/
#
#    File Name:  server.c
#    Created  :  Sat 25 Mar 2020 14:43:39 PM CST
*/

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

typedef struct _info {
    char name[10];
    char text[54];
}info;

int main(int argc, char *argv[])
{
    int my_socket;
    unsigned int len;
    int ret, i, j;

    // 创建套接字
    my_socket = socket(AF_INET, SOCK_STREAM, 0); // IPV4, TCP socket
    if(my_socket == -1) { perror("Socket"); }
    printf("Creat a socket :[%d]\n", my_socket);

    // 用于接收消息
    info buf ={0};

    // 指定地址
    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;  // 地址协议族
        addr.sin_addr.s_addr = inet_addr("127.0.0.1");   //指定 IP地址
        addr.sin_port = htons(12345); //指定端口号

    int set = 1;
    int get = 0;
    int getlen = 0;

    // 服务器 绑定
    bind(my_socket, (struct sockaddr *)&addr, sizeof(addr));

    ret = listen(my_socket, 10);
    if(-1 == ret) { perror("listen"); }
    printf("Listening\n");


    int connect_sockets[100] = {0}; // 我们规定，为 0 的成员为无效socket
    int connected_cnt = 0;

    //struct sockaddr_in new = {0};
    //int new_addr_size = {0};

    fd_set read_sets;

    int max_fd = my_socket; // 一开始时，只有 一个新的 文件描述：my_socket ，所以它是最大的

    while(1) // 在循环中等待连接请求
    {
        FD_ZERO(&read_sets); // 每次都需要初始化
        FD_SET(my_socket, &read_sets); // 添加 要监听的 socket

        // 添加 之后经过 connect 过来的 套接字数组(一般在第一次循环时是空的)
        for( i = 0; i < connected_cnt; i++)
        {
            if(connect_sockets[i])
            {
                FD_SET(connect_sockets[i], &read_sets); // 添加经过accept保存下来，需要进行读响应的套接字到集合中
            }
        }

        // 设置监听超时时间
        // timeout.tv_sec  = 2;
        // timeout.tv_usec = 0;
        ret = select(max_fd + 1, &read_sets, NULL, NULL, NULL);
        // 判断返回值
        switch (ret) {
            case 0 :
                printf("Time out.\n"); // 监听超时
                break;
            case -1 :
                printf("Err occurs.\n"); // 监听错误
                break;
            default :
                if(FD_ISSET(my_socket, &read_sets)) //这个是原的被动socket，如果是它，则 意味着有新的连接进来了
                {
                    connect_sockets[connected_cnt] = accept(my_socket, NULL, NULL);
                    max_fd = connect_sockets[connected_cnt];
                    printf("New socket is %d, max_fd is %d\n", connect_sockets[connected_cnt], max_fd);
                    connected_cnt ++;
                    printf("Now we has [%d] connecter\n", connected_cnt);
                }else{ // 如果不是 被动socket，那么就意味着是 现有的连接 有消息发来（我们有数据可读）
                    printf("New message came in.\n");

                    // 求出是那个文件描述符可读
                    for(i = 0; i < connected_cnt; i++)
                    {
                        if(FD_ISSET(connect_sockets[i], &read_sets)) 
                        {
                            break;
                        }
                    }
		            // // printf("i is %d connected_cnt is %d\n", i, connected_cnt);
                    if( i >= connected_cnt) { continue; }

                    // printf("Socket [%d] send to server.\n", connect_sockets[i]);

                    // 接收消息
                    ret = recv(connect_sockets[i], &buf, sizeof(buf), 0);
                    if( ret <= 0 )
                    {
                        // 远程客户端断开处理（如果不处理，会导致服务器也断开）
                        printf("[%d]/[%d] Client [%d] disconnected.\n", i+1, connected_cnt, connect_sockets[i]);
                        close(connect_sockets[i]);
                        // 我们需要将对应的客户端从数组中移除  且 连接数 -1 (移除的方法： 数组成员前移覆盖)
                        for (j = i; j < connected_cnt - 1; ++j)
                        {
                            connect_sockets[j] = connect_sockets[ j + 1];
                        }
                        connected_cnt --;
                        continue;
                    }

                    // 打印消息
                    printf("recived message ret[%d] [%s] : %s\n", ret, buf.name, buf.text);
                    // 回复消息
                    sprintf(buf.name, "Server");
                    sprintf(buf.text, "Had recvied your[%d] message", connect_sockets[i]);
                    //sendto(my_socket, &buf, sizeof(buf), 0, NULL, NULL);
                    ret = send(connect_sockets[i], &buf, sizeof(buf), 0);
                    printf("send message ret[%d] [%s] : %s\n", ret, buf.name, buf.text);
                }
                break;
        }
        printf("while loop\n");
    }

    // 关闭连接
    //shutdown(my_socket, SHUT_RDWR); perror("shutdown");
    for(i = 0; i < connected_cnt; i++)
    {
        close(connect_sockets[i]); perror("close");
    }
    return close(my_socket);
}

