#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include "utils.h"

int start_server(int port)
{
    printf("Staring receiver...\n");

    struct sockaddr_in mySocketAddress;
    int s;
    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket failed");
        return -1;
    }
    makeReceiverSA(&mySocketAddress, port);
    if (bind(s, (struct sockaddr *)&mySocketAddress, sizeof(struct sockaddr_in)) != 0)
    {
        perror("Bind failed\n");
        close(s);
        return -1;
    }

    return s;
}