#ifndef ARRAY_H_
#define ARRAY_H_

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

struct array {
  char* block;
  size_t length;
  const size_t capacity;
} typedef array;

int get_bit(char* a, const size_t bit);

void set_bit_high(char* a, const size_t bit);

void set_bit_low(char* a, const size_t bit);

void print_array(array arr, const size_t item_width);

#endif