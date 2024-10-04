#include <stdint.h>
#include <stdio.h>

// Flight information structure
typedef struct
{
  uint32_t id;
  uint32_t src_len;
  char *src;
  uint32_t dest_len;
  char *dest;
  uint32_t dep; // Departure time in ms (32-bit)
  double fare;  // Airfare (64-bit double)
  uint32_t seat_avail;
} Flight;
