#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void makeReceiverSA(struct sockaddr_in *sa, int port)
{
    sa->sin_family = AF_INET;
    sa->sin_port = htons(port);
    sa->sin_addr.s_addr = htonl(INADDR_ANY);
}

void receiver(int port)
{
    printf("Staring receiver...\n");

    int SIZE = 255;
    char message[SIZE];
    char response[SIZE + 6];

    struct sockaddr_in mySocketAddress, aSocketAddress;
    int s, n;
    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket failed");
        return;
    }
    makeReceiverSA(&mySocketAddress, port);
    if (bind(s, (struct sockaddr *)&mySocketAddress, sizeof(struct sockaddr_in)) != 0)
    {
        perror("Bind failed\n");
        close(s);
        return;
    }
    unsigned int aLength = sizeof(aSocketAddress);
    aSocketAddress.sin_family = AF_INET; /* note that this is needed */

    while (true) {
        memset(message, 0, SIZE);
        memset(response, 0, SIZE + 6);

        if ((n = recvfrom(s, message, SIZE, 0, (struct sockaddr *)&aSocketAddress, &aLength)) < 0)
        {
            perror("Receive");
        }
        printf("message: %s\n", message);
        strcat(response, (char *)"Hello "); 
        strcat(response, message); 

        if ((n = sendto(s, response, strlen(response), 0, (struct sockaddr *)&aSocketAddress, aLength)) < 0) {
            perror("Send back");
        }
    }

    close(s);
}

int main()
{
    receiver(3000);
    return 0;
}
