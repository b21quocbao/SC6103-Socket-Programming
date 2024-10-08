#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

int random_int(int min, int max);
double random_fare(double min, double max);
void makeReceiverSA(struct sockaddr_in *sa, int port);
void extract_header(const uint8_t* request, uint8_t* message_type, uint8_t* request_id, uint8_t* service_type);
void prepend_header(uint8_t message_type, uint8_t request_id, uint8_t service_type, uint8_t* response, size_t* response_len);
void prepend_msg(uint8_t *buffer, uint8_t status, char* msg, size_t *len);