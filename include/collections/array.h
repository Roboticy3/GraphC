#ifndef ARRAY_H_
#define ARRAY_H_

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

struct array {
  char* block;
  size_t length;
  size_t capacity;
} typedef array;

void reverse(array arr, size_t item_width);

void print_arr(array arr);

#endif