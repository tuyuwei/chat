#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static void frameProtocolPack(char  *msg, char *sendBuffer)
{
    size_t inputLength = strlen(msg);
   	uint16_t len = htons((uint16_t)inputLength);
   	memcpy((void *)sendBuffer, (void *)&len, 2);
    memcpy((void *)sendBuffer + 2, (void *)msg, inputLength);
}

static char *frameProtocolUnpack(char *const msg)
{
    uint16_t len;
    memcpy(&len, msg, 2);
    uint16_t slen = ntohs(len);
    char *newMsg = malloc(sizeof(char)*slen);
    memcpy(newMsg, msg+2, slen);
    return newMsg;
}


//char inputBuffer[1024], sendBuffer[1024];


int main(int argc, char const *argv[])
{
	char sendBuffer[1024];
	char *inputBuffer = "abcd";
	frameProtocolPack(inputBuffer, sendBuffer);
	uint16_t len = frameProtocolUnpack(sendBuffer);
	printf("len: %d\n", len);
    fputs(sendBuffer, stdout);
    return 0;
}
