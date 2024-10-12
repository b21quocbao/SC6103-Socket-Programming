#include <netinet/in.h>

typedef struct
{
    char ip_address[INET_ADDRSTRLEN];
    uint8_t request_id;
    uint8_t response[1000];
    size_t response_len;
} RequestEntry;

extern RequestEntry request_map[100];

int find_request(struct sockaddr_in *client_addr, uint8_t request_id);
void store_request(struct sockaddr_in *client_addr, uint8_t request_id, const uint8_t *response, size_t response_len);