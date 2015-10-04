cc = gcc
options = -Wall -g -std=c99
objs = ../objs/chat.o
target = ../objs/chatServer
source = chat.c chat.h

$(target): $(objs)
	$(cc) $(objs) -o $(target)

$(objs): chat.c chat.h
	$(cc) $(options) -c chat.c -o $(objs)

clean:
	rm -rf $(objs) $(target)