#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define SERVER_PORT 8080
#define SERVER_IP "172.96.242.88"
#define BUFF_LEN 1024

void handle_udp_msg(int fd){
    char recv_buf[BUFF_LEN],send_buf[BUFF_LEN];  //接收缓冲区，1024字节
    socklen_t len;
    int count;
    struct sockaddr_in client_addr;  //clent_addr用于记录发送方的地址信息
    while(1){
        memset(recv_buf, '\0', sizeof(recv_buf));
		memset(send_buf, '\0', sizeof(send_buf));

        count = recvfrom(fd, recv_buf, BUFF_LEN, 0, (struct sockaddr*)&client_addr, &len);  //recvfrom是拥塞函数，没有数据就一直拥塞
        if(count == -1){
            printf("recieve data fail!\n");
            return;
        }
        printf("client:%s\n",recv_buf);  //打印client发过来的信息

        printf("enter your message: ");
		fflush(stdout);
		ssize_t size = read(0, send_buf, sizeof(send_buf) - 1);
		if(size > 0){
			send_buf[size - 1] = '\0';
		}
		 else if(size == 0){
			printf("read is done...\n");
			break;
		}
		else{
			perror("read");
			return;
		}
        if(strncmp(send_buf,"exit",4)==0){
                return 0;
        }
        sendto(fd, send_buf, BUFF_LEN, 0, (struct sockaddr*)&client_addr, len);  //发送信息给client，注意使用了client_addr结构体指针

    }
}


/*
    server:
            socket-->bind-->recvfrom-->sendto-->close
*/

int main(int argc, char* argv[])
{
    int server_fd, ret;
    struct sockaddr_in ser_addr; 

    server_fd = socket(AF_INET, SOCK_DGRAM, 0); //AF_INET:IPV4;SOCK_DGRAM:UDP
    if(server_fd < 0){
        printf("create socket fail!\n");
        return -1;
    }

    memset(&ser_addr, 0, sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_addr.s_addr =htonl(INADDR_ANY) ; //IP地址，需要进行网络序转换，INADDR_ANY：本地地址
    ser_addr.sin_port = htons(SERVER_PORT);  //端口号，需要网络序转换

    ret = bind(server_fd, (struct sockaddr*)&ser_addr, sizeof(ser_addr));
    if(ret < 0)
    {
        printf("socket bind fail!\n");
        return -1;
    }

    handle_udp_msg(server_fd);   //处理接收到的数据

    close(server_fd);
    return 0;
}
