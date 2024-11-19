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

void print_array(array arr, size_t item_width);

#endif