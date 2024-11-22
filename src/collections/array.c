
#include <stddef.h>
#include <collections/array.h>
#include <stdio.h>

int get_bit(char* a, const size_t bit) {return a[bit / 8] & (1 << (bit % 8)) != 0;}

void set_bit_high(char* a, const size_t bit) {a[bit / 8] |= (1 << (bit % 8));}

void set_bit_low(char* a, const size_t bit) {a[bit / 8] &= ~(1 << (bit % 8));}

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