#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>     // For sleep
#include <pthread.h>    // For POSIX threads
#include <sys/socket.h> // For socket functions
#include <arpa/inet.h>  // For inet_addr
#include <time.h>       // For time management
#include "utils.h"
#include <stdlib.h>

#define MAX_CLIENTS 10

typedef struct
{
    struct sockaddr_in client_addr;
    socklen_t client_len;
    uint32_t flight_id;
    char ip_address[INET_ADDRSTRLEN];
    uint16_t port;
} Client;

Client client_list[MAX_CLIENTS];
int client_count = 0;

typedef struct
{
    int sockfd;
    int client_index;
    uint32_t monitor_time_ms;
} TimeoutArgs;

void *timeout_notify(void *args)
{
    TimeoutArgs *timeoutArgs = (TimeoutArgs *)args;
    int sockfd = timeoutArgs->sockfd;
    int client_index = timeoutArgs->client_index;
    Client client = client_list[client_index];
    uint32_t monitor_time_ms = timeoutArgs->monitor_time_ms;

    // Sleep for the monitor time duration
    usleep(monitor_time_ms * 1000);

    // Send a timeout notification to the client
    uint8_t timeout_output[1000]; // Define timeout message
    size_t timeout_output_len = 0;

    prepend_header(1, 0, 4, timeout_output, &timeout_output_len);
    prepend_msg(timeout_output, 0, "Monitor timeout expired", &timeout_output_len);
    sendto(sockfd, timeout_output, timeout_output_len, 0, (struct sockaddr *)&client.client_addr, client.client_len);
    printf("Sent timeout notification to client at %s:%u for Flight ID %u\n", client.ip_address, client.port, client.flight_id);

    // Remove client from the client_list after timeout
    printf("Removing client at index %d\n", client_index);
    if (client_index < client_count - 1)
    {
        client_list[client_index] = client_list[client_count - 1]; // Overwrite with the last client
    }
    client_count--;

    // Free the dynamically allocated memory for args
    free(timeoutArgs);

    return NULL;
}

void send_seat_update_to_client(int sockfd, uint8_t *output, size_t output_len, uint32_t flight_id)
{
    for (int i = 0; i < client_count; i++)
    {
        if (client_list[i].flight_id == flight_id)
        {
            sendto(sockfd, output, output_len, 0, (struct sockaddr *)&client_list[i].client_addr, client_list[i].client_len);
        }
    }
}

void add_client_for_seat_updates(int sockfd, struct sockaddr_in *client_addr, socklen_t client_len, uint32_t flight_id, uint32_t monitor_time_ms)
{
    if (client_count < MAX_CLIENTS)
    {
        client_list[client_count].flight_id = flight_id;
        inet_ntop(AF_INET, &(client_addr->sin_addr), client_list[client_count].ip_address, INET_ADDRSTRLEN);
        client_list[client_count].port = ntohs(client_addr->sin_port);
        client_list[client_count].client_addr = *client_addr;
        client_list[client_count].client_len = client_len;
        client_count++;
    }

    // Create arguments for the timeout thread
    TimeoutArgs *args = (TimeoutArgs *)malloc(sizeof(TimeoutArgs));
    args->sockfd = sockfd;
    args->client_index = client_count - 1;
    args->monitor_time_ms = monitor_time_ms;

    // Create a thread to notify the client after the timeout
    pthread_t timeout_thread;
    pthread_create(&timeout_thread, NULL, timeout_notify, (void *)args);
    pthread_detach(timeout_thread); // Automatically clean up the thread after it finishes
}
