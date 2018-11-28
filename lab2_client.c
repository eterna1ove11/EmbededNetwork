#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>   
#include <string.h>

#define SERVER_PORT 8080
#define BUFF_LEN 1024
#define SERVER_IP "172.96.242.88"


/*
    client:
            socket-->sendto-->revcfrom-->close
*/

int main(int argc, char* argv[]){
    int client_fd;
    struct sockaddr_in ser_addr;

    client_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(client_fd < 0){
        printf("create socket fail!\n");
        return -1;
    }


    memset(&ser_addr, 0, sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_addr.s_addr = inet_addr(SERVER_IP) ; //注意网络序转换
    ser_addr.sin_port = htons(SERVER_PORT);  //注意网络序转换


    socklen_t len;
    struct sockaddr_in src;
    char send_buf[BUFF_LEN],recv_buf[BUFF_LEN];
    len = sizeof(ser_addr);

    while(1){   
        memset(recv_buf, '\0', sizeof(recv_buf));
		memset(send_buf, '\0', sizeof(send_buf));

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
			return 1;
		}
        sendto(client_fd, send_buf, sizeof(send_buf), 0, (struct sockaddr *)&ser_addr, len);
        if(strncmp(send_buf,"exit",4)==0){
                return 0;
            }
     
        socklen_t recv_len;
        recvfrom(client_fd, recv_buf, BUFF_LEN, 0, (struct sockaddr*)&src, &recv_len);  //接收来自server的信息
        printf("server:%s\n",recv_buf);
    }

    close(client_fd);

    return 0;
}
