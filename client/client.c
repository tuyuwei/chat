#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <stdint.h>

#define C_OK 0
#define C_ERR -1

#define MYPORT  8888
#define BUFFER_SIZE 1024

static size_t frameProtocolPack(char *const msg, char *const sendBuffer, size_t size) {
    uint16_t bodyLength = htons(size);
    memcpy(sendBuffer, &bodyLength, 2);
    memcpy(sendBuffer+2, msg, size);
    return size + 2;
}

static int frameProtocolUnpack(char *const msg) {
    char *len = malloc(sizeof(char)*2);
    memcpy(len, msg, 1);
    memcpy(len+1, msg+1, 1);
    uint16_t slen = ntohs((uint16_t)len);
    return slen;
}

int main()
{
    ///定义sockfd
    int fd = socket(AF_INET,SOCK_STREAM, 0);

    ///定义sockaddr_in
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(MYPORT);  ///服务器端口
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");  ///服务器ip

    ///连接服务器，成功返回0，错误返回-1
    if (connect(fd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("connect");
        exit(1);
    }
    unsigned short int msgLen;
    char sendBuffer[BUFFER_SIZE], inputBuffer[BUFFER_SIZE];
    char recvBuffer[BUFFER_SIZE];
    while (fgets(inputBuffer, BUFFER_SIZE, stdin) != NULL) {
        size_t sendLength = frameProtocolPack(inputBuffer, sendBuffer, strlen(inputBuffer) - 1);
        send(fd, sendBuffer, sendLength, 0); //发送
        if(strcmp(sendBuffer,"exit\n")==0)
            break;
        /*recv(fd, recvBuffer, strlen(recvBuffer),0); ///接收
        fputs(recvBuffer, stdout);*/
        
        memset(sendBuffer, 0, strlen(sendBuffer));
        memset(recvBuffer, 0, strlen(recvBuffer));
    }

    close(fd);
    return 0;
}