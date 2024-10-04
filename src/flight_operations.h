#include <stdint.h>
#include <stdio.h>
#include "utils.h"
#include "serialization.h"

#define SUCCESS 1
#define ERROR 0

void seed_flight_data();
void print_flight_data();
void query_flights_by_src_dest(uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len);
void query_flight_by_id(uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len);
void reserve_seats(uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len);
void register_for_seat_updates(uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len, uint32_t monitor_duration);
void query_flights_by_src_fare_range(uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len);
void delay_flight(uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len);