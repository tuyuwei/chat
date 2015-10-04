cc = gcc
options = -Wall -g -std=c99
objs = ../objs/client.o
target = ../objs/client
source = client.c

$(target): $(objs)
	$(cc) $(objs) -o $(target)

$(objs): client.c
	$(cc) $(options) -c client.c -o $(objs)

clean:
	rm -rf $(objs) $(target)