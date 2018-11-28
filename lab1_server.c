#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <netinet/in.h>
 
const int port = 8080;
const char* ip = "172.96.242.88";
char recv_buf[1024];
char send_buf[1024];
 
int main(){
	int socket_s = socket(AF_INET, SOCK_STREAM, 0); 
	if(socket_s < 0){
	//创建失败
		perror("socket");
		return 1;
	}
 
	//绑定信息，即命名socket 
	struct sockaddr_in addr; 
	addr.sin_family = AF_INET; 
	addr.sin_port = htons(port); 
	addr.sin_addr.s_addr = inet_addr(ip); 

    //套接字绑定
	if(bind(socket_s, (struct sockaddr*)&addr, sizeof(addr)) < 0) {	            
	    perror("bind error"); 
	    return 2; 	   
	} 
 
	//监听socket
	if(listen(socket_s, 5) < 0){
		//监听失败
		perror("listen");
		return 3;
	}
 
	struct sockaddr_in client;
	socklen_t client_len;

	int accept_fd = accept(socket_s, (struct sockaddr*)&client, &client_len);
	if(accept_fd < 0){
		perror("accept");
		return 4;
	}
	else
		printf("connected with ip: %s  and port: %d\n", inet_ntop(AF_INET,&client.sin_addr, recv_buf, 1024), ntohs(client.sin_port));

	while(1){
		memset(recv_buf, '\0', sizeof(recv_buf));
		memset(send_buf, '\0', sizeof(send_buf));

		//服务器接受信息
		ssize_t read_size = recv(accept_fd, recv_buf, sizeof(recv_buf),0);
		if(read_size > 0){
			printf("client#: %s\n", recv_buf);
		}
		else if(read_size == 0){
			printf("read is done...\n");
			break;
		}
		else{
			perror("read");
			break;
		}

		//服务器发送信息
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
			return 4;
		}
		send(accept_fd, send_buf, strlen(send_buf),0);

		//是否达到退出条件
		char *exit = "exit";
		if(strcmp(exit,recv_buf)==0){
			break;
		}
	}
	close(socket_s);
	return 0;
}
