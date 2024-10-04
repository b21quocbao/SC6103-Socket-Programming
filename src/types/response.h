#include <stdint.h>
#include <stdio.h>

// Serialized message structure (request/response)
typedef struct
{
  uint8_t status;   // 1 byte
  uint32_t msg_len; // 32-bit message length
  char *message;    // Success/Error message
} ResponseMessage;