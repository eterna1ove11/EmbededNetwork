#include<stdio.h>
#include<string.h>
#include<malloc.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<termios.h>

#define MAX_BUFFER_SIZE (512)

int fd,flag_close;

int open_serial(){
    fd = open("/dev/ttyS2",O_RDWR);
    if(fd == -1){
        perror("open serial porterror!\n");
        return -1;
    }

    printf("Open serial portsuccess!\n");
    return 0;
}

int main(int argc, char*argv[]){
    char send_buf[MAX_BUFFER_SIZE],read_buf[MAX_BUFFER_SIZE];
    int retv;
    struct termios opt;

    retv =open_serial();
    if(retv <0){
        perror("open serial porterror!\n");
        return -1;
    }
    
    printf("Ready for sending data...\n");

    tcgetattr(fd,&opt);
    cfmakeraw(&opt);

    cfsetispeed(&opt,B9600);
    cfsetospeed(&opt,B9600);

    tcsetattr(fd, TCSANOW,&opt);

    pid_t p = fork();

    while(1){
        memset(send_buf,'\0',MAX_BUFFER_SIZE);
        memset(read_buf,'\0',MAX_BUFFER_SIZE);

        printf("Enter your message:");
        ssize_t size = read(0,send_buf,MAX_BUFFER_SIZE);
        if(size < 0){
            perror("read error!\n");
            return -1;
        }
        int length = strlen(send_buf);
        //printf("size of buf %d\n",length);
        retv = write(fd, send_buf,length);
        
        if(retv == -1){
            perror("Write data error!\n");
            return -1;
        }
        printf("%d characters has been sent!\n",retv);

        size = read(fd,read_buf,MAX_BUFFER_SIZE-1);
        if (size>0){
            printf("read %d characters from server: %s\n",(int)size,read_buf);
        }

        if(strcmp(send_buf,"exit\n")==0){
            close(fd);
            return 0;
        }
    }

    

    return 0;
}