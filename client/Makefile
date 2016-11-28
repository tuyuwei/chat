cc = gcc
options = -Wall -W -g -std=c99
objs = client.o
target = client
source = client.c

$(target): $(objs)
	$(cc) $(objs) -o $(target)

$(objs): client.c
	$(cc) $(options) -c client.c -o $(objs)

clean:
	rm -rf $(objs) $(target)