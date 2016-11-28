#ifndef __CHAT_H
#define __CHAT_H

#include "setproctitle.c"
#include "ae.h"
#include "zmalloc.h"
#include "anet.h"
#include "networking.h"

#define EVENT_MAX 1024
/* Anti-warning macro... */
#define UNUSED(V) ((void) V)

#define C_OK                    0
#define C_ERR                   -1
#define CONFIG_BINDADDR_MAX        16
#define LOG_MAX_LEN    1024 /* Default maximum length of syslog messages */
#define NET_IP_STR_LEN 46

/* Log levels */
#define LL_DEBUG 0
#define LL_VERBOSE 1
#define LL_NOTICE 2
#define LL_WARNING 3
#define LL_RAW (1<<10) /* Modifier to log without timestamp */
#define CONFIG_DEFAULT_VERBOSITY LL_NOTICE
#define PROTO_REPLY_CHUNK_BYTES 2048

typedef struct {
    aeEventLoop *el;
    /* Configuration */
    int verbosity;                  /* Loglevel in redis.conf */
    /* Logging */
    char *logfile;                  /* Path of log file */
    /* Networking */
    int port;                   /* TCP listening port */
    int tcp_backlog;            /* TCP listen() backlog */
    char neterr[ANET_ERR_LEN];    /* Error buffer for anet.c */
    char *bindaddr[CONFIG_BINDADDR_MAX]; /* Addresses we should bind to */
    int ipfd[CONFIG_BINDADDR_MAX]; /* TCP socket file descriptors */
} chatServer;

typedef struct client {
    //uint64_t id;            /* Client incremental unique ID. */
    int fd;                 /* Client socket. */
    struct chatCommand *cmd, *lastcmd;  /* Last command executed. */
    /* Response buffer */
    int bufpos;
    int bufsize;
    char buf[PROTO_REPLY_CHUNK_BYTES];
} client;


void initServer(void);

int listenToPort(int, int *, int);

extern chatServer server;

#endif