#include <stdint.h>
#include <stdio.h>
#include <string.h>

int main(){
  // uint8_t input[] = { 115, 111, 117, 114, 99, 101};
  // char src[100];
  uint8_t buffer[] = {132, 57, 184, 201, 193, 252, 139, 64};
  double fare = 1234.5678;
  // uint64_t fare = 32;
  // memcpy(&fare, buffer, sizeof(fare));
  // printf("%f\n", fare);

  memcpy(buffer, &fare, sizeof(fare));
  for (int i = 0; i < 8; ++i) {
    printf("%02x %d\n", buffer[i], buffer[i]);
  }

  return 0;
}