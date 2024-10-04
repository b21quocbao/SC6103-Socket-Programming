#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "types/flight.h"

void serialize_flight(Flight *flight, uint8_t *buffer, size_t *len)
{
    // Copy id
    uint32_t htonl_id = htonl(flight->id);
    memcpy(buffer + *len, &htonl_id, sizeof(flight->id));
    *len += sizeof(flight->id);

    // Copy src length
    uint32_t htonl_src_len = htonl(flight->src_len);
    memcpy(buffer + *len, &htonl_src_len, sizeof(flight->src_len));
    *len += sizeof(flight->src_len);

    // Copy src string
    memcpy(buffer + *len, flight->src, flight->src_len);
    *len += flight->src_len;

    // Copy dest length
    uint32_t htonl_dest_len = htonl(flight->dest_len);
    memcpy(buffer + *len, &htonl_dest_len, sizeof(flight->dest_len));
    *len += sizeof(flight->dest_len);

    // Copy dest string
    memcpy(buffer + *len, flight->dest, flight->dest_len);
    *len += flight->dest_len;

    // Copy departure time
    uint32_t htonl_dep = htonl(flight->dep);
    memcpy(buffer + *len, &htonl_dep, sizeof(flight->dep));
    *len += sizeof(flight->dep);

    // Copy fare (double)
    memcpy(buffer + *len, &flight->fare, sizeof(flight->fare));
    *len += sizeof(flight->fare);

    // Copy seat availability
    uint32_t htonl_seat_avail = htonl(flight->seat_avail);
    memcpy(buffer + *len, &htonl_seat_avail, sizeof(flight->seat_avail));
    *len += sizeof(flight->seat_avail);
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