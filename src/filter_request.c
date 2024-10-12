#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>  // For inet_addr

#define MAX_ENTRIES 100 // Maximum number of entries we can store

// Define a struct to store request_id, response, and response_len
typedef struct
{
    char ip_address[INET_ADDRSTRLEN];
    uint8_t request_id;
    uint8_t *response;
    size_t response_len;
} RequestEntry;

// A simple array to store the request entries
RequestEntry request_map[MAX_ENTRIES];
int entry_count = 0; // Track the number of entries in the map

// Function to find request_id in the request_map
RequestEntry* find_request(struct sockaddr_in *client_addr, uint8_t request_id)
{
    char ip_address[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(client_addr->sin_addr), ip_address, INET_ADDRSTRLEN);

    printf("Extracted: ip_address=%s\n", ip_address);
    
    for (int i = 0; i < entry_count; ++i)
    {
        if (strcmp(request_map[i].ip_address, ip_address) == 0 && request_map[i].request_id == request_id)
        {
            printf("Retrieved response_len %d", request_map[i].response_len);
            return &request_map[i]; // Return the index if found
        }
    }
    return NULL; // Return -1 if not found
}

// Function to store the request_id, response, and response_len
// Returns request_id if duplicate, otherwise returns 0 for success
void store_request(struct sockaddr_in *client_addr, uint8_t request_id, const uint8_t *response, size_t response_len)
{
    // Store new request_id and response
    inet_ntop(AF_INET, &(client_addr->sin_addr), request_map[entry_count].ip_address, INET_ADDRSTRLEN);
    request_map[entry_count].request_id = request_id;
    request_map[entry_count].response = (uint8_t *)malloc(response_len); // Allocate memory for the response
    memcpy(request_map[entry_count].response, response, response_len);
    request_map[entry_count].response_len = response_len;
    printf("Stored response_len %d", request_map[entry_count].response_len);
    entry_count++;
}