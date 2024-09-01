#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <chrono>

int s, n;
struct sockaddr_in mySocketAddress, yourSocketAddress;
std::string input;

void setBroadcast(int s)
{
    int arg;
#ifdef SO_BROADCAST
    arg = 1;
    if (setsockopt(s, SOL_SOCKET, SO_BROADCAST, &arg, sizeof(arg)) < 0)
    {
        perror("setsockopt SO_BROADCAST---");
        exit(-1);
    }
#endif
}

void makeLocalSA(struct sockaddr_in *sa)
{
    sa->sin_family = AF_INET;
    sa->sin_port = htons(0);
    sa->sin_addr.s_addr = htonl(INADDR_ANY);
}

void makeDestSA(struct sockaddr_in *sa, char *hostname, int port)
{
    struct hostent *host;
    sa->sin_family = AF_INET;
    if ((host = gethostbyname(hostname)) == NULL)
    {
        printf("Unknown host name\n");
        exit(-1);
    }
    sa->sin_addr = *(struct in_addr *)(host->h_addr);
    sa->sin_port = htons(port);
}

/* send 2 messages to machine at port number */
void construct_sender(char *machine, int port)
{
    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket failed");
        return;
    }
    setBroadcast(s); /*see Section 2.7 */
    makeLocalSA(&mySocketAddress);
    if (bind(s, (struct sockaddr *)&mySocketAddress, sizeof(struct sockaddr_in)) != 0)
    {
        perror("Bind failed\n");
        close(s);
        return;
    }
    makeDestSA(&yourSocketAddress, machine, port);
}

int main()
{
    int SIZE = 400;
    char message[SIZE];
    unsigned int aLength = sizeof(struct sockaddr_in);
    std::string machine, port;

    std::cout << "Enter server host: ";
    std::cin >> machine; // Read input from stdin
    std::cout << "Enter server port: ";
    std::cin >> port; // Read input from stdin

    construct_sender((char*)machine.c_str(), std::stoi(port));

    while (true)
    {
        memset(message, 0, SIZE);

        std::cout << "Enter your name: ";
        std::cin >> input; // Read input from stdin

        // Convert std::string to char* using c_str()
        const char *cstr = input.c_str();

        auto start = std::chrono::high_resolution_clock::now();

        if ((n = sendto(s, cstr, strlen(cstr), 0, (struct sockaddr *)&yourSocketAddress, aLength)) < 0)
        {
            perror("Send 2 failed\n");
        }

        if ((n = recvfrom(s, message, SIZE, 0, (struct sockaddr *)&yourSocketAddress, &aLength)) < 0)
        {
            perror("Receive");
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::micro> duration = end - start;
        std::string latencyMsg = ". Latency in milliseconds: ";

        strcat(message, (latencyMsg + std::to_string(duration.count() / 1000)).c_str()); 

        printf("Message from server: %s\n", message);
    }

    return 0;
}
