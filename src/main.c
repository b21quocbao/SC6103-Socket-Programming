#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "socket_server.h"
#include "flight_operations.h"
#include "utils.h"

void receiver(int port)
{
    int SIZE = 1000;
    size_t response_len = 0;
    uint8_t request[SIZE];
    uint8_t response[SIZE];

    int sockfd = start_server(port);
    int n;
    struct sockaddr_in client_address;
    unsigned int client_len = sizeof(client_address);
    client_address.sin_family = AF_INET; /* note that this is needed */

    uint8_t message_type, request_id, service_type;

    while (1)
    {
        // Reset bytes
        response_len = 0;
        memset(request, 0, SIZE);
        memset(response, 0, SIZE);

        // Receive request from client
        if ((n = recvfrom(sockfd, request, SIZE, 0, (struct sockaddr *)&client_address, &client_len)) < 0)
        {
            perror("Receive");
        }

        // Extract message_type, request_id, service_type from request
        extract_header(request, &message_type, &request_id, &service_type);

        // Prepend message_type, request_id, service_type to response
        prepend_header(message_type, request_id, service_type, response, &response_len);

        // Process request based on service type
        switch (service_type)
        {
        case 1:
            // Call query flights by source and destination
            query_flights_by_src_dest(request, sizeof(request), response, &response_len);
            break;
        case 2:
            // Call query flight by ID
            query_flight_by_id(request, sizeof(request), response, &response_len);
            break;
        case 3:
            // Call reserve seats
            reserve_seats(sockfd, request, sizeof(request), response, &response_len);
            break;
        case 4:
            // Call register for seat availability updates
            register_for_seat_updates(sockfd, &client_address, client_len, request, sizeof(request), response, &response_len, 10000); // Example monitor_duration
            break;
        case 5:
            // Call query flights by source and fare range
            query_flights_by_src_fare_range(request, sizeof(request), response, &response_len);
            break;
        case 6:
            // Call delay flight
            delay_flight(request, sizeof(request), response, &response_len);
            break;
        default:
            // Invalid service type
            printf("Invalid service type: %d\n", service_type);
            response[3] = ERROR; // Error in response
            response_len = 3;    // Length of error message
            break;
        }

        printf("Length: %ld. Response: [", response_len);
        for (int i = 0; i < response_len; ++i)
        {
            printf("%d, ", response[i]);
        }
        printf("]\n\n");

        if ((n = sendto(sockfd, response, response_len, 0, (struct sockaddr *)&client_address, client_len)) < 0)
        {
            perror("Send back");
        }
    }

    close(sockfd);
}

int main()
{
    // Seed flight data
    seed_flight_data();

    // Print the seeded flight data
    print_flight_data();

    // Start server at port 3000
    receiver(3000);
    return 0;
}
