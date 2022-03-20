/*
#    Copyright By Schips, All Rights Reserved
#    https://gitee.com/schips/
#
#    File Name:  client.c
#    Created  :  Sat 25 Mar 2020 14:44:19 PM CST
*/

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
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
    int ret, i = 0;

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

    int new_socket;
    struct sockaddr_in new = {0};
    int new_addr_size;

    ret = connect(my_socket, (struct sockaddr *)(&addr), sizeof(struct sockaddr_in));
    if(-1 == ret) { perror("connect"); }
    printf("connected\n");

    for (i = 0; i < 5; ++i)
    {
        sleep(2);

        // 回复消息
        sprintf(buf.name, "Client");
        sprintf(buf.text, "Hello tcp text [%d].", i);
        //sendto(my_socket, &buf, sizeof(buf), 0, NULL, NULL);
        int ret = send(my_socket, &buf, sizeof(buf), 0);
        perror("sendto");
	    printf("sendto result is %d\n", ret);

        // 接收并打印消息
        //recvfrom(my_socket, &buf, sizeof(buf), 0, NULL, NULL);
        recv(my_socket, &buf, sizeof(buf), 0);
        perror("recvfrom");
	    printf("recvfrom result is %d\n", ret);
        printf("[%s] : %s i[%d]\n", buf.name, buf.text, i);
    }

    // 关闭连接
    //shutdown(my_socket, SHUT_RDWR); perror("shutdown");
    //printf("%d\n", errno);
    return close(my_socket); perror("close");
    printf("%d\n", errno);
    return errno;
}