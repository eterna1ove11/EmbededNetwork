#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <netinet/in.h>
 
char buf[1024];
char recv_buf[1024];

char *ip = "127.0.0.1";
int port = 8080;


int main(int argc, char *argv[]){
	  //创建套接字,即创建socket 
	  int socket_c = socket(AF_INET, SOCK_STREAM, 0); 
	  if(socket_c < 0){
		  //创建失败
		  perror("socket");
		  return 1;
	  }
 
	  //绑定信息
	  struct sockaddr_in addr; 
	  addr.sin_family = AF_INET; 
	  addr.sin_port = htons(port); 
	  addr.sin_addr.s_addr = inet_addr(ip); 
 
 
	  //发起连接
	  socklen_t addr_len = sizeof(addr);
	  int connect_fd = connect(socket_c, (struct sockaddr*)&addr, addr_len);
	  if(connect_fd < 0){
		  perror("connect");
		  return 2;
	  }

 
	  while(1){
		  memset(buf, '\0', sizeof(buf));
          memset(recv_buf, '\0', sizeof(recv_buf));
		  printf("client please enter: ");
		  fflush(stdout);
		  ssize_t size = read(0, buf, sizeof(buf) - 1);
		  if(size > 0){
			  buf[size - 1] = '\0';
		  }
		  else if(size == 0){
			  printf("read is done...\n");
			  break;
		  }
		  else{
			  perror("read");
			  return 4;
		  }

          send(socket_c, buf, strlen(buf),0);

          recv(socket_c, recv_buf, sizeof(recv_buf),0);
		  printf("message from server: %s\n", recv_buf);

		  char *exit = "exit";
          if(strcmp(exit,buf)==0){
			close(socket_c);
			break;
		}
	}
	return 0;
}
