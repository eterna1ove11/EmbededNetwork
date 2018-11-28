//客户端
#include <sys/types.h> 
#include <sys/socket.h> 
#include <stdio.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
 
int main() 
{ 
    int client_sockfd; 
    int len; 
    struct sockaddr_in address;//服务器端网络地址结构体 
     int result; 
    char send_buf[1024],recv_buf[1024];
    client_sockfd = socket(AF_INET, SOCK_STREAM, 0);//建立客户端socket 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(8888); 
    len = sizeof(address); 
    result = connect(client_sockfd, (struct sockaddr *)&address, len); 
    if(result == -1) 
    { 
         perror("oops: client2"); 
         exit(1); 
    } 

    char* exit_s = "exit"; 
    while(1){
        memset(send_buf,'\0',1024);
        memset(recv_buf,'\0',1024);
        read(0, send_buf, sizeof(send_buf) - 1);
        for(int i=0;i<1024;i++){
            if(send_buf[i]=='\n')
                send_buf[i]='\0';
        }
        write(client_sockfd, send_buf, sizeof(send_buf)); 
        read(client_sockfd, recv_buf, sizeof(recv_buf));
        printf("str receiverd from server = %s\n", recv_buf); 
    
        if(strcmp(send_buf,exit_s)==0){
            close(client_sockfd);
            return 0;
        }  
    }
    return 0; 
}
