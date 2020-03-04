#include "networking.h"

int connect_to_client(int *sockfd){ 
    unsigned int len;
    struct sockaddr_in serveraddr, client;

    *sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (*sockfd == -1) {
        fprintf(stderr, "Socket failed to initialize\n");
        return -1;
    }
    bzero(&serveraddr, sizeof(serveraddr));

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SOCKET_PORT);

    if ((bind(*sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr))) != 0) {
        fprintf(stderr, "Socket failed to bind\n");
        return -1;
    }

    if ((listen(*sockfd, 5)) != 0) {
        fprintf(stderr, "Socket failed to start listening\n");
        return -1;
    }

    len = sizeof(client);

    int conn_fd = accept(*sockfd, (struct sockaddr *)(&client), &len);
    if (conn_fd < 0) {
        fprintf(stderr, "Socket failed to accept\n");
        return -1;
    }

    return conn_fd;
}
