//
// Created by tywei on 2016/11/24.
//

#include "networking.h"

#define MAX_ACCEPTS_PER_CALL 1000
static void acceptCommonHandler(int fd, int flags, char *ip) {
    client *c = (client *)zmalloc(sizeof(client));
    c->fd = fd;
    if (aeCreateFileEvent(server.el, fd, AE_READABLE, readClient, (void *) c) == AE_ERR) {
        close(fd);
        zfree(c);
        return;
    }
    anetNonBlock(NULL, fd);
    return;
}

void acceptTcpHandler(aeEventLoop *el, int fd, void *privdata, int mask) {
    int cport, cfd, max = MAX_ACCEPTS_PER_CALL;
    char cip[NET_IP_STR_LEN];
    UNUSED(el);
    UNUSED(mask);
    UNUSED(privdata);

    while (max--) {
        cfd = anetTcpAccept(server.neterr, fd, cip, sizeof(cip), &cport);
        if (cfd == ANET_ERR) {
            if (errno != EWOULDBLOCK)
                serverLog(LL_WARNING,
                          "Accepting client connection: %s", server.neterr);
            return;
        }
        serverLog(LL_VERBOSE, "Accepted %s:%d", cip, cport);
        acceptCommonHandler(cfd, 0, cip);
    }
}

void readClient(aeEventLoop *eventLoop, int fd, void *clientData, int mask) {
    UNUSED(mask);
    int nread;
    client *c = (client *)clientData;
    //取body长度
    if (c->bufsize <= 0) { 
        uint16_t size;
        nread = read(fd, (void *)&size, 2);
        if (nread <= 0) {
            serverLog(LL_VERBOSE, "Client closed connection fd:%d", c->fd);
            freeClient(c);
            return;
        }
        c->bufsize = (int)ntohs(size);
        return;
    }

    //取body
    nread = read(fd, (void *)(c->buf + c->bufpos), c->bufsize - c->bufpos);
    if (nread == -1) {
        if (errno == EAGAIN) {
            return;
        } else {
            serverLog(LL_VERBOSE, "Reading from client: %s",strerror(errno));
            return;
        }
    } else if (nread == 0) {
        serverLog(LL_VERBOSE, "body Client closed connection fd:%d", c->fd);
        freeClient(c);
        return;
    }
    c->bufpos += nread;
    if (c->bufpos >= c->bufsize) {
        serverLog(LL_VERBOSE, "msg: %s", c->buf);
        //init
        c->bufpos = c->bufsize = 0;
        memset(c->buf, 0, PROTO_REPLY_CHUNK_BYTES);
        return;
    }
}

void freeClient(client *c) {
    if (c->fd != -1) {
        aeDeleteFileEvent(server.el, c->fd, AE_READABLE);
        aeDeleteFileEvent(server.el, c->fd, AE_WRITABLE);
        close(c->fd);
    }
    zfree(c);
}

