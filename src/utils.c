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

// Extracts [messageType][requestId][serviceType] from the request
void extract_header(const uint8_t *request, uint8_t *messageType, uint8_t *requestId, uint8_t *serviceType)
{
    // Assuming request is valid and contains at least 3 bytes
    *messageType = request[0]; // Extract messageType (1 byte)
    *requestId = request[1];   // Extract requestId (1 byte)
    *serviceType = request[2]; // Extract serviceType (1 byte)

    printf("Extracted: messageType=%d, requestId=%d, serviceType=%d\n", *messageType, *requestId, *serviceType);
}

// Prepends [messageType][requestId][serviceType] to the response
void prepend_header(uint8_t messageType, uint8_t requestId, uint8_t serviceType, uint8_t *response, size_t *response_len)
{
    // Prepend the three extracted values to the start of the response
    response[0] = 1;
    response[1] = requestId;
    response[2] = serviceType;

    // Adjust the response length to account for the prepended 3-byte header
    *response_len += 3;

    printf("Prepended: messageType=%d, requestId=%d, serviceType=%d\n", messageType, requestId, serviceType);
}

void prepend_msg(uint8_t *buffer, uint8_t status, char* msg, size_t *len) {
    buffer[3] = status;
    *len += 1;
    
    uint32_t msg_len = strlen(msg);
    memcpy(buffer + *len, htonl(msg_len), sizeof(msg_len));
    *len += sizeof(msg_len);

    memcpy(buffer + *len, msg, msg_len);
    *len += strlen(msg);
}