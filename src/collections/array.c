
#include <stddef.h>
#include <stdlib.h>
#include <collections/array.h>
#include <stdio.h>

int get_bit(char* a, const size_t bit) {
  return a[bit / 8] & (1 << (bit % 8)) != 0;
}

void set_bit_high(char* a, const size_t bit) {a[bit / 8] |= (1 << (bit % 8));}

void set_bit_low(char* a, const size_t bit) {a[bit / 8] &= ~(1 << (bit % 8));}

void print_array(array arr, int item_width) {

  char* current_item = malloc(item_width);
  printf("[");
  for (size_t i = 0; i < arr.length; i += item_width) {
    
    strncpy(current_item, arr.block + i, item_width);
    printf("%lld", *(long long*)current_item & ~(-1 << item_width));
    if (i + item_width < arr.length) {
      printf(", ");
    }
  }
  printf("]\n");
  free(current_item);
}