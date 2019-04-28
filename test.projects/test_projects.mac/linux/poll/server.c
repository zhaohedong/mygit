//server.c
#include<stdio.h>  
#include<stdlib.h>  
#include<string.h>  
#include<errno.h>  
#include<sys/types.h>  
#include<sys/socket.h>  
#include<netinet/in.h>  
#include<poll.h>

#define DEFAULT_PORT 8000  
#define MAXLINE 4096  
#define IN_FILES 1 //我们这里poll机制只监听一个文件描述符

void socket_accept(int socket_fd) {
    int connect_fd;  
    char    buff[MAXLINE];  
    //阻塞直到有客户端连接，不然浪费CPU资源,但是显然这里通过poll机制是有可读的信息才会进入本函数，因此应该是不会阻塞的。
    if ((connect_fd = accept(socket_fd, (struct sockaddr*)NULL, NULL)) == -1) { 
        printf("accept socket error: %s(errno: %d)",strerror(errno),errno);
    }

    int n = recv(connect_fd, buff, MAXLINE, 0);
    buff[n] = '\0';
    printf("recv msg from client: %s\n", buff);

    if (send(connect_fd, "Hello,you are connected!\n", 26, 0) == -1) {
        perror("send error");
    }

    close(connect_fd);
}

int main(int argc, char** argv)  
{  
    int    socket_fd, connect_fd;  
    struct sockaddr_in     servaddr;  
    struct pollfd fds[IN_FILES];

    //初始化Socket  
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {  
        printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);  
        exit(0);  
    }

    //初始化  
    memset(&servaddr, 0, sizeof(servaddr));  
    servaddr.sin_family = AF_INET;  
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);//IP地址设置成INADDR_ANY,让系统自动获取本机的IP地址。  
    servaddr.sin_port = htons(DEFAULT_PORT);//设置的端口为DEFAULT_PORT  
  
    //将本地地址绑定到所创建的套接字上  
    if (bind(socket_fd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1) {  
        printf("bind socket error: %s(errno: %d)\n",strerror(errno),errno);  
        exit(0);  
    }  

    //开始监听是否有客户端连接  
    if (listen(socket_fd, 10) == -1) {  
        printf("listen socket error: %s(errno: %d)\n",strerror(errno),errno);  
        exit(0);  
    }  


    fds[0].fd = socket_fd;
    fds[0].events = POLLIN;
    int result;
    while(1){
        printf("begin to poll()\n");
        result = poll(fds, IN_FILES, 3000);  //It may block with timeout
        if (result == 0) {
            printf("Poll timeout\n");
        } else if (result > 0) {
            printf("Poll successfully\n");
        } else {
            printf("Poll error\n");
        } 

        if (fds[0].revents == POLLIN){ 
            printf("===receive a connetction====\n");
            socket_accept(socket_fd);
        }

        printf("do something else\n");
        sleep(1);
        printf("do something else end\n");
    }

    close(socket_fd);  
}
