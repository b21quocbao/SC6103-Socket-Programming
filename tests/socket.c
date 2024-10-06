#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_PORT 3000
#define SERVER_IP "127.0.0.1" // Change to your server's IP address
#define BUFFER_SIZE 1000

void send_test_payload(int sockfd, struct sockaddr_in *server_addr, int service_type)
{
    // Payload 1
    uint8_t payload1[] = {0, 1, 1, 0, 0, 0, 9, 83, 105, 110, 103, 97, 112, 111, 114, 101, 0, 0, 0, 7, 74, 97, 107, 97, 114, 116, 97};
    // Payload 2
    uint8_t payload2[] = {0, 2, 2, 0, 0, 0, 11};
    // Payload 3
    uint8_t payload3[] = {0, 3, 3, 0, 0, 0, 11, 0, 0, 0, 2};
    // Payload 4
    uint8_t payload4[] = {0, 4, 4, 0, 0, 0, 11, 0, 54, (uint8_t)-18, (uint8_t)-128};
    // Payload 5
    uint8_t payload5[] = {0, 5, 5, 0, 0, 0, 9, 83, 105, 110, 103, 97, 112, 111, 114, 101, 64, 114, (uint8_t)-64, 0, 0, 0, 0, 0, 64, (uint8_t)-113, 64, 0, 0, 0, 0, 0};
    // Payload 6
    uint8_t payload6[] = {0, 6, 6, 0, 0, 0, 11, 0, 54, (uint8_t)-18, (uint8_t)-128};

    switch (service_type)
    {
    case 1:
        sendto(sockfd, payload1, sizeof(payload1), 0, (struct sockaddr *)server_addr, sizeof(*server_addr));
        break;
    case 2:
        sendto(sockfd, payload2, sizeof(payload2), 0, (struct sockaddr *)server_addr, sizeof(*server_addr));
        break;
    case 3:
        sendto(sockfd, payload3, sizeof(payload3), 0, (struct sockaddr *)server_addr, sizeof(*server_addr));
        break;
    case 4:
        sendto(sockfd, payload4, sizeof(payload4), 0, (struct sockaddr *)server_addr, sizeof(*server_addr));
        break;
    case 5:
        sendto(sockfd, payload5, sizeof(payload5), 0, (struct sockaddr *)server_addr, sizeof(*server_addr));
        break;
    case 6:
        sendto(sockfd, payload6, sizeof(payload6), 0, (struct sockaddr *)server_addr, sizeof(*server_addr));
        break;
    default:
        // Invalid service type
        printf("Invalid service type: %d\n", service_type);
    }
}

void listen_for_responses(int sockfd)
{
    uint8_t buffer[BUFFER_SIZE];
    ssize_t recv_len;
    struct sockaddr_in server_addr;
    socklen_t server_addr_len = sizeof(server_addr);

    // Wait to receive the response from the server
    recv_len = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&server_addr, &server_addr_len);

    printf("Length: %ld. Response: [", recv_len);
    for (int i = 0; i < recv_len; ++i)
    {
        printf("%d", buffer[i]);
        if (i < recv_len - 1)
        {
            printf(", ");
        }
    }
    printf("]\n");
}

int main()
{
    int sockfd;
    struct sockaddr_in server_addr;

    // Create a UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("Socket creation failed");
        return -1;
    }

    // Configure server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    for (int i = 1; i <= 6; ++i) {
        sleep(1);
        printf("Sending payload%d\n", i);
        send_test_payload(sockfd, &server_addr, i);

        // Listen for the server response (seat updates or timeout notification)
        listen_for_responses(sockfd);
    }


    // Close the socket
    close(sockfd);
    return 0;
}
