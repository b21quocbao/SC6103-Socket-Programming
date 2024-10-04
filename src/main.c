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
    size_t response_len;
    uint8_t request[SIZE];
    uint8_t response[SIZE];

    int s = start_server(port);
    int n;
    struct sockaddr_in aSocketAddress;
    unsigned int aLength = sizeof(aSocketAddress);
    aSocketAddress.sin_family = AF_INET; /* note that this is needed */

    uint8_t messageType, requestId, serviceType;

    while (1)
    {
        // Reset bytes
        response_len = 0;
        memset(request, 0, SIZE);
        memset(response, 0, SIZE);

        // Receive request from client
        if ((n = recvfrom(s, request, SIZE, 0, (struct sockaddr *)&aSocketAddress, &aLength)) < 0)
        {
            perror("Receive");
        }

        // Extract messageType, requestId, serviceType from request
        extract_header(request, &messageType, &requestId, &serviceType);

        // Prepend messageType, requestId, serviceType to response
        prepend_header(messageType, requestId, serviceType, response, &response_len);

        // Process request based on service type
        switch (serviceType)
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
            reserve_seats(request, sizeof(request), response, &response_len);
            break;
        case 4:
            // Call register for seat availability updates
            register_for_seat_updates(request, sizeof(request), response, &response_len, 10000); // Example monitor_duration
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
            printf("Invalid service type: %d\n", serviceType);
            response[3] = ERROR; // Error in response
            response_len = 3;    // Length of error message
            break;
        }

        if ((n = sendto(s, response, response_len, 0, (struct sockaddr *)&aSocketAddress, aLength)) < 0)
        {
            perror("Send back");
        }
    }

    close(s);
}

int main()
{
    // Seed flight data for 20 flights
    seed_flight_data(20);

    // Print the seeded flight data
    print_flight_data();

    // Start server at port 3000
    receiver(3000);
    return 0;
}
