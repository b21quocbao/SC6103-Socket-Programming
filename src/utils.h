#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

int random_int(int min, int max);
double random_fare(double min, double max);
void makeReceiverSA(struct sockaddr_in *sa, int port);
void extract_header(const uint8_t* request, uint8_t* messageType, uint8_t* requestId, uint8_t* serviceType);
void prepend_header(uint8_t messageType, uint8_t requestId, uint8_t serviceType, uint8_t* response, size_t* response_len);