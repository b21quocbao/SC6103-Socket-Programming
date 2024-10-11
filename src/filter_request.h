#include <netinet/in.h>

typedef struct
{
    uint8_t request_id;
    uint8_t *response;
    size_t response_len;
} RequestEntry;

RequestEntry* find_request(struct sockaddr_in *client_addr, uint8_t request_id);
void store_request(struct sockaddr_in *client_addr, uint8_t request_id, const uint8_t *response, size_t response_len);