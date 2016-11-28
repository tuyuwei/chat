#include "chat.h"

chatServer server;


/* Like serverLogRaw() but with printf-alike support. This is the function that
 * is used across the code. The raw version is only used in order to dump
 * the INFO output on crash. */
void serverLog(int level, const char *fmt, ...) {
    va_list ap;
    char msg[LOG_MAX_LEN];

    if ((level & 0xff) < server.verbosity) return;

    va_start(ap, fmt);
    vsnprintf(msg, sizeof(msg), fmt, ap);
    va_end(ap);
    char *lv;
    switch(level) {
        case LL_DEBUG: lv = "debug"; break;
        case LL_NOTICE: lv = "notice"; break;
        case LL_WARNING: lv = "warning"; break;
        case LL_VERBOSE: lv = "verbose"; break;
        default: return;
    }
    fprintf(stdout, "[%s] %s\n", lv, msg);
    fflush(stdout);
}

void initServer(void)
{
    server.port = 8888;
    server.verbosity = LL_DEBUG;
    server.el = aeCreateEventLoop(EVENT_MAX);
    if (server.port != 0 && listenToPort(server.port, server.ipfd, 0) == C_ERR) {
        serverLog(LL_WARNING, "listenToPort error.");
        return;
    }

    if (aeCreateFileEvent(server.el, server.ipfd[0], AE_READABLE, acceptTcpHandler, NULL) == AE_ERR) {
        serverLog(LL_WARNING, "aeCreateFileEvent error.");
        return;
    }
}

int listenToPort(int port, int *fds, int count)
{
    fds[count] = anetTcpServer(server.neterr, port, NULL,
                               server.tcp_backlog);
    if (fds[count] == ANET_ERR) {
        serverLog(LL_WARNING, "anetTcpServer error port:%d", port);
        return C_ERR;
    }
    anetNonBlock(NULL, fds[count]);
    return C_OK;
}

int main(int argc, char const *argv[])
{
    initServer();
    aeMain(server.el);
    aeDeleteEventLoop(server.el);
}

