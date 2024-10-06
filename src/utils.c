#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Utility function to generate a random integer between two values
int random_int(int min, int max)
{
    return min + rand() % (max - min + 1);
}

// Utility function to generate random airfare between two values
double random_fare(double min, double max)
{
    return min + (double)rand() / RAND_MAX * (max - min);
}

void makeReceiverSA(struct sockaddr_in *sa, int port)
{
    sa->sin_family = AF_INET;
    sa->sin_port = htons(port);
    sa->sin_addr.s_addr = htonl(INADDR_ANY);
}

// Extracts [message_type][request_id][service_type] from the request
void extract_header(const uint8_t *request, uint8_t *message_type, uint8_t *request_id, uint8_t *service_type)
{
    // Assuming request is valid and contains at least 3 bytes
    *message_type = request[0]; // Extract message_type (1 byte)
    *request_id = request[1];   // Extract request_id (1 byte)
    *service_type = request[2]; // Extract service_type (1 byte)

    printf("Extracted: message_type=%d, request_id=%d, service_type=%d\n", *message_type, *request_id, *service_type);
}

// Prepends [message_type][request_id][service_type] to the response
void prepend_header(uint8_t message_type, uint8_t request_id, uint8_t service_type, uint8_t *response, size_t *response_len)
{
    // Prepend the three extracted values to the start of the response
    response[0] = 1;
    response[1] = request_id;
    response[2] = service_type;

    // Adjust the response length to account for the prepended 3-byte header
    *response_len += 3;

    printf("Prepended: message_type=%d, request_id=%d, service_type=%d\n", message_type, request_id, service_type);
}

void prepend_msg(uint8_t *buffer, uint8_t status, char* msg, size_t *len) {
    buffer[3] = status;
    *len += 1;
    
    uint32_t msg_len = strlen(msg);
    uint32_t htonl_msg_len = htonl(msg_len);
    memcpy(buffer + *len, &htonl_msg_len, sizeof(msg_len));
    *len += sizeof(msg_len);

    memcpy(buffer + *len, msg, msg_len);
    *len += strlen(msg);
}