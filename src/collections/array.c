
#include <stddef.h>
#include <collections/array.h>
#include <stdio.h>

void print_array(array arr, size_t item_width) {
  printf("[");
  for (size_t i = 0; i < arr.length; i++) {
    printf("%x", arr.block[i]);
    if (((i + 1) % item_width) == 0) {
      printf(", ");
    }
  }
  printf("]\n");
}