# 借鉴redis Makefile

CC = gcc
STD = -std=c99 -pedantic -DREDIS_STATIC=''
WARN = -Wall -W

CHAT_OBJ = chat.o ae.o ae_epoll.o zmalloc.o setproctitle.o anet.o networking.o
CHAT_SERVER_NAME = chatServer

$(CHAT_SERVER_NAME): $(CHAT_OBJ)
	$(CC) $(CHAT_OBJ) -g $(STD) -o $(CHAT_SERVER_NAME)

%.o: %.c
	$(CC) $(WARN) -g -c $< -o $@

clean:
	rm -rf $(CHAT_OBJ) $(CHAT_SERVER_NAME)