#include <stdint.h>
#include <stdio.h>
#include "types/flight.h"

void serialize_flight(Flight *flight, uint8_t *buffer, size_t *len)
{
    size_t offset = 3;

    // Copy id
    memcpy(buffer + offset, &flight->id, sizeof(flight->id));
    offset += sizeof(flight->id);

    // Copy src length
    memcpy(buffer + offset, &flight->src_len, sizeof(flight->src_len));
    offset += sizeof(flight->src_len);

    // Copy src string
    memcpy(buffer + offset, flight->src, flight->src_len);
    offset += flight->src_len;

    // Copy dest length
    memcpy(buffer + offset, &flight->dest_len, sizeof(flight->dest_len));
    offset += sizeof(flight->dest_len);

    // Copy dest string
    memcpy(buffer + offset, flight->dest, flight->dest_len);
    offset += flight->dest_len;

    // Copy departure time
    memcpy(buffer + offset, &flight->dep, sizeof(flight->dep));
    offset += sizeof(flight->dep);

    // Copy fare (double)
    memcpy(buffer + offset, &flight->fare, sizeof(flight->fare));
    offset += sizeof(flight->fare);

    // Copy seat availability
    memcpy(buffer + offset, &flight->seat_avail, sizeof(flight->seat_avail));
    offset += sizeof(flight->seat_avail);

    // Return the total length of the serialized data
    *len = offset;
}

void deserialize_flight(uint8_t *buffer, Flight *flight)
{
    size_t offset = 3;

    // Read id
    memcpy(&flight->id, buffer + offset, sizeof(flight->id));
    offset += sizeof(flight->id);

    // Read src length
    memcpy(&flight->src_len, buffer + offset, sizeof(flight->src_len));
    offset += sizeof(flight->src_len);

    // Read src string
    flight->src = malloc(flight->src_len);
    memcpy(flight->src, buffer + offset, flight->src_len);
    offset += flight->src_len;

    // Read dest length
    memcpy(&flight->dest_len, buffer + offset, sizeof(flight->dest_len));
    offset += sizeof(flight->dest_len);

    // Read dest string
    flight->dest = malloc(flight->dest_len);
    memcpy(flight->dest, buffer + offset, flight->dest_len);
    offset += flight->dest_len;

    // Read departure time
    memcpy(&flight->dep, buffer + offset, sizeof(flight->dep));
    offset += sizeof(flight->dep);

    // Read fare
    memcpy(&flight->fare, buffer + offset, sizeof(flight->fare));
    offset += sizeof(flight->fare);

    // Read seat availability
    memcpy(&flight->seat_avail, buffer + offset, sizeof(flight->seat_avail));
}