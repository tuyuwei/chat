#include "chat.h"

// chatClient client[EVENTS_MAX];


int main(int argc, char const *argv[])
{
	int port = 8888;
	signal(SIGINT,signalHandler);
	int listenFd = initSocket(0, port);
	chatClient *client;
	client = (chatClient *)malloc(sizeof(chatClient));
	client->msg = (chatMsg *)malloc(sizeof(chatMsg));
	
	while (1) {
		connectClient(listenFd, (void *)client);
		while (1) {
			recvClient(client->fd, (void *)client);
			if (client->statusCode != 0) {
				break;
			}
			sendClient(client->fd, (void *)client);
			if (client->statusCode != 0) {
				continue;
			}
		}
		close(client->fd);
	}
	close(listenFd);
}


int initSocket(int efd, int port)
{
	int listenFd = socket(AF_INET, SOCK_STREAM, 0);
	if (listenFd == -1) {
		exit(1);
	}
	printf("listen fd=[%d]\n", listenFd);
	struct sockaddr_in *listenAddr;
	listenAddr = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));
	memset(listenAddr, 0, sizeof(struct sockaddr_in));
	listenAddr->sin_family = AF_INET;
	listenAddr->sin_port = htons(port);
	(listenAddr->sin_addr).s_addr = htonl(INADDR_ANY);
	if (bind(listenFd, (struct sockaddr*) listenAddr, sizeof(struct sockaddr_in)) < 0) {
		perror("bind");
		exit(1);
	}
	if (listen(listenFd, LISTEN_MAX) < 0) {
		perror("listen");
		exit(1);
	}
	printf("initSocket ok [port=%d]\n", port);
	return listenFd;
}

void connectClient(int fd, void *data)
{
	struct sockaddr_in clientAddr;
	chatClient *client = (chatClient *)data;
	int len = sizeof(struct sockaddr_in);
	int cfd = accept(fd, (struct sockaddr *)&clientAddr, (socklen_t *)&len);
	if (cfd < 0) {
		printf("accept error: %s\n", strerror(-1));
		client->statusCode = 1;
	} else {
		client->fd = cfd;
		client->msg = calloc(sizeof(chatMsg), BUFF_MAX);
		printf("new cfd[%d] conn[%s:%d]\n", cfd, inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
	}
}

void sendClient(int cfd, void *data)
{
	chatClient *client = (chatClient *)data;
	ssize_t len = write(client->fd, (void *)(client->msg->data), strlen(client->msg->data) + client->msg->len);
	if (len > 0) {
		printf("sendClient: [cfd=%d], [len=%d], msg:[%s]\n", cfd, (int)len, (char *)(client->msg->data));
		memset(client->msg, 0, sizeof(chatMsg));
	} else {
		close(client->fd);
		client->statusCode = 1;
		printf("sendClient: [fd=%d] [len=%d] errorCode:[%d], errorMsg:[%s]\n", cfd, (int)len, errno, strerror(errno));
	}
}

void recvClient(int cfd, void *data)
{
	chatClient *client = (chatClient *)data;
	chatMsg *msg = (chatMsg *)client->msg;
	int readLen = 2;
	printf("msg len=[%d]\n", (int)msg->len);
	if (msg->len != 0) {
		readLen = msg->len;
	}
	ssize_t len = recv(client->fd, (void *)(msg->data), readLen, 0);
	printf("msg 2 len=[%d]\n", len);
	if ((int)len < 0) {
		close(client->fd);
		client->statusCode = 1;
		printf("recvClient: [fd=%d] error:[%d]\n", cfd, errno);
	}
	if (msg->len == 0){
		msg->len = ntohs(*(short*)(msg->data));
		// memset(msg->data, 0, sizeof(msg->data));
	}
	printf("recvClient: [cfd=%d], [len=%d], msg:[%s]\n", cfd, (int)len, (char *)(msg->data));
}

/**
 * 增加event
 */
void eventAdd(int eventFd, int event, chatClient *client)
{

}

void eventSet(chatClient *client, int cfd, void(*callBack)(int, void *), void *args)
{
	client->fd = cfd;
	client->callBackFunction = callBack;
	client->statusCode = 0;
	memset(client->msg, 0, sizeof(client->msg));

	// epoll_ctl();
}

void eventDelete(int eventFd, int event, chatClient *client)
{

}