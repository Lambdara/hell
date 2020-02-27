#ifndef NETWORKING_H
#define NETWORKING_H
#include <stdlib.h>
#include <strings.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SOCKET_PORT 6824
#define CONNECTION_BUFFER_SIZE 512

int connect_to_client(int *);

#endif
