// (c) 2025 Pablo Ibarz
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef ARRAY_H_
#define ARRAY_H_

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

// Simple structured buffer.
// `block` is the contents of the array, while `length` should describe the 
// number of bytes after block[0] that are populated or valid, and `capacity`
// describes the maximum size of the array
// 
// The main use case of this struct is to have multiple bounds checks on the 
// same buffer between different functions, and to have a nice print-out of the
// buffer's contents.
struct array {
  char* block;
  size_t length;
  const size_t capacity;
} typedef array;

int get_bit(char* a, const size_t bit);

void set_bit_high(char* a, const size_t bit);

void set_bit_low(char* a, const size_t bit);

// Prints an array with items of width `item_width` bytes, for example:
// print_array(arr, 8) -> [0x0000, 0x0011, 0x00A0] implies the block contains at
// least 3 64-bit values and shows their contents. 
// `item_width` should be 1 to sizeof(long long).
void print_array(array arr, const int item_width);

#endif