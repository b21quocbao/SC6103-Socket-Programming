#include <stdint.h>
#include <stdio.h>
#include "types/flight.h"

void serialize_flight(Flight *flight, uint8_t *buffer, size_t *len);
void deserialize_flight(uint8_t *buffer, Flight *flight);