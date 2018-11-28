#include<stdio.h>
#include<string.h>
#include<malloc.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<termios.h>
#include<math.h>

#define MAX_BUFFER_SIZE (512)

int fd,s;

int open_serial(){
    fd = open("/dev/ttyS1",O_RDWR);
    if(fd == -1){
        perror("open serial porterror!\n");
        return -1;
    }

    printf("open /dev/ttyS1.\n");
    return 0;
}

int main(){
    char buf[MAX_BUFFER_SIZE],tmp[5];
    memset(tmp,'\0',5);
    int flag_close,retv;
    struct termios opt;

    retv =open_serial();
    if(retv <0){
        printf("Open serrial porterror!\n");
        return -1;
    }

    tcgetattr(fd,&opt);
    cfmakeraw(&opt);
    cfsetispeed(&opt,B9600);
    cfsetospeed(&opt,B9600);
    tcsetattr(fd, TCSANOW,&opt);
    printf("Ready for receiving data...\n");

    pid_t p = fork();
    if(p==0){
        while(1){
            memset(buf,'\0',MAX_BUFFER_SIZE);
            //int i = 0;
            //while((retv = read(fd, tmp, 1)) > 0 && tmp[0]!='\n')
            //    buf[i++] = tmp[0];
            if(retv = read(fd,buf,MAX_BUFFER_SIZE-1)>0){
                printf( "message read from client : %s\n",buf);
                ssize_t size = write(fd, buf,strlen(buf));
            }
            //ssleep(1);
        }
    }
    else if(p>0){
        //printf("%d",p);
        char buf_[10];
        do{
            memset(buf_,'\0',10);
            ssize_t size = read(0,buf_,10);
        }while(strncmp(buf_,"exit",4)!=0);
        return 0;
    }

    flag_close =close(fd);
    if(flag_close ==-1)
        printf("Close the device failure!\n");

    return 0;
}