#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/poll.h>
#define MAX_SOCKET 5

typedef struct _info {
    char name[10];
    char text[54];
}info;

int main(int argc, char *argv[])
{
    info send_buf;
    info recv_buf;
    int listen_socket,newsk;
    int connected_sockets[MAX_SOCKET];
    int connected_cnt = 0;
    int i;

    // 1 创建一个套接字，用于网络通信
    listen_socket = socket(PF_INET, SOCK_STREAM, 0);
    if (listen_socket == -1)
    {
        perror("socket");
        return -1;
    }

    // 2 绑定服务的IP与端口
    struct sockaddr_in ser_addr;
    ser_addr.sin_family  = PF_INET;
    ser_addr.sin_port  = htons (12345) ;
    ser_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    int ret =  bind(listen_socket, (struct sockaddr *)&ser_addr,sizeof(ser_addr));

    if (ret == -1)
    {
        perror("bind");
        return -1;
    }

    // 3 监听端口的数据
    ret = listen(listen_socket,MAX_SOCKET);
    if (ret == -1)
    {
        perror("listen");
        return -1;
    }

    // 监听的总套接字数组
    struct pollfd  pollfds[MAX_SOCKET+1];
    int poll_ret;

    // 主套接字信息
    pollfds[0].fd  = listen_socket;
    pollfds[0].events = POLLIN|POLLPRI;  //设置为 任意优先级可读 事件监听

    while(1)
    {
        printf("Poll all fds\n");
        poll_ret =  poll(pollfds, connected_cnt + 1, -1);  //-1 阻塞模式进行监听
        printf("ret = %d\n", poll_ret);  //返回值为1 ：  正确监听到变化
        printf("Need to handld %d fd(s)\n", poll_ret);  //返回值为1 ：  正确监听到变化
        if(poll_ret == 0)
        {
            printf("timeout!\n");   //监听超时
        } else if( poll_ret == -1 )
        {
            perror("err!");         //监听出错
        }

        //正确监听到变化
        if(pollfds[0].revents  & POLLIN || pollfds[0].revents  & POLLPRI)  //如果新客户端连接
        {
            // 响应用户连接
            connected_sockets[connected_cnt] = accept(listen_socket,NULL,NULL); //返回 新的连接响应的套接字
            if(connected_sockets[connected_cnt] == -1)
            {
                perror("accept");
                return -1;
            }
            printf("new accept from %d!\n", connected_sockets[connected_cnt]);

            //更新客户端套接字集合
            pollfds[connected_cnt+1].fd     = connected_sockets[connected_cnt];
            pollfds[connected_cnt+1].events = POLLIN|POLLPRI;  //任意优先级可读
            connected_cnt++;
        } else {
            printf("new read/write !\n");
            for(i = 1; i < MAX_SOCKET + 1; i++) {
                if (pollfds[i].revents & POLLIN) {    //客户端发生了数据可读
                    //4   接收与发送数据
                    memset(&recv_buf, 0, sizeof(recv_buf));
                    ret =  recv(connected_sockets[i-1], &recv_buf, sizeof(recv_buf), 0);
                    if (ret == -1)
                    {
                        perror("recv");
                        return -1;
                    }

                    if(errno == EINTR) continue;
                    if(ret == 0 )   //客户端断开
                    {
                        printf("%d disconnectded\n", connected_sockets[i-1]);
                        close(connected_sockets[i-1]);

                        connected_sockets[i-1]  = -1;
                        memset(&pollfds[i] , 0, sizeof(struct pollfd )); //清空断开套接字监听信息
                        continue;
                    }
                    printf("[%d],[%s] : %s\n", connected_sockets[i-1], recv_buf.name, recv_buf.text);
                    sprintf(send_buf.name, "Server");
                    sprintf(send_buf.text, "Had recvied your[%d] message", connected_sockets[i-1]);
                    //sendto(my_socket, &buf, sizeof(buf), 0, NULL, NULL);
                    send(connected_sockets[i-1], &send_buf, sizeof(send_buf), 0);
                }
            }
        }
        sleep(2);
    }

    // 5 关闭套接字
    for(i = 0; i < connected_cnt; i++)
    {
        close(connected_sockets[i]);
    }

    return 0;
}
