#ifndef CHAT_T
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <signal.h>

#include <fcntl.h>
#include <sys/shm.h>

#define LISTEN_MAX 10
#define EVENTS_MAX 1000
#define BUFF_MAX 1024


typedef struct chatMsg {
	char data[BUFF_MAX];
	int len, offset;
}chatMsg;

typedef struct chatClient {
	int fd;
	chatMsg *msg;
	int statusCode;
	void (*callBackFunction)(int cfd, void *data);
} chatClient;

void signalHandler(int signo);
int initSocket(int epollFd, int port);
void connectClient(int fd, void *data);
void sendClient(int cfd, void *data);
void recvClient(int cfd, void *data);
/*void eventAdd(int eventFd, int event, chatClient *client);
void eventDelete(int eventFd, int event, chatClient *client);
void eventSet(int eventFd, int event, chatClient *client);*/

inline void signalHandler(int signo)
{
	switch (signo) {
		case SIGINT:
			printf("receive a signal\n");
			exit(0);
			break;

	}
}



#endif