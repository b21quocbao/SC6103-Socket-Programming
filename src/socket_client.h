#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/socket.h> // For socket functions

void send_seat_update_to_client(int sockfd, uint8_t *output, size_t output_len, uint32_t flight_id);
void add_client_for_seat_updates(int sockfd, struct sockaddr_in *client_addr, socklen_t client_len, uint32_t flight_id, uint32_t monitor_time_ms);