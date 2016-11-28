//
// Created by tywei on 2016/11/24.
//

#ifndef __CHAT_NETWORKING_H
#define __CHAT_NETWORKING_H


#include "setproctitle.c"
#include "chat.h"
#include "anet.h"
#include "ae.h"

void acceptTcpHandler(aeEventLoop *el, int fd, void *privdata, int mask);
void readClient(aeEventLoop *eventLoop, int fd, void *clientData, int mask);
void unlinkClient(client *c);
#endif
