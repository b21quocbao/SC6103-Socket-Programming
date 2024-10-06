#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include "utils.h"

int start_server(int port)
{
    printf("Staring receiver...\n");

    struct sockaddr_in server_address;
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket failed");
        return -1;
    }
    makeReceiverSA(&server_address, port);
    if (bind(sockfd, (struct sockaddr *)&server_address, sizeof(struct sockaddr_in)) != 0)
    {
        perror("Bind failed\n");
        close(sockfd);
        return -1;
    }

    return sockfd;
}