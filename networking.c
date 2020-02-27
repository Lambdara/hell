#include "networking.h"

int connect_to_client(int *sockfd){ 
    unsigned int len;
    struct sockaddr_in serveraddr, client;

    *sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (*sockfd == -1) {
        printf("Socket failed to initialize\n");
        return -1;
    } else {
        printf("Socket created\n");
    }
    bzero(&serveraddr, sizeof(serveraddr));

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SOCKET_PORT);

    if ((bind(*sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr))) != 0) {
        printf("Socket failed to bind\n");
        return -1;
    } else {
        printf("Socket bound\n");
    }

    if ((listen(*sockfd, 5)) != 0) {
        printf("Socket failed to start listening\n");
        return -1;
    } else {
        printf("Socket started listening\n");
    }

    len = sizeof(client);

    int conn_fd = accept(*sockfd, (struct sockaddr *)(&client), &len);
    if (conn_fd < 0) {
        printf("Socket failed to accept\n");
        return -1;
    } else {
        printf("Socket accepted\n");
    }

    return conn_fd;
}
